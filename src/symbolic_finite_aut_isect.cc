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
	const SymbolicFiniteAutCore & rhs
)
{
  // alphabets are equal
  assert(lhs.symbolVars_ == rhs.symbolVars_);

  SymbolicFiniteAutCore result;
  SymbolicFiniteAutBDD tempLhs, tempRhs;
  SymbolicFiniteAutBDD::IntersectionApplyFunctor isectFunc;

  // number of state and symbol variables
  result.stateVars_ = lhs.stateVars_ + rhs.stateVars_;
  result.symbolVars_ = lhs.symbolVars_;

  // rename lhs transitions
  tempLhs = lhs.transitions_->Rename(
    result.stateVars_ + lhs.symbolVars_ + result.stateVars_,
    [lhs, rhs](const size_t var){
      if (var < lhs.stateVars_) return var;
      else return var + rhs.stateVars_;
    }
  );

  // rename rhs transitions
  tempRhs = rhs.transitions_->Rename(
    result.stateVars_ + lhs.symbolVars_ + result.stateVars_,
    [lhs, rhs](const size_t var){
      if (var < rhs.stateVars_ + rhs.symbolVars_) return var + lhs.stateVars_;
      else return var + lhs.stateVars_ + lhs.stateVars_;
    }
  );

  // apply intersection on transitions
  *(result.transitions_) = SymbolicFiniteAutBDD(
    isectFunc(
       tempLhs.GetBDD(), tempRhs.GetBDD()
    ),
    result.stateVars_ + result.symbolVars_ + result.stateVars_
  );

  // rename rhs initial states
  tempRhs = rhs.initialStates_->Rename(
    result.stateVars_,
    [lhs](const size_t var){return var + lhs.stateVars_;}
  );

  // apply intersection on initial states
  *(result.initialStates_) = SymbolicFiniteAutBDD(
    isectFunc(
       lhs.initialStates_->GetBDD(), tempRhs.GetBDD()
    ),
    result.stateVars_
  );

  // rename rhs final states
  tempRhs = rhs.finalStates_->Rename(
    result.stateVars_,
    [lhs](const size_t var){return var + lhs.stateVars_;}
  );

  // apply intersection on final states
  *(result.finalStates_) = SymbolicFiniteAutBDD(
    isectFunc(
       lhs.finalStates_->GetBDD(), tempRhs.GetBDD()
    ),
    result.stateVars_
  );

  return result;
}

void SymbolicFiniteAutCore::GenIsectTransl(
  const StateDict & lhs,
  const StateDict & rhs,
  ProductTranslMap * stateDictIsect
)
{
  assert(stateDictIsect != nullptr);

  // number of states
  size_t sizeLhs = lhs.size();
  size_t sizeRhs = rhs.size();

  // number of state variables
  size_t lhsStateVars = ((sizeLhs < 2) ? 1 : ceil(log2(sizeLhs)));
  size_t rhsStateVars = ((sizeRhs < 2) ? 1 : ceil(log2(sizeRhs)));

  for (size_t i = 0; i < sizeLhs; i++)
  { // lhs states
    for (size_t j = 0; j < sizeRhs; j++)
    { // rhs states
      SymbolicVarAsgn asgn(lhsStateVars, i);
      asgn.append(SymbolicVarAsgn(rhsStateVars, j));
      stateDictIsect->insert(
        std::make_pair(
          std::make_pair(i,j),
          SymbolicFiniteAutBDD::FromSymbolic(asgn)
        )
      );
    }
  }
}