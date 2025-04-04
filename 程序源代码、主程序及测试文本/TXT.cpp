//
// Created by 窦正 on 2023/11/5.
//

#include "TXT.h"
#include <algorithm>
#include <iostream>

using namespace std;

TXT::TXT(ifstream &txtFile) {
    char ch;
    while(txtFile.get(ch)) {
        bool appear = false;
        for(auto &p: chars) {
            if(p.first == ch) {
                appear = true;
                p.second.cnt++;
                break;
            }
        }
        if(!appear) {
            Info info;
            info.cnt = 1;
            chars.emplace_back(ch, info);
        }
    }
}

void TXT::generateCharCode(){
    const unsigned n = chars.size();

    if(n == 0) {
        err = true;
        system("cls");
        cout << endl;
        cout << "ERROR: 文本文件为空！";
        return;
    }
    if(n == 1) {
        minLen = 1;
        maxLen = 1;
        chars.begin()->second.HuffmanCode.append(0);
        chars.begin()->second.HuffmanCode.resize(1);
        lenCnt[1] = 1;
        return;
    }

    sort(chars.begin(),chars.end(),
         [](const pair<char, Info> &a, const pair<char, Info> &b) { return a.second.cnt < b.second.cnt; });

    unsigned long long tree[2*n];
    tree[0] = -1;
    for(int i = 0, j = 1; i < n; i++, j++)
        tree[j] = chars[i].second.cnt;

    bool flags[2*n];
    for(auto &b: flags)
        b = false;

    //模拟生成Huffman tree
    unsigned s1, s2;
    for(unsigned i = n + 1; i < 2*n; i++) {
        s1 = s2 = 0;
        for(unsigned j = 1; j < i; j++) {
            if(!flags[j]) {
                if(tree[j] < tree[s1]) {
                    s2 = s1;
                    s1 = j;
                } else if(tree[j] < tree[s2]) {
                    s2 = j;
                }
            }
        }
        tree[i] = tree[s1] + tree[s2];
        tree[s1] = tree[s2] = i;
        flags[s1] = flags[s2] = true;
    }

    tree[2*n-1] = 0;                    //根节点码长为0，从根开始，求每个编码的码长
    for(int i = 2*n - 2; i > 0; i--)
        tree[i] = tree[tree[i]] + 1;    //节点码长等于双亲节点码长加1

    minLen = tree[n];
    maxLen = tree[1];

    unsigned long long bin = 0;     //记录当前编码的二进制数值，初值为0
    unsigned i = n;                 //码长数组tree中的下标，初始化为n
    unsigned len = tree[n];

    for(auto it = chars.rbegin(); it < chars.rend(); it++) {
        while(tree[i] > len) {     //码长不等
            bin <<= 1;
            len++;
            lenCnt[len] = 0;
        }

        boost::dynamic_bitset<unsigned char> tmp(tree[i], bin);
        it->second.HuffmanCode = tmp;

        lenCnt[len]++;
        bin++;
        i--;
    }
}

void TXT::initDict() {
    for(auto &p: chars)
        dict[p.first] = p.second.HuffmanCode;
}

void TXT::saveFile(ifstream &txtFile, ofstream &binFile) {
    if(chars.empty())
        return;
    binFile.write((const char*) &minLen, sizeof(minLen));
    binFile.write((const char*) &maxLen, sizeof(maxLen));
    for(auto &p: lenCnt)
        binFile.write((const char*) &p.second, sizeof(p.second));
    for(auto it = chars.rbegin(); it < chars.rend(); it++)
        binFile.write((const char*) &(*it).first, sizeof((*it).first));
    txtFile.clear();
    txtFile.seekg(0, ios::beg);
    boost::dynamic_bitset<unsigned char> CompressedCode(0);
    char ch;
    while(txtFile.get(ch)) {
        CompressedCode.resize(CompressedCode.size() + dict[ch].size(), 0);
        boost::dynamic_bitset<unsigned char> tmp = dict[ch];
        tmp.resize(CompressedCode.size(), 0);
        CompressedCode <<= dict[ch].size();
        CompressedCode |= tmp;
    }
    extra = (8 - CompressedCode.size() % 8) % 8;
    if(extra)
        CompressedCode.resize((CompressedCode.size() / 8 + 1) * 8, 0);
    binFile.write((const char*) &extra, sizeof(extra));
    boost::to_block_range(CompressedCode, ostream_iterator<unsigned char>(binFile));
}