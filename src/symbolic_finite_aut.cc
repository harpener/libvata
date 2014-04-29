/*****************************************************************************
 *  VATA Finite Automata Library
 *
 *  Copyright (c) 2014 Jiri Chromecka <xchrom12@stud.fit.vutbr.cz>
 *
 *  Description:
 *    Source file for a symbolically represented finite automaton (facade).
 *****************************************************************************/

#include <vata/symbolic_finite_aut.hh>
#include "symbolic_finite_aut_core.hh"
#include "symbolic_loadable_aut.hh"

using VATA::SymbolicFiniteAut;
using VATA::SymbolicFiniteAutBDD;

SymbolicFiniteAut::SymbolicFiniteAut()
  : core_(new Core(Core::ParentAut()))
{}

SymbolicFiniteAut::SymbolicFiniteAut(
  const SymbolicFiniteAut & aut
) : core_(new Core(*aut.core_))
{}

SymbolicFiniteAut::SymbolicFiniteAut(
  SymbolicFiniteAut && aut
) : core_(std::move(aut.core_))
{
  aut.core_ = nullptr;
}

SymbolicFiniteAut::~SymbolicFiniteAut()
{}

SymbolicFiniteAut & SymbolicFiniteAut::operator=(
  const SymbolicFiniteAut & rhs
)
{
  if (this != &rhs)
  { // if two automata differs
    assert(this->core_ != nullptr);

    *this->core_ = *rhs.core_;
  }

  return *this;
}

SymbolicFiniteAut & SymbolicFiniteAut::operator=(
  SymbolicFiniteAut && rhs
)
{
  assert(this != &rhs);
  assert(this->core_ != nullptr);
  assert(rhs.core_ != nullptr);

  this->core_ = std::move(rhs.core_);

  return *this;
}

SymbolicFiniteAut::SymbolicFiniteAut(
  Core && core
) : core_(new Core(std::move(core)))
{}

void SymbolicFiniteAut::LoadFromString(
  VATA::Parsing::AbstrParser & parser,
  const std::string &          str,
  const std::string &          params
)
{
  assert(this->core_ != nullptr);

  return this->core_->LoadFromString(
    parser,
    str,
    params
  );
}

void SymbolicFiniteAut::LoadFromString(
  VATA::Parsing::AbstrParser & parser,
  const std::string &          str,
  StateDict &                  stateDict,
  SymbolDict &                 symbolDict,
  const std::string &          params
)
{
  assert(this->core_ != nullptr);

  return this->core_->LoadFromString(
    parser,
    str,
    stateDict,
    symbolDict,
    params
  );
}

void SymbolicFiniteAut::LoadFromString(
  VATA::Parsing::AbstrParser & parser,
  const std::string &          str,
  StringToStateTranslWeak &    stateTransl,
  StringToSymbolTranslWeak &   symbolTransl,
  const std::string &          params
)
{
  assert(this->core_ != nullptr);

  return this->core_->LoadFromString(
    parser,
    str,
    stateTransl,
    symbolTransl,
    params
  );
}

void SymbolicFiniteAut::LoadFromAutDesc(
  const AutDescription & desc,
  const std::string &    params
)
{
  assert(this->core_ != nullptr);

  return this->core_->LoadFromAutDesc(
    desc,
    params
  );
}

void SymbolicFiniteAut::LoadFromAutDesc(
  const AutDescription & desc,
  StateDict &            stateDict,
  SymbolDict &           symbolDict,
  const std::string &    params
)
{
  assert(this->core_ != nullptr);

  return this->core_->LoadFromAutDesc(
    desc,
    stateDict,
    symbolDict,
    params
  );
}

void SymbolicFiniteAut::LoadFromAutDesc(
  const AutDescription &     desc,
  StringToStateTranslWeak &  stateTransl,
  StringToSymbolTranslWeak & symbolTransl,
  const std::string &        params
)
{
  assert(this->core_ != nullptr);

  return this->core_->LoadFromAutDesc(
    desc,
    stateTransl,
    symbolTransl,
    params
  );
}

std::string SymbolicFiniteAut::DumpToString(
  const std::string & params
) const
{
  assert(this->core_ != nullptr);

  return this->core_->DumpToString(params);
}

std::string SymbolicFiniteAut::DumpToString(
  VATA::Serialization::AbstrSerializer & serializer,
  const std::string &                    params
) const
{
  assert(this->core_ != nullptr);

  return this->core_->DumpToString(
    serializer,
    params
  );
}

std::string SymbolicFiniteAut::DumpToString(
  VATA::Serialization::AbstrSerializer & serializer,
  const StateDict &                      stateDict,
  const SymbolDict &                     symbolDict,
  const std::string &                    params
) const
{
  assert(this->core_ != nullptr);

  return this->core_->DumpToString(
    serializer,
    stateDict,
    symbolDict,
    params
  );
}

std::string SymbolicFiniteAut::DumpToString(
  VATA::Serialization::AbstrSerializer & serializer,
  StateBackTranslStrict &                stateBackTransl,
  SymbolBackTranslStrict &               symbolBackTransl,
  const std::string &                    params
) const
{
  assert(this->core_ != nullptr);

  return this->core_->DumpToString(
    serializer,
    stateBackTransl,
    symbolBackTransl,
    params);
}

SymbolicFiniteAut::AutDescription SymbolicFiniteAut::DumpToAutDesc(
  const std::string & params
) const
{
  return this->core_->DumpToAutDesc(params);
}

SymbolicFiniteAut::AutDescription SymbolicFiniteAut::DumpToAutDesc(
  const StateDict &   stateDict,
  const SymbolDict &  symbolDict,
  const std::string & params
) const
{
  return this->core_->DumpToAutDesc(
    stateDict,
    symbolDict,
    params
  );
}

SymbolicFiniteAut::AutDescription SymbolicFiniteAut::DumpToAutDesc(
  StateBackTranslStrict &  stateBackTransl,
  SymbolBackTranslStrict & symbolBackTransl,
  const std::string &      params
) const
{
  return this->core_->DumpToAutDesc(
    stateBackTransl,
    symbolBackTransl,
    params
  );
}

void SymbolicFiniteAut::AddTransition(
  const std::string & lstate,
  const std::string & symbol,
  const std::string & rstate
)
{
  assert(this->core_ != nullptr);

  this->core_->AddTransition(
    SymbolicVarAsgn(lstate),
    SymbolicVarAsgn(symbol),
    SymbolicVarAsgn(rstate)
  );
}

void SymbolicFiniteAut::AddTransition(
    const SymbolicVarAsgn & lstate,
    const SymbolicVarAsgn & symbol,
    const SymbolicVarAsgn & rstate
)
{
  assert(this->core_ != nullptr);

  this->core_->AddTransition(lstate, symbol, rstate);
}

void SymbolicFiniteAut::AddInitialState(
  const std::string & state
)
{
  assert(this->core_ != nullptr);

  this->core_->AddInitialState(SymbolicVarAsgn(state));
}

void SymbolicFiniteAut::AddInitialState(
  const SymbolicVarAsgn & state
)
{
  assert(this->core_ != nullptr);

  this->core_->AddInitialState(state);
}

void SymbolicFiniteAut::AddFinalState(
  const std::string & state
)
{
  assert(this->core_ != nullptr);

  this->core_->AddFinalState(SymbolicVarAsgn(state));
}

void SymbolicFiniteAut::AddFinalState(
  const SymbolicVarAsgn & state
)
{
  assert(this->core_ != nullptr);

  this->core_->AddFinalState(state);
}

SymbolicFiniteAut SymbolicFiniteAut::Union(
		const SymbolicFiniteAut & lhs,
		const SymbolicFiniteAut & rhs
)
{
  assert(lhs.core_ != nullptr);
  assert(rhs.core_ != nullptr);

  return SymbolicFiniteAut(
    Core::Union(
      *lhs.core_,
      *rhs.core_
    )
  );
}

void SymbolicFiniteAut::GenUnionTransl(
  const StateDict & lhs,
  const StateDict & rhs,
  StateToStateMap * lhsUnion,
  StateToStateMap * rhsUnion
)
{
  assert(lhsUnion != nullptr);
  assert(rhsUnion != nullptr);

  Core::GenUnionTransl(lhs, rhs, lhsUnion, rhsUnion);
}

SymbolicFiniteAut SymbolicFiniteAut::Intersection(
		const SymbolicFiniteAut & lhs,
		const SymbolicFiniteAut & rhs
)
{
  assert(lhs.core_ != nullptr);
  assert(rhs.core_ != nullptr);

  return SymbolicFiniteAut(
    Core::Intersection(
      *lhs.core_,
      *rhs.core_
    )
  );
}

void SymbolicFiniteAut::GenIsectTransl(
  const StateDict & lhs,
  const StateDict & rhs,
  ProductTranslMap * stateDictIsect
)
{
  assert(stateDictIsect != nullptr);

  Core::GenIsectTransl(lhs, rhs, stateDictIsect);
}

SymbolicFiniteAut::SymbolicFiniteAutBDD SymbolicFiniteAut::ComputeSimulation(
  const SymbolicFiniteAut & aut
)
{
  assert(aut.core_ != nullptr);

  return Core::ComputeSimulation(*aut.core_);
}

std::string SymbolicFiniteAut::DumpSimulation(
  const SymbolicFiniteAutBDD & sim,
  StateDict *                  stateDict
)
{
  return Core::DumpSimulation(sim, stateDict);
}