/*
 * (c) copyright 1987 by the Vrije Universiteit, Amsterdam, The Netherlands.
 * See the copyright notice in the ACK home directory, in the file "Copyright".
 */
/* $Header$ */
/* MAIN PROGRAM */

#include	"lint.h"
#include	<system.h>
#include	"nopp.h"
#include	"target_sizes.h"
#include	"debug.h"
#include	"use_tmp.h"
#include	"inputtype.h"
#include	"input.h"
#include	"level.h"
#include	"idf.h"
#include	"arith.h"
#include	"type.h"
#include	"proto.h"
#include	"declar.h"
#include	"tokenname.h"
#include	"Lpars.h"
#include	"LLlex.h"
#include	<alloc.h>
#include	"specials.h"
#include	"nocross.h"
#include	"sizes.h"
#include	"align.h"

extern struct tokenname tkidf[];
extern char *symbol2str();
extern char options[128];

#ifndef NOPP
int inc_pos = 1;			/* place where next -I goes */
int inc_total = 0;
int inc_max;
char **inctable;

extern char *getwdir();
#endif NOPP

struct sp_id special_ids[] =	{
	{"setjmp", SP_SETJMP},	/* non-local goto's are registered	*/
	{0, 0}
};

#ifndef NOCROSS
arith
	short_size = SZ_SHORT,
	word_size = SZ_WORD,
	dword_size = (2 * SZ_WORD),
	int_size = SZ_INT,
	long_size = SZ_LONG,
	float_size = SZ_FLOAT,
	double_size = SZ_DOUBLE,
	lngdbl_size = SZ_LNGDBL,
	pointer_size = SZ_POINTER;

int
	short_align = AL_SHORT,
	word_align = AL_WORD,
	int_align = AL_INT,
	long_align = AL_LONG,
	float_align = AL_FLOAT,
	double_align = AL_DOUBLE,
	lngdbl_align = AL_LNGDBL,
	pointer_align = AL_POINTER,
	struct_align = AL_STRUCT,
	union_align = AL_UNION;
#endif NOCROSS

#ifndef NOPP
arith ifval;	/* ifval will contain the result of the #if expression	*/
#endif NOPP

char *prog_name;

main(argc, argv)
	char *argv[];
{
	/* parse and interpret the command line options	*/
	prog_name = argv[0];

#ifndef NOPP
	inctable = (char **) Malloc(10 * sizeof(char *));
	inctable[0] = "";
	inctable[1] = "/usr/include";
	inctable[2] = 0;
	inc_total = 2;
	inc_max = 10;

	init_pp();	/* initialise the preprocessor macros	*/
#endif NOPP

	/*	Note: source file "-" indicates that the source is supplied
		as standard input.  This is only allowed if INP_READ_IN_ONE is
		not defined!
	*/
#ifdef INP_READ_IN_ONE
	while (argc > 1 && *argv[1] == '-')
#else INP_READ_IN_ONE
	while (argc > 1 && *argv[1] == '-' && argv[1][1] != '\0')
#endif INP_READ_IN_ONE
	{
		char *par = &argv[1][1];

		do_option(par);
		argc--, argv++;
	}
#ifdef	LINT
	lint_init();
#endif	LINT
	compile(argc - 1, &argv[1]);

#ifdef	DEBUG
	hash_stat();
#endif	DEBUG

	sys_stop(err_occurred ? S_EXIT : S_END);
	/*NOTREACHED*/
}

char *source = 0;

#ifdef GEN_NM_LIST
char *nmlist = 0;
#endif GEN_NM_LIST

compile(argc, argv)
	char *argv[];
{
	char *result;
#ifndef	LINT
	register char *destination = 0;
#endif	LINT

#ifdef DEBUG
#ifndef NOPP
	int pp_only = options['E'] || options['P'] || options['C'];
#endif NOPP
#endif

	switch (argc) {
	case 1:
#ifndef	LINT
#ifdef DEBUG
#ifndef NOPP
		if (!pp_only)
#endif NOPP
#endif
			fatal("%s: destination file not specified", prog_name);
#endif	LINT
		break;

#ifndef	LINT
	case 2:
		destination = argv[1];
		break;
#ifdef GEN_NM_LIST
	case 3:
		nmlist = argv[2];
		destination = argv[1];
		break;
#endif GEN_NM_LIST
#endif	LINT

	default:
#ifndef	LINT
#ifdef GEN_NM_LIST
		fatal("use: %s source destination [namelist]", prog_name);
#else GEN_NM_LIST
		fatal("use: %s source destination", prog_name);
#endif GEN_NM_LIST
#else	LINT
		fatal("use: %s source", prog_name);
#endif	LINT
		break;
	}

	if (strcmp(argv[0], "-"))
		FileName = source = argv[0];
	else {
		source = 0;
		FileName = "standard input";
	}

	if (!InsertFile(source, (char **) 0, &result)) /* read the source file	*/
		fatal("%s: no source file %s\n", prog_name, FileName);
	File_Inserted = 1;
	init();
	LineNumber = 0;
	nestlow = -1;
#ifndef NOPP
	WorkingDir = getwdir(source);
	PushLex();			/* initialize lex machine */
#else NOPP
	GetToken(&ahead);
#endif NOPP

#ifdef DEBUG
#ifndef NOPP
	if (pp_only) /* run the preprocessor as if it is stand-alone	*/
		preprocess();
	else
#endif NOPP
#endif DEBUG
	{
#ifndef	LINT
		init_code(destination && strcmp(destination, "-") != 0 ?
					destination : 0);
#endif	LINT

		/* compile the source text			*/
		C_program();

#ifdef PREPEND_SCOPES
		prepend_scopes();
#endif PREPEND_SCOPES
		end_code();

#ifdef	DEBUG
		if (options['u'])	{
			unstack_level();	/* unstack L_GLOBAL */
		}
		if (options['f'] || options['t'])
			dumpidftab("end of main", options['f'] ? 7 : 0);
#endif	DEBUG
	}
#ifndef	NOPP
	PopLex();
#endif	/* NOPP */
}

init()
{
	init_cst();	/* initialize variables of "cstoper.c"		*/
	reserve(tkidf);		/* mark the C reserved words as such	*/
	init_specials(special_ids);	/* mark special ids as such	*/

	schar_type = standard_type(CHAR, 0, 1, (arith)1);
	uchar_type = standard_type(CHAR, UNSIGNED, 1, (arith)1);

	short_type = standard_type(SHORT, 0, short_align, short_size);
	ushort_type = standard_type(SHORT, UNSIGNED, short_align, short_size);

	/*	Treat type `word' as `int', having its own size and
		alignment requirements.
		This type is transparent to the user.
	*/
	word_type = standard_type(INT, 0, word_align, word_size);
	uword_type = standard_type(INT, UNSIGNED, word_align, word_size);

	int_type = standard_type(INT, 0, int_align, int_size);
	uint_type = standard_type(INT, UNSIGNED, int_align, int_size);

	long_type = standard_type(LONG, 0, long_align, long_size);
	ulong_type = standard_type(LONG, UNSIGNED, long_align, long_size);

	float_type = standard_type(FLOAT, 0, float_align, float_size);
	double_type = standard_type(DOUBLE, 0, double_align, double_size);
	lngdbl_type = standard_type(LNGDBL, 0, lngdbl_align, lngdbl_size);
	void_type = standard_type(VOID, 0, 1, (arith)-1);
	label_type = standard_type(LABEL, 0, 0, (arith)0);
	error_type = standard_type(ERRONEOUS, 0, 1, (arith)1);

	/*	Pointer Arithmetic type: all arithmetics concerning
		pointers is supposed to be performed in the
		pointer arithmetic type which is equal to either
		int_type or long_type, depending on the pointer_size
	*/
	if ((int)pointer_size == (int)int_size)
		pa_type = int_type;
	else
	if ((int)pointer_size == (int)long_size)
		pa_type = long_type;
	else
		fatal("pointer size incompatible with any integral size");

	if ((int)int_size != (int)word_size)
		fatal("int_size and word_size are not equal");
	if ((int)short_size > (int)int_size || (int)int_size > (int)long_size)
		fatal("sizes of short/int/long decreasing");
	if ((int)float_size > (int)double_size || (int)double_size > (int)lngdbl_size)
		fatal("sizes of float/double/long double decreasing");

	/* Build a type for function returning int (3.3.2.2) */
	funint_type = construct_type(FUNCTION, int_type, 0, (arith)0, NO_PROTO);
	string_type = construct_type(POINTER, schar_type, 0, (arith)0, NO_PROTO);

	/* Define the standard type identifiers. */
	add_def(str2idf("char"), TYPEDEF, schar_type, L_UNIVERSAL);
	add_def(str2idf("int"), TYPEDEF, int_type, L_UNIVERSAL);
	add_def(str2idf("float"), TYPEDEF, float_type, L_UNIVERSAL);
	add_def(str2idf("double"), TYPEDEF, double_type, L_UNIVERSAL);
	add_def(str2idf("void"), TYPEDEF, void_type, L_UNIVERSAL);
	stack_level();
}

init_specials(si)
	register struct sp_id *si;
{
	while (si->si_identifier)	{
		struct idf *idf = str2idf(si->si_identifier);
		
		if (idf->id_special)
			fatal("maximum identifier length insufficient");
		idf->id_special = si->si_flag;
		si++;
	}
}

#ifdef DEBUG
#ifndef NOPP
preprocess()
{
	/*	preprocess() is the "stand-alone" preprocessor which
		consecutively calls the lexical analyzer LLlex() to get
		the tokens and prints them in a suitable way.
	*/
	static unsigned int lastlineno = 0;
	static char *lastfilenm = "";

	while (LLlex() !=  EOI)	{
		if (lastlineno != dot.tk_line)	{
			if (strcmp(lastfilenm, dot.tk_file) == 0)	{
				if (dot.tk_line - lastlineno <= 1)	{
					lastlineno++;
					print("\n");
				}
				else	{
					lastlineno = dot.tk_line;
					if (!options['P'])
						print("\n#line %ld \"%s\"\n",
							lastlineno,
							lastfilenm
						);
				}
			}
			else	{
				lastfilenm = dot.tk_file;
				lastlineno = dot.tk_line;
				if (!options['P'])
					print("\n#line %ld \"%s\"\n",
						lastlineno, lastfilenm);
			}
		}
		else
		if (strcmp(lastfilenm, dot.tk_file) != 0)	{
			lastfilenm = dot.tk_file;
			if (!options['P'])
				print("\n#line %ld \"%s\"\n",
					lastlineno, lastfilenm);
		}
		switch (DOT)	{
		case IDENTIFIER:
		case TYPE_IDENTIFIER:
			print("%s ", dot.tk_idf->id_text);
			break;
		case STRING:
		{
			char sbuf[1024];	/* a transient buffer */
			char *bts2str();

			print("\"%s\" ", bts2str(dot.tk_bts, dot.tk_len -
			1, sbuf));
			break;
		}
		case INTEGER:
			print("%ld ", dot.tk_ival);
			break;
		case FLOATING:
			print("%s ", dot.tk_fval);
			break;
		case EOI:
		case EOF:
			return;
		default:	/* very expensive...	*/
			print("%s ", symbol2str(DOT));
		}
	}
}
#endif NOPP
#endif DEBUG

No_Mem()				/* called by alloc package */
{
	fatal("out of memory");
}

C_failed()				/* called by EM_code module */
{
	fatal("write failed");
}

