// example11.cc - intersection of two explicitly loaded automata

// VATA headers
#include <vata/symbolic_finite_aut.hh>
#include <vata/parsing/timbuk_parser.hh>
#include <vata/serialization/timbuk_serializer.hh>
#include <vata/util/util.hh>

const char* aut1Str =
	"Ops           x:0 a:1 b:1\n"
	"Automaton     aut1\n"
	"States        p q\n"
	"Final States  q\n"
	"Transitions\n"
	"x    -> p\n"
	"a(p) -> q\n"
  "b(p) -> p\n";

const char* aut2Str =
	"Ops           x:0 a:1 b:1\n"
	"Automaton     aut2\n"
	"States        r s\n"
	"Final States  s\n"
	"Transitions\n"
	"x    -> r\n"
	"a(r) -> s\n"
  "b(r) -> r\n";

typedef VATA::SymbolicFiniteAut Automaton;

int main()
{
	// create parser for the Timbuk format
	std::unique_ptr<VATA::Parsing::AbstrParser> parser(
		new VATA::Parsing::TimbukParser());

	// create dictionaries for translating state names to internal state numbers
	Automaton::StateDict stateDict1, stateDict2;
  size_t state1(0), state2(0);
	Automaton::StringToStateTranslWeak stateTransl1(
    stateDict1,
    [&state1](const std::string &){return state1++;}
  );
	Automaton::StringToStateTranslWeak stateTransl2(
    stateDict2,
    [&state2](const std::string &){return state2++;}
  );

	// create dictionary for translating symbol names to internal symbol numbers
  Automaton::SymbolDict symbolDict;
  size_t symbol(0);
	Automaton::StringToSymbolTranslWeak symbolTransl(
    symbolDict,
    [&symbol](const std::string &){return symbol++;}
  );

	// create and load the automata
	Automaton aut1, aut2;
	aut1.LoadFromString(*parser, aut1Str, stateTransl1, symbolTransl, "explicit");
  aut2.LoadFromString(*parser, aut2Str, stateTransl2, symbolTransl, "explicit");

  // execute intersection operation
  Automaton::ProductTranslMap stateTranslMap;
  Automaton result = Automaton::Intersection(
    aut1,
    aut2,
    &stateTranslMap
  );
	Automaton::StateDict stateDictIsect =
  VATA::Util::CreateProductStringToStateMap(
    stateDict1,
    stateDict2,
		stateTranslMap
  );

	// create serializer for the Timbuk format
	std::unique_ptr<VATA::Serialization::AbstrSerializer> serializer(
		new VATA::Serialization::TimbukSerializer());

	// dump the result
	std::cout << result.DumpToString(
    *serializer,
    stateDictIsect,
    symbolDict,
    "explicit"
  );
}
