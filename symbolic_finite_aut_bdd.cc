/*****************************************************************************
 *  VATA Finite Automata Library
 *
 *  Copyright (c) 2014 Jiri Chromecka <xchrom12@stud.fit.vutbr.cz>
 *
 *  Description:
 *  Source file for a BDD proxy.
 *****************************************************************************/
 
#include "symbolic_finite_aut_bdd.hh"

using VATA::SymbolicFiniteAutBDD;

SymbolicFiniteAutBDD::SymbolicFiniteAutBDD(
  const SymbolicFiniteAutBDD & bdd
) : mtbdd_(new SymbolicFiniteAutBDD::BDD(*bdd.mtbdd_))
{}

SymbolicFiniteAutBDD::SymbolicFiniteAutBDD(
  const SymbolicFiniteAutBDD && bdd
) : mtbdd_(std::move(bdd.mtbdd_))
{
  bdd.mtbdd_ = nullptr;
}

explicit SymbolicFiniteAutBDD::SymbolicFiniteAutBDD(
  const SymbolicVarAsgn & asgn
) : mtbdd_(new SymbolicFiniteAutBDD::BDD(asgn, true, false))
{}

explicit SymbolicFiniteAutBDD::SymbolicFiniteAutBDD(
  const std::vector<SymbolicVarAsgn> & vec
)
{
  if (!vec.empty())
  {
    UnionApplyFunctor unionfunc;
    mtbdd_(new BDD(vec[0], true, false))

    for (auto item : vec)
    {
      BDD rhs = new BDD(item, true, false);
      mtbdd_ = unionfunc(mtbdd_, rhs);
    }
  }
}

SymbolicFiniteAutBDD::~SymbolicFiniteAutBDD()
{}

SymbolicFiniteAutBDD & SymbolicFiniteAutBDD::operator=(
  const SymbolicFiniteAutBDD & rhs
)
{
  if (this != &rhs)
  { // if two automata differs
    assert(mtbdd_ != nullptr);

    *mtbdd_ = *rhs.mtbdd_;
  }

  return *this;
}

SymbolicFiniteAutBDD & SymbolicFiniteAutBDD::operator=(
  SymbolicFiniteAutBDD && rhs
)
{
  assert(this != &rhs);
  assert(mtbdd_ != nullptr);
  assert(rhs.mtbdd_ != nullptr);

  mtbdd_ = std::move(rhs.mtbdd_);

  return *this;
}