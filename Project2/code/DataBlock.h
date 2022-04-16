#ifndef CODE_DATABLOCK_H
#define CODE_DATABLOCK_H

#include "Include.h"

class DataBlock {
private:
	bool valid{false};
	bool dirty{false};
	uint32_t tag{0};
	uint32_t block_size{0};
	std::chrono::time_point<std::chrono::steady_clock> last_use{std::chrono::steady_clock::now()};
public:

	/**
	 * Explicit constructor that receives desired Block Size.
	 * @param block_size System-wide how many bytes should a DataBlock hold
	 */
	explicit DataBlock(const uint32_t &block_size = 0) {
		this->block_size = block_size;
	}

	/**
	 * Copy valid,tag,and block_size from received Datablock, and record current time
	 * @param _data_block Received DataBlock to copy from
	 * @return
	 */
	DataBlock &operator=(const DataBlock &_data_block) {
		this->valid = _data_block.valid;
		this->tag = _data_block.tag;
		this->block_size = _data_block.block_size;
		this->last_use = std::chrono::steady_clock::now();
		return *this;
	}

	/**
	 * Update this DataBlock with new tag value, make it valid, and record current time
	 * @param _tag New tag value to be assigned to this DataBlock
	 */
	void update(const uint32_t &_tag) {
		this->valid = true;
		this->tag = _tag;
		this->last_use = std::chrono::steady_clock::now();
	}

	/**
	 * Compare if this DataBlock's tag matches a given
	 * @param _tag Tag value to be compared to
	 * @return True if tag matches; False otherwise
	 */
	bool compareTag(const uint32_t &_tag) const {
		return (this->tag == _tag && this->valid);
	}

	/**
	 * Mark this DataBlock Dirty
	 * Meaning this DataBlock needs to sync with parents
	 */
	void markDirty() {
		this->last_use = std::chrono::steady_clock::now();
		this->dirty = true;
	}

	/**
	 * Test if this Datablock needs to sync with parents
	 * @return True if dirty, false otherwise
	 */
	bool getDirty() const {
		if (!valid)
			throw std::runtime_error("ERR Dirty Bit Cannot be Retrieved - Datablock NOT Valid");
		return this->dirty;
	}

	/**
	 * Test if this Datablock has been used at least once
	 * @return True if valid, false otherwise
	 */
	bool getValid() const {
		return this->valid;
	}

	/**
	 * Retrieve the tag of this DataBlock
	 * @return Tag retrieved
	 */
	uint32_t getTag() const {
		if (!valid)
			throw std::runtime_error("ERR Tag Cannot be Retrieved - Datablock NOT Valid");
		return this->tag;
	}

};

#endif //CODE_DATABLOCK_H
