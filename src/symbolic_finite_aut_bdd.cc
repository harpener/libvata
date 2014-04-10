/*****************************************************************************
 *  VATA Finite Automata Library
 *
 *  Copyright (c) 2014 Jiri Chromecka <xchrom12@stud.fit.vutbr.cz>
 *
 *  Description:
 *    Source file for a BDD proxy.
 *****************************************************************************/
 
#include "symbolic_finite_aut_bdd.hh"

using VATA::SymbolicFiniteAutBDD;

SymbolicFiniteAutBDD::SymbolicFiniteAutBDD()
  : mtbdd_(new BDD(false))
{}

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

SymbolicFiniteAutBDD::BDD SymbolicFiniteAutBDD::GetBDD()
{
  assert(this->mtbdd_ != nullptr);

  return *this->mtbdd_;
}

void SymbolicFiniteAutBDD::SetBDD(BDD bdd)
{
  assert(this->mtbdd_ != nullptr);

  *this->mtbdd_ = bdd;
}

void SymbolicFiniteAutBDD::AddAssignment(
  const std::string & str
)
{
  SymbolicVarAsgn asgn(str);

  this->AddAssignment(asgn);
}

void SymbolicFiniteAutBDD::AddAssignment(
  const SymbolicVarAsgn & asgn
)
{
  assert(this->mtbdd_ != nullptr);

  UnionApplyFunctor unionFunc;
  *this->mtbdd_ = unionFunc(*this->mtbdd_, BDD(asgn, true, false));
}

SymbolicFiniteAutBDD::AssignmentList SymbolicFiniteAutBDD::GetAllAssignments(
  const size_t & vars,
  const bool & concretize
)
const
{
  assert(this->mtbdd_ != nullptr);

  AssignmentList list = this->mtbdd_->GetAllAssignments();

  for (AssignmentList::iterator it = list.begin(); it != list.end(); it++)
  { // completes the assignment according to desired length
    (*it).append(SymbolicVarAsgn(std::string(vars - (*it).length(), 'X')));
  }

  if (concretize)
  { // we want concrete assignments
    AssignmentList tempList;

    for (auto elem : list)
    { // for all assignments create a vector of concrete assignments
      AssignmentList concreteList = elem.GetVectorOfConcreteSymbols();

      for (auto concrete : concreteList)
      { // for all concrete assignments push them back to a temporary vector
        tempList.push_back(concrete);
      }
    }

    list = tempList;
  }

  return list;
}

size_t SymbolicFiniteAutBDD::FromSymbolic(
  const SymbolicVarAsgn & asgn
)
{
  return SymbolicFiniteAutBDD::FromSymbolic(asgn.ToString());
}

size_t SymbolicFiniteAutBDD::FromSymbolic(
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
      case '1': result += pow(2, i); break;
      default: assert(false); break;
    }

    ++i;
  }

  return result;
}

SymbolicFiniteAutBDD::SymbolicVarAsgn SymbolicFiniteAutBDD::MergeTransition(
  const size_t & stateVars,
  const size_t & symbolVars,
  const size_t & lstate,
  const size_t & symbol,
  const size_t & rstate
)
{
  return SymbolicFiniteAutBDD::MergeTransition(
    SymbolicVarAsgn(stateVars, lstate),
    SymbolicVarAsgn(symbolVars, symbol),
    SymbolicVarAsgn(stateVars, rstate)
  );
}

SymbolicFiniteAutBDD::SymbolicVarAsgn SymbolicFiniteAutBDD::MergeTransition(
  const std::string & lstate,
  const std::string & symbol,
  const std::string & rstate
)
{
  return SymbolicFiniteAutBDD::MergeTransition(
    SymbolicVarAsgn(lstate),
    SymbolicVarAsgn(symbol),
    SymbolicVarAsgn(rstate)
  );
}

SymbolicFiniteAutBDD::SymbolicVarAsgn SymbolicFiniteAutBDD::MergeTransition(
  const SymbolicVarAsgn & lstate,
  const SymbolicVarAsgn & symbol,
  const SymbolicVarAsgn & rstate
)
{
  SymbolicVarAsgn asgn(lstate);
  asgn.append(symbol);
  asgn.append(rstate);

  return asgn;
}

void SymbolicFiniteAutBDD::SplitTransition(
  const SymbolicVarAsgn & transition,
  const size_t &                stateVars,
  const size_t &                symbolVars,
  size_t &                lstate,
  size_t &                symbol,
  size_t &                rstate
)
{
  std::string lstateStr;
  std::string symbolStr;
  std::string rstateStr;

  SymbolicFiniteAutBDD::SplitTransition(
    transition,
    stateVars,
    symbolVars,
    lstateStr,
    symbolStr,
    rstateStr
  );

  lstate = SymbolicFiniteAutBDD::FromSymbolic(lstateStr);
  symbol = SymbolicFiniteAutBDD::FromSymbolic(symbolStr);
  rstate = SymbolicFiniteAutBDD::FromSymbolic(rstateStr);
}

void SymbolicFiniteAutBDD::SplitTransition(
  const SymbolicVarAsgn & transition,
  const size_t &          stateVars,
  const size_t &          symbolVars,
  SymbolicVarAsgn &       lstate,
  SymbolicVarAsgn &       symbol,
  SymbolicVarAsgn &       rstate
)
{
  std::string lstateStr;
  std::string symbolStr;
  std::string rstateStr;

  SymbolicFiniteAutBDD::SplitTransition(
    transition,
    stateVars,
    symbolVars,
    lstateStr,
    symbolStr,
    rstateStr
  );

  lstate = SymbolicVarAsgn(lstateStr);
  symbol = SymbolicVarAsgn(symbolStr);
  rstate = SymbolicVarAsgn(rstateStr);
}

void SymbolicFiniteAutBDD::SplitTransition(
  const SymbolicVarAsgn & transition,
  const size_t &          stateVars,
  const size_t &          symbolVars,
  std::string &           lstate,
  std::string &           symbol,
  std::string &           rstate
)
{
  std::string str = transition.ToString();

  lstate = str.substr(0, stateVars);
  symbol = str.substr(stateVars, symbolVars);
  rstate = str.substr(stateVars + symbolVars, stateVars);
}

SymbolicFiniteAutBDD SymbolicFiniteAutBDD::Exists(
  const SymbolicFiniteAutBDD & bdd,
  const size_t & allVars,
  const size_t & vars
)
{
  SymbolicFiniteAutBDD result;

  AssignmentList list = bdd.GetAllAssignments(allVars, true);

  for (auto elem : list)
  {
    std::string str = elem.ToString();
    size_t size = str.size();
    str = str.substr(0, size - vars);
    result.AddAssignment(str);
  }

  return result;
}

SymbolicFiniteAutBDD SymbolicFiniteAutBDD::ForAll(
  const SymbolicFiniteAutBDD & bdd,
  const size_t & allVars,
  const size_t & vars
)
{
  SymbolicFiniteAutBDD result;
  AssignmentList list = bdd.GetAllAssignments(allVars, true);
  std::set<std::string> allSets, prevSets, lastSet;

  for (auto elem : list)
  {
    std::string elemStr = elem.ToString();
    size_t size = elemStr.size();
    allSets.insert(elemStr);
    std::string prevElemStr = elemStr.substr(0, size - vars);
    std::string lastElemStr = elemStr.substr(size - vars, size);
    prevSets.insert(prevElemStr);
    lastSet.insert(lastElemStr);
  }

  for (auto prevElem : prevSets)
  {
    bool found = true;

    for (auto lastElem : lastSet)
    {
      if (allSets.find(prevElem + lastElem) == allSets.end())
      {
        found = false;
      }
    }

    if (found)
    {
      result.AddAssignment(prevElem);
    }
  }

  return result;
}