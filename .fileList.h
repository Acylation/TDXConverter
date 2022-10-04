#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <io.h> 
#include <algorithm>
using namespace std;

//写class解决递归使用vector的问题 https://zhuanlan.zhihu.com/p/464734574
class fileList
{
private:
    vector<string> m_pathList;
    string m_srcDir;//拆分成srcDir（忠实于输入）, root（文件则修剪，文件夹则继续）
    bool m_recursion = 0;      //参数，是否递归，1为递归获取文件夹下的所有文件，0是仅读取本层文件
    void getFilePath(string);

public:
    fileList(string); //构造时初始化
    ~fileList() {}
    vector<string> exportList();
    void setRecursion(bool flag);
    //新增mk功能（查询到不存在的路径并mk出目录树）
};

//---implement---

fileList::fileList(string srcDir) //构造时初始化
{
    m_srcDir = srcDir;
}

vector<string> fileList::exportList()
{
    getFilePath(m_srcDir);
    return m_pathList;
}

//Criterions,重构过了，需要测试
bool existItem(string srcPath)
{
    fileInfo item;
    intptr_t HANDLE = _findfirst(srcPath.c_str(), &item);
    if (HANDLE != -1L)
    {
        _findclose(HANDLE);
        return true;
    }
    else
        return false;
}

bool isFile(string srcPath)
{
    if(existItem(srcPath))
    {
        fileInfo item;
        intptr_t HANDLE;
        HANDLE = _findfirst((srcPath + "\\*.*").c_str(), &item);
        if (HANDLE == -1L)
            return true;
        else
            return false;
        _findclose(HANDLE);
    }
    else
        return false;
}

bool isFolder(string srcPath)
{
    if(existItem(srcPath))
    {
        fileInfo item;
        intptr_t HANDLE;
        HANDLE = _findfirst(srcPath.c_str(), &item);
        if (item.attrib & _A_SUBDIR)
            return true;
        else
            return false;
        _findclose(HANDLE);
    }
    else
        return false;
}

bool isFolder(fileInfo item)
{
    if (item.attrib & _A_SUBDIR)
        return true;
    else
        return false;
}

bool validFolder(fileInfo folder)
{
    if ((strcmp(folder.name, ".") != 0) && (strcmp(folder.name, "..") != 0))
        return true;
    else
        return false;
}

void fileList::getFilePath(string srcDir)
/*
    Reference: https://blog.csdn.net/xiexu911/article/details/79990774
    CSDN User: nancheng911
    使用io.h中的_findfirst/_findnext读母文件夹下的所有文件（递归）
    关键：_finddata_t结构体
*/
{
    fileInfo item;
    intptr_t HANDLE;
    string specifiedDir = srcDir + "\\*.*";
    HANDLE = _findfirst(specifiedDir.c_str(), &item);
    if (HANDLE == -1L) //没有子项目
    {
        if (existItem(srcDir)) //没有子项目的同时是条目，是文件
        {
            m_pathList.push_back(srcDir);
            return;
        }
        else
        {
            cout << "路径错误" << endl; //该路径下没有子项目，且不是文件
            return;
        }   
    }
    do
    {
        if (!isFolder(item)) //路径正确，又不是subFolder，所以是subFile
            m_pathList.push_back(srcDir + "\\" + item.name);
        else if (m_recursion && validFolder(item))//确认递归排除".",".."
            getFilePath(srcDir + "\\" + item.name);
    } while (_findnext(HANDLE, &item) == 0);
    _findclose(HANDLE);
}

void fileList::setRecursion(bool flag)
{
    m_recursion = flag ;
}

string splitPath(string srcPath,string path, int flag)//复用性不好，修
{
    int pos1 = path.rfind("\\");
    int pos2 = path.rfind(".");
    switch(flag)
    {
        case 0:
            return path;//src+dir+name+suffix
        case 1:
            return path.substr(srcPath.size(), pos1-srcPath.size());//dir only
        case 2:
            return path.substr(pos1, path.size() - pos2 - 1);//name+suffix
        case 3:
            return path.substr(pos1 + 1, pos2 - pos1 - 1);//name
        case 4:
            return path.substr(pos2 + 1, path.size() - pos2 - 1);//suffix
    }
    return 0;
}
/*
bool isFile(string srcPath)
{
    _finddata_t item;
    intptr_t HD1, HD2;
    HD1 = _findfirst(srcPath.c_str(), &item);
    HD2 = _findfirst((srcPath + "\\*.*").c_str(), &item);
    if (HD1 != -1L && HD2 == -1L)
        return true;
    _findclose(HD1);
    _findclose(HD2);
    return false;
}

bool isFolder(string srcPath)
{
    _finddata_t item;
    intptr_t HD1, HD2;
    HD1 = _findfirst(srcPath.c_str(), &item);
    HD2 = _findfirst((srcPath + "\\*.*").c_str(), &item);
    if (HD1 != -1L && HD2 == -1L)
        return true;
    _findclose(HD1);
    _findclose(HD2);
    return false;
}
*/