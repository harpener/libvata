/*****************************************************************************
 *  VATA Finite Automata Library
 *
 *  Copyright (c) 2014 Jiri Chromecka <xchrom12@stud.fit.vutbr.cz>
 *
 *  Description:
 *    Header file for a symbolically represented finite automaton (facade).
 *****************************************************************************/

#ifndef _VATA_SYMBOLIC_FINITE_AUT_HH_
#define _VATA_SYMBOLIC_FINITE_AUT_HH_

#include <vata/vata.hh>
#include <vata/parsing/abstr_parser.hh>
#include <vata/serialization/abstr_serializer.hh>
#include <vata/util/aut_description.hh>
#include <vata/util/two_way_dict.hh>
#include <vata/util/transl_weak.hh>
#include <vata/util/transl_strict.hh>
#include <vata/util/binary_relation.hh>

#include "../../src/symbolic_finite_aut_bdd.hh"

/// @brief  VATA library namespace
namespace VATA
{
  class SymbolicFiniteAut;
  template <typename> class SymbolicLoadableAut;
  class SymbolicFiniteAutCore;
  class SymbolicFiniteAutBDD;
}

GCC_DIAG_OFF(effc++) // non-virtual destructors warnings suppress

/// @brief  Facade class for a symbolically represented finite automaton
class VATA::SymbolicFiniteAut
{

GCC_DIAG_ON(effc++)

public: // data types

  /// @brief  Structure describing an automaton
  using AutDescription = VATA::Util::AutDescription;

  /// @brief  BDD
  using SymbolicFiniteAutBDD = VATA::SymbolicFiniteAutBDD;

  /// @brief  Symbolic assignment
  using SymbolicVarAsgn = VATA::SymbolicFiniteAutBDD::SymbolicVarAsgn;

  /// @brief  List of symbolic assignments
  using AssignmentList = VATA::SymbolicFiniteAutBDD::AssignmentList;

  /**
   * @brief  Bidirectional dictionary translating between string
             and internal representation of a state
   */
  using StateDict = VATA::Util::TwoWayDict<std::string, size_t>;

  /**
   * @brief  Bidirectional dictionary translating between string
             and internal representation of a symbol
   */
  using SymbolDict = VATA::Util::TwoWayDict<std::string, size_t>;

  /// @brief  Translator using StateDict with addition allowed
  using StringToStateTranslWeak = VATA::Util::TranslatorWeak<StateDict>;

  /// @brief  Translator using SymbolDict with addition allowed
  using StringToSymbolTranslWeak = VATA::Util::TranslatorWeak<SymbolDict>;

  /// @brief  Reverse translator using StateDict with addition forbidden
  using StateBackTranslStrict = VATA::Util::TranslatorStrict<
                                  StateDict::MapBwdType
                                >;

  /// @brief  Reverse translator using SymbolDict with addition forbidden
  using SymbolBackTranslStrict = VATA::Util::TranslatorStrict<
                                   SymbolDict::MapBwdType
                                 >;

  /**
   * @brief  Maps internal representation of a state to another
             for purposes of some operations (e.g. union)
   */
	using StateToStateMap = std::unordered_map<size_t, size_t>;

  /// @brief  Translator using StateToStateMap with addition allowed
	using StateToStateTranslWeak = VATA::Util::TranslatorWeak<StateToStateMap>;

  /// @brief  Translator using StateToStateMap with addition forbidden
	using StateToStateTranslStrict =
  VATA::Util::TranslatorStrict<StateToStateMap>;

  /// @brief  A pair of states
	using StatePair = std::pair<size_t, size_t>;

  /**
   * @brief  Maps internal representations of a pair of states to a single state
             for purposes of some operations (e.g. intersection)
   */
	using ProductTranslMap =
		std::unordered_map<StatePair, size_t, boost::hash<StatePair>>;

  /// @brief  Matrix of states for simulation purposes
  using StateBinaryRelation = VATA::Util::BinaryRelation;

private: // data types

  /// @brief  Loadable symbolically represented finite automaton core
  using Core = VATA::SymbolicLoadableAut<VATA::SymbolicFiniteAutCore>;

  /// @brief  Core pointer
  using CorePtr = std::unique_ptr<Core>;

private: // data members

  /// @brief  Core instance
  CorePtr core_;

public: // public instantiation

  /// @brief  Default constructor
  SymbolicFiniteAut();

  /**
   * @brief  Copy constructor
   *
   * @param[in]  aut  Automaton to copy
   */
  SymbolicFiniteAut(
    const SymbolicFiniteAut & aut
  );

  /**
   * @brief  Move constructor
   *
   * @param[in]  aut  Automaton to move
   */
  SymbolicFiniteAut(
    SymbolicFiniteAut && aut
  );

  /// @brief  Default destructor
  ~SymbolicFiniteAut();

  /**
   * @brief  Copy assignment operator
   *
   * @param[in]  rhs  Automaton to copy
   *
   * @return  Symbolic automaton
   */
  SymbolicFiniteAut & operator=(
    const SymbolicFiniteAut & rhs
  );

  /**
   * @brief  Move assignment operator
   *
   * @param[in]  rhs  Automaton to move
   *
   * @return  Symbolic automaton
   */
  SymbolicFiniteAut & operator=(
    SymbolicFiniteAut && rhs
  );

private: // private instantiation

  /**
   * @brief  Create constructor
   *
   * @param[in]  core  Automaton core for new automaton
   */
  explicit SymbolicFiniteAut(
    Core && core
  );

public: // public methods

  /**
   * @brief  Loads an automaton from a string
   *
   * @param[in]  parser  Tool parsing a string describing an automaton
   * @param[in]  str     String describing an automaton
   * @param[in]  params  Explicit or symbolic method of loading
   */
  void LoadFromString(
    VATA::Parsing::AbstrParser & parser,
    const std::string &          str,
    const std::string &          params = "explicit"
  );

  /**
   * @brief  Loads an automaton from a string
   *
   * @param[in]  parser      Tool parsing a string describing an automaton
   * @param[in]  str         String describing an automaton
   * @param[in]  stateDict   Bidirectional dictionary translating between
                             string and internal representation of a state
   * @param[in]  symbolDict  Bidirectional dictionary translating between
                             string and internal representation of a symbol
   * @param[in]  params      Explicit or symbolic method of loading
   */
  void LoadFromString(
    VATA::Parsing::AbstrParser & parser,
    const std::string &          str,
    StateDict &                  stateDict,
    SymbolDict &                 symbolDict,
    const std::string &          params = "explicit"
  );

  /**
   * @brief  Loads an automaton from a string
   *
   * @param[in]  parser        Tool parsing a string describing an automaton
   * @param[in]  str           String describing an automaton
   * @param[in]  stateTransl   Translator between string and internal
                               representation of a state
   * @param[in]  symbolTransl  Translator between string and internal
                               representation of a symbol
   * @param[in]  params        Explicit or symbolic method of loading
   */
  void LoadFromString(
    VATA::Parsing::AbstrParser & parser,
    const std::string &          str,
    StringToStateTranslWeak &    stateTransl,
    StringToSymbolTranslWeak &   symbolTransl,
    const std::string &          params = "explicit"
  );

  /**
   * @brief  Loads an automaton from an AutDescription structure
   *
   * @param[in]  desc    Structure describing an automaton
   * @param[in]  params  Explicit or symbolic method of loading
   */
  void LoadFromAutDesc(
    const AutDescription & desc,
    const std::string &    params = "explicit"
  );

  /**
   * @brief  Loads an automaton from an AutDescription structure
   *
   * @param[in]  desc        Structure describing an automaton
   * @param[in]  stateDict   Bidirectional dictionary translating between
                             string and internal representation of a state
   * @param[in]  symbolDict  Bidirectional dictionary translating between
                             string and internal representation of a symbol
   * @param[in]  params      Explicit or symbolic method of loading
   */
  void LoadFromAutDesc(
    const AutDescription & desc,
    StateDict &            stateDict,
    SymbolDict &           symbolDict,
    const std::string &    params = "explicit"
  );

  /**
   * @brief  Loads an automaton from an AutDescription structure
   *
   * @param[in]  desc          Structure describing an automaton
   * @param[in]  stateTransl   Translator between string and internal
                               representation of a state
   * @param[in]  symbolTransl  Translator between string and internal
                               representation of a symbol
   * @param[in]  params        Explicit or symbolic method of loading
   */
  void LoadFromAutDesc(
    const AutDescription &     desc,
    StringToStateTranslWeak &  stateTransl,
    StringToSymbolTranslWeak & symbolTransl,
    const std::string &        params = "explicit"
  );

  /**
   * @brief  Dumps an automaton to a string
   *
   * @param[in]  params  Explicit or symbolic method of dumping
   *
   * @return  String describing an automaton
   */
  std::string DumpToString(
    const std::string & params = "explicit"
  ) const;

  /**
   * @brief  Dumps an automaton to a string
   *
   * @param[in]  serializer  A tool serializing an AutDescription structure
   * @param[in]  params      Explicit or symbolic method of dumping
   *
   * @return  String describing an automaton
   */
  std::string DumpToString(
    VATA::Serialization::AbstrSerializer & serializer,
    const std::string &                    params = "explicit"
  ) const;

  /**
   * @brief  Dumps an automaton to a string
   *
   * @param[in]  serializer  Tool serializing a structure describing
                             an automaton
   * @param[in]  stateDict   Bidirectional dictionary translating between
                             string and internal representation of a state
   * @param[in]  symbolDict  Bidirectional dictionary translating between
                             string and internal representation of a symbol
   * @param[in]  params      Explicit or symbolic method of dumping
   *
   * @return  String describing an automaton
   */
  std::string DumpToString(
    VATA::Serialization::AbstrSerializer & serializer,
    const StateDict &                      stateDict,
    const SymbolDict &                     symbolDict,
    const std::string &                    params = "explicit"
  ) const;

  /**
   * @brief  Dumps an automaton to a string
   *
   * @param[in]  serializer        Tool serializing a structure describing
                                   an automaton
   * @param[in]  stateBackTransl   Reverse translator between string
                                   and internal representation of a state
   * @param[in]  symbolBackTransl  Reverse translator translating between string
                                   and internal representation of a symbol
   * @param[in]  params            Explicit or symbolic method of dumping
   *
   * @return  String describing an automaton
   */
  std::string DumpToString(
    VATA::Serialization::AbstrSerializer & serializer,
    StateBackTranslStrict &                stateBackTransl,
    SymbolBackTranslStrict &               symbolBackTransl,
    const std::string &                    params = "explicit"
  ) const;

  /**
   * @brief  Dumps an automaton to an AutDescription structure
   *
   * @param[in]  params  Explicit or symbolic method of dumping
   *
   * @return  Structure describing an automaton
   */
  AutDescription DumpToAutDesc(
    const std::string & params = "explicit"
  ) const;

  /**
   * @brief  Dumps an automaton to an AutDescription structure
   *
   * @param[in]  stateDict   Bidirectional dictionary translating between
                             string and internal representation of a state
   * @param[in]  symbolDict  Bidirectional dictionary translating between
                             string and internal representation of a symbol
   * @param[in]  params      Explicit or symbolic method of dumping
   *
   * @return  Structure describing an automaton
   */
  AutDescription DumpToAutDesc(
    const StateDict &   stateDict,
    const SymbolDict &  symbolDict,
    const std::string & params = "explicit"
  ) const;

  /**
   * @brief  Dumps an automaton to an AutDescription structure
   *
   * @param[in]  stateBackTransl   Reverse translator between string
                                   and internal representation of a state
   * @param[in]  symbolBackTransl  Reverse translator between string
                                   and internal representation of a symbol
   * @param[in]  params            Explicit or symbolic method of dumping
   *
   * @return  Structure describing an automaton
   */
  AutDescription DumpToAutDesc(
    StateBackTranslStrict &  stateBackTransl,
    SymbolBackTranslStrict & symbolBackTransl,
    const std::string &      params = "explicit"
  ) const;

  /**
   * @brief  Adds a transition
   *
   * @param[in]  lstate  Symbolic (string) representation of a left-side state
   * @param[in]  symbol  Symbolic (string) representation of a symbol
   * @param[in]  rstate  Symbolic (string) representation of a right-side state
   */
  void AddTransition(
    const std::string & lstate,
    const std::string & symbol,
    const std::string & rstate
  );

  /**
   * @brief  Adds a transition
   *
   * @param[in]  lstate  Symbolic representation of a left-side state
   * @param[in]  symbol  Symbolic representation of a symbol
   * @param[in]  rstate  Symbolic representation of a right-side state
   */
  void AddTransition(
    const SymbolicVarAsgn & lstate,
    const SymbolicVarAsgn & symbol,
    const SymbolicVarAsgn & rstate
  );

  /**
   * @brief  Adds an initial state
   *
   * @param[in]  state  Symbolic (string) representation of an initial state
   */
  void AddInitialState(
    const std::string & state
  );

  /**
   * @brief  Adds an initial state
   *
   * @param[in]  state  Symbolic representation of an initial state
   */
  void AddInitialState(
    const SymbolicVarAsgn & state
  );

  /**
   * @brief  Adds a final state
   *
   * @param[in]  state  Symbolic (string) representation of a final state
   */
  void AddFinalState(
    const std::string & state
  );

  /**
   * @brief  Adds a final state
   *
   * @param[in]  state  Symbolic representation of a final state
   */
  void AddFinalState(
    const SymbolicVarAsgn & state
  );

  /**
   * @brief  Union of two automata
   *
   * @param  lhs  First automaton for union
   * @param  rhs  Second automaton for union
   *
   * return  Union of given automata
   */
	static SymbolicFiniteAut Union(
		const SymbolicFiniteAut & lhs,
		const SymbolicFiniteAut & rhs
  );

  /**
   * @brief  Generates state translation dictionaries after union
   *
   * @param  lhs       State translation dictionary of first automaton
   * @param  rhs       State translation dictionary of second automaton
   * @param  lhsUnion  @p lhs translation map after union
   * @param  rhsUnion  @p rhs translation map after union
   */
  static void GenUnionTransl(
    const StateDict & lhs,
    const StateDict & rhs,
    StateToStateMap * lhsUnion,
    StateToStateMap * rhsUnion
  );

  /**
   * @brief  Intersection of two automata
   *
   * @note
   * Symbolically represented states must have the same number of variables?
   *
   * @param  lhs  First automaton for intersection
   * @param  rhs  Second automaton for intersection
   *
   * return  Intersection of given automata
   */
	static SymbolicFiniteAut Intersection(
  	const SymbolicFiniteAut & lhs,
  	const SymbolicFiniteAut & rhs
  );

  /**
   * @brief  Generates state translation dictionary after intersection
   *
   * @param  lhs             State translation dictionary of first automaton
   * @param  rhs             State translation dictionary of second automaton
   * @param  stateDictIsect  @p stateDict translation map after intersection
   */
  static void GenIsectTransl(
    const StateDict & lhs,
    const StateDict & rhs,
    ProductTranslMap * stateDictIsect
  );

  /**
   * @brief  Compute a simulation relation
   *
   * @param  aut  Given automaton
   *
   * return  Simulation relation BDD
   */
	static SymbolicFiniteAutBDD ComputeSimulation(
    const SymbolicFiniteAut & aut
  );

  /**
   * @brief  Dumps simulation relation into a string
   *
   * @param  sim        Given simulation relation
   * @param  stateDict  State translation dictionary
   *
   * return  String describing simulation relation
   */
  static std::string DumpSimulation(
    const SymbolicFiniteAutBDD & sim,
    StateDict *                  stateDict = nullptr
  );
};

#endif // _VATA_SYMBOLIC_FINITE_AUT_HH_