//
// Created by Yinhao on 4/9/2022.
//

#ifndef CODE_ADDRESS_H
#define CODE_ADDRESS_H

class Address {
private:
    uint32_t address_val;
    std::tuple<uint32_t, uint32_t, uint32_t> field_val;
    std::tuple<size_t, size_t, size_t> field_partition;
public:
    Address(const std::tuple<size_t, size_t, size_t> &sz) {
        std::get<0>(this->field_partition) = std::get<0>(sz);
        std::get<1>(this->field_partition) = std::get<1>(sz);
        std::get<2>(this->field_partition) = std::get<2>(sz);
        this->address_val = 0;
        if (std::get<0>(this->field_partition) + std::get<1>(this->field_partition) +
            std::get<2>(this->field_partition) != 32)
            throw std::logic_error("SZ_TAG_BITS+SZ_INDEX_BITS+SZ_OFFSET_BITS does not equal 32");
    }

    void set(uint32_t address_val) {
        this->address_val = address_val;
        std::get<0>(field_val) =
                address_val >> (std::get<2>(this->field_partition) + std::get<1>(this->field_partition));
        std::get<1>(field_val) =
                address_val << std::get<0>(this->field_partition)
                            >> (std::get<2>(this->field_partition) + std::get<0>(this->field_partition));
        std::get<2>(field_val) =
                address_val << (std::get<0>(this->field_partition) + std::get<1>(this->field_partition))
                            >> (std::get<0>(this->field_partition) + std::get<1>(this->field_partition));
    }

    uint32_t getTagValue() {
        return std::get<0>(field_val);
    }

    uint32_t getIndexValue() {
        return std::get<1>(field_val);
    }

    uint32_t getOffsetValue() {
        return std::get<2>(field_val);
    }

    uint32_t getAddressValue() {
        return this->address_val;
    }


};


#endif //CODE_ADDRESS_H
