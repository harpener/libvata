/*****************************************************************************
 *  VATA Finite Automata Library
 *
 *  Copyright (c) 2014 Jiri Chromecka <xchrom12@stud.fit.vutbr.cz>
 *
 *  Description:
 *  Source file for a symbolically represented FA.
 *****************************************************************************/

#include "symbolic_finite_aut_core.hh"

using VATA::SymbolicFiniteAutCore;

SymbolicFiniteAutCore::SymbolicFiniteAutCore()
  : stateVars_(0),
    symbolVars_(0),
    SymTransitionsVec_(new SymTransitionsVec()),
    SymInitialStatesVec_(new SymInitialStatesVec()),
    SymFinalStatesVec_(new SymFinalStatesVec())
{}

SymbolicFiniteAutCore::SymbolicFiniteAutCore(
  const SymbolicFiniteAutCore & core
) : stateVars_(core.stateVars_),
    symbolVars_(core.symbolVars_),
    SymTransitionsVec_(new SymTransitionsVec(*core.SymTransitionsVec_)),
    SymInitialStatesVec_(new SymInitialStatesVec(*core.SymInitialStatesVec_)),
    SymFinalStatesVec_(new SymFinalStatesVec(*core.SymFinalStatesVec_))
{}

SymbolicFiniteAutCore::SymbolicFiniteAutCore(
  SymbolicFiniteAutCore && core
) : stateVars_(std::move(core.stateVars_)),
    symbolVars_(std::move(core.symbolVars_)),
    SymTransitionsVec_(std::move(core.SymTransitionsVec_)),
    SymInitialStatesVec_(std::move(core.SymInitialStatesVec_)),
    SymFinalStatesVec_(std::move(core.SymFinalStatesVec_))
{
  core.SymTransitionsVec_   = nullptr;
  core.SymInitialStatesVec_ = nullptr;
  core.SymFinalStatesVec_   = nullptr;
}

SymbolicFiniteAutCore::~SymbolicFiniteAutCore()
{}

SymbolicFiniteAutCore & SymbolicFiniteAutCore::operator=(
  const SymbolicFiniteAutCore & rhs
)
{
  if (this != & rhs)
  { // if two automata differs
    this->stateVars_            = rhs.stateVars_;
    this->symbolVars_           = rhs.symbolVars_;
    *this->SymTransitionsVec_   = *rhs.SymTransitionsVec_;
    *this->SymInitialStatesVec_ = *rhs.SymInitialStatesVec_;
    *this->SymFinalStatesVec_   = *rhs.SymFinalStatesVec_;
  }

  return *this;
}

SymbolicFiniteAutCore & SymbolicFiniteAutCore::operator=(
  SymbolicFiniteAutCore && rhs
)
{
  assert(this != & rhs);

  this->stateVars_           = std::move(rhs.stateVars_);
  this->symbolVars_          = std::move(rhs.symbolVars_);
  this->SymTransitionsVec_   = std::move(rhs.SymTransitionsVec_);
  this->SymInitialStatesVec_ = std::move(rhs.SymInitialStatesVec_);
  this->SymFinalStatesVec_   = std::move(rhs.SymFinalStatesVec_);

  return *this;
}

void SymbolicFiniteAutCore::LoadFromAutDescSymbolic(
  const AutDescription & desc
)
{
  assert(this->SymTransitionsVec_   != nullptr);
  assert(this->SymInitialStatesVec_ != nullptr);
  assert(this->SymFinalStatesVec_   != nullptr);

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

      SymStateType state = SymbolicVarAsgn(transition.third);

      this->SymInitialStatesVec_->push_back(state);
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

      SymStateType lstate = SymbolicVarAsgn(transition.first.front());
      SymSymbolType symbol = SymbolicVarAsgn(transition.second);
      SymStateType rstate = SymbolicVarAsgn(transition.third);
      SymTransitionType triple = this->MergeTransition(lstate, symbol, rstate);

      this->SymTransitionsVec_->push_back(triple);
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

    SymStateType state = SymbolicVarAsgn(finalState);

    this->SymFinalStatesVec_->push_back(state);
  }
}

SymbolicFiniteAutCore::AutDescription SymbolicFiniteAutCore::DumpToAutDescSymbolic() const
{
  assert(this->SymTransitionsVec_   != nullptr);
  assert(this->SymInitialStatesVec_ != nullptr);
  assert(this->SymFinalStatesVec_   != nullptr);

  AutDescription desc;

  for (auto transition : *this->SymTransitionsVec_)
  { // transitions
    std::string lstate;
    std::string symbol;
    std::string rstate;

    this->SplitTransition(transition, lstate, symbol, rstate);

    std::vector<std::string> tuple{lstate};
    VATA::Util::Triple<
      std::vector<std::string>,
      std::string,
      std::string
    > triple(tuple, symbol, rstate);

    desc.transitions.insert(triple);
  }

  for (auto initialState : *this->SymInitialStatesVec_)
  { // initial states
    std::string state = initialState.ToString();

    std::vector<std::string> tuple;
    std::string str(this->symbolVars_, '0');
    VATA::Util::Triple<
      std::vector<std::string>,
      std::string,
      std::string
    > triple(tuple, str, state);

    desc.transitions.insert(triple);
  }

  for (auto finalState : *this->SymFinalStatesVec_)
  { // final states
    std::string state = finalState.ToString();

    desc.finalStates.insert(state);
  }

  return desc;
}

void SymbolicFiniteAutCore::AddTransition(
  const std::string & lstate,
  const std::string & symbol,
  const std::string & rstate
)
{
  assert(this->SymTransitionsVec_ != nullptr);

  if (this->stateVars_ == 0)
  { // state is new
    this->stateVars_ = lstate.size();
  }

  else if (this->stateVars_ != lstate.size())
  { // check number of variables in state
    throw std::runtime_error("Number of variables in states differs.");
  }

  if (this->symbolVars_ == 0)
  { // symbol is new
    this->symbolVars_ = symbol.size();
  }

  else if (this->symbolVars_ != symbol.size())
  { // check number of variables in symbol
    throw std::runtime_error("Number of variables in symbols differs.");
  }

  if (this->stateVars_ == 0)
  { // state is new
    this->stateVars_ = rstate.size();
  }

  else if (this->stateVars_ != rstate.size())
  { // check number of variables in state
    throw std::runtime_error("Number of variables in states differs.");
  }

  SymStateType symlstate = SymbolicVarAsgn(lstate);
  SymSymbolType symsymbol = SymbolicVarAsgn(symbol);
  SymStateType symrstate = SymbolicVarAsgn(rstate);
  SymTransitionType triple = this->MergeTransition(symlstate, symsymbol, symrstate);

  this->SymTransitionsVec_->push_back(triple);
}

void SymbolicFiniteAutCore::AddInitialState(
  const std::string & state
)
{
  assert(this->SymInitialStatesVec_ != nullptr);

  if (this->stateVars_ == 0)
  { // state is new
    this->stateVars_ = state.size();
  }

  else if (this->stateVars_ != state.size())
  { // check number of variables in state
    throw std::runtime_error("Number of variables in states differs.");
  }

  SymStateType symstate = SymbolicVarAsgn(state);

  this->SymInitialStatesVec_->push_back(symstate);
}

void SymbolicFiniteAutCore::AddFinalState(
  const std::string & state
)
{
  assert(this->SymFinalStatesVec_ != nullptr);

  if (this->stateVars_ == 0)
  { // state is new
    this->stateVars_ = state.size();
  }

  else if (this->stateVars_ != state.size())
  { // check number of variables in state
    throw std::runtime_error("Number of variables in states differs.");
  }

  SymStateType symstate = SymbolicVarAsgn(state);

  this->SymFinalStatesVec_->push_back(symstate);
}

size_t SymbolicFiniteAutCore::SymbolicVarAsgn2Size_t(
  const SymbolicVarAsgn & asgn
)
{
  std::string str = asgn.ToString();

  return SymbolicVarAsgn2Size_t(str);
}

size_t SymbolicFiniteAutCore::SymbolicVarAsgn2Size_t(
  const std::string & str
)
{
  size_t result = 0;
  int i = 0;

  for(std::string::const_iterator it = str.begin(); it != str.end(); it++)
  { // iterate through string
    switch(*it)
    {
      case '0': break;
      case '1': result += pow(2, i);
      default: assert(false); break;
    }

    ++i;
  }

  return result;
}

SymbolicFiniteAutCore::SymbolicVarAsgn & SymbolicFiniteAutCore::MergeTransition(
  const StateType & lstate,
  const SymbolType & symbol,
  const StateType & rstate
) const
{
  SymStateType symLeftState  = SymbolicVarAsgn(this->stateVars_, lstate);
  SymSymbolType symSymbol    = SymbolicVarAsgn(this->symbolVars_, symbol);
  SymStateType symRightState = SymbolicVarAsgn(this->stateVars_, rstate);

  return this->MergeTransition(symLeftState, symSymbol, symRightState);
}

SymbolicFiniteAutCore::SymTransitionType & SymbolicFiniteAutCore::MergeTransition(
  const SymStateType & lstate,
  const SymSymbolType & symbol,
  const SymStateType & rstate
) const
{
  SymbolicVarAsgn * asgn  = new SymbolicVarAsgn(lstate);
  asgn->append(symbol);
  asgn->append(rstate);

  return *asgn;
}

void SymbolicFiniteAutCore::SplitTransition(
  const SymTransitionType & transition,
  StateType & lstate,
  SymbolType & symbol,
  StateType & rstate
) const
{
  std::string strlstate;
  std::string strsymbol;
  std::string strrstate;

  this->SplitTransition(transition, strlstate, strsymbol, strrstate);

  lstate = this->SymbolicVarAsgn2Size_t(strlstate);
  symbol = this->SymbolicVarAsgn2Size_t(strsymbol);
  rstate = this->SymbolicVarAsgn2Size_t(strrstate);
}

void SymbolicFiniteAutCore::SplitTransition(
  const SymTransitionType & transition,
  std::string & lstate,
  std::string & symbol,
  std::string & rstate
) const
{
  size_t lstateOffset = 0;
  size_t symbolOffset = lstateOffset + this->stateVars_;
  size_t rstateOffset = symbolOffset + this->symbolVars_;

  std::string str = transition.ToString();

  lstate = str.substr(lstateOffset, this->stateVars_);
  symbol = str.substr(symbolOffset, this->symbolVars_);
  rstate = str.substr(rstateOffset, this->stateVars_);
}