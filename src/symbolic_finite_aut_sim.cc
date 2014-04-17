/*****************************************************************************
 *  VATA Finite Automata Library
 *
 *  Copyright (c) 2014 Jiri Chromecka <xchrom12@stud.fit.vutbr.cz>
 *
 *  Description:
 *    Implementation of simulation computation on BDD symbolic finite automata.
 *****************************************************************************/

#include "symbolic_finite_aut_core.hh"

using VATA::SymbolicFiniteAutCore;

SymbolicFiniteAutCore::SymbolicFiniteAutBDD SymbolicFiniteAutCore::
ComputeInitialRelation(
  const SymbolicFiniteAutCore & aut
)
{
  SymbolicFiniteAutBDD result, temp, lhs, rhs;
  SymbolicFiniteAutBDD::ConsequenceApplyFunctor conseqFunc;
  SymbolicFiniteAutBDD::IntersectionApplyFunctor isectFunc;

  result = aut.transitions_->Exists(aut.stateVars_);

  lhs = result.AddPostfix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  rhs = result.AddPrefix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  temp = SymbolicFiniteAutBDD(
    isectFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_
  );

  temp = temp.ForAll(aut.symbolVars_);

  result = SymbolicFiniteAutBDD(
    conseqFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_
  );

  result = result.ForAll(aut.symbolVars_);

  result = SymbolicFiniteAutBDD(
    isectFunc(temp.GetBDD(), result.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  lhs = aut.finalStates_->AddPostfix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  rhs = aut.finalStates_->AddPrefix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  temp = SymbolicFiniteAutBDD(
    conseqFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  result = SymbolicFiniteAutBDD(
    isectFunc(temp.GetBDD(), result.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  return result;
}

SymbolicFiniteAutCore::SymbolicFiniteAutBDD SymbolicFiniteAutCore::
ComputeNextIndex(
  const SymbolicFiniteAutCore & aut,
  const SymbolicFiniteAutBDD &  prevIndex
)
{
  SymbolicFiniteAutBDD result, temp, lhs, rhs;
  SymbolicFiniteAutBDD::ConsequenceApplyFunctor conseqFunc;
  SymbolicFiniteAutBDD::IntersectionApplyFunctor isectFunc;

  lhs = aut.transitions_->AddPostfix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(aut.stateVars_, aut.symbolVars_)
  );

  rhs = prevIndex.AddPrefix(
    std::string(aut.stateVars_ + aut.symbolVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  result = SymbolicFiniteAutBDD(
    isectFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.symbolVars_ + aut.stateVars_ + aut.stateVars_
  );

  result = result.Exists(aut.stateVars_);

  lhs = aut.transitions_->AddPostfix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  rhs = result.AddPrefix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  result = SymbolicFiniteAutBDD(
    conseqFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_ + aut.stateVars_
  );

  result = result.ForAll(aut.stateVars_);

  result = result.ForAll(aut.symbolVars_);

  result = SymbolicFiniteAutBDD(
    isectFunc(result.GetBDD(), prevIndex.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  lhs = aut.finalStates_->AddPostfix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  rhs = aut.finalStates_->AddPrefix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  temp = SymbolicFiniteAutBDD(
    conseqFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  result = SymbolicFiniteAutBDD(
    isectFunc(temp.GetBDD(), result.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  return result;
}

std::string SymbolicFiniteAutCore::ComputeSimulation(
  const SymbolicFiniteAutCore & aut,
  StateDict *                   stateDict
)
{
  SymbolicFiniteAutBDD prevIndexRelation, nextIndexRelation;

  // compute inital simulation relation
  prevIndexRelation = ComputeInitialRelation(aut);

  // compute simulation relation with next index
  nextIndexRelation = ComputeNextIndex(aut, prevIndexRelation);

  // while newly computed simulation relation changes
  while(prevIndexRelation != nextIndexRelation)
  {
    prevIndexRelation = nextIndexRelation;
    nextIndexRelation = ComputeNextIndex(aut, prevIndexRelation);
  }

  std::string result = "Simulation relation:\n";

  // states are translated to string representation
  if (stateDict != nullptr)
  {
    AssignmentList list = nextIndexRelation.GetAllAssignments(true);
    StateBackTranslStrict StateBackTranslFunc(stateDict->GetReverseMap());

    for (auto elem : list)
    {
      std::string str = elem.ToString();
      std::string str1 = str.substr(0 , aut.stateVars_);
      std::string str2 = str.substr(aut.stateVars_, aut.stateVars_);
      result += StateBackTranslFunc(SymbolicFiniteAutBDD::FromSymbolic(str1));
      result += " is simulated by ";
      result += StateBackTranslFunc(SymbolicFiniteAutBDD::FromSymbolic(str2));
      result += "\n";
    }
  }

  // symbolic representation of states
  else
  {
    AssignmentList list = nextIndexRelation.GetAllAssignments();

    for (auto elem : list)
    {
      std::string str = elem.ToString();
      std::string str1 = str.substr(0 , aut.stateVars_);
      std::string str2 = str.substr(aut.stateVars_, aut.stateVars_);
      result += str1 + " is simulated by " + str2 + "\n";
    }
  }

  return result;
}