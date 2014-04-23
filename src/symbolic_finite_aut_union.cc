/*****************************************************************************
 *  VATA Finite Automata Library
 *
 *  Copyright (c) 2014 Jiri Chromecka <xchrom12@stud.fit.vutbr.cz>
 *
 *  Description:
 *    Implementation of union on BDD symbolic finite automata.
 *****************************************************************************/

#include "symbolic_finite_aut_core.hh"

using VATA::SymbolicFiniteAutCore;

SymbolicFiniteAutCore SymbolicFiniteAutCore::Union(
	const SymbolicFiniteAutCore & lhs,
	const SymbolicFiniteAutCore & rhs,
	StateToStateMap *             pTranslMapLhs,
	StateToStateMap *             pTranslMapRhs
)
{
  SymbolicFiniteAutCore result;
  SymbolicFiniteAutBDD::UnionApplyFunctor unionFunc;

  // TODO: optimize
  // add one distinct bit to both lhs and rhs states,
  // so they can exist in one BDD next to each other
  SymbolicFiniteAutCore tempLhs = lhs.ReindexStates("0", true, pTranslMapLhs);
  SymbolicFiniteAutCore tempRhs = rhs.ReindexStates("1", true, pTranslMapRhs);

  // number of state and symbol variables for tempLhs and tempRhs is the same
  result.stateVars_  = tempLhs.stateVars_;
  result.symbolVars_ = tempLhs.symbolVars_;

  // apply union on transitions
  *(result.transitions_) = SymbolicFiniteAutBDD(
    unionFunc(
      tempLhs.transitions_->GetBDD(), tempRhs.transitions_->GetBDD()
    ),
    result.stateVars_ + result.symbolVars_ + result.stateVars_
  );

  // apply union on initial states
  *(result.initialStates_) = SymbolicFiniteAutBDD(
    unionFunc(
      tempLhs.initialStates_->GetBDD(), tempRhs.initialStates_->GetBDD()
    ),
    result.stateVars_
  );

  // apply union on final states
  *(result.finalStates_) = SymbolicFiniteAutBDD(
    unionFunc(
      tempLhs.finalStates_->GetBDD(), tempRhs.finalStates_->GetBDD()
    ),
    result.stateVars_
  );

  return result;
}