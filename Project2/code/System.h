#ifndef CODE_SYSTEM_H
#define CODE_SYSTEM_H

#include "Include.h"

#include "Cache.h"
#include "Task.h"

class System {

private:
	//Number of Clock Cycles the System has Passed (No need to initialize)
	uint64_t clock_count{0};

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
	std::unique_ptr<Cache> top_cache_ptr;

	//#4: Total Clock Cycles Needed to Complete (Each cache has a copy of this)
	uint64_t memory_latency{0};//#4

	/* #5 Pointer of File Writer
	 *
	 * use (*global_writer_ptr).operator<<() to write
	 * e.g. (*global_writer_ptr)<<("THINGS TO WRITE")<<std::endl;
	 */
	std::ofstream *global_writer_ptr;

	/* #6 Container holding tasks need to be done
	 * Ready if Sorted.
	 */
	std::vector<Task> task_queue;

	// Status of Initialization. All Members MUST be true before System Initialization
	std::array<bool, 7> ready{false, false, false, false, false, false, false};

	/**
	 * Retrieve the Pointer of Cache of Specific Level
	 * @param _cache_level Cache Level of Cache Wanted (Top Cache being 1)
	 * @return The Retrieved Pointer of Cache
	 */
	[[nodiscard]] Cache *getCacheAtPtr(const uint32_t &_cache_level) {
		size_t _cache_index = _cache_level - 1;
		if (_cache_index > this->cache_count)
			throw std::out_of_range("ERR Cache Level Out-of-range");
		if (top_cache_ptr == nullptr)
			throw std::runtime_error("ERR Top Cache Ptr is Null");
		Cache *this_cache_ptr = top_cache_ptr.get();
		for (size_t i = 0; i < _cache_index; i++) {
			this_cache_ptr = this_cache_ptr->getParentPtr();
		}
		return this_cache_ptr;
	}

	/**
	 * Sort the Task Queue by Arriving Time
	 * Mark Task Queue as Ready
	 */
	void sortTaskQueue() {
		std::sort(task_queue.begin(), task_queue.end());
		this->ready.at(6) = true;
	}


	uint64_t reqReadAdr(const uint32_t &_address, const uint64_t &_arrive_time) {
		uint64_t clock_span = top_cache_ptr->read(_address, this->read_write_policy);
		return clock_span;
	}

	//TO-DO Algorithms incomplete
	uint64_t reqWrtAdr(const uint32_t &_address, const uint64_t &_arrive_time) {
		uint64_t clock_span = 0;
		return clock_span;
	}

	uint64_t reqRptHmr(const uint32_t &_cache_level, const uint64_t &_arrive_time) {
		this->getCacheAtPtr(_cache_level)->printHitMissRate(_arrive_time);
		return 0;
	}

	uint64_t reqRepImg(const uint32_t &_cache_level, const uint64_t &_arrive_time) {
		this->getCacheAtPtr(_cache_level)->printCacheImage(_arrive_time);
		return 0;
	}


public:

	/**
	 * Check if ALL Data Members Are Initialized
	 * @return True if All Initialized, false if At Least One Member if NOT Initialized
	 */
	explicit operator bool() {
		bool if_no_false_found = std::find(this->ready.begin(), this->ready.end(), false) == this->ready.end();
		return if_no_false_found;
	}

	/**
	 * con	[cache_count]	[block_size]	[policy_num]	-
	 * Set Configurations
	 * Perform Format Check for Policy Number
	 * Mark Cache Count, Block Size, Policy Number as Ready
	 * Warning: Policy number MUST be EITHER 1 OR 2
	 * @param _cache_count Number of Cache Layers in this System
	 * @param _block_size Number of Bytes that Each DataBlock can hold
	 * @param _policy_num Policy Number this System should Implement
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
	bool setConfig(std::tuple<uint32_t, uint32_t, uint32_t> *_arguments) {
		if (_arguments == nullptr)
			throw std::runtime_error("ERR Argument Tuple is NULL");
		uint32_t _cache_count = std::get<0>(*_arguments);
		uint32_t _block_size = std::get<1>(*_arguments);
		uint32_t _policy_num = std::get<2>(*_arguments);
		if (ready.at(0) || ready.at(1) || ready.at(2) || ready.at(3))
			throw std::invalid_argument("ERR con called twice");
		if (_policy_num < 1 || _policy_num > 2)
			throw std::runtime_error("ERR Policy Number Unrecognized");
		else if (_policy_num == 1)
			this->read_write_policy = POLICY_WBWA;
		else if (_policy_num == 2)
			this->read_write_policy = POLICY_WTNWA;
		this->ready.at(0) = true;
		this->cache_count = _cache_count;
		if (_cache_count < 1)
			throw std::runtime_error("ERR At Least 1 Cache is Needed");
		this->ready.at(1) = true;
		this->block_size = _block_size;
		this->ready.at(2) = true;
		this->top_cache_ptr = std::make_unique<Cache>();//create childest cache
		Cache *this_cache_ptr = top_cache_ptr.get();//cache pointer iterator
		if (this_cache_ptr->operator bool())
			throw std::invalid_argument("ERR con Called after inc");
		this_cache_ptr->setId(1);
		this_cache_ptr->setChild(nullptr);
		if (cache_count == 1) this_cache_ptr->makeAsTopCache();
		for (size_t i = 2; i <= _cache_count; i++) {
			this_cache_ptr->makeParent();
			this_cache_ptr = this_cache_ptr->getParentPtr();
			this_cache_ptr->setId(i);
		}
		this->ready.at(3) = true;
		return true;
	}

	/**
	 * scd	[cache_number]	[total_size]	[set_assoc]		-
	 * Set Cache Size and Set Assoc
	 * Perform Bound Checks for Cache Level
	 * Warning: Function will Mark Ready in Cache, not System
	 * @param _cache_level The level(index) of cache with lowest being 1
	 * @param _total_size Total Number of Bytes this Cache needs to Store
	 * @param _set_assoc Number of DataBlock for a Each Given Index
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
	bool setCacheDimension(std::tuple<uint32_t, uint32_t, uint32_t> *_arguments) {
		if (_arguments == nullptr)
			throw std::runtime_error("ERR Argument Tuple is NULL");
		if (!this->ready.at(3))
			throw std::invalid_argument("ERR scd Called Before con");
		uint32_t _cache_level = std::get<0>(*_arguments);
		uint32_t _total_size = std::get<1>(*_arguments);
		uint32_t _set_assoc = std::get<2>(*_arguments);
		if (_cache_level > this->cache_count) return false;
		if (block_size == 0) return false;
		Cache *this_cache_ptr = getCacheAtPtr(_cache_level);
		if (this_cache_ptr->operator bool())
			throw std::invalid_argument("ERR scd called after inc");
		this_cache_ptr->setParam(block_size, _total_size, _set_assoc);
		return true;
	}


	/**
	 * scl	[cache_number]	[latency]						-
	 * Set Cache Latency
	 * Perform Bound Checks for Cache Level
	 * Warning: Function will Mark Ready in Cache, not System
	 * @param _cache_level The level(index) of cache with lowest being 1
	 * @param _latency Number of Clock Cycles to Complete Read for this Cache
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
	bool setCacheLatency(std::tuple<uint32_t, uint32_t, uint32_t> *_arguments) {
		if (_arguments == nullptr)
			throw std::runtime_error("ERR Argument Tuple is NULL");
		uint32_t _cache_level = std::get<0>(*_arguments);
		uint32_t _latency = std::get<1>(*_arguments);
		if (_cache_level > this->cache_count) return false;
		Cache *this_cache = this->getCacheAtPtr(_cache_level);
		if (this_cache->operator bool())
			throw std::invalid_argument("ERR scl called after inc");
		this_cache->setLatency(_latency);
		return true;
	}

	/**
	 * sml	[latency]										-
	 * Set Memory Latency
	 * Mark Memory Latency as Ready
	 * Warning: This Function does NOT Set Memory Latency in Cache Array
	 * @param _latency
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
	bool setMemoryLatency(std::tuple<uint32_t, uint32_t, uint32_t> *_arguments) {
		if (_arguments == nullptr)
			throw std::runtime_error("ERR Argument Tuple is NULL");
		if (!this->ready.at(3))
			throw std::invalid_argument("ERR sml called after inc");
		uint32_t _latency = std::get<0>(*_arguments);
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
	bool setWriterPtr(std::ofstream *_global_writer) {
		this->global_writer_ptr = _global_writer;
		this->ready.at(5) = true;
		return true;
	}

	/**
	 * inc	[cache_number]									-
	 * Initialize Cache
	 * Perform Bound Checks for Cache Level
	 * @param _cache_level The level(index) of cache with lowest being 1
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
	bool initCache(std::tuple<uint32_t, uint32_t, uint32_t> *_arguments) {
		if (_arguments == nullptr)
			throw std::runtime_error("ERR Argument Tuple is NULL");
		if (!this->ready.at(4))
			throw std::invalid_argument("ERR inc called before sml");
		uint32_t _cache_level = std::get<0>(*_arguments);
		if (_cache_level > this->cache_count) return false;
		Cache *this_cache_ptr = this->getCacheAtPtr(_cache_level);
		this_cache_ptr->setMemoryLatency(this->memory_latency);
		this_cache_ptr->initCacheArray();
		return true;
	}

	/**
	 * tre	[address]		[arr_time]						-
	 * Task Read Address at Time
	 * @param _address Raw 32-bit Address to be Read
	 * @param _arrive_time	Clock Cycle at when This Specific Task is Scheduled
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
	bool taskReadAddress(std::tuple<uint32_t, uint32_t, uint32_t> *_arguments) {
		if (_arguments == nullptr)
			throw std::runtime_error("ERR Argument Tuple is NULL");
		if (this->operator bool())
			throw std::invalid_argument("ERR Cannot Task Once System is Initialized");
		uint32_t _address = std::get<0>(*_arguments);
		uint32_t _arrive_time = std::get<1>(*_arguments);
		this->task_queue.emplace_back(task_t::task_readAddress, _address, _arrive_time);
		return true;
	}

	/**
	 * twr	[address]		[arr_time]						-
	 * Task Write Address at Time
	 * @param _address Raw 32-bit Address to be Written
	 * @param _arrive_time Clock Cycle at when This Specific Task is Scheduled
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
	bool taskWriteAddress(std::tuple<uint32_t, uint32_t, uint32_t> *_arguments) {
		if (_arguments == nullptr)
			throw std::runtime_error("ERR Argument Tuple is NULL");
		if (this->operator bool())
			throw std::invalid_argument("ERR Cannot Task Once System is Initialized");
		uint32_t _address = std::get<0>(*_arguments);
		uint32_t _arrive_time = std::get<1>(*_arguments);
		this->task_queue.emplace_back(task_t::task_writeAddress, _address, _arrive_time);
		return true;
	}

	/**
	 * ins													-
	 * Initialize System
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
	bool initSystem(std::tuple<uint32_t, uint32_t, uint32_t> *_arguments) {
		if (_arguments == nullptr)
			throw std::runtime_error("ERR Argument Tuple is NULL");
		sortTaskQueue();
		this->ready.at(6) = true;
		if (!this->operator bool())
			throw std::runtime_error("ERR System Cannot Initialize - System Not Ready");
		runTaskQueue();
		return true;
	}

	/**
	 * pcr [cache_number]									-
	 * Print Cache Hit/Miss Rate
	 * Perform Bound Checks for Cache Level
	 * @param _cache_level The level(index) of cache with lowest being 1
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
	bool taskPrintCacheRate(std::tuple<uint32_t, uint32_t, uint32_t> *_arguments) {
		if (_arguments == nullptr)
			throw std::runtime_error("ERR Argument Tuple is NULL");
		if (this->operator bool())
			throw std::invalid_argument("ERR Cannot Report Once System is Initialized");
		uint32_t _cache_level = std::get<0>(*_arguments);
		uint32_t _arrive_time = std::get<1>(*_arguments);
		if (_cache_level > this->cache_count) return false;
		this->task_queue.emplace_back(task_t::task_reportHitMiss, _cache_level, _arrive_time);
		return true;
	}

	/**
	 * pci	[cache_number]									-
	 * Print Cache Image
	 * Perform Bound Checks for Cache Level
	 * @param _cache_level The level(index) of cache with lowest being 1
	 * @return True if Instruction Ran without Errors, false otherwise
	 */
	bool taskPrintCacheImage(std::tuple<uint32_t, uint32_t, uint32_t> *_arguments) {
		if (_arguments == nullptr)
			throw std::runtime_error("ERR Argument Tuple is NULL");
		if (this->operator bool())
			throw std::invalid_argument("ERR Cannot Report Once System is Initialized");
		uint32_t _cache_level = std::get<0>(*_arguments);
		uint32_t _arrive_time = std::get<1>(*_arguments);
		if (_cache_level > this->cache_count) return false;
		this->task_queue.emplace_back(task_t::task_reportImage, _cache_level, _arrive_time);
		return true;
	}

	/**
	 * hat
	 * Stop Fetching Instruction
	 * @return ALWAYS TRUE
	 */
	bool haltProgram(std::tuple<uint32_t, uint32_t, uint32_t> *_arguments) {
		if (_arguments == nullptr)
			throw std::runtime_error("ERR Argument Tuple is NULL");
		uint32_t _arrive_time = std::get<0>(*_arguments);
		task_queue.emplace_back(task_t::task_halt, 0, _arrive_time);
		return false;
	}

	void runTaskQueue() {
		auto current_task_ptr = this->task_queue.begin();
		while (current_task_ptr != this->task_queue.end() && current_task_ptr->getTaskType() != task_t::task_halt) {
			while (current_task_ptr->getArriveTime() <= clock_count) {
				task_t this_task = current_task_ptr->getTaskType();
				uint32_t this_value = current_task_ptr->getTaskValue();
				uint32_t this_arrive_time = current_task_ptr->getArriveTime();
				uint64_t add_clk{0};
				if (this_task == task_t::task_readAddress)
					add_clk = this->reqReadAdr(this_value, this_arrive_time);
				else if (this_task == task_t::task_writeAddress)
					add_clk = this->reqWrtAdr(this_value, this_arrive_time);
				else if (this_task == task_t::task_reportHitMiss)
					add_clk = this->reqRptHmr(this_value, this_arrive_time);
				else if (this_task == task_t::task_reportImage)
					add_clk = this->reqRepImg(this_value, this_arrive_time);
				current_task_ptr++;
				clock_count += add_clk;
			}
			clock_count += 1;
		}
	}

};

#endif //CODE_SYSTEM_H
