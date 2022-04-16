#ifndef CODE_SYSTEM_H
#define CODE_SYSTEM_H

#include "Include.h"

#include "Cache.h"
#include "Task.h"

class System {

private:
	//Number of Clock Cycles the System has Passed (No need to initialize)
	uint64_t time_count{0};

	/* #0: Policy Number this System should Implement
	 * false : Write-Back + Write-Allocate
	 *
	 * true : Write-Thru + Non-Write-Allocate
	 */
	bool read_write_policy{POLICY_WBWA};

	//#1: Number of Cache Layers in this System
	size_t cache_count{0};

	//#2: Number of Bytes each DataBlock should hold (Each cache has a copy of this)
	uint32_t block_size{0};

	//#3: Pointer of Top Cache (Second-top layer cache\memory has a copy of this)
	std::shared_ptr<Cache> top_cache_ptr;

	//#4: Total Clock Cycles Needed to Complete (Each cache has a copy of this)
	uint64_t memory_latency{0};//#4

	/* #5 Pointer of File Writer
	 *
	 * use (*global_writer).operator<<() to write
	 * e.g. (*global_writer)<<("THINGS TO WRITE")<<std::endl;
	 */
	std::shared_ptr<std::ofstream> global_writer;

	/* #6 Container holding tasks need to be done
	 * Ready if Sorted.
	 */
	std::vector<Task> process_queue;

	// Current task running in progress (No need to initialize)
	std::vector<Task>::iterator current_process{process_queue.begin()};

	// Status of Initialization. All Members MUST be true before System Initialization
	std::array<bool, 7> ready{false, false, false, false, false, false, false};


	/**
	 * Retrieve the Pointer of Cache of Specific Level
	 * @param _cache_level Cache Level of Cache Wanted (Top Cache being 1)
	 * @return The Retrieved Pointer of Cache
	 */
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

	/**
	 * Check if ALL Data Members Are Initialized
	 * @return True if All Initialized, false if At Least One Member if NOT Initialized
	 */
	explicit operator bool(){
		bool if_no_false_found = std::find(this->ready.begin(), this->ready.end(), false) == this->ready.end();
		return if_no_false_found;
	}

	/**
	 * con	[CACHE_COUNT]	[BLOCK_SIZE]	[POLICY_NUM]	-Set Configurations
	 * Perform Format Check for Policy Number
	 * Mark Cache Count, Block Size, Policy Number as Ready
	 * Warning: Policy number MUST be EITHER 1 OR 2
	 * @param _cache_count Number of Cache Layers in this System
	 * @param _block_size Number of Bytes that Each DataBlock can hold
	 * @param _policy_num Policy Number this System should Implement
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
	bool setConfig(const uint32_t &_cache_count, const uint32_t &_block_size, const uint32_t &_policy_num) {
		if (_policy_num < 1 || _policy_num > 2)
			throw std::runtime_error("ERR Policy Number Unrecognized");
		else if (_policy_num == 1)
			this->read_write_policy = POLICY_WBWA;
		else if (_policy_num == 2)
			this->read_write_policy = POLICY_WTNWA;
		this->ready.at(0) = true;
		this->cache_count = _cache_count;
		this->ready.at(1) = true;
		this->block_size = _block_size;
		this->ready.at(2) = true;
		return true;
	}

	/**
	 * scd	[CACHE_NUMBER]	[TOTAL_SIZE]	[SET_ASSOC]		-Set Cache Size and Set Assoc
	 * Perform Bound Checks for Cache Level
	 * Warning: Function will Mark Ready in Cache, not System
	 * @param _cache_level The level(index) of cache with lowest being 1
	 * @param _total_size Total Number of Bytes this Cache needs to Store
	 * @param _set_assoc Number of DataBlock for a Each Given Index
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
	bool setCacheDimension(const uint32_t &_cache_level, const uint32_t &_total_size, const uint32_t &_set_assoc) {
		if (_cache_level > this->cache_count) return false;
		if (block_size == 0) return false;
		getCacheAtPtr(_cache_level - 1)->setParam(block_size, _total_size, _set_assoc);
		return true;
	}


	/**
	 * scl	[CACHE_NUMBER]	[LATENCY]						-Set Cache Latency
	 * Perform Bound Checks for Cache Level
	 * Warning: Function will Mark Ready in Cache, not System
	 * @param _cache_level The level(index) of cache with lowest being 1
	 * @param _latency Number of Clock Cycles to Complete Read for this Cache
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
	bool setCacheLatency(const uint32_t &_cache_level, const uint32_t &_latency) {
		if (_cache_level > this->cache_count) return false;
		this->getCacheAtPtr(_cache_level - 1)->setLatency(_latency);
		return true;
	}

	/**
	 * sml	[LATENCY]										-Set Memory Latency
	 * Mark Memory Latency as Ready
	 * Warning: This Function does NOT Set Memory Latency in Cache Array
	 * @param _latency
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
	bool setMemoryLatency(const uint32_t &_latency) {
		this->memory_latency = _latency;
		this->ready.at(4) = true;
		return true;
	}

	/**
	 * Set the Pointer of File Writer
	 * Mark Pointer of File Writer as Ready
	 * @param _global_writer Pointer of File Writer
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
	bool setWriterPtr(const std::shared_ptr<std::ofstream> &_global_writer) {
		this->global_writer = _global_writer;
		this->ready.at(5) = true;
		return true;
	}

	/**
	 * inc	[CACHE_NUMBER]									-Initialize Cache
	 * @param _cache_level The level(index) of cache with lowest being 1
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
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
