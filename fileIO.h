#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <io.h> 

using namespace std;

/*
struct _finddata64i32_t
{
    unsigned    attrib;
    __time64_t  time_create;    // -1 for FAT file systems
    __time64_t  time_access;    // -1 for FAT file systems
    __time64_t  time_write;
    _fsize_t    size;
    char        name[260];
};
*/

//先构建无需微软方言版本的，即，暂不考虑time和attrib的输出，但保留fileInfo类以便拓展
//windows方言包括locale、wchar &　wstring 等，适配中文文件名
//fileInfo类的作用：将本地版本转化成标准版本数据结构，并提供存储、输出、显示接口

class fileInfo
{
private:
    //类型属性，位或判断
    unsigned attrib;

    //时间信息，需要调用time库做本地化，便于输出
    /*
    __time64_t time_create;    // -1 for FAT file systems
    __time64_t time_access;    // -1 for FAT file systems
    __time64_t time_write;
    */

    //unsigned long
    unsigned long size;

    //from char name[260]
    string name;

public:
    fileInfo();
    fileInfo(_finddata_t item);
    ~fileInfo();
    void print();
    void localize();
    //void export();
};

fileInfo::fileInfo(_finddata_t item)
{
    attrib = item.attrib;
    size = item.size;
    name = item.name;
}

fileInfo::~fileInfo()
{
}


//模块备注
//mkdir module (精确mk or 记录下目录树后直接mk? 前者需要校验路径存在)
//之前的操作时保持目录结构，直接在root下新建一个processed，把目录树放进去
//考虑新建一个root_processed文件夹，放目录树，但需要排除原目录是盘符
//不过对盘符使用..\命令，会自动判定为本层次文件夹


//写class解决递归使用vector的问题 https://zhuanlan.zhihu.com/p/464734574
class fileList
{
private:
    vector<fileInfo> m_fileInfoList;
    vector<string> m_pathList;
    string m_srcDir;//initiation
    string m_dstDir;//ask input when folder
    string m_filter;//ask input when folder
    bool m_recursion = 0;//参数，是否递归，1为递归获取文件夹下的所有文件，0是仅读取本层文件
    void getFilePath(string);//封装实现

public:
    fileList(string); //构造时初始化
    ~fileList() {}
    vector<string> exportList();
    void setRecursion();//if folder
    void setFilter();//改进:多filter支持
    void setDst();//if folder
    //新增mk功能（查询到不存在的路径并mk出目录树）
};

//---implement---

fileList::fileList(string inputString) //构造时初始化
{
    m_srcDir = inputString;
}

//Criterions,验证输入用
bool existItem(string itemPath)//暂时只支持英文
{
    _finddata_t item;
    intptr_t HANDLE = _findfirst(itemPath.c_str(), &item);
    if (HANDLE != -1L)
    {
        _findclose(HANDLE);
        return true;
    }
    else
    {
        _findclose(HANDLE);
        return false;
    }
}

bool isFile(string itemPath)
{
    _finddata_t item;
    intptr_t HANDLE;
    HANDLE = _findfirst((itemPath + "\\*.*").c_str(), &item);
    if (HANDLE == -1L)//只有文件会返回空Handle
    {
        _findclose(HANDLE);
        return true;
    }
    else//文件夹至少能找到./和../
    {
        _findclose(HANDLE);
        return false;
    }
}

bool isFolder(string itemPath)
{
    _finddata_t item;
    intptr_t HANDLE;
    HANDLE = _findfirst((itemPath + ".\\").c_str(), &item);
    if (HANDLE != -1L)
    {
        _findclose(HANDLE);
        return false;
    }
    else
    {
        _findclose(HANDLE);
        return true;
    }

    /*
    HANDLE = _findfirst(itemPath.c_str(), &item);
    if (item.attrib & _A_SUBDIR)
    {
        _findclose(HANDLE);
        return true;
    }
    else
    {
        _findclose(HANDLE);
        return false;
    }
    */
}

//Criterion,查找中使用
bool isFolder(_finddata_t item)
{
    if (item.attrib & _A_SUBDIR)
        return true;
    else
        return false;
}

bool validFolder(_finddata_t folder)
{
    if ((strcmp(folder.name, ".") != 0) && (strcmp(folder.name, "..") != 0))
        return true;
    else
        return false;
}

//请求输入
void fileList::setRecursion()
{
    string flag;
    cout<<"是否递归查找? Y:是 N:否"<<endl;
    cin>>flag;
    if(flag == "Y" || flag == "1")
        m_recursion =  true;
    else if(flag == "N" || flag == "0")
        m_recursion = false;
    else
        setRecursion();
}

void fileList::setFilter()//改进:多filter支持
{
    string filter = "*.";//default
    string suffix;
    cout<<"请输入所需文件格式缩写,"<<endl;
    cout<<"如csv, dat, 查找所有文件请输入星号*:"<<endl;
    cin>>suffix;
    filter += suffix;
    m_filter = filter;
}

void fileList::setDst()//if folder
{
    string dstDir;
    cout<<"请输入目标文件夹:"<<endl;
    cin>>dstDir;
    m_dstDir = dstDir;
}

//已知是文件夹时，查找全部文件
void fileList::getFilePath(string folderPath)//Reference: https://blog.csdn.net/xiexu911/article/details/79990774
{
    _finddata_t item;
    intptr_t HANDLE;
    string specifiedDir = folderPath + "\\" + m_filter;
    HANDLE = _findfirst(specifiedDir.c_str(), &item);
    do
    {
        if (!isFolder(item)) //路径正确，又不是subFolder，所以是subFile
        {
            m_pathList.push_back(folderPath + "\\" + item.name);
            //m_fileInfoList.push_back();
        }
        else if (m_recursion && validFolder(item))//确认递归排除".",".."
            getFilePath(folderPath + "\\" + item.name);
    } while (_findnext(HANDLE, &item) == 0);
    _findclose(HANDLE);
}

void printAll(string folderPath)
{
    vector<string> list;
    _finddata_t item;
    intptr_t HANDLE;
    string specifiedDir = folderPath + "\\*.*";
    HANDLE = _findfirst(specifiedDir.c_str(), &item);
    do
    {
        list.push_back(folderPath + "\\" + item.name);//m_fileInfoList.push_back();
    } while (_findnext(HANDLE, &item) == 0);
    _findclose(HANDLE);
    for(int i=0; i<list.size(); i++)
    {
        cout<<list.at(i)<<endl;
    }
}

void getDrives()
{

}

/*
vector<string> fileList::exportList()
{
    getFilePath(m_srcDir);
    return m_pathList;
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
*/