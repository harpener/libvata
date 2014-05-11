/*****************************************************************************
 *  VATA Finite Automata Library
 *
 *  Copyright (c) 2014 Jiri Chromecka <xchrom12@stud.fit.vutbr.cz>
 *
 *  Description:
 *    Header file for a BDD proxy.
 *****************************************************************************/

#ifndef _VATA_SYMBOLIC_FINITE_AUT_BDD_HH_
#define _VATA_SYMBOLIC_FINITE_AUT_BDD_HH_

#include <vata/sym_var_asgn.hh>
#include "mtbdd/ondriks_mtbdd.hh"
#include "mtbdd/bool_apply2func.hh"

/// @brief  VATA library namespace
namespace VATA
{
  class SymbolicFiniteAutBDD;
}

GCC_DIAG_OFF(effc++) // non-virtual destructors warnings suppress

/// @brief  Proxy class for a symbolic BDD representation
class VATA::SymbolicFiniteAutBDD
{

GCC_DIAG_ON(effc++)

public: // public data types

  /// @brief  Symbolic assignment
  using SymbolicVarAsgn = VATA::SymbolicVarAsgn;

  /// @brief  List of symbolic assignments
  using AssignmentList = VATA::SymbolicVarAsgn::AssignmentList;

public: // private data types

  /// @brief  Representation of a BDD
  using BDD = VATA::MTBDDPkg::OndriksMTBDD<bool>;

  /// @brief  BDD pointer
  using BDDPtr = std::unique_ptr<BDD>;

public: // apply functors

  /// @brief  Negation functor
  using NegationApplyFunctor = VATA::MTBDDPkg::NegationApplyFunctor;

  /// @brief  Union functor
  using  UnionApplyFunctor = VATA::MTBDDPkg::UnionApplyFunctor;

  /// @brief  Intersection functor
  using IntersectionApplyFunctor = VATA::MTBDDPkg::IntersectionApplyFunctor;

  /// @brief  Consequence functor
  using ConsequenceApplyFunctor = VATA::MTBDDPkg::ConsequenceApplyFunctor;

  /// @brief  Equivalence functor
  using EquivalenceApplyFunctor = VATA::MTBDDPkg::EquivalenceApplyFunctor;

private: // data members

  /// @brief  Number of variables in assignment
  size_t vars_;

  /// @brief  BDD instance
  BDDPtr mtbdd_;

public: // instantiation

  /// @brief  Default constructor
  SymbolicFiniteAutBDD();

  /**
   * @brief  Copy constructor
   *
   * @param[in]  bdd  BDD to copy
   */
  SymbolicFiniteAutBDD(
    const SymbolicFiniteAutBDD & bdd
  );

  /**
   * @brief  Move constructor
   *
   * @param[in]  bdd  BDD to move
   */
  SymbolicFiniteAutBDD(
    SymbolicFiniteAutBDD && bdd
  );

  /**
   * @brief  BDD constructor
   *
   * @param[in]  bdd   Member representing BDD
   * @param[in]  vars  Member representing number of variables in assignments
   */
  explicit SymbolicFiniteAutBDD(
    const BDD &    bdd,
    const size_t & vars
  );

  /**
   * @brief  Assignment constructor
   *
   * @param[in]  asgn  Assignment to be represented in a BDD
   */
  explicit SymbolicFiniteAutBDD(
    const SymbolicVarAsgn & asgn
  );

  /// @brief  Default destructor
  ~SymbolicFiniteAutBDD();

  /**
   * @brief  Copy assignment operator
   *
   * @param[in]  rhs  BDD to copy
   *
   * @return  BDD
   */
  SymbolicFiniteAutBDD & operator=(
    const SymbolicFiniteAutBDD & rhs
  );

  /**
   * @brief  Move assignment operator
   *
   * @param[in]  rhs  BDD to move
   *
   * @return  BDD
   */
  SymbolicFiniteAutBDD & operator=(
    SymbolicFiniteAutBDD && rhs
  );

  /**
   * @brief  Compare operator
   *
   * @param[in]  rhs  BDD to compare
   *
   * @return  True if BDDs are equivalent, false if they aren't
   */
  bool operator==(
    const SymbolicFiniteAutBDD & rhs
  ) const;

  /**
   * @brief  Compare operator
   *
   * @param[in]  rhs  BDD to compare
   *
   * @return  False if BDDs are equivalent, true if they aren't
   */
  bool operator!=(
    const SymbolicFiniteAutBDD & rhs
  ) const;

public: // getters and setters

  /**
   * @brief  Getter of number of variables
   *
   * @return  Number of variables
   */
  const size_t & GetVars() const;

  /**
   * @brief  Setter of number of variables
   *
   * @param[in]  vars  Assigned number of variables
   */
  void SetVars(
    const size_t & vars
  );

  /**
   * @brief  Getter of BDD
   *
   * @return  BDD
   */
  const BDD & GetBDD() const;

  /**
   * @brief  Setter of BDD
   *
   * @param[in]  bdd  Assigned BDD
   */
  void SetBDD(
    const BDD & bdd
  );

  /**
   * @brief  Getter of default value
   *
   * @return Default value
   */
  const bool & GetDefaultValue() const;

  /**
   * @brief  Setter of default value in BDD
   *
   * @param[in]  defaultValue  Default value
   */
  void SetDefaultValue(
    const bool & defaultValue
  );

public: // methods

  /**
   * @brief  Adds an assignment to a BDD
   *
   * @param[in]  str  Assignment (string) to be added
   */
  void AddAssignment(
    const std::string & str
  );

  /**
   * @brief  Adds an assignment to a BDD
   *
   * @param[in]  asgn  Assignment to be added
   */
  void AddAssignment(
    const SymbolicVarAsgn & asgn
  );

  /**
   * @brief  Obtain all assignments in a BDD
   *
   * @param  concretize  Concretize all assignments
   *
   * @return  List of assignments.
   */
  AssignmentList GetAllAssignments(
    const bool & concretize = false
  ) const;

  /**
   * @brief  Conversion from SymbolicVarAsgn to size_t
   *
   * @param[in]  asgn  Symbolic representation
   *
   * @return  Internal representation
   */
  static size_t FromSymbolic(
    const SymbolicVarAsgn & asgn
  );

  /**
   * @brief  Conversion from string to size_t.
   *
   * @param[in]  str  Symbolic (string) representation
   *
   * @return  Internal representation
   */
  static size_t FromSymbolic(
    const std::string & str
  );

  /**
   * @brief  Merges a left-side state, a symbol and a right-side state
             into a transition
   *
   * @param[in]  stateVars   Number of variables in desired symbolic
                             representation of a state
   * @param[in]  symbolVars  Number of variables in desired symbolic
                             representation of a symbol
   * @param[in]  lstate      Internal representation of a left-side state
   * @param[in]  symbol      Internal representation of a symbol
   * @param[in]  rstate      Internal representation of a right-side state
   *
   * @return  Symbolic representation of a transition
   */
  static SymbolicVarAsgn MergeTransition(
    const size_t & stateVars,
    const size_t & symbolVars,
    const size_t & lstate,
    const size_t & symbol,
    const size_t & rstate
  );

  /**
   * @brief  Merges a left-side state, a symbol and a right-side state
             into a transition
   *
   * @param[in]  lstate  Symbolic (string) representation of a left-side state
   * @param[in]  symbol  Symbolic (string) representation of a symbol
   * @param[in]  rstate  Symbolic (string) representation of a right-side state
   *
   * @return  Symbolic representation of a transition
   */
  static SymbolicVarAsgn MergeTransition(
    const std::string & lstate,
    const std::string & symbol,
    const std::string & rstate
  );

  /**
   * @brief  Merges a left-side state, a symbol and a right-side state
             into a transition
   *
   * @param[in]  lstate  Symbolic representation of a left-side state
   * @param[in]  symbol  Symbolic representation of a symbol
   * @param[in]  rstate  Symbolic representation of a right-side state
   *
   * @return  Symbolic representation of a transition
   */
  static SymbolicVarAsgn MergeTransition(
    const SymbolicVarAsgn & lstate,
    const SymbolicVarAsgn & symbol,
    const SymbolicVarAsgn & rstate
  );

  /**
   * @brief  Splits a transition into a left-side state, a symbol
             and a right-side state
   *
   * @param[in]  transition  Symbolic representation of a transition
   * @param[in]  stateVars   Number of variables in symbolic representation
                             of a state
   * @param[in]  symbolVars  Number of variables in symbolic representation
                             of a symbol
   * @param[out]  lstate     Internal representation of a left-side state
   * @param[out]  symbol     Internal representation of a symbol
   * @param[out]  rstate     Internal representation of a right-side state
   */
  static void SplitTransition(
    const SymbolicVarAsgn & transition,
    const size_t &          stateVars,
    const size_t &          symbolVars,
    size_t &                lstate,
    size_t &                symbol,
    size_t &                rstate
  );

  /**
   * @brief  Splits a transition into a left-side state, a symbol
             and a right-side state
   *
   * @param[in]  transition  Symbolic representation of a transition
   * @param[in]  stateVars   Number of variables in symbolic representation
                             of a state
   * @param[in]  symbolVars  Number of variables in symbolic representation
                             of a symbol
   * @param[out]  lstate     Symbolic representation of a left-side state
   * @param[out]  symbol     Symbolic representation of a symbol
   * @param[out]  rstate     Symbolic representation of a right-side state
   */
  static void SplitTransition(
    const SymbolicVarAsgn & transition,
    const size_t &          stateVars,
    const size_t &          symbolVars,
    SymbolicVarAsgn &       lstate,
    SymbolicVarAsgn &       symbol,
    SymbolicVarAsgn &       rstate
  );

  /**
   * @brief  Splits a transition into a left-side state, a symbol
             and a right-side state
   *
   * @param[in]  transition  Symbolic representation of a transition
   * @param[in]  stateVars   Number of variables in symbolic representation
                             of a state
   * @param[in]  symbolVars  Number of variables in symbolic representation
                             of a symbol
   * @param[out]  lstate     Symbolic (string) representation
                             of a left-side state
   * @param[out]  symbol     Symbolic (string) representation of a symbol
   * @param[out]  rstate     Symbolic (string) representation
                             of a right-side state
   */
  static void SplitTransition(
    const SymbolicVarAsgn & transition,
    const size_t &          stateVars,
    const size_t &          symbolVars,
    std::string &           lstate,
    std::string &           symbol,
    std::string &           rstate
  );

	/**
	 * @brief  Project out variables specified by a predicate
	 *
	 * This method returns an MTBDD with removed nodes that satisfy the @p pred
	 * predicate. If such a node is encountered in a traversal of the MTBDD, its
	 * children are combined using a binary apply operation that performs the @p
	 * applyFunc function on the leaves.
	 *
   * @param[in]  vars       Result number of variables of assignments
	 * @param[in]  pred       The predicate that denotes the nodes to be removed
	 * @param[in]  applyFunc  The apply functor to apply on children of a node
	 *                        projected out
	 *
	 * @returns  An MTBDD that corresponds to the projection given by the input
	 *           parameters
	 */
	template <class VarPredicate, class ApplyFunc>
  SymbolicFiniteAutBDD Project(
    const size_t & vars,
		VarPredicate pred,
		ApplyFunc &  applyFunc
  ) const
  {
    return SymbolicFiniteAutBDD(
      GetBDD().Project(pred, applyFunc),
      vars
    );
  }

	/**
	 * @brief  Rename variables
	 *
	 * This method renames the variables of the MTBDD according to the renaming
	 * functor given in @p renamer.
	 *
   * @param[in]      vars     Result number of variables of assignments
	 * @param[in,out]  renamer  A functor that renames the variables
	 *
	 * @returns  An MTBDD with renamed variables
	 *
	 * @note  The @p renamer must respect the ordering of ALL variables. That is,
	 *        for all x, y, if x < y, then it must hold that renamer(x) <
	 *        renamer(y)
	 */
	template <class RenamingF>
  SymbolicFiniteAutBDD Rename(
    const size_t & vars,
		RenamingF renamer
  ) const
  {
    return SymbolicFiniteAutBDD(
      GetBDD().Rename(renamer),
      vars
    );
  }
};

#endif // _VATA_SYMBOLIC_FINITE_AUT_BDD_HH_