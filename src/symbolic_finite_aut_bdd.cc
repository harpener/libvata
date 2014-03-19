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

SymbolicFiniteAutBDD::SymbolicFiniteAutBDD(
  const SymbolicVarAsgn & asgn
) : mtbdd_(new SymbolicFiniteAutBDD::BDD(asgn, true, false))
{}

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

void SymbolicFiniteAutBDD::AddElement(
  const SymbolicVarAsgn & asgn
)
{
  assert(mtbdd_ != nullptr);

  UnionApplyFunctor unionFunc;
  mtbdd_ = unionFunc(mtbdd_, BDD(asgn, true, false));
}

SymbolicFiniteAutBDD::AssignmentList SymbolicFiniteAutBDD::GetAllElements() const
{
  assert(mtbdd_ != nullptr);

  return mtbdd_->TraverseMtbdd(vec);
}