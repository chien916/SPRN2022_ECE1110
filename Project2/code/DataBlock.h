//
// Created by Yinhao on 4/9/2022.
//

#ifndef CODE_DATABLOCK_H
#define CODE_DATABLOCK_H

#include "Include.h"

template<size_t SZ_TAG_BITS, size_t SZ_BLOCK_SIZE>
class _DATA_BLOCK {
private:
    bool valid;
    uint32_t tag;
    std::array<uint8_t, SZ_BLOCK_SIZE> data;
    std::chrono::time_point<std::chrono::steady_clock> last_use;
public:
    explicit _DATA_BLOCK(bool valid = false, uint32_t tag = 0, uint32_t data = 0) {
        if (uint32_t(round(log2(tag))) > SZ_TAG_BITS) throw std::overflow_error("SZ_TAG_BITS could not fit TAG");
        if (uint32_t(round(log2(data))) > SZ_BLOCK_SIZE) throw std::overflow_error("SZ_DATA_BITS could not fit DATA");
        this->last_use = std::chrono::steady_clock::now();
        this->valid = valid;
        this->tag = tag;
        this->data = data;
    }

    inline bool ifValid() {
        return this->valid;
    }

    uint32_t getTag() {
        return this->tag;
    }

    uint32_t getData(uint32_t offset) {
        this->last_use = std::chrono::steady_clock::now();
        return this->data.at(offset);
    }

    void setTagAndData(uint32_t tag, const std::array<uint8_t, SZ_BLOCK_SIZE> &data) {
        if (uint32_t(round(log2(tag))) > SZ_TAG_BITS) throw std::overflow_error("SZ_TAG_BITS could not fit TAG");
        this->last_use = std::chrono::steady_clock::now();
        this->tag = tag;
        std::copy(data.begin(), data.end(), this->data.begin());
    }


};

#endif //CODE_DATABLOCK_H
