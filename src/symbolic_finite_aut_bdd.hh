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

#include <vata/symbolic_finite_aut.hh>
#include "mtbdd/ondriks_mtbdd.hh"
#include "mtbdd/apply2func.hh"

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
  using SymbolicVarAsgn = VATA::SymbolicFiniteAut::SymbolicVarAsgn;

  /// @brief  List of symbolic assignments
  using AssignmentList = VATA::SymbolicFiniteAut::AssignmentList;

private: // private data types

  /// @brief  Representation of a BDD
  using BDD = VATA::MTBDDPkg::OndriksMTBDD<bool>;

  /// @brief  BDD pointer
  using BDDPtr = std::unique_ptr<BDD>;

public: // apply functors

	GCC_DIAG_OFF(effc++) // suppress missing virtual destructor warning

  /// @brief  Apply functor with union operation
	class UnionApplyFunctor
    : public VATA::MTBDDPkg::Apply2Functor<
        UnionApplyFunctor,
        bool,
        bool,
        bool
      >
	{

	GCC_DIAG_ON(effc++)

	public: // methods

    /**
     * @brief  Union operation
     *
     * @param[in]  lhs  First data value for union
     * @param[in]  rhs  Second data value for union
     *
     * @return  Union of given data values
     */
		bool ApplyOperation(
      const bool & lhs,
      const bool & rhs
    )
    {
			return (lhs | rhs);
		}
	};

  GCC_DIAG_OFF(effc++) // suppress missing virtual destructor warning

  /// @brief  Apply functor with intersection operation
	class IntersectApplyFunctor
    : public VATA::MTBDDPkg::Apply2Functor<
        IntersectApplyFunctor,
        bool,
        bool,
        bool
      >
	{

	GCC_DIAG_ON(effc++)

	public: // methods

    /**
     * @brief  Intersection operation
     *
     * @param[in]  lhs  First data value for intersection
     * @param[in]  rhs  Second data value for intersection
     *
     * @return  Intersection of given data values
     */
		bool ApplyOperation(
      const bool & lhs,
      const bool & rhs
    )
		{
			return (lhs & rhs);
		}
	};

  GCC_DIAG_OFF(effc++) // suppress missing virtual destructor warning

  /// @brief  Apply functor with consequence operation
	class ConsequenceApplyFunctor
    : public VATA::MTBDDPkg::Apply2Functor<
        ConsequenceApplyFunctor,
        bool,
        bool,
        bool
      >
	{

	GCC_DIAG_ON(effc++)

	public: // methods

    /**
     * @brief  Consequence operation
     *
     * @param[in]  lhs  First data value for consequence
     * @param[in]  rhs  Second data value for consequence
     *
     * @return  Consequence of given data values
     */
		bool ApplyOperation(
      const bool & lhs,
      const bool & rhs
    )
		{
			return (~lhs | rhs);
		}
	};

  GCC_DIAG_OFF(effc++) // suppress missing virtual destructor warning

  /// @brief  Apply functor with equivalence operation
	class EquivalenceApplyFunctor
    : public VATA::MTBDDPkg::Apply2Functor<
        EquivalenceApplyFunctor,
        bool,
        bool,
        bool
      >
	{

	GCC_DIAG_ON(effc++)

	public: // methods

    /**
     * @brief  Equivalence operation
     *
     * @param[in]  lhs  First data value for equivalence
     * @param[in]  rhs  Second data value for equivalence
     *
     * @return  Equivalence of given data values
     */
		bool ApplyOperation(
      const bool & lhs,
      const bool & rhs
    )
		{
			return ((~lhs | rhs) & (~rhs | lhs));
		}
	};

private: // data members

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

public: // methods

  /**
   * @brief  BDD getter
   *
   * @return  BDD pointer
   */
  BDD GetBDD();

  /**
   * @brief  BDD setter
   *
   * @param[in] BDD to be set
   */
  void SetBDD(BDD bdd);

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
   * @return  List of assignments.
   */
  AssignmentList GetAllAssignments() const;

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
};

#endif // _VATA_SYMBOLIC_FINITE_AUT_BDD_HH_