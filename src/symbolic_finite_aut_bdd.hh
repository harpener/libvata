/*****************************************************************************
 *  VATA Finite Automata Library
 *
 *  Copyright (c) 2014 Jiri Chromecka <xchrom12@stud.fit.vutbr.cz>
 *
 *  Description:
 *  Header file for a BDD proxy.
 *****************************************************************************/

#ifndef _VATA_SYMBOLIC_FINITE_AUT_BDD_HH_
#define _VATA_SYMBOLIC_FINITE_AUT_BDD_HH_

#include <vata/sym_var_asgn.hh>
#include "mtbdd/ondriks_mtbdd.hh"
#include "mtbdd/apply2func.hh"

/// @brief VATA library namespace.
namespace VATA
{
  class SymbolicFiniteAutBDD;
}

GCC_DIAG_OFF(effc++) // non-virtual destructors warnings suppress

/// @brief Proxy class for a symbolic BDD representation.
class VATA::SymbolicFiniteAutBDD
{

GCC_DIAG_ON(effc++)

public: // public data types

  /// @brief A symbolic representation.
  using SymbolicVarAsgn = VATA::SymbolicVarAsgn;

  /// @brief A symbolic representation.
  using AssignmentList = VATA::SymbolicVarAsgn::AssignmentList;

private: // private data types

  /// @brief BDD is represented by a boolean MTBDD.
  using BDD = VATA::MTBDDPkg::OndriksMTBDD<bool>;

  /// @brief BDD pointer.
  using BDDPtr = std::shared_ptr<BDD>;

private: // apply functors

	GCC_DIAG_OFF(effc++) // suppress missing virtual destructor warning

  /// @brief Apply with union.
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
     * @brief Apply operation.
     * @param[in] lhs First element for union.
     * @param[in] rhs Second element for union.
     * @return Union of given elements.
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

  /// @brief Apply with intersection.
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
     * @brief Apply operation.
     * @param[in] lhs First element for intersection.
     * @param[in] rhs Second element for intersection.
     * @return Intersection of given elements.
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

  /// @brief Apply with consequence.
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
     * @brief Apply operation.
     * @param[in] lhs First element for consequence.
     * @param[in] rhs Second element for consequence.
     * @return Consequence of given elements.
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

  /// @brief Apply with equivalence.
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
     * @brief Apply operation.
     * @param[in] lhs First element for equivalence.
     * @param[in] rhs Second element for equivalence.
     * @return Equivalence of given elements.
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

  /// @brief A BDD instance.
  BDDPtr mtbdd_;

public: // instantiation

  /// @brief Default constructor.
  SymbolicFiniteAutBDD();

  /**
   * @brief Copy constructor.
   * @param[in] bdd A BDD to copy.
   */
  SymbolicFiniteAutBDD(
    const SymbolicFiniteAutBDD & bdd
  );

  /**
   * @brief Move constructor.
   * @param[in] bdd A BDD to move.
   */
  SymbolicFiniteAutBDD(
    SymbolicFiniteAutBDD && bdd
  );

  /**
   * @brief Create constructor.
   * @param[in] asgn A symbolic item to be represented by BDD.
   */
  explicit SymbolicFiniteAutBDD(
    const SymbolicVarAsgn & asgn
  );

  /// @brief Default destructor.
  ~SymbolicFiniteAutBDD();

  /**
   * @brief Copy assignment operator.
   * @param[in] rhs A BDD to copy.
   * @return A BDD.
   */
  SymbolicFiniteAutBDD & operator=(
    const SymbolicFiniteAutBDD & rhs
  );

  /**
   * @brief Move assignment operator.
   * @param[in] rhs A BDD to move.
   * @return A BDD.
   */
  SymbolicFiniteAutBDD & operator=(
    SymbolicFiniteAutBDD && rhs
  );

public: // methods

  /**
   * @brief Adds an assignment to BDD.
   * @param[in] asgn An assignment to be added.
   */
  void AddAssignment(
    const SymbolicVarAsgn & asgn
  );

  /**
   * @brief Traverse BDD and obtain all valid paths.
   * @return A list of all valid path in BDD.
   */
  AssignmentList GetAllAssignments() const;
};

#endif // _VATA_SYMBOLIC_FINITE_AUT_BDD_HH_