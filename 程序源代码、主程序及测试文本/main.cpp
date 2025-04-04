#include <iostream>
#include "TXT.h"
#include "BIN.h"
#include <chrono>
#include <sys/stat.h>

using namespace std;

size_t getFileSize(const char *fileName) {
    struct stat file{};
    stat(fileName, &file);
    size_t filesize = file.st_size;
    return filesize;
}

int main() {
    //将代码页字符集更改为UTF-8
    system("chcp 65001");
    system("cls");
    cout << endl;

    //简介
    cout << "本程序是由 窦正 和 郑鸿鑫 联合开发的文本编解码测试终端。" << endl;
    cout << "我们采用 数组模拟建立哈夫曼树 和 范式哈夫曼编码 的方式，实现了比传统算法更高效的一种ASCII码文本编解码算法。" << endl;
    cout << endl;
    cout << "首先，用户需要指定一个仅含英文字符的工作目录，程序的所有文件读写操作均在指定的工作目录下进行。" << endl;
    cout << endl;

    //主程序
    bool exit = false;
    while(!exit) {
        //指定工作目录
        string path;
        cout << "请指定一个工作目录：";
        cin.clear();
        cin.sync();
        getline(cin, path);
        while(chdir(path.c_str())) {
            system("cls");
            cout << endl;
            cout << "工作目录指定失败！" << endl;
            cout << "请重新输入工作目录：";
            path.clear();
            cin.clear();
            cin.sync();
            getline(cin, path);
        }
        system("cls");
        cout << endl;

        //功能选单
        bool reset = false;
        while(!reset) {
            system("cls");
            cout << endl;
            cout << "功能选单：" << endl;
            cout << endl;
            cout << "0. 重新指定工作目录" << endl;
            cout << "1. 压缩文本文件" << endl;
            cout << "2. 解压二进制文件" << endl;
            cout << "3. 退出程序" << endl;
            cout << endl;
            cout << "请键入数字，按回车继续：";
            char f;
            cin.clear();
            cin.sync();
            cin >> f;
            system("cls");
            cout << endl;
            switch(f) {
                //重新指定工作目录
                case '0': {
                    reset = true;
                    break;
                }
                //压缩文本文件
                case '1': {
                    //读入文本文件
                    string fileName;
                    cout << "请输入要压缩的文本文件名：";
                    cin.clear();
                    cin.sync();
                    getline(cin, fileName);
                    ifstream txtFile(fileName);
                    while (!txtFile) {
                        system("cls");
                        cout << endl;
                        cout << "文件打开失败！请重新输入文件名：";
                        fileName.clear();
                        cin.clear();
                        cin.sync();
                        getline(cin, fileName);
                        txtFile.open(fileName);
                    }
                    size_t txtSize = getFileSize(fileName.c_str());
                    system("cls");
                    cout << endl;
                    //压缩文本文件并存储二进制文件
                    do {
                        cout << "请输入压缩后的二进制文件名：";
                        cin.clear();
                        cin.sync();
                        getline(cin, fileName);
                        system("cls");
                        cout << endl;
                    } while(fileName.empty());
                    ofstream binFile(fileName, ios::out | ios::trunc | ios::binary);
                    cout << "压缩中......";
                    auto start = chrono::high_resolution_clock::now();      //压缩计时起点
                    TXT txt(txtFile);
                    txt.generateCharCode();
                    if(!txt.err) {
                        txt.initDict();
                        txt.saveFile(txtFile, binFile);
                        auto end = chrono::high_resolution_clock::now();        //压缩计时终点
                        txtFile.close();
                        binFile.close();
                        system("cls");
                        cout << endl;
                        //统计并输出压缩信息
                        size_t binSize = getFileSize(fileName.c_str());
                        double ratio = (double)binSize / (double)txtSize * 100;
                        chrono::duration<double> duration = end - start;
                        cout << "压缩用时  " << duration.count() << " 秒" << endl;
                        cout << endl;
                        cout << "文本文件大小为  " << txtSize << " 字节" << endl;
                        cout << "压缩文件大小为  " << binSize << " 字节" << endl;
                        cout << endl;
                        cout << "压缩率为  " << ratio << " %" << endl;
                        cout << endl;
                    } else {
                        txtFile.close();
                    }
                    system("pause");
                    break;
                }
                //解压二进制文件
                case '2': {
                    //读入二进制文件
                    string fileName;
                    cout << "请输入要解压的二进制文件名：";
                    cin.clear();
                    cin.sync();
                    getline(cin, fileName);
                    ifstream binFile(fileName, ios::in | ios::binary);
                    while (!binFile) {
                        system("cls");
                        cout << endl;
                        cout << "文件打开失败！请重新输入文件名：";
                        fileName.clear();
                        cin.clear();
                        cin.sync();
                        getline(cin, fileName);
                        binFile.open(fileName, ios::in | ios::binary);
                    }
                    system("cls");
                    cout << endl;
                    //解压二进制文件并存储文本文件
                    do {
                        cout << "请输入解压后的文本文件名：";
                        cin.clear();
                        cin.sync();
                        getline(cin, fileName);
                        system("cls");
                        cout << endl;
                    } while(fileName.empty());
                    cout << "解压中......";
                    ofstream txtFile(fileName);
                    auto start = chrono::high_resolution_clock::now();      //压缩计时起点
                    BIN bin(binFile, txtFile);
                    if(!bin.err) {
                        auto end = chrono::high_resolution_clock::now();        //压缩计时终点
                        system("cls");
                        cout << endl;
                        //统计并输出解压信息
                        chrono::duration<double> duration = end - start;
                        cout << "解压完成，用时 " << duration.count() << " 秒" << endl;
                        txtFile.close();
                        binFile.close();
                        cout << endl;
                    } else {
                        txtFile.close();
                        binFile.close();
                        remove(fileName.c_str());
                    }
                    system("pause");
                    break;
                }
                //退出程序
                case '3': {
                    reset = true;
                    exit = true;
                    break;
                }
                default:
                    cout << "输入非法！";
                    system("pause");
            }
        }
    }
    return 0;
}