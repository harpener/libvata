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
	const SymbolicFiniteAutCore & rhs
)
{
  // alphabets are equal
  assert(lhs.symbolVars_ == rhs.symbolVars_);

  SymbolicFiniteAutCore result;
  SymbolicFiniteAutBDD tempLhs, tempRhs;
  SymbolicFiniteAutBDD::UnionApplyFunctor unionFunc;
  SymbolicFiniteAutBDD::IntersectionApplyFunctor isectFunc;
  std::string str;

  // number of state variables
  result.stateVars_  = (lhs.stateVars_ >= rhs.stateVars_) ?
                       (lhs.stateVars_ + 1) :
                       (rhs.stateVars_ + 1);

  // number of symbol variables
  result.symbolVars_ = lhs.symbolVars_;

  // rename lhs transitions
  tempLhs = lhs.transitions_->Rename(
    result.stateVars_ + lhs.symbolVars_ + result.stateVars_,
    [result, lhs](const size_t var){
      if (var < (lhs.stateVars_ + lhs.symbolVars_))
        return var + (result.stateVars_ - lhs.stateVars_);
      else
        return var + 2 * (result.stateVars_ - lhs.stateVars_);
    }
  );

  str =  std::string(result.stateVars_ - lhs.stateVars_, '0');
  str += std::string(lhs.stateVars_ + lhs.symbolVars_, 'X');
  str += std::string(result.stateVars_ - lhs.stateVars_, '0');
  str += std::string(lhs.stateVars_, 'X');
  tempLhs.SetBDD(
    isectFunc(
      tempLhs.GetBDD(),
      SymbolicFiniteAutBDD(SymbolicVarAsgn(str)).GetBDD()
    )
  );

  // rename rhs transitions
  tempRhs = rhs.transitions_->Rename(
    rhs.stateVars_ + rhs.symbolVars_ + rhs.stateVars_ + 2,
    [result, rhs](const size_t var){
      if (var < (rhs.stateVars_ + rhs.symbolVars_))
        return var + (result.stateVars_ - rhs.stateVars_);
      else
        return var + 2 * (result.stateVars_ - rhs.stateVars_);
    }
  );

  str =  std::string(result.stateVars_ - rhs.stateVars_, '1');
  str += std::string(rhs.stateVars_ + rhs.symbolVars_, 'X');
  str += std::string(result.stateVars_ - rhs.stateVars_, '1');
  str += std::string(rhs.stateVars_, 'X');
  tempRhs.SetBDD(
    isectFunc(
      tempRhs.GetBDD(),
      SymbolicFiniteAutBDD(SymbolicVarAsgn(str)).GetBDD()
    )
  );

  // apply union on transitions
  *(result.transitions_) = SymbolicFiniteAutBDD(
    unionFunc(tempLhs.GetBDD(), tempRhs.GetBDD()),
    result.stateVars_ + result.symbolVars_ + result.stateVars_
  );

  // rename lhs initial states
  tempLhs = lhs.initialStates_->Rename(
    lhs.stateVars_ + 1,
    [result, lhs](const size_t var){
      return var + (result.stateVars_ - lhs.stateVars_);
    }
  );

  str =  std::string(result.stateVars_ - lhs.stateVars_, '0');
  str += std::string(lhs.stateVars_, 'X');
  tempLhs.SetBDD(
    isectFunc(
      tempLhs.GetBDD(),
      SymbolicFiniteAutBDD(SymbolicVarAsgn(str)).GetBDD()
    )
  );

  // rename rhs initial states
  tempRhs = rhs.initialStates_->Rename(
    rhs.stateVars_ + 1,
    [result, rhs](const size_t var){
      return var + (result.stateVars_ - rhs.stateVars_);
    }
  );

  str =  std::string(result.stateVars_ - rhs.stateVars_, '1');
  str += std::string(rhs.stateVars_, 'X');
  tempRhs.SetBDD(
    isectFunc(
      tempRhs.GetBDD(),
      SymbolicFiniteAutBDD(SymbolicVarAsgn(str)).GetBDD()
    )
  );

  // apply union on initial states
  *(result.initialStates_) = SymbolicFiniteAutBDD(
    unionFunc(tempLhs.GetBDD(), tempRhs.GetBDD()),
    result.stateVars_
  );

  // rename lhs final states
   tempLhs = lhs.finalStates_->Rename(
    lhs.stateVars_ + 1,
    [result, lhs](const size_t var){
      return var + (result.stateVars_ - lhs.stateVars_);
    }
  );

  str =  std::string(result.stateVars_ - lhs.stateVars_, '0');
  str += std::string(lhs.stateVars_, 'X');
  tempLhs.SetBDD(
    isectFunc(
      tempLhs.GetBDD(),
      SymbolicFiniteAutBDD(SymbolicVarAsgn(str)).GetBDD()
    )
  );

  // rename rhs final states
  tempRhs = rhs.finalStates_->Rename(
    rhs.stateVars_ + 1,
    [result, rhs](const size_t var){
      return var + (result.stateVars_ - rhs.stateVars_);
    }
  );

  str =  std::string(result.stateVars_ - rhs.stateVars_, '1');
  str += std::string(rhs.stateVars_, 'X');
  tempRhs.SetBDD(
    isectFunc(
      tempRhs.GetBDD(),
      SymbolicFiniteAutBDD(SymbolicVarAsgn(str)).GetBDD()
    )
  );

  // apply union on final states
  *(result.finalStates_) = SymbolicFiniteAutBDD(
    unionFunc(tempLhs.GetBDD(), tempRhs.GetBDD()),
    result.stateVars_
  );

  return result;
}

void SymbolicFiniteAutCore::GenUnionTransl(
  const StateDict & lhs,
  const StateDict & rhs,
  StateToStateMap * lhsUnion,
  StateToStateMap * rhsUnion
)
{
  assert(lhsUnion != nullptr);
  assert(rhsUnion != nullptr);

  // number of states
  size_t sizeLhs = lhs.size();
  size_t sizeRhs = rhs.size();

  // number of state variables
  size_t lhsStateVars = ((sizeLhs < 2) ? 1 : ceil(log2(sizeLhs)));
  size_t rhsStateVars = ((sizeRhs < 2) ? 1 : ceil(log2(sizeRhs)));
  size_t resultStateVars = (lhsStateVars >= rhsStateVars) ?
                           (lhsStateVars + 1) :
                           (rhsStateVars + 1);

  for (size_t i = 0; i < sizeLhs; i++)
  { // lhs translation map
    SymbolicVarAsgn asgn(std::string(resultStateVars - lhsStateVars, '0'));
    asgn.append(SymbolicVarAsgn(lhsStateVars, i));
    lhsUnion->insert(
      std::make_pair(i, SymbolicFiniteAutBDD::FromSymbolic(asgn))
    );
  }

  for (size_t i = 0; i < sizeRhs; i++)
  { // rhs translation map
    SymbolicVarAsgn asgn(std::string(resultStateVars - rhsStateVars, '1'));
    asgn.append(SymbolicVarAsgn(rhsStateVars, i));
    rhsUnion->insert(
      std::make_pair(i, SymbolicFiniteAutBDD::FromSymbolic(asgn))
    );
  }
}