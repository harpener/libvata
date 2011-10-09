/*****************************************************************************
 *  Vojnar's Army Tree Automata Library
 *
 *  Copyright (c) 2011  Jiri Simacek <isimacek@fit.vutbr.cz>
 *
 *  Description:
 *    Source for explicit LTS simulation algorithm.
 *
 *****************************************************************************/

#include <ostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include <vata/util/binary_relation.hh>
#include <vata/util/smart_set.hh>
#include <vata/util/caching_allocator.hh>
#include <vata/util/shared_list.hh>
#include <vata/explicit_lts.hh>

using VATA::Util::BinaryRelation;
using VATA::Util::SmartSet;
using VATA::Util::CachingAllocator;
using VATA::Util::SharedList;

typedef CachingAllocator<std::vector<size_t>> VectorAllocator;

struct SharedListInitF {

	VectorAllocator& allocator_;

	SharedListInitF(VectorAllocator& allocator) : allocator_(allocator) {}

	void operator()(SharedList<std::vector<size_t>>* list) {

		auto sublist = this->allocator_();

		sublist->clear();

		list->init(sublist);

	}

};

class HashedCounter {

	std::vector<std::unordered_map<size_t, size_t>*> data_;

protected:

	HashedCounter& operator=(const HashedCounter& rhs);

public:

	HashedCounter(size_t labels) : data_(labels) {}

	HashedCounter(HashedCounter& counter) : data_(counter.data_.size()) {}

	void incr(size_t label, size_t state) {

		assert(label < this->data_.size());

		if (!this->data_[label])
			this->data_[label] = new std::unordered_map<size_t, size_t>();

		++this->data_[label]->insert(std::make_pair(state, 0)).first->second;

	} 
	
	size_t decr(size_t label, size_t state) {

		assert(label < this->data_.size());

		auto& table = this->data_[label];

		assert(table);

		auto iter = table->find(state);

		assert(iter != table->end());

		if (iter->second == 1) {

			table->erase(iter);

			return 0;

		}

		return --iter->second;

	}

	void copyRow(size_t label, HashedCounter& cnt) {

		assert(label < this->data_.size());
		assert(this->data_.size() == cnt.data_.size());
		assert(cnt.data_[label]);
		assert(this->data_[label] == nullptr);

		this->data_[label] = new std::unordered_map<size_t, size_t>(*cnt.data_[label]);

	}

	friend std::ostream& operator<<(std::ostream& os, const HashedCounter& cnt) {

		size_t i = 0;
		
		for (auto& row : cnt.data_) {
			
			if (row) {

				os << i << ". (";
	
				for (auto& col : *row)
					os << ' ' << col.first << ':' << col.second;
	
				os << " )" << std::endl;

			}

			++i;

		}

		return os;

	}

};

struct StateListElem {
	
	size_t index_;
	class OLRTBlock* block_;
	StateListElem* next_;
	StateListElem* prev_;

	static void link(StateListElem* elem1, StateListElem* elem2) {

		elem1->next_ = elem2;
		elem2->prev_ = elem1;

	}

	void move(StateListElem*& src, StateListElem*& dst) {

		assert(src);

		if (this == this->next_) {

			assert(this == this->prev_);
			assert(this == src);

			src = nullptr;

			if (!dst) {

				dst = this;

				return;

			}

		} else {

			src = this->next_;
			src->prev_ = this->prev_;
			src->prev_->next_ = src;
	
			if (!dst) {
	
				dst = this;
	
				dst->next_ = dst;
				dst->prev_ = dst;

				return;
	
			}

		}
		
		this->next_ = dst;
		this->prev_ = dst->prev_;
		dst->prev_->next_ = this;
		dst->prev_ = this;

	}

};

typedef SharedList<std::vector<size_t>> RemoveList;
typedef CachingAllocator<RemoveList, SharedListInitF> RemoveAllocator;

struct OLRTBlock {

	size_t _index;
	StateListElem* _states;
	std::vector<RemoveList*> _remove;
	HashedCounter _counter;
	SmartSet _inset;
	StateListElem* _tmp;
	std::unordered_set<OLRTBlock*> bigger_;
	std::unordered_set<OLRTBlock*> smaller_;

protected:

	OLRTBlock(const OLRTBlock&);

	OLRTBlock& operator=(const OLRTBlock&);

public:

	OLRTBlock(const VATA::ExplicitLTS& lts, size_t index, StateListElem* states) : _index(index),
	_states(states), _remove(lts.labels()),	_counter(lts.labels()), _inset(lts.labels()),
	_tmp(nullptr), bigger_(), smaller_() {

		StateListElem* elem = this->_states;

		do {

			for (auto& a : lts.bwLabels(elem->index_))
				this->_inset.add(a);

			elem->block_ = this;
			elem = elem->next_;

		} while (elem != this->_states);

	}
	
	OLRTBlock(const VATA::ExplicitLTS& lts, size_t index, OLRTBlock& parent) : _index(index),
		_states(parent._tmp), _remove(lts.labels()), _counter(parent._counter),
		_inset(lts.labels()), _tmp(nullptr), bigger_(), smaller_() {

		assert(this->_states);

		parent._tmp = nullptr;

		StateListElem* elem = this->_states;

		do {

			for (auto& a : lts.bwLabels(elem->index_)) {

				parent._inset.removeStrict(a);

				this->_inset.add(a);

			}

			elem->block_ = this;
			elem = elem->next_;

		} while (elem != this->_states);

	}

	bool isRelated(OLRTBlock* block) {

		return this->bigger_.find(block) != this->bigger_.end();

	} 

	bool eraseIfRelated(OLRTBlock* block) {

		auto iter = this->bigger_.find(block);
		if (iter == this->bigger_.end())
			return false;
	
		this->bigger_.erase(iter);
		block->smaller_.erase(this);

		return true;

	} 

	void makeRelated(OLRTBlock* block) {

		this->bigger_.insert(block);

	}

	void breakRelated(OLRTBlock* block) {

		this->bigger_.erase(block);

	}

	template <class T>
	void initBigger(OLRTBlock* parent, const T& candidates) {

		for (auto& block : candidates) {

			if (block->isRelated(parent))
				block->makeRelated(this);
				
		}

		this->bigger_ = parent->bigger_;
		this->makeRelated(this);

	}

	void initSmaller() {

		for (auto& block : this->bigger_)
			block->smaller_.insert(this);

		this->smaller_.insert(this);

	}

	void initRelation(OLRTBlock* parent) {

		this->bigger_ = parent->bigger_;
		this->smaller_ = parent->smaller_;

		for (auto& bigger : this->bigger_)
			bigger->smaller_.insert(this);

		for (auto& smaller : this->smaller_)
			smaller->bigger_.insert(this);

		this->bigger_.insert(this);
		this->smaller_.insert(this);

	}
	
	StateListElem* states() {
		return this->_states;
	}
	
	StateListElem* tmp() {
		return this->_tmp;
	}

	void moveToTmp(StateListElem& elem) {

		elem.move(this->_states, this->_tmp);	

	}
	
	bool checkEmpty() {

		if (this->_states)
			return false;

		this->_states = this->_tmp;
		this->_tmp = nullptr;

		return true;

	}
	
	HashedCounter& counter() {
		return this->_counter;
	}

	SmartSet& inset() {
		return this->_inset;
	}

	size_t index() const {
		return this->_index;
	}
	
	friend std::ostream& operator<<(std::ostream& os, const OLRTBlock& block) {

		assert(block._states);

		os << block._index << " (";

		const StateListElem* elem = block._states;

		do {

			os << " " << elem->index_;

			elem = elem->next_;

		} while (elem != block._states);

		return os << " )";

	}
	
};

class OLRTAlgorithm {

	const VATA::ExplicitLTS& _lts;

	VectorAllocator vectorAllocator_;
	RemoveAllocator removeAllocator_;

	std::vector<OLRTBlock*> _partition;
	std::vector<StateListElem> _index;
	std::vector<std::pair<OLRTBlock*, size_t> > _queue;

	OLRTAlgorithm(const OLRTAlgorithm&);

	OLRTAlgorithm& operator=(const OLRTAlgorithm&);

protected:

	void enqueueToRemove(OLRTBlock* block, size_t label, size_t state) {

		if (RemoveList::append(block->_remove[label], state, this->removeAllocator_))
			this->_queue.push_back(std::make_pair(block, label));

	}

	template <class T>
	void buildPre(T& pre, StateListElem* states, size_t label) const {

		std::vector<bool> blockMask(this->_partition.size(), false);

		StateListElem* elem = states;

		do {

			for (auto& q : this->_lts.pre(label)[elem->index_]) {

				OLRTBlock* block = this->_index[q].block_;

				assert(block);

				if (blockMask[block->index()])
					continue;

				blockMask[block->index()] = true;

				pre.push_back(block);

			}

			elem = elem->next_;

		} while (elem != states);

	}

	template <class T1, class T2>
	void internalSplit(T1& modifiedBlocks, const T2& remove) {

		std::vector<bool> blockMask(this->_partition.size(), false);

		for (auto& q : remove) {

			assert(q < this->_index.size());

			StateListElem& elem = this->_index[q];

			OLRTBlock* block = elem.block_;

			assert(block);

			block->moveToTmp(elem);

			assert(block->index() < this->_partition.size());

			if (blockMask[block->index()])
				continue;

			blockMask[block->index()] = true;

			modifiedBlocks.push_back(block);

		}

	}

	template <class T>
	void fastSplit(const T& remove) {

		std::vector<OLRTBlock*> modifiedBlocks;

		this->internalSplit(modifiedBlocks, remove);

		for (auto& block : modifiedBlocks) {

			block->checkEmpty();

			if (!block->_tmp)
				continue;

			auto newBlock = new OLRTBlock(this->_lts, this->_partition.size(), *block);

			newBlock->initBigger(block, this->_partition);

			this->_partition.push_back(newBlock);

		}

	}

	template <class T1, class T2>
	void split(T1& removeList, const T2& remove) {

		std::vector<OLRTBlock*> modifiedBlocks;

		this->internalSplit(modifiedBlocks, remove);

		for (auto& block : modifiedBlocks) {

			if (block->checkEmpty()) {

				removeList.push_back(block);

				continue;

			}

			assert(block->_tmp);

			OLRTBlock* newBlock = new OLRTBlock(this->_lts, this->_partition.size(), *block);

			newBlock->initRelation(block);

			this->_partition.push_back(newBlock);

			removeList.push_back(newBlock);

			for (auto& a : newBlock->inset()) {

				newBlock->counter().copyRow(a, block->counter());

				if (block->_remove[a]) {

					this->_queue.push_back(std::make_pair(newBlock, a));

					newBlock->_remove[a] = block->_remove[a]->copy();

				}

			}

		}

	}

	template <class T>
	void makeBlock(const T& states, size_t blockIndex) {

		assert(states.size() > 0);

		StateListElem* list = &this->_index[states.back()];

		for (auto& q : states) {

			StateListElem::link(list, &this->_index[q]);

			list = list->next_;
			list->index_ = q;

		}

		this->_partition.push_back(new OLRTBlock(this->_lts, blockIndex, list));

	}

	void processRemove(OLRTBlock* block, size_t label) {

		assert(block);

		RemoveList* remove = block->_remove[label];

		block->_remove[label] = nullptr;

		assert(remove);

		std::vector<OLRTBlock*> preList, removeList;

		this->buildPre(preList, block->states(), label);

		this->split(removeList, *remove);

		remove->unsafeRelease(
			[this](RemoveList* list){
					this->vectorAllocator_.reclaim(list->subList());
					this->removeAllocator_.reclaim(list);
			}
		);

		for (auto& b1 : preList) {

			for (auto& b2 : removeList) {

				assert(b1->index() != b2->index());

				if (!b1->eraseIfRelated(b2))
					continue;

				for (auto a : b2->inset()) {

					if (!b1->inset().contains(a))
						continue;

					StateListElem* elem2 = b2->states();

					do {

						for (auto& pre2 : this->_lts.pre(a)[elem2->index_]) {

							if (!b1->counter().decr(a, pre2))
								this->enqueueToRemove(b1, a, pre2);

						}

						elem2 = elem2->next_;

					} while (elem2 != b2->states());

				}

			}

		}

	}

	static bool isPartition(const std::vector<std::vector<size_t>>& part, size_t states) {
	
		std::vector<bool> mask(states, false);
	
		for (auto& cls : part) {
	
			for (auto& q : cls) {
	
				if (mask[q])
					return false;
	
				mask[q] = true;
	
			}
	
		}

		for (auto b : mask) {

			if (!b)
				return false;

		}
	
		return true;
	
	}

	static bool isConsistent(const std::vector<std::vector<size_t>>& part,
		const BinaryRelation& rel) {

		if (part.size() != rel.size())
			return false;

		for (size_t i = 0; i < rel.size(); ++i) {

			if (!rel.get(i, i))
				return false;

		}

		return true;

	}

public:

	OLRTAlgorithm(const VATA::ExplicitLTS& lts) : _lts(lts), vectorAllocator_(),
		removeAllocator_(SharedListInitF(vectorAllocator_)), _partition(), _index(lts.states()),
		_queue() {

		assert(this->_index.size());

	}

	~OLRTAlgorithm() {

		for (auto& block : this->_partition)
			delete block;

	}

	void init(const std::vector<std::vector<size_t>>& partition, const BinaryRelation& relation) {

		assert(OLRTAlgorithm::isPartition(partition, this->_lts.states()));
		assert(OLRTAlgorithm::isConsistent(partition, relation));

		std::vector<SmartSet> delta, delta1;

		this->_lts.buildDelta(delta, delta1);

		// initilize patition-relation

		for (size_t i = 0; i < partition.size(); ++i)
			this->makeBlock(partition[i], i);

		BinaryRelation::IndexType index;

		relation.buildIndex(index);

		for (size_t i = 0; i < this->_partition.size(); ++i) {

			OLRTBlock*& block = this->_partition[i];

			for (auto& bigger : index[i]) {

				assert(bigger < this->_partition.size());

				block->makeRelated(this->_partition[bigger]);

			}

		}

		// make initial refinement

		for (size_t a = 0; a < this->_lts.labels(); ++a)
			this->fastSplit(delta1[a]);

		// prune relation

		std::vector<std::vector<size_t>> pre(this->_partition.size());
		std::vector<std::vector<OLRTBlock*>> noPre(this->_lts.labels());

		for (auto& block : this->_partition) {

			StateListElem* elem = block->states();

			do {

				for (size_t a = 0; a < this->_lts.labels(); ++a) {

					delta1[a].contains(elem->index_)
						? pre[block->_index].push_back(a)
						: noPre[a].push_back(block);

				}

				elem = elem->next_;

			} while (elem != block->states());

		}

		for (auto& b1 : this->_partition) {

			for (auto& a : pre[b1->_index]) {

				for (auto& b2 : noPre[a]) {

					assert(b1 != b2);

					b1->breakRelated(b2);

				}

			}

			b1->initSmaller();

		}

		// initialize counters

		SmartSet s;

		for (std::vector<OLRTBlock*>::reverse_iterator i = this->_partition.rbegin(); i != this->_partition.rend(); ++i) {

			for (auto& a : (*i)->inset()) {

				for (auto q : delta1[a]) {

					for (auto r : this->_lts.post(a)[q]) {

						if ((*i)->isRelated(this->_index[r].block_))
							(*i)->counter().incr(a, q);

					}

				}

				s.assignFlat(delta1[a]);

				for (auto& b2 : this->_partition) {

					if (!(*i)->isRelated(b2))
						continue;

					StateListElem* elem = b2->states();

					do {

						for (auto& q : this->_lts.pre(a)[elem->index_])
							s.remove(q);

						elem = elem->next_;

					} while (elem != b2->states());

				}

				if (s.empty())
					continue;

				(*i)->_remove[a] = new RemoveList(new std::vector<size_t>(s.begin(), s.end()));

				this->_queue.push_back(std::make_pair(*i, a));

				assert(s.size() == (*i)->_remove[a]->subList()->size());

			}

		}

	}

	void run() {

	    while (!this->_queue.empty()) {

			std::pair<OLRTBlock*, size_t> tmp(this->_queue.back());

			this->_queue.pop_back();

			this->processRemove(tmp.first, tmp.second);

		}

	}
	
	void buildResult(BinaryRelation& result, size_t size) const {

		result.resize(size);

		for (size_t i = 0; i < size; ++i) {

			auto& block = this->_index[i].block_;

			for (size_t j = 0; j < size; ++j)

				result.set(i, j, block->isRelated(this->_index[j].block_));

		}

	}

	friend std::ostream& operator<<(std::ostream& os, const OLRTAlgorithm& alg) {

		for (auto& block : alg._partition)
	  		os << *block;

		os << "relation:" << std::endl;

		for (size_t i = 0; i < alg._partition.size(); ++i) {

			auto& block = alg._index[i].block_;

			for (size_t j = 0; j < alg._partition.size(); ++j)

				os << block->isRelated(alg._index[j].block_);

		}

		return os;

	}

};

BinaryRelation VATA::ExplicitLTS::computeSimulation(
	const std::vector<std::vector<size_t>>& partition,
	const BinaryRelation& relation,
	size_t outputSize
) {

	if (this->states_ == 0)
		return BinaryRelation();

	OLRTAlgorithm alg(*this);

	alg.init(partition, relation);
	alg.run();

	BinaryRelation result;

	alg.buildResult(result, outputSize);

	return result;

}
