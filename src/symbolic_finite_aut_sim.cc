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

SymbolicFiniteAutCore::SymbolicFiniteAutBDD SymbolicFiniteAutCore::ComputeInitialRelation(
  const SymbolicFiniteAutCore & aut
)
{
  SymbolicFiniteAutBDD result = SymbolicFiniteAutBDD::Exists(
    *(aut.transitions_),
    aut.stateVars_ + aut.symbolVars_ + aut.stateVars_,
    aut.stateVars_
  );

  return result;
}

std::string SymbolicFiniteAutCore::ComputeSimulation(
  const SymbolicFiniteAutCore & aut
)
{
  std::string result = "State relation:\n";

  SymbolicFiniteAutBDD initialRelation = ComputeInitialRelation(aut);

  AssignmentList list = initialRelation.GetAllAssignments(
    aut.stateVars_ + aut.symbolVars_,
    true
  );

  for (auto elem : list)
  {
    std::string str = elem.ToString();
    std::string str1 = str.substr(0, aut.stateVars_);
    std::string str2 = str.substr(
      aut.stateVars_,
      aut.stateVars_ + aut.symbolVars_
    );
    result += str1 + " is simulated by " + str2 + "\n";
  }

  return result;
}