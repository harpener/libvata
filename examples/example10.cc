// example9.cc - union of two symbolically loaded automata

// VATA headers
#include <vata/symbolic_finite_aut.hh>
#include <vata/parsing/timbuk_parser.hh>
#include <vata/serialization/timbuk_serializer.hh>
#include <vata/util/util.hh>

const char* aut1Str =
	"Ops           0000:0 00X1:1 11X0:1\n"
	"Automaton     aut\n"
	"States        00X0 10X0 00X1\n"
	"Final States  10X0 00X1\n"
	"Transitions\n"
	"0000       -> 00X0\n"
	"00X1(00X0) -> 10X0\n"
	"11X0(10X0) -> 00X1\n";

const char* aut2Str =
	"Ops           0000:0 00X1:1 11X0:1\n"
	"Automaton     aut\n"
	"States        10X1 10X0 11X1\n"
	"Final States  10X0 11X1\n"
	"Transitions\n"
	"0000       -> 10X1\n"
	"00X1(10X1) -> 10X0\n"
	"11X0(10X0) -> 11X1\n";

typedef VATA::SymbolicFiniteAut Automaton;

int main()
{
	// create parser for the Timbuk format
	std::unique_ptr<VATA::Parsing::AbstrParser> parser(
		new VATA::Parsing::TimbukParser());

	// create and load the automata
	Automaton aut1, aut2;
	aut1.LoadFromString(*parser, aut1Str, "symbolic");
  aut2.LoadFromString(*parser, aut2Str, "symbolic");

  // execute union operation
  Automaton result = Automaton::Union(
    aut1,
    aut2
  );

	// create serializer for the Timbuk format
	std::unique_ptr<VATA::Serialization::AbstrSerializer> serializer(
		new VATA::Serialization::TimbukSerializer());

	// dump the result
	std::cout << result.DumpToString(
    *serializer,
    "symbolic"
  );
}
