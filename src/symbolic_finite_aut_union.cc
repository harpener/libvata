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

  StateToStateMap translMapLhs;
  StateToStateMap translMapRhs;

  if (pTranslMapLhs == nullptr)
  { // unused lhs state translation
    pTranslMapLhs = &translMapLhs;
  }

  if (pTranslMapRhs == nullptr)
  { // unused rhs state translation
    pTranslMapRhs = &translMapRhs;
  }

  SymbolicFiniteAutCore tempLhs = lhs.ReindexStates("0", 0, pTranslMapLhs);
  SymbolicFiniteAutCore tempRhs = rhs.ReindexStates("1", 0, pTranslMapRhs);

  result.stateVars_ = tempLhs.stateVars_;
  result.symbolVars_ = tempLhs.symbolVars_;

  SymbolicFiniteAutBDD::UnionApplyFunctor unionFunc;

  result.transitions_->SetBDD(
    unionFunc(tempLhs.transitions_->GetBDD(), tempRhs.transitions_->GetBDD())
  );
  result.initialStates_->SetBDD(
    unionFunc(
      tempLhs.initialStates_->GetBDD(), tempRhs.initialStates_->GetBDD()
    )
  );
  result.finalStates_->SetBDD(
    unionFunc(tempLhs.finalStates_->GetBDD(), tempRhs.finalStates_->GetBDD())
  );

  return result;
}