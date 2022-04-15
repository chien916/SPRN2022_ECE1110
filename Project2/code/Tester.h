/*
//
// Created by Yinhao on 4/9/2022.
//

#ifndef CODE_TESTER_H
#define CODE_TESTER_H

#include "Cache.h"

class Tester {
public:
    void testAddress() {
        Address test1(std::make_tuple(4, 8, 20));
        for (uint32_t i = 1; i != 0; i *= 2) {
            std::string bits = std::bitset<32>(i).to_string();
            test1.set(i);
            std::cout << "Address - Testing Address<4, 8, 20> - Addr=" << i;
            assert(test1.getTagValue() ==
                   std::bitset<4>(std::string(bits.begin(), bits.begin() + 4)).to_ulong());
            assert(test1.getIndexValue() ==
                   std::bitset<8>(std::string(bits.begin() + 4, bits.begin() + 12)).to_ulong());
            assert(test1.getOffsetValue() ==
                   std::bitset<20>(std::string(bits.begin() + 12, bits.begin() + 32)).to_ulong());

            std::cout << "  - Passed" << std::endl;
        }
        auto i = 0;
    }
};


#endif //CODE_TESTER_H
*/
