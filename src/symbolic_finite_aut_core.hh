/*****************************************************************************
 *  VATA Finite Automata Library
 *
 *  Copyright (c) 2014 Jiri Chromecka <xchrom12@stud.fit.vutbr.cz>
 *
 *  Description:
 *  Header file for a symbolically represented FA.
 *****************************************************************************/

#ifndef _VATA_SYMBOLIC_FINITE_AUT_CORE_HH_
#define _VATA_SYMBOLIC_FINITE_AUT_CORE_HH_

#include <vata/symbolic_finite_aut.hh>
#include <vata/util/triple.hh>
#include "symbolic_finite_aut_bdd.hh"

/// @brief VATA library namespace.
namespace VATA
{
  class SymbolicFiniteAutCore;
}

GCC_DIAG_OFF(effc++) // non-virtual destructors warnings suppress

/// @brief Core class for a symbolically represented finite automaton.
class VATA::SymbolicFiniteAutCore
{

GCC_DIAG_ON(effc++)

public: // public data types

  /// @brief An automaton description structure.
  using AutDescription = VATA::SymbolicFiniteAut::AutDescription;

  /// @brief An internal state.
  using StateType = VATA::SymbolicFiniteAut::StateType;

  /// @brief An internal symbol.
  using SymbolType = VATA::SymbolicFiniteAut::SymbolType;

  /// @brief A state dictionary.
  using StateDict = VATA::SymbolicFiniteAut::StateDict;

  /// @brief A symbol dictionary.
  using SymbolDict = VATA::SymbolicFiniteAut::SymbolDict;

  /// @brief A weak translation of states (addition allowed).
  using StringToStateTranslWeak = VATA::SymbolicFiniteAut::StringToStateTranslWeak;

  /// @brief A weak translation of symbols (addition allowed).
  using StringToSymbolTranslWeak = VATA::SymbolicFiniteAut::StringToSymbolTranslWeak;

  /// @brief A strict reverse translation of states (addition forbidden).
  using StateBackTranslStrict = VATA::SymbolicFiniteAut::StateBackTranslStrict;

  /// @brief A strict reverse translation of symbols (addition forbidden).
  using SymbolBackTranslStrict = VATA::SymbolicFiniteAut::SymbolBackTranslStrict;

private: // private data types

  /// @brief An internal transition.
  using TransitionType = VATA::Util::Triple<StateType, SymbolType, StateType>;

  /// @brief A vector of internal transitions.
  using TransitionsVec = std::vector<TransitionType>;

  /// @brief A set of internal initial states.
  using InitialStatesSet = std::unordered_set<StateType>;

  /// @brief A set of internal final states.
  using FinalStatesSet = std::unordered_set<StateType>;

  /// @brief A symbolic BDD.
  using SymbolicFiniteAutBDD = VATA::SymbolicFiniteAutBDD;

  /// @brief A symbolic representation.
  using SymbolicVarAsgn = VATA::SymbolicFiniteAutBDD::SymbolicVarAsgn;

  /// @brief A symbolic vector.
  using AssignmentList = VATA::SymbolicFiniteAutBDD::AssignmentList;

  /// @brief A symbolic BDD of transitions.
  using TransitionsBDD = SymbolicFiniteAutBDD;

  /// @brief A symbolic BDD of initial states.
  using InitialStatesBDD = SymbolicFiniteAutBDD;

  /// @brief A symbolic BDD of final states.
  using FinalStatesBDD = SymbolicFiniteAutBDD;

  /// @brief TransitionsBDD pointer for transitions.
  using TransitionsBDDPtr = std::shared_ptr<TransitionsBDD>;

  /// @brief InitialStatesBDD pointer for initial states.
  using InitialStatesBDDPtr = std::shared_ptr<InitialStatesBDD>;

  /// @brief FinalStatesBDD pointer for final states.
  using FinalStatesBDDPtr = std::shared_ptr<FinalStatesBDD>;

private: // data members

  /// @brief A counter of variables in symbolic state.
  size_t stateVars_;

  /// @brief A counter of variables in symbolic symbol.
  size_t symbolVars_;

  /// @brief A BDD of transitions.
  TransitionsBDDPtr transitions_;

  /// @brief A BDD of initial states.
  InitialStatesBDDPtr initialStates_;

  /// @brief A BDD of final states.
  FinalStatesBDDPtr finalStates_;

public: // instantiation

  /// @brief Default constructor.
  SymbolicFiniteAutCore();

  /**
   * @brief Copy constructor.
   * @param[in] core An automaton to copy.
   */
  SymbolicFiniteAutCore(
    const SymbolicFiniteAutCore & core
  );

  /**
   * @brief Move constructor.
   * @param[in] core An automaton to move.
   */
  SymbolicFiniteAutCore(
    SymbolicFiniteAutCore && core
  );

  /// @brief Default destructor.
  ~SymbolicFiniteAutCore();

  /**
   * @brief Copy assignment operator.
   * @param[in] rhs An automaton to copy.
   * @return An automaton.
   */
  SymbolicFiniteAutCore & operator=(
    const SymbolicFiniteAutCore & rhs
  );

  /**
   * @brief Move assignment operator.
   * @param[in] rhs An automaton to move.
   * @return An automaton.
   */
  SymbolicFiniteAutCore & operator=(
    SymbolicFiniteAutCore && rhs
  );

public: // public methods

  /**
   * @brief Explicit loading an automaton from aut_description.
   * @tparam StateTranslFunc A functor for translation of states.
   * @tparam SymbolTranslFunc A functor translation of symbols.
   * @param[in] desc A structure describing and automaton.
   * @param[in] stateTransl A functor for translation of states.
   * @param[in] symbolTransl A functor translation of symbols.
   */
  template <typename StateTranslFunc, typename SymbolTranslFunc>
  void LoadFromAutDescExplicit(
    const AutDescription & desc,
    StateTranslFunc        stateTransl,
    SymbolTranslFunc       symbolTransl
  )
  {
    // store to internal representation before converting to symbolic
    TransitionsVec TransitionsVec;
    InitialStatesSet InitialStatesSet;
    FinalStatesSet FinalStatesSet;

    // count different states and symbols
    std::unordered_set<StateType> states;
    std::unordered_set<SymbolType> symbols;

    for (auto transition : desc.transitions)
    { // transitions and initial states
      if (transition.first.empty())
      { // an init state is represented as a transition with no left-side state
        StateType state = stateTransl(transition.third);

        states.insert(state);
        InitialStatesSet.insert(state);
      }

      else if (transition.first.size() > 1)
      { // finite automaton transitions are only unary
        throw std::runtime_error("Not a finite automaton.");
      }

      else
      { // common transition
        StateType lstate = stateTransl(transition.first.front());
        SymbolType symbol = symbolTransl(transition.second);
        StateType rstate = stateTransl(transition.third);
        TransitionType triple(lstate, symbol, rstate);

        states.insert(lstate);
        symbols.insert(symbol);
        states.insert(rstate);
        TransitionsVec.push_back(triple);
      }
    }

    for (auto finalState : desc.finalStates)
    { // final states
      StateType state = stateTransl(finalState);

      states.insert(state);
      FinalStatesSet.insert(state);
    }

    // determine number of variables to represent a symbolic state or symbol
    size_t size1 = states.size();
    size_t size2 = symbols.size();
    stateVars_ = ((size1 < 2) ? 1 : ceil(log2(size1)));
    symbolVars_ = ((size2 < 2) ? 1 : ceil(log2(size2)));

    assert(transitions_   != nullptr);
    assert(initialStates_ != nullptr);
    assert(finalStates_   != nullptr);

    for (auto transition : TransitionsVec)
    { // transitions
      SymbolicVarAsgn triple = MergeTransition(
        transition.first,
        transition.second,
        transition.third
      );

      transitions_->AddAssignment(triple);
    }

    for (auto initialState : InitialStatesSet)
    { // initial states
      SymbolicVarAsgn state(stateVars_, initialState);

      initialStates_->AddAssignment(state);
    }

    for (auto finalState : FinalStatesSet)
    { // final states
      SymbolicVarAsgn state(stateVars_, finalState);

      finalStates_->AddAssignment(state);
    }
  }

  /**
   * @brief Symbolic loading an automaton from aut_description.
   * @param[in] desc A structure describing and automaton.
   */
  void LoadFromAutDescSymbolic(
    const AutDescription & desc
  );

  /**
   * @brief Explicit dumping an automaton to aut_description.
   * @tparam StateBackTranslFunc A functor for reverse translation of states.
   * @tparam SymbolBackTranslFunc A functor for reverse translation of symbols.
   * @param[in] stateBackTransl A functor for reverse translation of states.
   * @param[in] symbolBackTransl A functor for reverse translation of symbols.
   * @return An aut_description structure describing an automaton.
   */
  template <typename StateBackTranslFunc, typename SymbolBackTranslFunc>
  AutDescription DumpToAutDescExplicit(
    StateBackTranslFunc                    stateBackTransl,
    SymbolBackTranslFunc                   symbolBackTransl
  ) const
  {
    assert(transitions_   != nullptr);
    assert(initialStates_ != nullptr);
    assert(finalStates_   != nullptr);

    AssignmentList transitionList = transitions_->GetAllAssignments();
    AssignmentList initialStateList = initialStates_->GetAllAssignments();
    AssignmentList finalStatesList = finalStates_->GetAllAssignments();

    AutDescription desc;

    for (auto transition : transitionList)
    { // transitions
      StateType lstate;
      SymbolType symbol;
      StateType rstate;

      SplitTransition(transition, lstate, symbol, rstate);

      std::string strlstate = stateBackTransl(lstate);
      std::string strsymbol = symbolBackTransl(symbol);
      std::string strrstate = stateBackTransl(rstate);
      std::vector<std::string> tuple{strlstate};
      VATA::Util::Triple<
        std::vector<std::string>,
        std::string,
        std::string
      > triple(tuple, strsymbol, strrstate);

      desc.transitions.insert(triple);
    }

    for (auto initialState : initialStateList)
    { // initial states
      StateType state = SymbolicVarAsgn2Size_t(initialState);
      std::string strstate = stateBackTransl(state);
      std::vector<std::string> tuple;
      VATA::Util::Triple<
        std::vector<std::string>,
        std::string,
        std::string
      > triple(tuple, "x", strstate);

      desc.transitions.insert(triple);
    }

    for (auto finalState : finalStatesList)
    { // final states
      StateType state = SymbolicVarAsgn2Size_t(finalState);
      std::string strstate = stateBackTransl(state);

      desc.finalStates.insert(strstate);
    }

    return desc;
  }

  /**
   * @brief Symbolic dumping an automaton to aut_description.
   * @return An aut_description structure describing an automaton.
   */
  AutDescription DumpToAutDescSymbolic() const;

  /**
   * @brief Adding a symbolic transition.
   * @param[in] lstate A string representation of a left-side state.
   * @param[in] symbol A string representation of a symbol.
   * @param[in] rstate A string representation of a right-side state.
   */
  void AddTransition(
    const std::string & lstate,
    const std::string & symbol,
    const std::string & rstate
  );

  /**
   * @brief Adding a symbolic initial state.
   * @param[in] state A string representation of a initial state.
   */
  void AddInitialState(
    const std::string & state
  );

  /**
   * @brief Adding a symbolic final state.
   * @param[in] state A string representation of a final state.
   */
  void AddFinalState(
    const std::string & state
  );

private: // private methods

  /**
   * @brief Conversion from SymbolicVarAsgn to size_t.
   * @param[in] asgn A symbolic representation of a state or symbol.
   * @return An internal representation of a state or symbol.
   */
  static size_t SymbolicVarAsgn2Size_t(
    const SymbolicVarAsgn & asgn
  );

  /**
   * @brief Conversion from string to size_t.
   * @param[in] str A symbolic (string) representation of a state or symbol.
   * @return An internal representation of a state or symbol.
   */
  static size_t SymbolicVarAsgn2Size_t(
    const std::string & str
  );

  /**
   * @brief Transition merge.
   * @param[in] lstate An internal representation of a left-side state.
   * @param[in] symbol An internal representation of a symbol.
   * @param[in] rstate An internal representation of a right-side state.
   * @return A merged transition.
   */
  SymbolicVarAsgn MergeTransition(
    const StateType & lstate,
    const SymbolType & symbol,
    const StateType & rstate
  ) const;

  /**
   * @brief Transition merge.
   * @param[in] lstate A symbolic representation of a left-side state.
   * @param[in] symbol A symbolic representation of a symbol.
   * @param[in] rstate A symbolic representation of a right-side state.
   * @return A merged transition.
   */
  SymbolicVarAsgn MergeTransition(
    const SymbolicVarAsgn & lstate,
    const SymbolicVarAsgn & symbol,
    const SymbolicVarAsgn & rstate
  ) const;

  /**
   * @brief Transition split.
   * @param[in] transition A symbolic representation of a transition.
   * @param[out] lstate An output internal representation of a left-side state.
   * @param[out] symbol An output internal representation of a symbol.
   * @param[out] rstate An output internal representation of a right-side state.
   */
  void SplitTransition(
    const SymbolicVarAsgn & transition,
    StateType & lstate,
    SymbolType & symbol,
    StateType & rstate
  ) const;

  /**
   * @brief Transition split.
   * @param[in] transition A symbolic representation of a transition.
   * @param[out] lstate An output string representation of a left-side state.
   * @param[out] symbol An output string representation of a symbol.
   * @param[out] rstate An output string representation of a right-side state.
   */
  void SplitTransition(
    const SymbolicVarAsgn & transition,
    std::string & lstate,
    std::string & symbol,
    std::string & rstate
  ) const;
};

#endif // _VATA_SYMBOLIC_FINITE_AUT_CORE_HH_