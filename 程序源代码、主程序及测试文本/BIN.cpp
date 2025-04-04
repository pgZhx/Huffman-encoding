//
// Created by 窦正 on 2023/11/5.
//

#include <iostream>
#include "BIN.h"
using namespace std;

BIN::BIN(ifstream &binFile, ofstream &txtFile) {
    try {
        binFile.read((char*) &minLen, sizeof(minLen));
        binFile.read((char*) &maxLen, sizeof(maxLen));
        if(minLen > maxLen)
            throw invalid_argument("ERROR: 二进制文件不符合编码规范！");
        for(unsigned char i = minLen; i <= maxLen; i++) {
            unsigned char cnt;
            binFile.read((char*) &cnt, sizeof(cnt));
            lenCnt[i] = cnt;
        }
        unsigned long long bin = 0;
        for(auto &p: lenCnt) {
            firstLenCode[p.first] = bin;
            for(int i = 0; i < p.second; i++, bin++) {
                char ch;
                binFile.read((char*) &ch, sizeof(ch));
                dict[ch].append(bin);
                dict[ch].resize(p.first);
                decodeDict[bin] = ch;
            }
            bin <<= 1;
        }

        binFile.read((char*) &extra, sizeof(extra));
        if(extra >= 8)
            throw invalid_argument("ERROR: 二进制文件不符合编码规范！");

        unsigned long long cur = binFile.tellg();
        binFile.seekg(0, ios::end);
        unsigned long long end = binFile.tellg();
        binFile.seekg(cur, ios::beg);
        boost::dynamic_bitset<unsigned char> CompressedCode;

        unsigned char tmp[end - cur];
        binFile.read((char*) &tmp, sizeof(tmp));

        for(unsigned i = 0; i < end - cur; i++) {
            CompressedCode.append(tmp[i]);
            CompressedCode.resize((i + 1) * 8);
        }

        CompressedCode.resize(CompressedCode.size() - extra);
        unsigned codeLen = minLen;
        while(!CompressedCode.empty()) {
            boost::dynamic_bitset<unsigned char> code(CompressedCode);
            code >>= CompressedCode.size() - codeLen;
            if(code.to_ulong() >= firstLenCode[codeLen]) {
                if(codeLen == maxLen || codeLen == CompressedCode.size()) {
                    char ch = decodeDict[code.to_ulong()];
                    txtFile << ch;
                    CompressedCode.resize(CompressedCode.size() - codeLen);
                    codeLen = minLen;
                } else {
                    codeLen++;
                }
            } else {
                code >>= 1;
                char ch = decodeDict[code.to_ulong()];
                txtFile << ch;
                CompressedCode.resize(CompressedCode.size() - codeLen + 1);
                codeLen = minLen;
            }
        }
    } catch(bad_alloc &ba) {
        err = true;
        system("cls");
        cout << endl;
        cout << "ERROR: 解码异常！";
        return;
    } catch(invalid_argument &ia) {
        err = true;
        system("cls");
        cout << endl;
        cout << ia.what();
        return;
    }
    err = false;
}