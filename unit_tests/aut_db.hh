/*****************************************************************************
 *  Vojnar's Army Tree Automata Library
 *
 *  Copyright (c) 2011  Ondra Lengal <ilengal@fit.vutbr.cz>
 *
 *  Description:
 *    The database of automata.
 *
 *****************************************************************************/

// VATA headers
#include <vata/vata.hh>


const char* AUT_TIMBUK_A1 =
	"Ops \n"
	"Automaton A1\n"
	"States\n"
	"Final States\n"
	"Transitions\n"
	;

const char* AUT_TIMBUK_A2 =
	"Ops a:0 b:1 c:2\n"
	"Automaton A2\n"
	"States p\n"
	"Final States p\n"
	"Transitions\n"
	"a -> p\n"
	;

const char* AUT_TIMBUK_A3 =
	"Ops a:0 b:1 c:2\n"
	"Automaton A3\n"
	"States p q\n"
	"Final States q\n"
	"Transitions\n"
	"a -> p\n"
	"b(p) -> q\n"
	"c(p, q) -> q\n"
	;

const char* AUT_TIMBUK_A4 =
	"Ops\n"
	"Automaton anonymous\n"
	"States p q\n"
	"Final States q\n"
	"Transitions\n"
	"a -> p\n"
	"a(p) -> q\n"
	"a(p, q) -> r\n"
	;

const char* AUT_TIMBUK_A5 =
	"Ops\n"
	"Automaton anonymous\n"
	"States\n"
	"Final States q\n"
	"Transitions\n"
	"a -> p\n"
	"a(p) -> q\n"
	"a(p, q) -> r\n"
	;

const char* AUT_TIMBUK_UNION_1 =
	"Ops\n"
	"Automaton anonymous\n"
	"States\n"
	"Final States r\n"
	"Transitions\n"
	"a -> p\n"
	"b(p) -> q\n"
	"c(p, q) -> r\n"
	;

const char* AUT_TIMBUK_UNION_2 =
	"Ops\n"
	"Automaton anonymous\n"
	"States\n"
	"Final States v\n"
	"Transitions\n"
	"a -> t\n"
	"b(t) -> u\n"
	"c(u, u) -> v\n"
	"c(v, v) -> v\n"
	;

const char* AUT_TIMBUK_UNION_3 =
	"Ops\n"
	"Automaton anonymous\n"
	"States\n"
	"Final States r\n"
	"Transitions\n"
	"a -> s\n"
	"b(s) -> q\n"
	"c(q, q) -> r\n"
	"d(r, r) -> r\n"
	;

const char* AUT_TIMBUK_UNION_12_RESULT =
	"Ops\n"
	"Automaton anonymous\n"
	"States\n"
	"Final States v r\n"
	"Transitions\n"
	"a -> p\n"
	"a -> t\n"
	"b(p) -> q\n"
	"b(t) -> u\n"
	"c(p, q) -> r\n"
	"c(u, u) -> v\n"
	"c(v, v) -> v\n"
	;

const char* AUT_TIMBUK_UNION_13_RESULT =
	"Ops\n"
	"Automaton anonymous\n"
	"States\n"
	"Final States 4 1\n"
	"Transitions\n"
	"a -> 2\n"
	"a -> 6\n"
	"b(2) -> 3\n"
	"b(6) -> 5\n"
	"c(2, 3) -> 1\n"
	"c(5, 5) -> 4\n"
	"d(4, 4) -> 4\n"
	;

const char* AUT_TIMBUK_ISECT_1 =
	"Ops\n"
	"Automaton anonymous\n"
	"States\n"
	"Final States r\n"
	"Transitions\n"
	"a -> p\n"
	"b(p) -> q\n"
	"b(q) -> q\n"
	"c(p, q) -> r\n"
	;

const char* AUT_TIMBUK_ISECT_2 =
	"Ops\n"
	"Automaton anonymous\n"
	"States\n"
	"Final States h\n"
	"Transitions\n"
	"a -> g\n"
	"z -> g\n"
	"b(g) -> f\n"
	"c(g, f) -> h\n"
	"c(g, h) -> h\n"
	;

const char* AUT_TIMBUK_ISECT_12_RESULT =
	"Ops\n"
	"Automaton anonymous\n"
	"States\n"
	"Final States 1\n"
	"Transitions\n"
	"a -> 2\n"
	"b(2) -> 4\n"
	"b(5) -> 4\n"
	"c(2, 4) -> 1\n"
	"c(2, 3) -> 1\n"
	;

const char* AUT_TIMBUK_UNREACHABLE_1 =
	"Ops\n"
	"Automaton anonymous\n"
	"States\n"
	"Final States q\n"
	"Transitions\n"
	"a -> p\n"
	"b(p) -> r\n"
	"b(p) -> s\n"
	"c(p, p) -> q\n"
	"c(p, r) -> q\n"
	"c(p, s) -> s\n"
	;

const char* AUT_TIMBUK_UNREACHABLE_1_RESULT =
	"Ops\n"
	"Automaton anonymous\n"
	"States\n"
	"Final States 5\n"
	"Transitions\n"
	"a -> 6\n"
	"b(6) -> 7\n"
	"c(6, 6) -> 5\n"
	"c(6, 7) -> 5\n"
	;

const char* AUT_TIMBUK_A53 =
	"Ops\n"
	"Automaton anonymous\n"
	"States\n"
	"Final States q47 q5\n"
	"\n"
	"Transitions\n"
	"yblack(q1,q19) -> q22\n"
	"yblack(q9,q9) -> q18\n"
	"yblack(q11,q9) -> q18\n"
	"yblack(q14,q14) -> q18\n"
	"yblack(q19,q19) -> q22\n"
	"yblack(q24,q24) -> q22\n"
	"xxpxppyNULL(q10,q14) -> q16\n"
	"xxppyNULL(q23,q24) -> q25\n"
	"xppred(q21,q22) -> q0\n"
	"xppred(q22,q19) -> q3\n"
	"xpblack(q0,q1) -> q20\n"
	"xpblack(q0,q19) -> q20\n"
	"xpblack(q7,q1) -> q6\n"
	"xpblack(q7,q3) -> q20\n"
	"xpblack(q7,q19) -> q6\n"
	"xpblack(q11,q9) -> q17\n"
	"xNULL(q12,q24) -> q25\n"
	"xNULL(q15,q14) -> q16\n"
	"rootxppblack(q17,q18) -> q15\n"
	"rootxpblack(q0,q1) -> q12\n"
	"rootxpblack(q0,q19) -> q12\n"
	"rootxpblack(q7,q1) -> q23\n"
	"rootxpblack(q7,q3) -> q12\n"
	"rootxpblack(q7,q19) -> q23\n"
	"rootblack(q2,q1) -> q23\n"
	"rootblack(q2,q3) -> q12\n"
	"rootblack(q2,q19) -> q23\n"
	"rootblack(q6,q1) -> q23\n"
	"rootblack(q6,q19) -> q23\n"
	"rootblack(q8,q9) -> q10\n"
	"rootblack(q8,q11) -> q10\n"
	"rootblack(q9,q9) -> q10\n"
	"rootblack(q11,q9) -> q10\n"
	"rootblack(q11,q11) -> q10\n"
	"rootblack(q20,q1) -> q12\n"
	"rootblack(q20,q19) -> q12\n"
	"red(q1,q19) -> q1\n"
	"red(q2,q19) -> q2\n"
	"red(q6,q19) -> q2\n"
	"red(q6,q19) -> q6\n"
	"red(q8,q9) -> q8\n"
	"red(q9,q9) -> q11\n"
	"red(q11,q9) -> q11\n"
	"red(q19,q19) -> q1\n"
	"red(q19,q19) -> q7\n"
	"red(q20,q19) -> q20\n"
	"normal(q4,q24) -> q5\n"
	"normal(q13,q14) -> q5\n"
	"bot2(q14,q14) -> q14\n"
	"bot2(q14,q14) -> q24\n"
	"bot0 -> q14\n"
	"black(q1,q19) -> q19\n"
	"black(q1,q19) -> q21\n"
	"black(q2,q3) -> q20\n"
	"black(q2,q19) -> q6\n"
	"black(q6,q1) -> q6\n"
	"black(q6,q19) -> q6\n"
	"black(q8,q9) -> q8\n"
	"black(q8,q11) -> q8\n"
	"black(q9,q9) -> q9\n"
	"black(q11,q9) -> q8\n"
	"black(q11,q9) -> q9\n"
	"black(q11,q11) -> q8\n"
	"black(q14,q14) -> q9\n"
	"black(q19,q19) -> q19\n"
	"black(q20,q1) -> q20\n"
	"black(q20,q19) -> q20\n"
	"black(q24,q24) -> q19\n"
	"UNDEF(q16,q14) -> q13\n"
	"UNDEF(q25,q24) -> q4\n"
	"yblack(q27,q35) -> q39\n"
	"yblack(q31,q32) -> q34\n"
	"yblack(q32,q32) -> q34\n"
	"yblack(q35,q35) -> q39\n"
	"yblack(q41,q41) -> q39\n"
	"yblack(q45,q45) -> q39\n"
	"yblack(q48,q45) -> q39\n"
	"yblack(q50,q50) -> q34\n"
	"xxppyNULL(q40,q41) -> q42\n"
	"xppred(q37,q39) -> q26\n"
	"xppred(q39,q35) -> q43\n"
	"xppred(q39,q45) -> q43\n"
	"xppred(q46,q39) -> q26\n"
	"xpblack(q26,q27) -> q36\n"
	"xpblack(q26,q35) -> q36\n"
	"xpblack(q26,q45) -> q36\n"
	"xpblack(q26,q48) -> q36\n"
	"xpblack(q31,q32) -> q33\n"
	"xpblack(q48,q43) -> q36\n"
	"xpblack(q48,q45) -> q51\n"
	"xpblack(q52,q27) -> q51\n"
	"xpblack(q52,q35) -> q51\n"
	"xpblack(q52,q43) -> q36\n"
	"xNULL(q29,q50) -> q28\n"
	"xNULL(q30,q41) -> q42\n"
	"rootxppblack(q33,q34) -> q29\n"
	"rootxpblack(q26,q27) -> q30\n"
	"rootxpblack(q26,q35) -> q30\n"
	"rootxpblack(q26,q45) -> q30\n"
	"rootxpblack(q26,q48) -> q30\n"
	"rootxpblack(q48,q43) -> q30\n"
	"rootxpblack(q48,q45) -> q40\n"
	"rootxpblack(q48,q48) -> q40\n"
	"rootxpblack(q52,q27) -> q40\n"
	"rootxpblack(q52,q35) -> q40\n"
	"rootxpblack(q52,q43) -> q30\n"
	"rootblack(q36,q27) -> q30\n"
	"rootblack(q36,q35) -> q30\n"
	"rootblack(q36,q45) -> q30\n"
	"rootblack(q36,q48) -> q30\n"
	"rootblack(q38,q27) -> q40\n"
	"rootblack(q38,q35) -> q40\n"
	"rootblack(q38,q43) -> q30\n"
	"rootblack(q38,q45) -> q40\n"
	"rootblack(q38,q48) -> q40\n"
	"rootblack(q51,q27) -> q40\n"
	"rootblack(q51,q35) -> q40\n"
	"rootblack(q51,q45) -> q40\n"
	"red(q27,q35) -> q27\n"
	"red(q31,q32) -> q31\n"
	"red(q32,q32) -> q31\n"
	"red(q35,q35) -> q27\n"
	"red(q35,q35) -> q52\n"
	"red(q36,q35) -> q36\n"
	"red(q36,q45) -> q36\n"
	"red(q38,q35) -> q38\n"
	"red(q38,q45) -> q38\n"
	"red(q45,q45) -> q48\n"
	"red(q51,q35) -> q38\n"
	"red(q51,q35) -> q51\n"
	"red(q51,q45) -> q38\n"
	"normal(q44,q41) -> q47\n"
	"normal(q49,q50) -> q47\n"
	"bot2(q50,q50) -> q41\n"
	"bot2(q50,q50) -> q50\n"
	"bot0 -> q50\n"
	"black(q27,q35) -> q35\n"
	"black(q27,q35) -> q37\n"
	"black(q31,q32) -> q32\n"
	"black(q32,q32) -> q32\n"
	"black(q35,q35) -> q35\n"
	"black(q36,q27) -> q36\n"
	"black(q36,q35) -> q36\n"
	"black(q36,q45) -> q36\n"
	"black(q36,q48) -> q36\n"
	"black(q38,q35) -> q51\n"
	"black(q38,q43) -> q36\n"
	"black(q38,q45) -> q51\n"
	"black(q41,q41) -> q35\n"
	"black(q41,q41) -> q45\n"
	"black(q45,q45) -> q45\n"
	"black(q48,q45) -> q45\n"
	"black(q48,q45) -> q46\n"
	"black(q50,q50) -> q32\n"
	"black(q51,q27) -> q51\n"
	"black(q51,q35) -> q51\n"
	"black(q51,q45) -> q51\n"
	"UNDEF(q28,q50) -> q49\n"
	"UNDEF(q42,q41) -> q44\n"
	;

const char* TIMBUK_AUTOMATA[] =
{
	AUT_TIMBUK_A1,
	AUT_TIMBUK_A2,
	AUT_TIMBUK_A3,
	AUT_TIMBUK_A4,
	AUT_TIMBUK_UNION_1,
	AUT_TIMBUK_UNION_2,
	AUT_TIMBUK_UNION_3,
	AUT_TIMBUK_UNION_12_RESULT,
	AUT_TIMBUK_UNION_13_RESULT,
	AUT_TIMBUK_A53
};
const size_t TIMBUK_AUTOMATA_SIZE = (sizeof(TIMBUK_AUTOMATA) /
	sizeof(const char*));

