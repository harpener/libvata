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
  using SymbolicVarAsgn = VATA::SymbolicFiniteAutBDD::SymbolicVarAsgn;

  /// @brief  List of symbolic assignments
  using AssignmentList = VATA::SymbolicFiniteAutBDD::AssignmentList;

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

  /// @brief  A pair of states
	using StatePair = VATA::SymbolicFiniteAut::StatePair;

  /**
   * @brief  Maps internal representations of a pair of states to a single state
             for purposes of some operations (e.g. intersection)
   */
	using ProductTranslMap = VATA::SymbolicFiniteAut::ProductTranslMap;

  /// @brief  Matrix of states for simulation purposes
  using StateBinaryRelation = VATA::SymbolicFiniteAut::StateBinaryRelation;

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
  using SymbolicFiniteAutBDD = VATA::SymbolicFiniteAut::SymbolicFiniteAutBDD;

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

  /// @brief  Transitions
  TransitionsBDDPtr transitions_;

  /// @brief  Initial states
  InitialStatesBDDPtr initialStates_;

  /// @brief  Final states
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

public: // getters and setters

  /**
   * @brief  Getter of number of variables representing state
   *
   * @return  Number of variables representing state
   */
  size_t GetStateVars() const;

  /**
   * @brief  Setter of number of variables representing state
   *
   * @param[in]  vars  Assigned number of variables representing state
   */
  void SetStateVars(
    const size_t & vars
  );

  /**
   * @brief  Getter of number of variables representing symbol
   *
   * @return  Number of variables representing symbol
   */
  size_t GetSymbolVars() const;

  /**
   * @brief  Setter of number of variables representing symbol
   *
   * @param[in]  vars  Assigned number of variables representing symbol
   */
  void SetSymbolVars(
    const size_t & vars
  );

  /**
   * @brief  Getter of transitions
   *
   * @return  Transitions
   */
  TransitionsBDD GetTransitions() const;

  /**
   * @brief  Setter of transitions
   *
   * @param[in]  transitions  Assigned transitions
   */
  void SetTransitions(
    const TransitionsBDD & transitions
  );

  /**
   * @brief  Getter of initial states
   *
   * @return  Initial states
   */
  InitialStatesBDD GetInitialStates() const;

  /**
   * @brief  Setter of initial states
   *
   * @param[in]  initialStates  Assigned initial states
   */
  void SetInitialStates(
    const InitialStatesBDD & initialStates
  );

  /**
   * @brief  Getter of final states
   *
   * @return  Final states
   */
  FinalStatesBDD GetFinalStates() const;

  /**
   * @brief  Setter of final states
   *
   * @param[in]  finalStates  Assigned final states
   */
  void SetFinalStates(
    const FinalStatesBDD & finalStates
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
    assert(transitions_   != nullptr);
    assert(initialStates_ != nullptr);
    assert(finalStates_   != nullptr);

    // store to internal representation before converting to symbolic
    TransitionsSet transitionsSet;
    InitialStatesSet initialStatesSet;
    FinalStatesSet finalStatesSet;

    // count different states and symbols
    std::set<size_t> states;
    std::set<size_t> symbols;

    // if states and symbols are known
    if (!desc.states.empty() && !desc.symbols.empty())
    {
      size_t size1 = desc.states.size();
      size_t size2 = desc.symbols.size();
      stateVars_ = ((size1 < 2) ? 1 : ceil(log2(size1)));
      symbolVars_ = ((size2 < 2) ? 1 : ceil(log2(size2)));
    }

    for (auto transition : desc.transitions)
    { // transitions and initial states
      if (transition.first.empty())
      { // an init state is represented as a transition with no left-side state
        size_t state = stateTransl(transition.third);

        if(stateVars_)
        {
          initialStates_->AddAssignment(SymbolicVarAsgn(stateVars_, state));
        }

        else
        {
          states.insert(state);
          initialStatesSet.insert(state);
        }
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

        if(stateVars_ && symbolVars_)
        {
          transitions_->AddAssignment(
            SymbolicFiniteAutBDD::MergeTransition(
              stateVars_,
              symbolVars_,
              lstate,
              symbol,
              rstate
            )
          );
        }

        else
        {
          states.insert(lstate);
          symbols.insert(symbol);
          states.insert(rstate);
          transitionsSet.insert(TransitionType(lstate, symbol, rstate));
        }
      }
    }

    for (auto finalState : desc.finalStates)
    { // final states
      size_t state = stateTransl(finalState);

      if(stateVars_)
      {
        finalStates_->AddAssignment(SymbolicVarAsgn(stateVars_, state));
      }

      else
      {
        states.insert(state);
        finalStatesSet.insert(state);
      }
    }

    if(!stateVars_ && !symbolVars_)
    {
      // determine number of variables to represent a symbolic state or symbol
      size_t size1 = states.size();
      size_t size2 = symbols.size();
      stateVars_ = ((size1 < 2) ? 1 : ceil(log2(size1)));
      symbolVars_ = ((size2 < 2) ? 1 : ceil(log2(size2)));

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

    AssignmentList transitionList   = transitions_->GetAllAssignments(true);
    AssignmentList initialStateList = initialStates_->GetAllAssignments(true);
    AssignmentList finalStatesList  = finalStates_->GetAllAssignments(true);

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
   *
   * return  Union of given automata
   */
	static SymbolicFiniteAutCore Union(
  	const SymbolicFiniteAutCore & lhs,
  	const SymbolicFiniteAutCore & rhs
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
	static SymbolicFiniteAutCore Intersection(
  	const SymbolicFiniteAutCore & lhs,
  	const SymbolicFiniteAutCore & rhs
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
  	const SymbolicFiniteAutCore & aut
  );

  /**
   * @brief  Compute an initial simulation relation
   *
   * @param  aut  Given automaton
   *
   * return  Initial simulation relation
   */
  static SymbolicFiniteAutBDD ComputeInitialSimulation(
    const SymbolicFiniteAutCore & aut
  );

  /**
   * @brief  Iterate a simulation relation
   *
   * @param  aut        Given automaton
   * @param  prevIndex  Simulation relationwith index i-1
   *
   * return  Simulation relation with index i
   */
  static SymbolicFiniteAutBDD IterateSimulation(
    const SymbolicFiniteAutCore & aut,
    const SymbolicFiniteAutBDD &  prevIndex
  );

  /**
   * @brief  Converts simulation relation to matrix
   *
   * @param  sim  Given simulation relation
   *
   * return  Matrix with simulation relation
   */
  static StateBinaryRelation MatrixSimulation(
    const SymbolicFiniteAutBDD & sim
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

#endif // _VATA_SYMBOLIC_FINITE_AUT_CORE_HH_