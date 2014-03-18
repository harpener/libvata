// example3.cc - loading and dumping an automaton

// VATA headers
#include <vata/symbolic_finite_aut.hh>
#include <vata/parsing/timbuk_parser.hh>
#include <vata/serialization/timbuk_serializer.hh>

const char* autStr =
	"Ops           000X:0 100X:1 010X:1 110X:1 001X:1\n"
	"Automaton     aut\n"
	"States        000X 100X 010X 110X 001X\n"
	"Final States  100X 010X 001X\n"
	"Transitions\n"
	"000X       -> 000X\n"
	"100X(000X) -> 100X\n"
	"010X(000X) -> 010X\n"
	"110X(100X) -> 110X\n"
	"001X(100X) -> 001X\n";

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
