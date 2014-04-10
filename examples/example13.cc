// example13.cc - simulation relation on automaton

// VATA headers
#include <vata/symbolic_finite_aut.hh>
#include <vata/parsing/timbuk_parser.hh>
#include <vata/serialization/timbuk_serializer.hh>
#include <vata/util/util.hh>

const char* autStr =
	"Ops           x:0 a:1 b:1\n"
	"Automaton     aut\n"
	"States        p q r s t u\n"
	"Final States  q s\n"
	"Transitions\n"
	"x    -> p\n"
	"a(p) -> q\n"
	"b(p) -> r\n"
	"a(q) -> t\n"
	"b(q) -> s\n"
	"a(r) -> s\n"
	"b(r) -> p\n"
	"a(s) -> u\n"
	"b(s) -> q\n"
	"a(t) -> p\n"
	"b(t) -> q\n"
	"a(u) -> r\n"
	"b(u) -> s\n";

typedef VATA::SymbolicFiniteAut Automaton;

int main()
{
	// create parser for the Timbuk format
	std::unique_ptr<VATA::Parsing::AbstrParser> parser(
		new VATA::Parsing::TimbukParser());

	// create dictionary for translating state names to internal state numbers
	Automaton::StateDict stateDict;

	// create dictionary for translating symbol names to internal symbol numbers
  Automaton::SymbolDict symbolDict;

	// create and load the automata
	Automaton aut;
	aut.LoadFromString(*parser, autStr, stateDict, symbolDict, "explicit");

  // compute simulation
  std::cout << Automaton::ComputeSimulation(aut);
}