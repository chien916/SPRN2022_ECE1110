#ifndef CODE_CACHE_H
#define CODE_CACHE_H


#include "DataBlock.h"

/*
 * SZ_TAG_BITS - How many bits from address are tag bits
 * SZ_DATA_BITS - How many bits do data fields have
 * SZ_SET_ASSO - How many data fields (block) does one
 */
/*template<size_t SZ_TAG_BITS, size_t SZ_BLOCK_SIZE, size_t SZ_SET_ASSO, size_t SZ_NUM_BLOCK>*/
class Cache {
private:
	std::shared_ptr<Cache> child_cache_ptr{nullptr};//#0
	std::shared_ptr<Cache> parent_cache_ptr{nullptr};//#1
	std::tuple<size_t, size_t, size_t> address_partition{0, 0, 0};//TAG-INDEX-OFFSET//#2
	std::tuple<uint32_t, uint32_t, uint32_t> dimensions{0, 0, 0};//BLOCKSIZE-SETASSOC-NUMOFBLOCK//#3
	std::vector<std::vector<DataBlock>> cache_array;//#4
	std::pair<uint64_t, uint64_t> hit_miss_count{0, 0};
	uint64_t latency{0};//#5
	uint64_t memory_latency{0};//#6
	size_t cache_id{0};//#7

	std::shared_ptr<std::ofstream> global_writer;//#7

	std::array<bool, 8> ready{false, false, false, false, false, false, false, false};//1 means ready

	std::tuple<uint32_t, uint32_t, uint32_t> addressDecode(const uint32_t &address_val) const {
		std::tuple<uint32_t, uint32_t, uint32_t> indices;
		std::get<0>(indices) =
				address_val >> (std::get<2>(this->address_partition) + std::get<1>(this->address_partition));
		std::get<1>(indices) =
				address_val << std::get<0>(this->address_partition)
							>> (std::get<2>(this->address_partition) + std::get<0>(this->address_partition));
		std::get<2>(indices) =
				address_val << (std::get<0>(this->address_partition) + std::get<1>(this->address_partition))
							>> (std::get<0>(this->address_partition) + std::get<1>(this->address_partition));
		return indices;
	}

public:

	void setParent(const std::shared_ptr<Cache> &_parent_ptr) {
		this->parent_cache_ptr = _parent_ptr;
		this->ready.at(1) = true;
	}

	std::shared_ptr<Cache> getParent() const {
		return this->parent_cache_ptr;
	}

	void setChild(const std::shared_ptr<Cache> &_child_ptr) {
		this->child_cache_ptr = _child_ptr;
		this->ready.at(0) = true;
	}

	std::shared_ptr<Cache> getChild() const {
		return this->child_cache_ptr;
	}

	void setParam(const uint32_t &block_size, const uint32_t &total_size, const uint32_t &set_assoc) {
		uint32_t num_of_cache_block = (total_size / block_size / set_assoc);
		std::get<0>(this->dimensions) = block_size;//Number of bytes each data-block can hold
		std::get<1>(this->dimensions) = set_assoc;//Number of data-blocks does one index maps to
		std::get<2>(this->dimensions) = num_of_cache_block;//Number of cache-blocks does cache-array have
		this->ready.at(3) = true;
		std::get<2>(this->address_partition) = lround(log2l(block_size));//Bits of Offset
		std::get<1>(this->address_partition) = lround(log2l(num_of_cache_block));//Bits of Index
		std::get<0>(this->address_partition) =
				32 - std::get<2>(this->address_partition) - std::get<1>(this->address_partition);//Bits of Tag
		this->ready.at(2) = true;
	}

	void setLatency(const uint64_t &_latency) {
		this->latency = _latency;
		this->ready.at(5) = true;
	}

	void setId(const size_t &_cache_id) {
		this->cache_id = _cache_id;
		this->ready.at(7) = true;
	}

	void setMemoryLatency(const uint32_t &_latency) {
		this->memory_latency = _latency;
		this->ready.at(6) = true;
	}

	void setWriterPtr(const std::shared_ptr<std::ofstream> &_global_writer) {
		this->global_writer = _global_writer;
		this->ready.at(8) = true;
	}

	void initCacheArray() {
		if (!this->ready.at(3))
			throw std::runtime_error("ERR Cannot Initialize Cache Array - Dimensions Not Ready");
		DataBlock empty_data_block{std::get<0>(this->dimensions)};
		std::vector<DataBlock> empty_cache_block{std::get<1>(this->dimensions), empty_data_block};
		this->cache_array.resize(std::get<2>(this->dimensions), empty_cache_block);
		this->ready.at(4) = true;
	}


	uint64_t read(const uint32_t &_address_val, const bool &_policy) {
		if (!this->getReady())
			throw std::runtime_error("ERR Cache Cannot Read - Cache Not Ready");
		std::tuple<uint32_t, uint32_t, uint32_t> indices = this->addressDecode(_address_val);
		for (const DataBlock &cb: cache_array.at(std::get<1>(indices))) {
			if (cb.compareTag(std::get<0>(indices))) {
				this->hit_miss_count.first++;
				return this->latency;//Hit
			} else {
				this->hit_miss_count.second++;
				if (this->parent_cache_ptr) {//Hit - Go to Parent Cache
					return this->latency + this->parent_cache_ptr->read(_address_val, _policy);//Miss
				} else {//Miss - Go to Memory
					return this->latency + this->memory_latency;
				}
			}
		}
		return false;
	}

	bool getReady() {
		bool if_found = std::find(this->ready.begin(), this->ready.end(), false) == this->ready.end();
		return if_found;
	}

	void printHitMissRate() {
		if (!this->ready.at(7))
			throw std::runtime_error("ERR Cannot Print Rate - Ofstream Not Ready");
		uint64_t sum = (this->hit_miss_count.first + this->hit_miss_count.second);
		uint32_t hit_rate = 100 * (this->hit_miss_count.first / sum);
		uint32_t miss_rate = 100 * (this->hit_miss_count.second / sum);
		(*global_writer)
				<< "##HITS MISSES FOR CACHE L" << this->cache_id << std::endl
				<< "    HITS:      " << std::setw(10) << this->hit_miss_count.first << std::endl
				<< "    MISSES:    " << std::setw(10) << this->hit_miss_count.second << std::endl
				<< "    HIT  RATE: " << std::setw(10) << hit_rate << std::endl
				<< "    MISS RATE: " << std::setw(10) << miss_rate << std::endl;
	}

	/*
	 * To access individual element from cache array, do the following:
	 * this->cache_array.at(A).at(B).getValid() access Cache[A][B].Valid - bool type is returned
	 * this->cache_array.at(A).at(B).getDirty() access Cache[A][B].Dirty - bool type is returned
	 * this->cache_array.at(A).at(B).getTag() access Cache[A][B].Tag - uint32_t type is returned
	 */
	void printCacheImage() {
		if (!this->ready.at(7))
			throw std::runtime_error("ERR Cannot Print Rate - Ofstream Not Ready");
		//TODO - Print Contents
	}


};


#endif //CODE_CACHE_H
