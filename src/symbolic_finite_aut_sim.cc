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
  SymbolicFiniteAutBDD::UnionApplyFunctor unionFunc;
  SymbolicFiniteAutBDD::IntersectionApplyFunctor isectFunc;
  SymbolicFiniteAutBDD::ConsequenceApplyFunctor conseqFunc;
  

  // for a pair "q1" in Q1 and "a" in Sigma there exists such "q2" in Q2
  // that the triple "q1", "a" and "q2" are in Delta
  result = aut.transitions_->Exists(aut.stateVars_);

  // reindex Q1 so it can overlap
  lhs = result.AddPostfix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  // reindex Q1 so it can overlap
  rhs = result.AddPrefix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  // save Q1-Sigma times Q1-Sigma for later
  temp = SymbolicFiniteAutBDD(
    isectFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_
  );

  // remove Sigma and leave Q1 times Q1
  temp = temp.ForAll(aut.symbolVars_);

  // if "q1" in Q1 transitions with "a" in Sigma
  // then "q2" in Q1 must transition with "a" as well
  result = SymbolicFiniteAutBDD(
    conseqFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_
  );

  // previous statement applies to all "a" in Sigma
  result = result.ForAll(aut.symbolVars_);

  // all pairs "q1" and "q2" belongs to Q1 times Q1
  // note: the result of implication in case of symbolic representation
  //       violates this condition and therefore this is necessary
  result = SymbolicFiniteAutBDD(
    isectFunc(temp.GetBDD(), result.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  // reindex F so it can overlap
  lhs = aut.finalStates_->AddPostfix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  // reindex F so it can overlap
  rhs = aut.finalStates_->AddPrefix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  // if "q1" belongs to F then "q2" must belong to F as well
  temp = SymbolicFiniteAutBDD(
    conseqFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  // apply previous statement on simulation relation
  result = SymbolicFiniteAutBDD(
    isectFunc(temp.GetBDD(), result.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  // if there are no transitions then cartesian product of final states is valid
  temp = SymbolicFiniteAutBDD(
    isectFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  // apply previous statement on simulation relation
  result = SymbolicFiniteAutBDD(
    unionFunc(temp.GetBDD(), result.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  // reindex I so it can overlap
  lhs = aut.initialStates_->AddPostfix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  // reindex I so it can overlap
  rhs = aut.initialStates_->AddPrefix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  // if there are no transitions then cartesian product of initial states is valid
  temp = SymbolicFiniteAutBDD(
    isectFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  // apply previous statement on simulation relation
  result = SymbolicFiniteAutBDD(
    unionFunc(temp.GetBDD(), result.GetBDD()),
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

  // reindex Q2 so it can overlap
  lhs = aut.transitions_->AddPostfix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(aut.stateVars_, aut.symbolVars_)
  );

  // reindex Q1 so it can overlap
  rhs = prevIndex.AddPrefix(
    std::string(aut.stateVars_ + aut.symbolVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  // "q2" is simulated by "q3" AND "q4" transition to "q3" using "a"
  result = SymbolicFiniteAutBDD(
    isectFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.symbolVars_ + aut.stateVars_ + aut.stateVars_
  );

  // there exists such "q3" that applies to previous statement
  result = result.Exists(aut.stateVars_);

  // reindex Q1 so it can overlap
  lhs = aut.transitions_->AddPostfix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  // reindex Q1 so it can overlap
  rhs = result.AddPrefix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  // if "q1" transitions to "q2" using "a"
  // then previous statements are valid
  result = SymbolicFiniteAutBDD(
    conseqFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_ + aut.stateVars_
  );

  // previous statement applies to all "q2" in Q
  result = result.ForAll(aut.stateVars_);

  // previous statement applies to all "a" in Sigma
  result = result.ForAll(aut.symbolVars_);

  // all pairs "q1" and "q4" were in previous simulation relation as well
  // note: the result of implication in case of symbolic representation
  //       violates this condition and therefore this is necessary
  result = SymbolicFiniteAutBDD(
    isectFunc(result.GetBDD(), prevIndex.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  // rename F so it can overlap
  lhs = aut.finalStates_->AddPostfix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  // rename F so it can overlap
  rhs = aut.finalStates_->AddPrefix(
    std::string(aut.stateVars_, 'X'),
    std::make_pair(0, aut.stateVars_)
  );

  // if "q1" belongs to F then "q4" must belong to F as well
  temp = SymbolicFiniteAutBDD(
    conseqFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  // apply previous statement on simulation relation
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
  std::string result;
  SymbolicFiniteAutBDD prevIndexRelation, nextIndexRelation;

  // compute inital simulation relation
  prevIndexRelation = ComputeInitialRelation(aut);
  // states are translated to string representation
  if (stateDict != nullptr)
  {
    AssignmentList list = prevIndexRelation.GetAllAssignments(true);
    StateBackTranslStrict StateBackTranslFunc(stateDict->GetReverseMap());

    for (auto elem : list)
    {
      std::string str = elem.ToString();
      std::string str1 = str.substr(0 , aut.stateVars_);
      std::string str2 = str.substr(aut.stateVars_, aut.stateVars_);
      result += StateBackTranslFunc(SymbolicFiniteAutBDD::FromSymbolic(str1));
      result += " <= ";
      result += StateBackTranslFunc(SymbolicFiniteAutBDD::FromSymbolic(str2));
      result += "\n";
    }
  }

  // symbolic representation of states
  else
  {
    AssignmentList list = prevIndexRelation.GetAllAssignments();

    for (auto elem : list)
    {
      std::string str = elem.ToString();
      std::string str1 = str.substr(0 , aut.stateVars_);
      std::string str2 = str.substr(aut.stateVars_, aut.stateVars_);
      result += str1 + " <= " + str2 + "\n";
    }
  } result += "\n";
  // compute simulation relation with next index
  nextIndexRelation = ComputeNextIndex(aut, prevIndexRelation);

  // while newly computed simulation relation changes
  while(prevIndexRelation != nextIndexRelation)
  {
    prevIndexRelation = nextIndexRelation;

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
      result += " <= ";
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
      result += str1 + " <= " + str2 + "\n";
    }
  } result += "\n";

    nextIndexRelation = ComputeNextIndex(aut, prevIndexRelation);
  }

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
      result += " <= ";
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
      result += str1 + " <= " + str2 + "\n";
    }
  }

  return result;
}