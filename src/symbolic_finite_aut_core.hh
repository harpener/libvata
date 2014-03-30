/*****************************************************************************
 *  VATA Finite Automata Library
 *
 *  Copyright (c) 2014 Jiri Chromecka <xchrom12@stud.fit.vutbr.cz>
 *
 *  Description:
 *    Header file for a symbolically represented finite automaton.
 *****************************************************************************/

#ifndef _VATA_SYMBOLIC_FINITE_AUT_CORE_HH_
#define _VATA_SYMBOLIC_FINITE_AUT_CORE_HH_

#include <vata/symbolic_finite_aut.hh>
#include <vata/util/triple.hh>
#include "symbolic_finite_aut_bdd.hh"

/// @brief  VATA library namespace
namespace VATA
{
  class SymbolicFiniteAutCore;
}

GCC_DIAG_OFF(effc++) // non-virtual destructors warnings suppress

/// @brief  Core class for a symbolically represented finite automaton
class VATA::SymbolicFiniteAutCore
{

GCC_DIAG_ON(effc++)

public: // public data types

  /// @brief  Structure describing an automaton
  using AutDescription = VATA::SymbolicFiniteAut::AutDescription;

  /// @brief  Symbolic assignment
  using SymbolicVarAsgn = VATA::SymbolicFiniteAut::SymbolicVarAsgn;

  /// @brief  List of symbolic assignments
  using AssignmentList = VATA::SymbolicFiniteAut::AssignmentList;

  /**
   * @brief  Bidirectional dictionary translating between string
             and internal representation of a state
   */
  using StateDict = VATA::SymbolicFiniteAut::StateDict;

  /**
   * @brief  Bidirectional dictionary translating between string
             and internal representation of a symbol
   */
  using SymbolDict = VATA::SymbolicFiniteAut::SymbolDict;

  /// @brief  Translator using StateDict with addition allowed
  using StringToStateTranslWeak =
  VATA::SymbolicFiniteAut::StringToStateTranslWeak;

  /// @brief  Translator using SymbolDict with addition allowed
  using StringToSymbolTranslWeak =
  VATA::SymbolicFiniteAut::StringToSymbolTranslWeak;

  /// @brief  Reverse translator using StateDict with addition forbidden
  using StateBackTranslStrict = VATA::SymbolicFiniteAut::StateBackTranslStrict;

  /// @brief  Reverse translator using SymbolDict with addition forbidden
  using SymbolBackTranslStrict =
  VATA::SymbolicFiniteAut::SymbolBackTranslStrict;

  /**
   * @brief  Maps internal representation of a state to another
             for purposes of some operations (e.g. union, intersection)
   */
	using StateToStateMap = VATA::SymbolicFiniteAut::StateToStateMap;

  /// @brief  Translator using StateToStateMap with addition allowed
	using StateToStateTranslWeak =
  VATA::SymbolicFiniteAut::StateToStateTranslWeak;

  /// @brief  Translator using StateToStateMap with addition forbidden
	using StateToStateTranslStrict =
   VATA::SymbolicFiniteAut::StateToStateTranslStrict;
  
private: // private data types

  /// @brief  Internal transition
  using TransitionType = VATA::Util::Triple<size_t, size_t, size_t>;

  /// @brief  Set of transitions
  using TransitionsSet = std::set<TransitionType>;

  /// @brief  Set of initial states
  using InitialStatesSet = std::set<size_t>;

  /// @brief  Set of final states
  using FinalStatesSet = std::set<size_t>;

  /// @brief  BDD
  using SymbolicFiniteAutBDD = VATA::SymbolicFiniteAutBDD;

  /// @brief  BDD of transitions
  using TransitionsBDD = SymbolicFiniteAutBDD;

  /// @brief  BDD of initial states
  using InitialStatesBDD = SymbolicFiniteAutBDD;

  /// @brief  BDD of final states
  using FinalStatesBDD = SymbolicFiniteAutBDD;

  /// @brief  TransitionsBDD pointer
  using TransitionsBDDPtr = std::unique_ptr<TransitionsBDD>;

  /// @brief  InitialStatesBDD pointer
  using InitialStatesBDDPtr = std::unique_ptr<InitialStatesBDD>;

  /// @brief  FinalStatesBDD pointer
  using FinalStatesBDDPtr = std::unique_ptr<FinalStatesBDD>;

private: // data members

  /// @brief  Number of variables in a symbolic representation of a state
  size_t stateVars_;

  /// @brief  Number of variables in a symbolic representation of a symbol
  size_t symbolVars_;

  /// @brief  TransitionsBDD instance
  TransitionsBDDPtr transitions_;

  /// @brief  InitialStatesBDD instance
  InitialStatesBDDPtr initialStates_;

  /// @brief  FinalStatesBDD instance
  FinalStatesBDDPtr finalStates_;

public: // instantiation

  /// @brief  Default constructor
  SymbolicFiniteAutCore();

  /**
   * @brief  Copy constructor
   *
   * @param[in]  aut  Automaton to copy
   */
  SymbolicFiniteAutCore(
    const SymbolicFiniteAutCore & core
  );

  /**
   * @brief  Move constructor
   *
   * @param[in]  aut  Automaton to move
   */
  SymbolicFiniteAutCore(
    SymbolicFiniteAutCore && core
  );

  /// @brief Default destructor.
  ~SymbolicFiniteAutCore();

  /**
   * @brief  Copy assignment operator
   *
   * @param[in]  rhs  Automaton to copy
   *
   * @return  Symbolic automaton
   */
  SymbolicFiniteAutCore & operator=(
    const SymbolicFiniteAutCore & rhs
  );

  /**
   * @brief  Move assignment operator
   *
   * @param[in]  rhs  Automaton to move
   *
   * @return  Symbolic automaton
   */
  SymbolicFiniteAutCore & operator=(
    SymbolicFiniteAutCore && rhs
  );

public: // public methods

  /**
   * @brief  Explicitly loads an automaton from an AutDescription structure
   *
   * @param[in]  desc          Structure describing an automaton
   * @param[in]  stateTransl   Translator between string and internal
                               representation of a state
   * @param[in]  symbolTransl  Translator between string and internal
                               representation of a symbol
   */
  template <typename StateTranslFunc, typename SymbolTranslFunc>
  void LoadFromAutDescExplicit(
    const AutDescription & desc,
    StateTranslFunc        stateTransl,
    SymbolTranslFunc       symbolTransl
  )
  {
    // store to internal representation before converting to symbolic
    TransitionsSet transitionsSet;
    InitialStatesSet initialStatesSet;
    FinalStatesSet finalStatesSet;

    // count different states and symbols
    std::set<size_t> states;
    std::set<size_t> symbols;

    for (auto transition : desc.transitions)
    { // transitions and initial states
      if (transition.first.empty())
      { // an init state is represented as a transition with no left-side state
        size_t state = stateTransl(transition.third);

        states.insert(state);
        initialStatesSet.insert(state);
      }

      else if (transition.first.size() > 1)
      { // finite automaton transitions are only unary
        throw std::runtime_error("Not a finite automaton.");
      }

      else
      { // common transition
        size_t lstate = stateTransl(transition.first.front());
        size_t symbol = symbolTransl(transition.second);
        size_t rstate = stateTransl(transition.third);

        states.insert(lstate);
        symbols.insert(symbol);
        states.insert(rstate);
        transitionsSet.insert(TransitionType(lstate, symbol, rstate));
      }
    }

    for (auto finalState : desc.finalStates)
    { // final states
      size_t state = stateTransl(finalState);

      states.insert(state);
      finalStatesSet.insert(state);
    }

    // determine number of variables to represent a symbolic state or symbol
    size_t size1 = states.size();
    size_t size2 = symbols.size();
    stateVars_ = ((size1 < 2) ? 1 : ceil(log2(size1)));
    symbolVars_ = ((size2 < 2) ? 1 : ceil(log2(size2)));

    assert(transitions_   != nullptr);
    assert(initialStates_ != nullptr);
    assert(finalStates_   != nullptr);

    for (auto transition : transitionsSet)
    { // transitions
      transitions_->AddAssignment(
        SymbolicFiniteAutBDD::MergeTransition(
          stateVars_,
          symbolVars_,
          transition.first,
          transition.second,
          transition.third
        )
      );
    }

    for (auto initialState : initialStatesSet)
    { // initial states
      initialStates_->AddAssignment(SymbolicVarAsgn(stateVars_, initialState));
    }

    for (auto finalState : finalStatesSet)
    { // final states
      finalStates_->AddAssignment(SymbolicVarAsgn(stateVars_, finalState));
    }
  }

  /**
   * @brief  Symbolically loads an automaton from an AutDescription structure
   *
   * @param[in]  desc  Structure describing an automaton
   */
  void LoadFromAutDescSymbolic(
    const AutDescription & desc
  );

  /**
   * @brief  Explicitly dumps an automaton to an AutDescription
   *
   * @param[in]  stateBackTransl   Reverse translator between string
                                   and internal representation of a state
   * @param[in]  symbolBackTransl  Reverse translator between string
                                   and internal representation of a symbol
   *
   * @return  Structure describing an automaton
   */
  template <typename StateBackTranslFunc, typename SymbolBackTranslFunc>
  AutDescription DumpToAutDescExplicit(
    StateBackTranslFunc    stateBackTransl,
    SymbolBackTranslFunc   symbolBackTransl
  ) const
  {
    assert(transitions_   != nullptr);
    assert(initialStates_ != nullptr);
    assert(finalStates_   != nullptr);

    AssignmentList tempTransitionList   = transitions_->GetAllAssignments();
    AssignmentList tempInitialStateList = initialStates_->GetAllAssignments();
    AssignmentList tempFinalStatesList  = finalStates_->GetAllAssignments();

    AssignmentList transitionList;
    AssignmentList initialStateList;
    AssignmentList finalStatesList;

    for (auto asgn : tempTransitionList)
    {
      AssignmentList concreteAsgnList = asgn.GetVectorOfConcreteSymbols();

      for (auto concreteAsgn : concreteAsgnList)
      {
        transitionList.push_back(concreteAsgn);
      }
    }

    for (auto asgn : tempInitialStateList)
    {
      AssignmentList concreteAsgnList = asgn.GetVectorOfConcreteSymbols();

      for (auto concreteAsgn : concreteAsgnList)
      {
        initialStateList.push_back(concreteAsgn);
      }      
    }

    for (auto asgn : tempFinalStatesList)
    {
      AssignmentList concreteAsgnList = asgn.GetVectorOfConcreteSymbols();
      
      for (auto concreteAsgn : concreteAsgnList)
      {
        finalStatesList.push_back(concreteAsgn);
      }
    }

    AutDescription desc;

    for (auto transition : transitionList)
    { // transitions
      size_t lstate;
      size_t symbol;
      size_t rstate;

      SymbolicFiniteAutBDD::SplitTransition(
        transition,
        stateVars_,
        symbolVars_,
        lstate,
        symbol,
        rstate
      );

      desc.transitions.insert(
        VATA::Util::Triple<
          std::vector<std::string>,
          std::string,
          std::string
        >(
          std::vector<std::string>{stateBackTransl(lstate)},
          symbolBackTransl(symbol),
          stateBackTransl(rstate)
        )
      );
    }

    for (auto initialState : initialStateList)
    { // initial states
      desc.transitions.insert(
        VATA::Util::Triple<
          std::vector<std::string>,
          std::string,
          std::string
        >(
          std::vector<std::string>(),
          "x",
          stateBackTransl(SymbolicFiniteAutBDD::FromSymbolic(initialState))
        )
      );
    }

    for (auto finalState : finalStatesList)
    { // final states
      desc.finalStates.insert(
        stateBackTransl(SymbolicFiniteAutBDD::FromSymbolic(finalState))
      );
    }

    return desc;
  }

  /**
   * @brief  Symbolically dumps an automaton to an AutDescription
   *
   * @return  Structure describing an automaton
   */
  AutDescription DumpToAutDescSymbolic() const;

  /**
   * @brief  Adding a symbolic transition
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
   * @brief  Adding a symbolic initial state
   *
   * @param[in]  state  Symbolic representation of a initial state
   */
  void AddInitialState(
    const SymbolicVarAsgn & state
  );

  /**
   * @brief  Adding a symbolic final state
   *
   * @param[in]  state  Symbolic representation of a final state
   */
  void AddFinalState(
    const SymbolicVarAsgn & state
  );

  /**
   * @brief  Union of two automata
   *
   * @note
   * Symbolically represented states must have the same number of variables?
   *
   * @param  lhs  First automaton for union
   * @param  rhs  Second automaton for union
   * @param  pTranslMapLhs  Translation map of states of first automaton
   * @param  pTranslMapRhs  Translation map of states of second automaton
   *
   * return  Union of given automata
   */
	static SymbolicFiniteAutCore Union(
  	const SymbolicFiniteAutCore & lhs,
  	const SymbolicFiniteAutCore & rhs,
  	StateToStateMap *             pTranslMapLhs = nullptr,
  	StateToStateMap *             pTranslMapRhs = nullptr
  );
};

#endif // _VATA_SYMBOLIC_FINITE_AUT_CORE_HH_