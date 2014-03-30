// example6.cc - explicit loading and dumping an automaton

// VATA headers
#include <vata/symbolic_finite_aut.hh>
#include <vata/parsing/timbuk_parser.hh>
#include <vata/serialization/timbuk_serializer.hh>

const char* autStr =
	"Ops           x:0 a:1 b:1 c:1 d:1\n"
	"Automaton     aut\n"
	"States        q0 q1 q2 q3 q4\n"
	"Final States  q1 q2 q4\n"
	"Transitions\n"
	"x     -> q0\n"
	"a(q0) -> q1\n"
	"c(q0) -> q2\n"
	"b(q1) -> q3\n"
	"d(q1) -> q4\n";

typedef VATA::SymbolicFiniteAut Automaton;

int main()
{
	// create the parser for the Timbuk format
	std::unique_ptr<VATA::Parsing::AbstrParser> parser(
		new VATA::Parsing::TimbukParser());

	// create the dictionary for translating state names to internal state numbers
	Automaton::StateDict stateDict;

	// create the dictionary for translating symbol names to internal symbol numbers
	Automaton::SymbolDict symbolDict;

	// create and load the automaton
	Automaton aut;
	aut.LoadFromString(*parser, autStr, stateDict, symbolDict, "explicit");

	// create the serializer for the Timbuk format
	std::unique_ptr<VATA::Serialization::AbstrSerializer> serializer(
		new VATA::Serialization::TimbukSerializer());

	// dump the automaton
	std::cout << aut.DumpToString(*serializer, stateDict, symbolDict, "explicit");
}
