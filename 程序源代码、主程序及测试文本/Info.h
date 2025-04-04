//
// Created by 窦正 on 2023/11/5.
//

#ifndef HUFFMAN_CODING_INFO_H
#define HUFFMAN_CODING_INFO_H

#include <boost/dynamic_bitset.hpp>

struct Info {
    Info() : cnt(0) {}
    unsigned long long cnt;
    boost::dynamic_bitset<unsigned char> HuffmanCode;
};

#endif //HUFFMAN_CODING_INFO_H
