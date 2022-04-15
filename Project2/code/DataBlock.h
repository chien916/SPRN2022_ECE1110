//
// Created by Yinhao on 4/9/2022.
//

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
	explicit DataBlock(const uint32_t &block_size = 0) {
		this->block_size = block_size;
	}

	DataBlock &operator=(const DataBlock &db) {
		this->valid = db.valid;
		this->tag = db.tag;
		this->block_size = db.block_size;
		this->last_use = std::chrono::steady_clock::now();
	}

	void update(const uint32_t &_tag) {
		this->last_use = std::chrono::steady_clock::now();
		this->tag = _tag;
		this->valid = true;
	}

	bool compareTag(const uint32_t &_tag) const {
		return (this->tag == _tag && this->valid);
	}

	void markDirty(const bool &_dirty) {
		this->last_use = std::chrono::steady_clock::now();
		this->dirty = _dirty;
	}

	bool getDirty() const {
		return this->dirty;
	}

	bool getValid() const{
		return this->valid;
	}

	uint32_t getTag() const{
		return this->tag;
	}


};

#endif //CODE_DATABLOCK_H
