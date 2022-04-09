#ifndef CODE_CACHE_H
#define CODE_CACHE_H

#include <bits/stdc++.h>
#include "DataBlock.h"
#include "Address.h"


/*
 * SZ_TAG_BITS - How many bits from address are tag bits
 * SZ_DATA_BITS - How many bits do data fields have
 * SZ_BLOCK_SIZE - How many data fields (block) does one
 */
template<size_t SZ_TAG_BITS, size_t SZ_DATA_BYTES, size_t SZ_BLOCK_SIZE, size_t SZ_NUM_BLOCK>
class _CACHE {
    using _CACHE_BLOCK = std::array<_DATA_BLOCK<SZ_TAG_BITS, SZ_DATA_BYTES>, SZ_BLOCK_SIZE>;
    using _CACHE_ARRAY = std::array<_CACHE_BLOCK, SZ_NUM_BLOCK>;
    using _PARTITION = std::tuple<size_t, size_t, size_t>;
private:
    unsigned int latency;
    _PARTITION field_partition;
    _CACHE *parent;
    _CACHE *child;
    _CACHE_ARRAY cache_array;

public:
    explicit _CACHE(unsigned int l = 0, _CACHE *p = nullptr, _CACHE *c = nullptr) : latency(l), parent(p), child(c) {
        if (SZ_DATA_BYTES % 8 != 0)
            throw std::logic_error("SZ_BLOCK_SIZE is not in BYTES.");
        std::get<0>(this->field_partition) = SZ_TAG_BITS;
        std::get<2>(this->field_partition) = round(log2(SZ_BLOCK_SIZE));
        std::get<1>(this->field_partition) = round(log2(SZ_NUM_BLOCK));
        if (SZ_TAG_BITS != (32 - std::get<2>(this->field_partition) - std::get<1>(this->field_partition)))
            throw std::logic_error("SZ_TAG_BITS doesn't match SZ_BLOCK_SIZE and SZ_NUM_BLOCK");
    }

    bool read_data(const Address &address) {
        return false;
    }


};


#endif //CODE_CACHE_H
