// example13.cc - simulation relation on symbolically loaded automaton

// VATA headers
#include <vata/symbolic_finite_aut.hh>
#include <vata/parsing/timbuk_parser.hh>
#include <vata/serialization/timbuk_serializer.hh>
#include <vata/util/util.hh>

const char* autStr =
	"Ops           000:0 001:1 010:1\n"
	"Automaton     aut\n"
	"States        000 001 010 011 100 101\n"
	"Final States  001 011\n"
	"Transitions\n"
	"000       -> 000\n"
	"001(000) -> 000\n"
	"010(000) -> 001\n"
	"001(001) -> 001\n"
	"010(001) -> 011\n"
	"001(010) -> 011\n"
	"010(010) -> 100\n"
	"001(011) -> 101\n"
	"010(011) -> 001\n"
	"001(100) -> 000\n"
	"010(100) -> 001\n"
	"001(101) -> 010\n"
	"010(101) -> 011\n";

typedef VATA::SymbolicFiniteAut Automaton;

int main()
{
	// create parser for the Timbuk format
	std::unique_ptr<VATA::Parsing::AbstrParser> parser(
		new VATA::Parsing::TimbukParser());

	// create and load the automata
	Automaton aut;
	aut.LoadFromString(*parser, autStr, "symbolic");

  // compute simulation
  Automaton::SymbolicFiniteAutBDD simResult = Automaton::ComputeSimulation(aut);
  std::cout << Automaton::DumpSimulation(simResult);
}