//
// Created by 窦正 on 2023/11/5.
//

#ifndef HUFFMAN_CODING_BIN_H
#define HUFFMAN_CODING_BIN_H

#include <fstream>
#include <map>
#include <boost/dynamic_bitset.hpp>
using namespace std;

class BIN {
    public:
        BIN(ifstream &binFile, ofstream &txtFile);
        bool err;
    private:
        map<char, boost::dynamic_bitset<unsigned char>> dict;
        map<unsigned long long, char> decodeDict;
        unsigned char minLen, maxLen, extra;
        map<unsigned char, unsigned char> lenCnt;
        map<unsigned char, unsigned long long> firstLenCode;
};

#endif //HUFFMAN_CODING_BIN_H
