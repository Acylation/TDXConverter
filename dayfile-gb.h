#pragma once
#include <iostream>
#include <fstream>
using namespace std;

//数据源通达信，未复权日线
//数据类型需要做计算机适配
//2022.02.11 by Acylation
//二进制数据需要顺序读写，用write()和read()，不用readline()和<<、>>
//ref:C++二进制读写 https://www.cnblogs.com/cofludy/p/7871777.html
//ref:通达信*.day文件的解析 https://www.cnblogs.com/zeroone/archive/2013/07/10/3181251.html

struct Data//一共32字节
{
    int date;
    int open;//开盘价
    int high;//最高价*100
    int low;//最低价*100
    int close;//收盘价*100
    float amount;//成交额，单位元，数量级亿
    int volume;//成交量，单位股，常用手（100股为一手）为单位
    int unknown;//未解析位置，占位
    friend ofstream& operator<<(ofstream&, Data&);
    friend ostream& operator<<(ostream&, Data&);
};

void binary2csv(string, string);

//---implement---

ofstream& operator<<(ofstream& out, Data& r_data)
{
    out<<r_data.date<<','<<r_data.open/100.0<<','<<r_data.high/100.0<<','<<r_data.low/100.0<<','<<
        r_data.close/100.0<<','<<r_data.amount<<','<<r_data.volume<<endl;
    return out;
}

ostream& operator<<(ostream& out, Data& r_data)
{
    out<<r_data.date<<','<<r_data.open/100.0<<','<<r_data.high/100.0<<','<<r_data.low/100.0<<','<<
        r_data.close/100.0<<','<<r_data.amount<<','<<r_data.volume<<endl;
    return out;
}

void binary2csv(string src, string dst)
{
    ifstream srcFile(src, ifstream::in | ifstream::binary);
    if(!srcFile.is_open())
    {
        cerr<<"Fail to open binfile!"<<endl;
        return;
    }
    ofstream dstFile(dst, ofstream::out | ofstream::trunc);//重新覆写，待改进成增量覆写
    if(!dstFile.is_open())
    {
        cerr<<"Fail to create new file!"<<endl;
        return;
    }
    dstFile<<"日期"<<','<<"开盘价"<<','<<"最高价"<<','<<"最低价"<<','<<"收盘价"<<','<<"成交额"<<','<<"成交量"<<endl;
    Data data;
    while(srcFile.read((char*) &data, sizeof(Data)))//操作内存接收
    {
        dstFile<<data;
    }
}