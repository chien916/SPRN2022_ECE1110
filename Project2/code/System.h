#ifndef CODE_SYSTEM_H
#define CODE_SYSTEM_H

#include "Include.h"

#include "Cache.h"

class System {

private:
	// Total latency:
	uint64_t time_count{0};


	// System-level configurable parameters:
	bool read_write_policy{false};//#0
	size_t cache_count{0};//#1
	uint32_t block_size{0};//#2

	// The cache objects:
	std::shared_ptr<Cache> top_cache_ptr;//#3

	// Memory:
	uint64_t memory_latency{0};//#4

	std::shared_ptr<std::ofstream> global_writer;//#5

	std::array<bool, 6> ready{false, false, false, false, false, false};//0 means ready

	std::shared_ptr<Cache> getCacheAtPtr(const uint32_t &_cache_level) {
		if (_cache_level > this->cache_count)
			throw std::out_of_range("ERR Cache Level Out-of-range");
		if (!top_cache_ptr)
			throw std::runtime_error("ERR Top Cache Ptr is Null");
		std::shared_ptr<Cache> this_cache_ptr = top_cache_ptr;
		for (size_t i = 0; i < _cache_level; i++) {
			this_cache_ptr = this_cache_ptr->getChild();
		}
		return this_cache_ptr;
	}

public:
	//setter functions for configurable parameters:

	bool setConfig(const uint32_t &_cache_count, const uint32_t &_block_size, const uint32_t &_policy_num) {
		//Set Global Parameters:
		if (_policy_num < 1 || _policy_num > 2)
			throw std::runtime_error("ERR Policy Number Unrecognized");
		else if (_policy_num == 1)
			this->read_write_policy = false;
		else if (_policy_num == 2)
			this->read_write_policy = true;
		this->ready.at(0) = true;
		this->cache_count = _cache_count;
		this->ready.at(1) = true;
		this->block_size = _block_size;
		this->ready.at(2) = true;
	}


	bool setCacheParam(const uint32_t &_cache_level, const uint32_t &_total_size, const uint32_t &_set_assoc) {
		if (_cache_level > this->cache_count) return false;
		if (block_size == 0) return false;
		getCacheAtPtr(_cache_level - 1)->setParam(block_size, _total_size, _set_assoc);
		return true;
	}

	bool setCacheLatency(const uint32_t &_cache_level, const uint32_t &_latency) {
		if (_cache_level > this->cache_count) return false;
		this->getCacheAtPtr(_cache_level - 1)->setLatency(_latency);
		return true;
	}


	bool setMemoryLatency(const uint32_t &_latency) {
		this->memory_latency = _latency;
		this->ready.at(4) = true;
		return true;
	}

	bool setWriterPtr(const std::shared_ptr<std::ofstream> &_global_writer) {
		this->global_writer = _global_writer;
		this->ready.at(5) = true;
		return true;
	}

	bool initCache(const uint32_t &_cache_level) {
		if (_cache_level > this->cache_count) return false;
		std::shared_ptr<Cache> this_cache_ptr = this->getCacheAtPtr(_cache_level - 1);
		if (_cache_level < this->cache_count)
			this_cache_ptr->setParent(this->getCacheAtPtr(_cache_level - 2));
		else if (_cache_level > 1)
			this_cache_ptr->setChild(this->getCacheAtPtr(_cache_level));
		this_cache_ptr->initCacheArray();
		this_cache_ptr->setId(_cache_level);
		return true;
	}


};

#endif //CODE_SYSTEM_H
