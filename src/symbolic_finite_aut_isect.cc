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

  ProductTranslMap translMap;

  if (pTranslMap == nullptr)
  { // unused state translation
    pTranslMap = &translMap;
  }

  std::string lhsSpace(lhs.stateVars_, 'X');
  std::string rhsSpace(rhs.stateVars_, 'X');

  SymbolicFiniteAutCore tempLhs = lhs.ReindexStates(rhsSpace, lhs.stateVars_);
  SymbolicFiniteAutCore tempRhs = rhs.ReindexStates(lhsSpace, 0);

  result.stateVars_ = tempLhs.stateVars_;
  result.symbolVars_ = tempLhs.symbolVars_;

  SymbolicFiniteAutBDD::IntersectApplyFunctor isectFunc;

  result.transitions_->SetBDD(
    isectFunc(tempLhs.transitions_->GetBDD(), tempRhs.transitions_->GetBDD())
  );
  result.initialStates_->SetBDD(
    isectFunc(
      tempLhs.initialStates_->GetBDD(), tempRhs.initialStates_->GetBDD()
    )
  );
  result.finalStates_->SetBDD(
    isectFunc(tempLhs.finalStates_->GetBDD(), tempRhs.finalStates_->GetBDD())
  );

  // intersection state translation after the operation

  AssignmentList transitionsList = result.transitions_->GetAllAssignments();
  AssignmentList initialStatesList = result.initialStates_->GetAllAssignments();
  AssignmentList finalStatesList = result.finalStates_->GetAllAssignments();

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

  return result;
}