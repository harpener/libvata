/*****************************************************************************
 *  VATA Finite Automata Library
 *
 *  Copyright (c) 2014 Jiri Chromecka <xchrom12@stud.fit.vutbr.cz>
 *
 *  Description:
 *    Source file for a symbolically represented finite automaton.
 *****************************************************************************/

#include "symbolic_finite_aut_core.hh"

using VATA::SymbolicFiniteAutCore;

SymbolicFiniteAutCore::SymbolicFiniteAutCore()
  : stateVars_(0),
    symbolVars_(0),
    transitions_(new SymbolicFiniteAutBDD),
    initialStates_(new SymbolicFiniteAutBDD),
    finalStates_(new SymbolicFiniteAutBDD)
{}

SymbolicFiniteAutCore::SymbolicFiniteAutCore(
  const SymbolicFiniteAutCore & core
) : stateVars_(core.stateVars_),
    symbolVars_(core.symbolVars_),
    transitions_(new SymbolicFiniteAutBDD(*core.transitions_)),
    initialStates_(new SymbolicFiniteAutBDD(*core.initialStates_)),
    finalStates_(new SymbolicFiniteAutBDD(*core.finalStates_))
{}

SymbolicFiniteAutCore::SymbolicFiniteAutCore(
  SymbolicFiniteAutCore && core
) : stateVars_(std::move(core.stateVars_)),
    symbolVars_(std::move(core.symbolVars_)),
    transitions_(std::move(core.transitions_)),
    initialStates_(std::move(core.initialStates_)),
    finalStates_(std::move(core.finalStates_))
{
  core.transitions_   = nullptr;
  core.initialStates_ = nullptr;
  core.finalStates_   = nullptr;
}

SymbolicFiniteAutCore::~SymbolicFiniteAutCore()
{}

SymbolicFiniteAutCore & SymbolicFiniteAutCore::operator=(
  const SymbolicFiniteAutCore & rhs
)
{
  if (this != & rhs)
  { // if two automata differs
    this->stateVars_      = rhs.stateVars_;
    this->symbolVars_     = rhs.symbolVars_;
    *this->transitions_   = *rhs.transitions_;
    *this->initialStates_ = *rhs.initialStates_;
    *this->finalStates_   = *rhs.finalStates_;
  }

  return *this;
}

SymbolicFiniteAutCore & SymbolicFiniteAutCore::operator=(
  SymbolicFiniteAutCore && rhs
)
{
  assert(this != & rhs);

  this->stateVars_     = std::move(rhs.stateVars_);
  this->symbolVars_    = std::move(rhs.symbolVars_);
  this->transitions_   = std::move(rhs.transitions_);
  this->initialStates_ = std::move(rhs.initialStates_);
  this->finalStates_   = std::move(rhs.finalStates_);

  return *this;
}

void SymbolicFiniteAutCore::LoadFromAutDescSymbolic(
  const AutDescription & desc
)
{
  assert(this->transitions_   != nullptr);
  assert(this->initialStates_ != nullptr);
  assert(this->finalStates_   != nullptr);

  for (auto transition : desc.transitions)
  { // transitions and initial states
    if (transition.first.empty())
    { // an init state is represented as a transition with no left-side state
      if (this->stateVars_ == 0)
      { // state is new
        this->stateVars_ = transition.third.size();
      }

      else if (this->stateVars_ != transition.third.size())
      { // check number of variables in state
        throw std::runtime_error("Number of variables in states differs.");
      }

      this->initialStates_->AddAssignment(SymbolicVarAsgn(transition.third));
    }

    else if (transition.first.size() > 1)
    { // finite automaton transitions are only unary
      throw std::runtime_error("Not a finite automaton");
    }

    else
    { // common transition
      if (this->stateVars_ == 0)
      { // state is new
        this->stateVars_ = transition.first.front().size();
      }

      else if (this->stateVars_ != transition.first.front().size())
      { // check number of variables in state
        throw std::runtime_error("Number of variables in states differs.");
      }

      if (this->symbolVars_ == 0)
      { // symbol is new
        this->symbolVars_ = transition.second.size();
      }

      else if (this->symbolVars_ != transition.second.size())
      { // check number of variables in symbol
        throw std::runtime_error("Number of variables in symbols differs.");
      }

      if (this->stateVars_ == 0)
      { // state is new
        this->stateVars_ = transition.third.size();
      }

      else if (this->stateVars_ != transition.third.size())
      { // check number of variables in state
        throw std::runtime_error("Number of variables in states differs.");
      }

      this->transitions_->AddAssignment(
        SymbolicFiniteAutBDD::MergeTransition(
          transition.first.front(),
          transition.second,
          transition.third
        )
      );
    }
  }

  for (auto finalState : desc.finalStates)
  { // final states
    if (this->stateVars_ == 0)
    { // state is new
      this->stateVars_ = finalState.size();
    }

    else if (this->stateVars_ != finalState.size())
    { // check number of variables in state
      throw std::runtime_error("Number of variables in states differs.");
    }

    this->finalStates_->AddAssignment(
      SymbolicVarAsgn(finalState)
    );
  }
}

SymbolicFiniteAutCore::AutDescription SymbolicFiniteAutCore::
DumpToAutDescSymbolic() const
{
  assert(this->transitions_   != nullptr);
  assert(this->initialStates_ != nullptr);
  assert(this->finalStates_   != nullptr);

  AssignmentList transitionList = transitions_->GetAllAssignments();
  AssignmentList initialStateList = initialStates_->GetAllAssignments();
  AssignmentList finalStatesList = finalStates_->GetAllAssignments();

  AutDescription desc;

  for (auto transition : transitionList)
  { // transitions
    std::string lstate;
    std::string symbol;
    std::string rstate;

    SymbolicFiniteAutBDD::SplitTransition(
      transition,
      this->stateVars_,
      this->symbolVars_,
      lstate,
      symbol,
      rstate
    );

    desc.transitions.insert(
      VATA::Util::Triple<
        std::vector<std::string>,
        std::string,
        std::string
      >(
        std::vector<std::string>{lstate},
        symbol,
        rstate
      )
    );
  }

  for (auto initialState : initialStateList)
  { // initial states
    desc.transitions.insert(
      VATA::Util::Triple<
        std::vector<std::string>,
        std::string,
        std::string
      >(
        std::vector<std::string>(),
        std::string(this->symbolVars_, '0'),
        initialState.ToString()
      )
    );
  }

  for (auto finalState : finalStatesList)
  { // final states
    desc.finalStates.insert(finalState.ToString());
  }

  return desc;
}

void SymbolicFiniteAutCore::AddTransition(
  const SymbolicVarAsgn & lstate,
  const SymbolicVarAsgn & symbol,
  const SymbolicVarAsgn & rstate
)
{
  assert(this->transitions_ != nullptr);

  if (this->stateVars_ == 0)
  { // state is new
    this->stateVars_ = lstate.length();
  }

  else if (this->stateVars_ != lstate.length())
  { // check number of variables in state
    throw std::runtime_error("Number of variables in states differs.");
  }

  if (this->symbolVars_ == 0)
  { // symbol is new
    this->symbolVars_ = symbol.length();
  }

  else if (this->symbolVars_ != symbol.length())
  { // check number of variables in symbol
    throw std::runtime_error("Number of variables in symbols differs.");
  }

  if (this->stateVars_ == 0)
  { // state is new
    this->stateVars_ = rstate.length();
  }

  else if (this->stateVars_ != rstate.length())
  { // check number of variables in state
    throw std::runtime_error("Number of variables in states differs.");
  }

  this->transitions_->AddAssignment(
    SymbolicFiniteAutBDD::MergeTransition(
      lstate,
      symbol,
      rstate
    )
  );
}

void SymbolicFiniteAutCore::AddInitialState(
  const SymbolicVarAsgn & state
)
{
  assert(this->initialStates_ != nullptr);

  if (this->stateVars_ == 0)
  { // state is new
    this->stateVars_ = state.length();
  }

  else if (this->stateVars_ != state.length())
  { // check number of variables in state
    throw std::runtime_error("Number of variables in states differs.");
  }

  this->initialStates_->AddAssignment(state);
}

void SymbolicFiniteAutCore::AddFinalState(
  const SymbolicVarAsgn & state
)
{
  assert(this->finalStates_ != nullptr);

  if (this->stateVars_ == 0)
  { // state is new
    this->stateVars_ = state.length();
  }

  else if (this->stateVars_ != state.length())
  { // check number of variables in state
    throw std::runtime_error("Number of variables in states differs.");
  }

  this->finalStates_->AddAssignment(state);
}