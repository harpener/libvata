/*****************************************************************************
 *  VATA Tree Automata Library
 *
 *  Copyright (c) 2011  Ondra Lengal <ilengal@fit.vutbr.cz>
 *
 *  Description:
 *    Binary Apply functor for OndriksMTBDD
 *
 *****************************************************************************/

#ifndef _VATA_BOOL_APPLY2FUNC_HH_
#define _VATA_BOOL_APPLY2FUNC_HH_

// VATA headers
#include	<vata/vata.hh>

// Standard library headers
#include  <unordered_set>

// Boost library headers
#include <boost/functional/hash.hpp>

#include "ondriks_mtbdd.hh"
#include "apply1func.hh"
#include "bool_classify_case.hh"

namespace VATA
{
	namespace MTBDDPkg
	{
		template <class Base>
		class BoolApply2Functor;

    class NegApplyFunctor;
    class OrApplyFunctor;
    class AndApplyFunctor;
    class ImplicApplyFunctor;
    class EquivApplyFunctor;
	}
}

GCC_DIAG_OFF(effc++)
class VATA::MTBDDPkg::NegApplyFunctor
  : public VATA::MTBDDPkg::Apply1Functor<
    NegApplyFunctor,
    bool,
    bool
  >
{
GCC_DIAG_ON(effc++)

public:

	bool ApplyOperation(const bool & val)
  {
    return !val;
	}
};

template <class Base>
class VATA::MTBDDPkg::BoolApply2Functor
{
public:   // Public data types

	typedef Base BaseClass;

	typedef OndriksMTBDD<bool> MTBDD1Type;
	typedef OndriksMTBDD<bool> MTBDD2Type;
	typedef OndriksMTBDD<bool> MTBDDOutType;

	typedef typename MTBDD1Type::NodePtrType   Node1PtrType;
	typedef typename MTBDD2Type::NodePtrType   Node2PtrType;
	typedef typename MTBDDOutType::NodePtrType NodeOutPtrType;

	typedef typename MTBDDOutType::VarType VarType;

private:  // Private data types

	typedef std::pair<Node1PtrType, Node2PtrType> CacheAddressType;

	typedef std::unordered_map<CacheAddressType, NodeOutPtrType,
		boost::hash<CacheAddressType>> CacheHashTable;

private:  // Private data members

	const MTBDD1Type* mtbdd1_;
	const MTBDD2Type* mtbdd2_;

	CacheHashTable ht;

private:  // Private methods

	BoolApply2Functor(const BoolApply2Functor&);
	BoolApply2Functor& operator=(const BoolApply2Functor&);

	NodeOutPtrType recDescend(const Node1PtrType& node1, const Node2PtrType& node2)
	{
		// Assertions
		assert(!IsNull(node1));
		assert(!IsNull(node2));

		CacheAddressType cacheAddress(node1, node2);
		typename CacheHashTable::iterator itHt;
		if ((itHt = ht.find(cacheAddress)) != ht.end())
		{	// if the result is already known
			assert(!IsNull(itHt->second));
			return itHt->second;
		}

		char relation = BoolClassifyCase(node1, node2);
		assert((relation & ~(NODE1MASK | NODE2MASK)) == 0x00);

		if (!relation)
		{	// for at least one terminal case
      NodeOutPtrType result = makeBase().ApplyOperation(node1, node2);
      IncrementRefCnt(result); // not sure about placement of this

			ht.insert(std::make_pair(cacheAddress, result));
			return result;
		}

		// in case there is something to be branched
		assert(relation);

		VarType var;
		Node1PtrType low1Tree = 0;
		Node2PtrType low2Tree = 0;
		Node1PtrType high1Tree = 0;
		Node2PtrType high2Tree = 0;

		if (relation & NODE1MASK)
		{	// if node1 is to be branched
			var = GetVarFromInternal(node1);
			low1Tree = GetLowFromInternal(node1);
			high1Tree = GetHighFromInternal(node1);
			assert(low1Tree != high1Tree);
		}
		else
		{	// if node1 is not to be branched
			low1Tree = node1;
			high1Tree = node1;
		}

		if (relation & NODE2MASK)
		{	// if node2 is to be branched
			var = GetVarFromInternal(node2);
			low2Tree = GetLowFromInternal(node2);
			high2Tree = GetHighFromInternal(node2);
			assert(low2Tree != high2Tree);
		}
		else
		{	// if node2 is not to be branched
			low2Tree = node2;
			high2Tree = node2;
		}

		NodeOutPtrType lowOutTree = recDescend(low1Tree, low2Tree);
		NodeOutPtrType highOutTree = recDescend(high1Tree, high2Tree);

		if (lowOutTree == highOutTree)
		{	// in case both trees are isomorphic (when caching is enabled)
			ht.insert(std::make_pair(cacheAddress, lowOutTree));
			return lowOutTree;
		}
		else
		{	// in case both trees are distinct
			NodeOutPtrType result =
				MTBDDOutType::spawnInternal(lowOutTree, highOutTree, var);

			ht.insert(std::make_pair(cacheAddress, result));
			return result;
		}
	}

	inline BaseClass& makeBase()
	{
		return static_cast<BaseClass&>(*this);
	}

public:   // Public methods

	BoolApply2Functor() :
		mtbdd1_(nullptr),
		mtbdd2_(nullptr),
		ht()
	{ }

	NodeOutPtrType operator()(const Node1PtrType& node1, const Node2PtrType& node2)
	{
		assert(!IsNull(node1));
		assert(!IsNull(node2));

		// clear the MTBDDs
		mtbdd1_ = nullptr;
		mtbdd2_ = nullptr;

		// clear the cache
		ht.clear();

		// recursively descend the nodes and generate a new one
		return recDescend(node1, node2);
	}

	MTBDDOutType operator()(const MTBDD1Type& mtbdd1, const MTBDD2Type& mtbdd2)
	{
		// store the MTBDDs
		mtbdd1_ = &mtbdd1;
		mtbdd2_ = &mtbdd2;

		// clear the cache
		ht.clear();

		// recursively descend the MTBDD and generate a new one
		NodeOutPtrType root = recDescend(mtbdd1_->getRoot(), mtbdd2_->getRoot());

		IncrementRefCnt(root);

		// wrap it all up
		return MTBDDOutType(root, false);
	}

protected:// Protected methods

	inline const MTBDD1Type& getMTBDD1() const
	{
		assert(mtbdd1_ != nullptr);
		return *mtbdd1_;
	}

	inline const MTBDD2Type& getMTBDD2() const
	{
		assert(mtbdd2_ != nullptr);
		return *mtbdd2_;
	}
};

GCC_DIAG_OFF(effc++)
class VATA::MTBDDPkg::OrApplyFunctor
  : public VATA::MTBDDPkg::BoolApply2Functor<VATA::MTBDDPkg::OrApplyFunctor>
{
GCC_DIAG_ON(effc++)

public:

	NodeOutPtrType ApplyOperation(
    const Node1PtrType & node1,
    const Node2PtrType & node2
  )
  {
    if (IsLeaf(node1))
    {
      if (GetDataFromLeaf(node1) == 0) return node2;
      else return node1;
    }

    else
    {
      if (GetDataFromLeaf(node2) == 0) return node1;
      else return node2;
    }
	}
};

GCC_DIAG_OFF(effc++)
class VATA::MTBDDPkg::AndApplyFunctor
  : public VATA::MTBDDPkg::BoolApply2Functor<VATA::MTBDDPkg::AndApplyFunctor>
{
GCC_DIAG_ON(effc++)

public:

	NodeOutPtrType ApplyOperation(
    const Node1PtrType & node1,
    const Node2PtrType & node2
  )
	{
    if (IsLeaf(node1))
    {
      if (GetDataFromLeaf(node1) == 0) return node1;
      else return node2;
    }

    else
    {
      if (GetDataFromLeaf(node2) == 0) return node2;
      else return node1;
    }
	}
};

GCC_DIAG_OFF(effc++)
class VATA::MTBDDPkg::ImplicApplyFunctor
  : public VATA::MTBDDPkg::BoolApply2Functor<VATA::MTBDDPkg::ImplicApplyFunctor>
{
GCC_DIAG_ON(effc++)

public:

	NodeOutPtrType ApplyOperation(
    const Node1PtrType & node1,
    const Node2PtrType & node2
  )
	{
    VATA::MTBDDPkg::NegApplyFunctor negFunc;

    if (IsLeaf(node1))
    {
      if (GetDataFromLeaf(node1) == 0) return negFunc(node1);
      else return node2;
    }

    else
    {
      if (GetDataFromLeaf(node2) == 0) return negFunc(node1);
      else return node2;
    }
	}
};

GCC_DIAG_OFF(effc++)
class VATA::MTBDDPkg::EquivApplyFunctor
  : public VATA::MTBDDPkg::BoolApply2Functor<VATA::MTBDDPkg::EquivApplyFunctor>
{
GCC_DIAG_ON(effc++)

public:

	NodeOutPtrType ApplyOperation(
    const Node1PtrType & node1,
    const Node2PtrType & node2
  )
	{
    VATA::MTBDDPkg::NegApplyFunctor negFunc;

    if (IsLeaf(node1))
    {
      if (GetDataFromLeaf(node1) == 0) return negFunc(node2);
      else return node2;
    }

    else
    {
      if (GetDataFromLeaf(node2) == 0) return negFunc(node1);
      else return node1;
    }
	}
};

#endif
