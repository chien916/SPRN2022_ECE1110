#ifndef CODE_CACHE_H
#define CODE_CACHE_H

#include "DataBlock.h"

class Cache {
private:
	//#0:Pointer of Cache one Unit closer to User, nullptr if is top cache
	Cache *child_cache_ptr{nullptr};

	//#1:Pointer of Cache one Unit closer to Memory, nullptr if is bottom cache
	std::unique_ptr<Cache> parent_cache_ptr{nullptr};

	/* #2 Information of the Partition of Address for this Specific Cache
	 *
	 * [Number of Tag Bits][Number Of Index Bits][Number Of Offset Bits]
	 *
	 */
	std::tuple<size_t, size_t, size_t> address_partition{0, 0, 0};

	/* #3 Information of the Dimensions for this Specific Cache
	 *
	 * [Block Size(in Bytes)][Set Associtivity][Number Of Blocks]
	 */
	std::tuple<uint32_t, uint32_t, uint32_t> dimensions{0, 0, 0};

	/* #4 2D Vector cache array containing DataBlock
	 *
	 * To access individual element from cache array, do the following:
	 * this->cache_array.at(A).at(B).getValid() access Cache[A][B].Valid - bool type is returned
	 * this->cache_array.at(A).at(B).getDirty() access Cache[A][B].Dirty - bool type is returned
	 * this->cache_array.at(A).at(B).getTag() access Cache[A][B].Tag - uint32_t type is returned
	 *
	 * A: Index of Blocks, B: Index of Associated Set
	 */
	std::vector<std::vector<DataBlock>> cache_array;

	/* Total Counts of Hits and Misses of this Cache (No need to initialize)
	 *
	 * [Number of Hits][Number of Misses]
	 */
	std::pair<uint64_t, uint64_t> hit_miss_count{0, 0};

	//#5: Number of Clock Cycles to Complete Read for this Cache
	uint64_t latency{0};//#5

	//#6 Number of Clock Cycles to Access Values from Memeory
	uint64_t memory_latency{0};//#6

	//#7 Layer ID of this Specific Cache with the lowest being 1
	size_t cache_id{0};//#7



	//Status of Initialization. All Members MUST be true before Cache Initialization
	std::array<bool, 8> ready{false, false, false, false, false, false, false, false};

	/**
	 * Decode a 32-bit Address to 3-fields Index-based Tuple indicating where the Data could be
	 * Warning: This function should ONLY be used inside this Cache Class
	 * @param address_val Raw 32-bit address
	 * @return [Index of Tag][Index of Index][Index of Offset]
	 */
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

	/**
	 * Retrieve the Pointer of Parent Cache
	 * @return Pointer of Parent Cache
	 */
	[[nodiscard]] Cache *getParentPtr() const {
		return this->parent_cache_ptr.get();
	}

	/**
	 * Set Child Cache
	 * Mark Child Cache has been set in Ready
	 * Warning: Receiving nullptr means Cache is Top Cache
	 * @param _child_ptr Pointer of Child Cache
	 */
	void setChild(Cache *_child_ptr) {
		this->child_cache_ptr = _child_ptr;
		this->ready.at(0) = true;
	}

	void makeAsTopCache() {
		this->parent_cache_ptr = nullptr;
		this->ready.at(1) = true;
	}

	/**
 * Create Parent Cache
 * Mark Parent Cache has been set in Ready
 */
	void makeParent() {
		this->parent_cache_ptr = std::make_unique<Cache>();
		this->parent_cache_ptr->setChild(this);
		this->ready.at(0) = true;
	}

	/**
	 * Retrieve the Pointer of Child Cache
	 * @return Pointer of Child Cache
	 */
	[[nodiscard]] Cache *getChildPtr() const {
		return this->child_cache_ptr;
	}

	/**
	 * Apply algorithms to find the correct Dimensions, Address Partitions of Cache.
	 * Mark Dimensions and Address Partitions has been set in Ready
	 * Warning: It does NOT initialize the Cache
	 * @param _block_size Number of Bytes that Each DataBlock can hold
	 * @param _total_size Total Number of Bytes this Cache needs to Store
	 * @param _set_assoc Number of DataBlock for a Each Given Index
	 */
	void setParam(const uint32_t &_block_size, const uint32_t &_total_size, const uint32_t &_set_assoc) {
		uint32_t num_of_cache_block = (_total_size / _block_size / _set_assoc);
		std::get<0>(this->dimensions) = _block_size;//Number of bytes each data-block can hold
		std::get<1>(this->dimensions) = _set_assoc;//Number of data-blocks does one index maps to
		std::get<2>(this->dimensions) = num_of_cache_block;//Number of cache-blocks does cache-array have
		this->ready.at(3) = true;
		std::get<2>(this->address_partition) = lround(log2l(_block_size));//Bits of Offset
		std::get<1>(this->address_partition) = lround(log2l(num_of_cache_block));//Bits of Index
		std::get<0>(this->address_partition) =
				32 - std::get<2>(this->address_partition) - std::get<1>(this->address_partition);//Bits of Tag
		this->ready.at(2) = true;
	}

	/**
	 * Set the Latency of this Specific Cache
	 * Mark Latency has been set in Ready
	 * @param _latency Number of Clock Cycles that Reading a Data Requires
	 */
	void setLatency(const uint64_t &_latency) {
		this->latency = _latency;
		this->ready.at(5) = true;
	}

	/**
	 * Set the Cache Level ID of this Specific Cache
	 * Mark Cache ID has been set in Ready
	 * @param _cache_id
	 */
	void setId(const size_t &_cache_id) {
		this->cache_id = _cache_id;
		this->ready.at(7) = true;
	}

	/**
	 * Set the MEMORY latency (should be the same across all cache layers)
	 * Mark Memory Latency has been Set in Ready
	 * @param _latency Number of Clock Cycles needed to Request Data from Memory
	 */
	void setMemoryLatency(const uint32_t &_latency) {
		this->memory_latency = _latency;
		this->ready.at(6) = true;
	}


	/**
	 * Perform Ready Check to See if Requisites are Met for Cache Array Initialization
	 * Initialize Cache Array to Correct Dimensions with Invalid Non-Dirty Zero-Tagged DataBlock
	 */
	void initCacheArray() {
		if (!this->ready.at(3))
			throw std::invalid_argument("ERR inc called before scd");
		DataBlock empty_data_block{std::get<0>(this->dimensions)};
		std::vector<DataBlock> empty_cache_block{std::get<1>(this->dimensions), empty_data_block};
		this->cache_array.resize(std::get<2>(this->dimensions), empty_cache_block);
		this->ready.at(4) = true;
	}

	/**
	 * Attempt to Read Data from a Raw 32-bit Address with Certain Policy
	 * @param _address_val Raw 32-bit Address (Memory Data)
	 * @param _policy True if WRITE-BACK, false if WRITE THRU
	 * @return Number of Clock Cycles Consumed to Read the Data
	 */
	uint64_t read(const uint32_t &_address_val, const bool &_policy) {
		//TODO - ALGORITHMS INCOMPLETE!!!
		if (!this->operator bool())
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

	/**
	 * Check if ALL Data Members Are Initialized, including Cache Array
	 * @return True if All Initialized, false if At Least One Member if NOT Initialized
	 */
	explicit operator bool() const {
		bool if_no_false_found = std::find(this->ready.begin(), this->ready.end(), false) == this->ready.end();
		return if_no_false_found;
	}

	/**
	 * Report Hit and Misses Count to File.
	 * @param _global_writer_ptr
	 */
	void printHitMissRate(const uint64_t &_arrive_time) {
/*		if (_global_writer_ptr == nullptr)
			throw std::runtime_error("ERR File Manipulator is NULL");
		if (!this->ready.at(7))
			throw std::runtime_error("ERR Cannot Print Rate - Ofstream Not Ready");
		uint64_t sum = (this->hit_miss_count.first + this->hit_miss_count.second);
		uint32_t hit_rate = 100 * (this->hit_miss_count.first / sum);
		uint32_t miss_rate = 100 * (this->hit_miss_count.second / sum);
		(*_global_writer_ptr)
				<< "##HITS MISSES FOR CACHE L" << this->cache_id << std::endl
				<< "    HITS:      " << std::setw(10) << this->hit_miss_count.first << std::endl
				<< "    MISSES:    " << std::setw(10) << this->hit_miss_count.second << std::endl
				<< "    HIT  RATE: " << std::setw(10) << hit_rate << std::endl
				<< "    MISS RATE: " << std::setw(10) << miss_rate << std::endl;*/
	}

	void printCacheImage(const uint64_t &_arrive_time) {
		std::string image_name = "img_" + std::to_string(this->cache_id) + "_" + std::to_string(_arrive_time) + ".csv";
		std::ofstream image_writer{image_name};
		//Print Titles
		image_writer << "BLOCK_INDEX,VALID,DIRTY";
		for (size_t db = 0; db < std::get<0>(this->dimensions); db++)
			image_writer << ",TAG[" + std::to_string(db) + "]";
		image_writer << std::endl;
		//Print Contents
		for (size_t row = 0; row < std::get<2>(this->dimensions); row++) {
			for (size_t col = 0; col < std::get<1>(this->dimensions); col++) {
				image_writer << "CACHE[" + std::to_string(row) + "][" + std::to_string(col) + "]";
				const DataBlock &this_db = this->cache_array.at(row).at(col);
				image_writer << "," + std::to_string(this_db.getValid()) + "," + std::to_string(this_db.getDirty());
				for (size_t db = 0; db < std::get<0>(this->dimensions); db++)
					image_writer << "," + std::to_string(this_db.getTag());
				image_writer << std::endl;
			}
		}
		image_writer.close();
	}

/*	*//**
	 * Report Contents of This Specific Cache to a New File
	 * Open and Write to "image_L(Cache Level).txt"
	 *//*
	void printCacheImage2() {


		std::ofstream outfile;
		std::string filename = "image_L" + std::to_string(cache_id) + ".txt";
		outfile.open(filename);
		if (!outfile.is_open()) {
			throw std::runtime_error("Couldn't open image file");
		}

		//Get the number of rows and columns
		uint32_t colNum = std::get<1>(this->dimensions);
		uint32_t rowNum = std::get<2>(this->dimensions);

		std::cout << "Printing Image for Cache" << cache_id << "with" << rowNum << "rows and " << colNum << "cols"
				  << std::endl;

		//Printing the first line:
		outfile << "Set Index || ";
		for (uint32_t i = 0; i < colNum - 1; i++) {
			outfile << "Block Index | Valid | Dirty | Tag || ";
		}
		outfile << "Block Index | Valid | Dirty | Tag" << std::endl;

		//Printing the content:
		uint32_t setIndex, blockIndex, tag;
		bool valid, dirty;
		for (uint32_t i = 0; i < rowNum; i++) {
			setIndex = i;
			outfile << setIndex << " || ";
			for (uint32_t j = 0; j < colNum - 1; j++) {
				blockIndex = setIndex + j;
				valid = cache_array.at(i).at(j).getValid();
				dirty = cache_array.at(i).at(j).getDirty();
				tag = cache_array.at(i).at(j).getTag();
				outfile << blockIndex << " | " << valid << " | " << dirty << " | " << tag << " || ";
			}
			// Last block in the row:
			blockIndex = setIndex + colNum - 1;
			valid = cache_array.at(i).at(colNum - 1).getValid();
			dirty = cache_array.at(i).at(colNum - 1).getDirty();
			tag = cache_array.at(i).at(colNum - 1).getTag();
			outfile << blockIndex << " | " << valid << " | " << dirty << " | " << tag << std::endl;
		}
		std::cout << "Finished Printing Image for Cache" << cache_id << std::endl;
		outfile.close();
	}*/


};


#endif //CODE_CACHE_H
