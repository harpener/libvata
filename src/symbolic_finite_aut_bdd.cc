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
) : mtbdd_(new BDD(*bdd.mtbdd_))
{}

SymbolicFiniteAutBDD::SymbolicFiniteAutBDD(
  SymbolicFiniteAutBDD && bdd
) : mtbdd_(std::move(bdd.mtbdd_))
{
  bdd.mtbdd_ = nullptr;
}

SymbolicFiniteAutBDD::SymbolicFiniteAutBDD(
  const SymbolicVarAsgn & asgn
) : mtbdd_(new BDD(asgn, true, false))
{}

SymbolicFiniteAutBDD::~SymbolicFiniteAutBDD()
{}

SymbolicFiniteAutBDD & SymbolicFiniteAutBDD::operator=(
  const SymbolicFiniteAutBDD & rhs
)
{
  if (this != &rhs)
  { // if two automata differs
    assert(this->mtbdd_ != nullptr);

    *this->mtbdd_ = *rhs.mtbdd_;
  }

  return *this;
}

SymbolicFiniteAutBDD & SymbolicFiniteAutBDD::operator=(
  SymbolicFiniteAutBDD && rhs
)
{
  assert(this != &rhs);
  assert(this->mtbdd_ != nullptr);
  assert(rhs.mtbdd_ != nullptr);

  this->mtbdd_ = std::move(rhs.mtbdd_);

  return *this;
}

void SymbolicFiniteAutBDD::AddAssignment(
  const SymbolicVarAsgn & asgn
)
{
  assert(this->mtbdd_ != nullptr);

  std::cout << "ASGN: " << asgn.ToString() << std::endl << std::endl;

  std::vector<const BDD *> vector;
  UnionApplyFunctor unionFunc;

  const BDD * bdd1 = new BDD(*this->mtbdd_);
  vector.push_back(bdd1);
  std::cout << "BEFORE" << std::endl << std::endl << BDD::DumpToDot(vector) << std::endl << std::endl;
  vector.clear();

  BDD * asgnBDD = new BDD(asgn, true, false);

  const BDD * bdd3 = new BDD(*asgnBDD);
  vector.push_back(bdd3);
  std::cout << "ASGN BDD" << std::endl << std::endl << BDD::DumpToDot(vector) << std::endl << std::endl;
  vector.clear();

  *this->mtbdd_ = unionFunc(*this->mtbdd_, *asgnBDD);

  const BDD * bdd2 = new BDD(*this->mtbdd_);
  vector.push_back(bdd2);
  std::cout << "AFTER" << std::endl << std::endl << BDD::DumpToDot(vector) << std::endl << std::endl;
  vector.clear();
}

SymbolicFiniteAutBDD::AssignmentList SymbolicFiniteAutBDD::GetAllAssignments() const
{
  assert(this->mtbdd_ != nullptr);

  return this->mtbdd_->GetAllAssignments();
}