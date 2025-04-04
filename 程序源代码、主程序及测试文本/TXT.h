//
// Created by 窦正 on 2023/11/5.
//

#ifndef HUFFMAN_CODING_TXT_H
#define HUFFMAN_CODING_TXT_H

#include <fstream>
#include <map>
#include <vector>
#include "Info.h"
using namespace std;

class TXT {
    public:
        TXT(ifstream &txtFile);
        bool err;
        void generateCharCode();
        void initDict();
        void saveFile(ifstream &txtFile, ofstream &binFile);
    private:
        vector<pair<char, Info>> chars;
        map<char, boost::dynamic_bitset<unsigned char>> dict;
        unsigned char minLen, maxLen, extra;
        map<unsigned char, unsigned char> lenCnt;
};

#endif //HUFFMAN_CODING_TXT_H
