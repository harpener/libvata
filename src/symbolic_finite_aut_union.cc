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

  // states are renamed by addition of a single variable
  result.stateVars_ = lhs.stateVars_ + 1;
  result.symbolVars_ = lhs.symbolVars_;

  AssignmentList lhsTransitions = lhs.transitions_->GetAllAssignments();
  AssignmentList rhsTransitions = rhs.transitions_->GetAllAssignments();
  AssignmentList lhsInitialStates = lhs.initialStates_->GetAllAssignments();
  AssignmentList rhsInitialStates = rhs.initialStates_->GetAllAssignments();
  AssignmentList lhsFinalStates = lhs.finalStates_->GetAllAssignments();
  AssignmentList rhsFinalStates = rhs.finalStates_->GetAllAssignments();

  for (auto asgn : lhsTransitions)
  {
    SymbolicVarAsgn lstate, symbol, rstate;
    SymbolicFiniteAutBDD::SplitTransition(
      asgn,
      lhs.stateVars_,
      lhs.symbolVars_,
      lstate,
      symbol,
      rstate
    );
    SymbolicVarAsgn newlstate = SymbolicVarAsgn("0");
    newlstate.append(lstate);
    SymbolicVarAsgn newrstate = SymbolicVarAsgn("0");
    newrstate.append(rstate);
    SymbolicVarAsgn newasgn = newlstate;
    newasgn.append(symbol);
    newasgn.append(newrstate);

    pTranslMapLhs->insert(
      std::pair<size_t, size_t>(
        SymbolicFiniteAutBDD::FromSymbolic(lstate),
        SymbolicFiniteAutBDD::FromSymbolic(newlstate)
      )
    );

    pTranslMapLhs->insert(
      std::pair<size_t, size_t>(
        SymbolicFiniteAutBDD::FromSymbolic(rstate),
        SymbolicFiniteAutBDD::FromSymbolic(newrstate)
      )
    );

    result.transitions_->AddAssignment(newasgn);
  }

  for (auto asgn : rhsTransitions)
  {
    SymbolicVarAsgn lstate, symbol, rstate;
    SymbolicFiniteAutBDD::SplitTransition(
      asgn,
      lhs.stateVars_,
      lhs.symbolVars_,
      lstate,
      symbol,
      rstate
    );
    SymbolicVarAsgn newlstate = SymbolicVarAsgn("1");
    newlstate.append(lstate);
    SymbolicVarAsgn newrstate = SymbolicVarAsgn("1");
    newrstate.append(rstate);
    SymbolicVarAsgn newasgn = newlstate;
    newasgn.append(symbol);
    newasgn.append(newrstate);

    pTranslMapRhs->insert(
      std::pair<size_t, size_t>(
        SymbolicFiniteAutBDD::FromSymbolic(lstate),
        SymbolicFiniteAutBDD::FromSymbolic(newlstate)
      )
    );

    pTranslMapRhs->insert(
      std::pair<size_t, size_t>(
        SymbolicFiniteAutBDD::FromSymbolic(rstate),
        SymbolicFiniteAutBDD::FromSymbolic(newrstate)
      )
    );

    result.transitions_->AddAssignment(newasgn);
  }

  for (auto asgn : lhsInitialStates)
  {
    SymbolicVarAsgn newasgn = SymbolicVarAsgn("0");
    newasgn.append(asgn);

    pTranslMapLhs->insert(
      std::pair<size_t, size_t>(
        SymbolicFiniteAutBDD::FromSymbolic(asgn),
        SymbolicFiniteAutBDD::FromSymbolic(newasgn)
      )
    );

    result.initialStates_->AddAssignment(newasgn);
  }

  for (auto asgn : rhsInitialStates)
  {
    SymbolicVarAsgn newasgn = SymbolicVarAsgn("1");
    newasgn.append(asgn);

    pTranslMapRhs->insert(
      std::pair<size_t, size_t>(
        SymbolicFiniteAutBDD::FromSymbolic(asgn),
        SymbolicFiniteAutBDD::FromSymbolic(newasgn)
      )
    );

    result.initialStates_->AddAssignment(newasgn);
  }

  for (auto asgn : lhsFinalStates)
  {
    SymbolicVarAsgn newasgn = SymbolicVarAsgn("0");
    newasgn.append(asgn);

    pTranslMapLhs->insert(
      std::pair<size_t, size_t>(
        SymbolicFiniteAutBDD::FromSymbolic(asgn),
        SymbolicFiniteAutBDD::FromSymbolic(newasgn)
      )
    );

    result.finalStates_->AddAssignment(newasgn);
  }

  for (auto asgn : rhsFinalStates)
  {
    SymbolicVarAsgn newasgn = SymbolicVarAsgn("1");
    newasgn.append(asgn);

    pTranslMapRhs->insert(
      std::pair<size_t, size_t>(
        SymbolicFiniteAutBDD::FromSymbolic(asgn),
        SymbolicFiniteAutBDD::FromSymbolic(newasgn)
      )
    );

    result.finalStates_->AddAssignment(newasgn);
  }

  return result;
}