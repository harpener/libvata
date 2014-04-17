/****************************************************************************
 *  VATA Finite Automata Library
 *
 *  Copyright (c) 2014 Jiri Chromecka <xchrom12@stud.fit.vutbr.cz>
 *
 *  Description:
 *    Implementation of intersection on BDD symbolic finite automata.
 *****************************************************************************/

#include "symbolic_finite_aut_core.hh"

using VATA::SymbolicFiniteAutCore;

SymbolicFiniteAutCore SymbolicFiniteAutCore::Intersection(
	const SymbolicFiniteAutCore & lhs,
	const SymbolicFiniteAutCore & rhs,
	ProductTranslMap *            pTranslMap
)
{
  SymbolicFiniteAutCore result;
  SymbolicFiniteAutBDD::IntersectionApplyFunctor isectFunc;

  // add such postfix to lhs states and such prefix to rhs states,
  // so they overlap each other for cartesian product
  SymbolicFiniteAutCore tempLhs = lhs.ReindexStates(
    std::string(rhs.stateVars_, 'X'),
    false
  );

  SymbolicFiniteAutCore tempRhs = rhs.ReindexStates(
    std::string(lhs.stateVars_, 'X'),
    true
  );

  // number of state and symbol variables for tempLhs and tempRhs is the same
  result.stateVars_ = tempLhs.stateVars_;
  result.symbolVars_ = tempLhs.symbolVars_;

  // apply intersection on transitions
  *(result.transitions_) = SymbolicFiniteAutBDD(
    isectFunc(
       tempLhs.transitions_->GetBDD(), tempRhs.transitions_->GetBDD()
    ),
    result.stateVars_ + result.symbolVars_ + result.stateVars_
  );

  // apply intersection on initial states
  *(result.initialStates_) = SymbolicFiniteAutBDD(
    isectFunc(
       tempLhs.initialStates_->GetBDD(), tempRhs.initialStates_->GetBDD()
    ),
    result.stateVars_
  );

  // apply intersection on final states
  *(result.finalStates_) = SymbolicFiniteAutBDD(
    isectFunc(
       tempLhs.finalStates_->GetBDD(), tempRhs.finalStates_->GetBDD()
    ),
    result.stateVars_
  );

  // state translation follows after the operation,
  // it can be predicted, but then it would have redundant translations
  if (pTranslMap != nullptr)
  {
    AssignmentList transitionsList =
    result.transitions_->GetAllAssignments(true);
    AssignmentList initialStatesList =
    result.initialStates_->GetAllAssignments(true);
    AssignmentList finalStatesList =
    result.finalStates_->GetAllAssignments(true);

    for (auto transition : transitionsList)
    {
      std::string lstate, symbol, rstate;

      SymbolicFiniteAutBDD::SplitTransition(
        transition,
        result.stateVars_,
        result.symbolVars_,
        lstate,
        symbol,
        rstate
      );

      pTranslMap->insert(
        std::make_pair(
          std::make_pair(
            SymbolicFiniteAutBDD::FromSymbolic(
              lstate.substr(0, lhs.stateVars_)
            ),
            SymbolicFiniteAutBDD::FromSymbolic(
              lstate.substr(lhs.stateVars_, rhs.stateVars_)
            )
          ),
          SymbolicFiniteAutBDD::FromSymbolic(lstate)
        )
      );

      pTranslMap->insert(
        std::make_pair(
          std::make_pair(
            SymbolicFiniteAutBDD::FromSymbolic(
              rstate.substr(0, lhs.stateVars_)
            ),
            SymbolicFiniteAutBDD::FromSymbolic(
              rstate.substr(lhs.stateVars_, rhs.stateVars_)
            )
          ),
          SymbolicFiniteAutBDD::FromSymbolic(rstate)
        )
      );
    }

    for (auto initialState : initialStatesList)
    {
      std::string state = initialState.ToString();

      pTranslMap->insert(
        std::make_pair(
          std::make_pair(
            SymbolicFiniteAutBDD::FromSymbolic(
              state.substr(0, lhs.stateVars_)
            ),
            SymbolicFiniteAutBDD::FromSymbolic(
              state.substr(lhs.stateVars_, rhs.stateVars_)
            )
          ),
          SymbolicFiniteAutBDD::FromSymbolic(state)
        )
      );
    }

    for (auto finalState : finalStatesList)
    {
      std::string state = finalState.ToString();

      pTranslMap->insert(
        std::make_pair(
          std::make_pair(
            SymbolicFiniteAutBDD::FromSymbolic(
              state.substr(0, lhs.stateVars_)
            ),
            SymbolicFiniteAutBDD::FromSymbolic(
              state.substr(lhs.stateVars_, rhs.stateVars_)
            )
          ),
          SymbolicFiniteAutBDD::FromSymbolic(state)
        )
      );
    }
  }

  return result;
}