// example3.cc - loading and dumping an automaton

// VATA headers
#include <vata/symbolic_finite_aut.hh>
#include <vata/parsing/timbuk_parser.hh>
#include <vata/serialization/timbuk_serializer.hh>

const char* autStr =
	"Ops           000X1:0 100X1:1 010X1:1 110X1:1 001X1:1\n"
	"Automaton     aut\n"
	"States        000X0 100X0 010X0 110X0 001X0\n"
	"Final States  100X0 010X0 001X0\n"
	"Transitions\n"
	"000X0        -> 000X0\n"
	"100X0(000X1) -> 100X0\n"
	"010X0(000X1) -> 010X0\n"
	"110X0(100X1) -> 110X0\n"
	"001X0(100X1) -> 001X0\n";

typedef VATA::SymbolicFiniteAut Automaton;

int main()
{
	// create the parser for the Timbuk format
	std::unique_ptr<VATA::Parsing::AbstrParser> parser(
		new VATA::Parsing::TimbukParser());

	// create and load the automaton
	Automaton aut;
	aut.LoadFromString(*parser, autStr, "symbolic");

	// create the serializer for the Timbuk format
	std::unique_ptr<VATA::Serialization::AbstrSerializer> serializer(
		new VATA::Serialization::TimbukSerializer());

	// dump the automaton
	std::cout << aut.DumpToString(*serializer, "symbolic");
}
