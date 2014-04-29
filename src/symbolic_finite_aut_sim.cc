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
ComputeInitialSimulation(
  const SymbolicFiniteAutCore & aut
)
{
  SymbolicFiniteAutBDD result, lhs, rhs;
  SymbolicFiniteAutBDD::UnionApplyFunctor unionFunc;
  SymbolicFiniteAutBDD::IntersectionApplyFunctor isectFunc;
  SymbolicFiniteAutBDD::ConsequenceApplyFunctor conseqFunc;

  // for a pair "q1" in Q1 and "a" in Sigma there exists such "q2" in Q2
  // that the triple "q1", "a" and "q2" are in Delta
  result = aut.transitions_->Project(
    aut.stateVars_ + aut.symbolVars_,
    [aut](const size_t var){
      if (var < (aut.stateVars_ + aut.symbolVars_)) return false;
      else return true;
    },
    unionFunc
  );

  // reindex Q1 so it can overlap
  lhs = result.Rename(
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_,
    [aut](const size_t var){
      if (var < aut.stateVars_) return var;
      else return var + aut.stateVars_;
    }
  );

  // reindex Q1 so it can overlap
  rhs = result.Rename(
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_,
    [aut](const size_t var){return var + aut.stateVars_;}
  );

  // if "q1" in Q1 transitions with "a" in Sigma
  // then "q2" in Q1 must transition with "a" as well
  result = SymbolicFiniteAutBDD(
    conseqFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_
  );

  // previous statement applies to all "a" in Sigma
  result = result.Project(
    aut.stateVars_ + aut.stateVars_,
    [aut](const size_t var){
      if (var < (aut.stateVars_ + aut.stateVars_)) return false;
      else return true;
    },
    isectFunc
  );

  // reindex F so it can overlap
  rhs = aut.finalStates_->Rename(
    aut.stateVars_ + aut.stateVars_,
    [aut](const size_t var){return var + aut.stateVars_;}
  );

  // if "q1" belongs to F then "q2" must belong to F as well
  rhs = SymbolicFiniteAutBDD(
    conseqFunc(aut.finalStates_->GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  // apply previous statement on simulation relation
  result = SymbolicFiniteAutBDD(
    isectFunc(result.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  // reindex F so it can overlap
  rhs = aut.finalStates_->Rename(
    aut.stateVars_ + aut.stateVars_,
    [aut](const size_t var){return var + aut.stateVars_;}
  );

  // if there are no transitions
  // then cartesian product of final states is valid
  rhs = SymbolicFiniteAutBDD(
    isectFunc(aut.finalStates_->GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  // apply previous statement on simulation relation
  result = SymbolicFiniteAutBDD(
    unionFunc(result.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  // reindex I so it can overlap
  rhs = aut.initialStates_->Rename(
    aut.stateVars_ + aut.stateVars_,
    [aut](const size_t var){return var + aut.stateVars_;}
  );

  // if there are no transitions
  // then cartesian product of initial states is valid
  rhs = SymbolicFiniteAutBDD(
    isectFunc(aut.initialStates_->GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  // apply previous statement on simulation relation
  result = SymbolicFiniteAutBDD(
    unionFunc(result.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  return result;
}

SymbolicFiniteAutCore::SymbolicFiniteAutBDD SymbolicFiniteAutCore::
IterateSimulation(
  const SymbolicFiniteAutCore & aut,
  const SymbolicFiniteAutBDD &  prevIndex
)
{
  SymbolicFiniteAutBDD result, lhs, rhs;
  SymbolicFiniteAutBDD::UnionApplyFunctor unionFunc;
  SymbolicFiniteAutBDD::IntersectionApplyFunctor isectFunc;
  SymbolicFiniteAutBDD::ConsequenceApplyFunctor conseqFunc;

  // reindex Q2 so it can overlap
  lhs = aut.transitions_->Rename(
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_ + aut.stateVars_ +
    aut.stateVars_,
    [aut](const size_t var){
      if (var < (aut.stateVars_ + aut.symbolVars_)) return var + aut.stateVars_;
      else return var + aut.stateVars_ + aut.stateVars_;
    }
  );

  // reindex Q1 so it can overlap
  rhs = prevIndex.Rename(
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_ + aut.stateVars_ +
    aut.stateVars_,
    [aut](const size_t var){
      return var + aut.stateVars_ + aut.stateVars_ + aut.symbolVars_;
    }
  );

  // "q2" is simulated by "q3" AND "q4" transitions to "q3" using "a"
  result = SymbolicFiniteAutBDD(
    isectFunc(lhs.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_ + aut.stateVars_ +
    aut.stateVars_
  );

  // there exists such "q3" that applies to previous statement
  result = result.Project(
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_ + aut.stateVars_,
    [aut](const size_t var){
      if (var < (aut.stateVars_ + aut.stateVars_ + aut.symbolVars_ +
                 aut.stateVars_))
        return false;
      else return true;
    },
    unionFunc
  );

  // reindex Q1 so it can overlap
  lhs = aut.transitions_->Rename(
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_ + aut.stateVars_,
    [aut](const size_t var){
      if (var < aut.stateVars_) return var;
      else return var + aut.stateVars_;
    }
  );

  // if "q1" transitions to "q2" using "a"
  // then previous statements are valid
  result = SymbolicFiniteAutBDD(
    conseqFunc(lhs.GetBDD(), result.GetBDD()),
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_ + aut.stateVars_
  );

  // previous statement applies to all "q2" in Q
  result = result.Project(
    aut.stateVars_ + aut.stateVars_ + aut.symbolVars_,
    [aut](const size_t var){
      if (var < (aut.stateVars_ + aut.stateVars_ + aut.symbolVars_))
        return false;
      else return true;
    },
    isectFunc
  );

  // previous statement applies to all "a" in Sigma
  result = result.Project(
    aut.stateVars_ + aut.stateVars_,
    [aut](const size_t var){
      if (var < (aut.stateVars_ + aut.stateVars_)) return false;
      else return true;
    },
    isectFunc
  );

  // rename F so it can overlap
  rhs = aut.finalStates_->Rename(
    aut.stateVars_ + aut.stateVars_,
    [aut](const size_t var){return var + aut.stateVars_;}
  );

  // if "q1" belongs to F then "q4" must belong to F as well
  rhs = SymbolicFiniteAutBDD(
    conseqFunc(aut.finalStates_->GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  // apply previous statement on simulation relation
  result = SymbolicFiniteAutBDD(
    isectFunc(result.GetBDD(), rhs.GetBDD()),
    aut.stateVars_ + aut.stateVars_
  );

  return result;
}

SymbolicFiniteAutCore::SymbolicFiniteAutBDD SymbolicFiniteAutCore::
ComputeSimulation(
  const SymbolicFiniteAutCore & aut
)
{
  // compute initial simulation relation
  SymbolicFiniteAutBDD prevRelation = ComputeInitialSimulation(aut);

  // iterate simulation relation
  SymbolicFiniteAutBDD nextRelation = IterateSimulation(aut,prevRelation);

  while(prevRelation != nextRelation)
  { // iterate simulation relation until it stops changing
    prevRelation = nextRelation;
    nextRelation = IterateSimulation(aut, prevRelation);
  }

  return nextRelation;
}

std::string SymbolicFiniteAutCore::DumpSimulation(
  const SymbolicFiniteAutBDD & sim,
  StateDict *                  stateDict
)
{
  std::string result;

  // number of variables in BDD should be even
  size_t stateVars = sim.GetVars() / 2;

  if (stateDict != nullptr)
  { // explicit serialization
    SymbolicFiniteAutBDD::IntersectionApplyFunctor isectFunc;
    SymbolicFiniteAutBDD Q1, Q2, simLimited;
    size_t size = stateDict->size();

    // generate Q BDD
    for (size_t i = 0; i < size; i++)
    {
      Q1.AddAssignment(SymbolicVarAsgn(stateVars, i));
    }

    // rename Q1 to Q2
    Q2 = Q1.Rename(
      2 * stateVars,
      [stateVars](const size_t var){return var + stateVars;}
    );

    // create Q x Q relation
    Q1.SetBDD(
      isectFunc(
        Q1.GetBDD(),
        Q2.GetBDD()
      )
    );

    // limit simulation relation to a valid set of states
    simLimited = SymbolicFiniteAutBDD(
      isectFunc(
        sim.GetBDD(),
        Q1.GetBDD()
      ),
      2 * stateVars
    );

    AssignmentList list = simLimited.GetAllAssignments(true);
    StateBackTranslStrict StateBackTranslFunc(stateDict->GetReverseMap());

    auto translFunc = [&StateBackTranslFunc](const std::string str){
      return StateBackTranslFunc(SymbolicFiniteAutBDD::FromSymbolic(str));
    };

    for (auto elem : list)
    {
      std::string str = elem.ToString();
      result += translFunc(str.substr(0 , stateVars)) + " <= ";
      result += translFunc(str.substr(stateVars, stateVars)) + "\n";
    }

    return result;
  }

  else
  { // symbolic serialization
    AssignmentList list = sim.GetAllAssignments();

    for (auto elem : list)
    {
      std::string str = elem.ToString();
      result += str.substr(0 , stateVars) + " <= ";
      result += str.substr(stateVars, stateVars) + "\n";
    }

    return result;
  }
}

SymbolicFiniteAutCore::StateBinaryRelation SymbolicFiniteAutCore::
MatrixSimulation(
  const SymbolicFiniteAutBDD & sim
)
{
  // number of variables in BDD should be even
  size_t stateVars = sim.GetVars() / 2;

  // result of this can be redundant
  StateBinaryRelation result(pow(2, stateVars));

  // surely not effective for symbolically loaded automata
  AssignmentList list = sim.GetAllAssignments(true);

  for (auto elem : list)
  {
    std::string str = elem.ToString();

    result.set(
      SymbolicFiniteAutBDD::FromSymbolic(str.substr(0 , stateVars)),
      SymbolicFiniteAutBDD::FromSymbolic(str.substr(stateVars, stateVars)),
      true
    );
  }

  return result;
}