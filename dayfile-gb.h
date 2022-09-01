#pragma once
#include <iostream>
#include <fstream>
using namespace std;

//����Դͨ���ţ�δ��Ȩ����
//����������Ҫ�����������
//2022.02.11 by Acylation
//������������Ҫ˳���д����write()��read()������readline()��<<��>>
//ref:C++�����ƶ�д https://www.cnblogs.com/cofludy/p/7871777.html
//ref:ͨ����*.day�ļ��Ľ��� https://www.cnblogs.com/zeroone/archive/2013/07/10/3181251.html

struct Data//һ��32�ֽ�
{
    int date;
    int open;//���̼�
    int high;//��߼�*100
    int low;//��ͼ�*100
    int close;//���̼�*100
    float amount;//�ɽ����λԪ����������
    int volume;//�ɽ�������λ�ɣ������֣�100��Ϊһ�֣�Ϊ��λ
    int unknown;//δ����λ�ã�ռλ
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
    ofstream dstFile(dst, ofstream::out | ofstream::trunc);//���¸�д�����Ľ���������д
    if(!dstFile.is_open())
    {
        cerr<<"Fail to create new file!"<<endl;
        return;
    }
    dstFile<<"����"<<','<<"���̼�"<<','<<"��߼�"<<','<<"��ͼ�"<<','<<"���̼�"<<','<<"�ɽ���"<<','<<"�ɽ���"<<endl;
    Data data;
    while(srcFile.read((char*) &data, sizeof(Data)))//�����ڴ����
    {
        dstFile<<data;
    }
}