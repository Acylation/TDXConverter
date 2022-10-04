#include "dayfile-gb.h"
#include "fileIO.h"

int main()
{
    //input
    string srcPath, dstPath;
    bool flag;
    cout<<"请输入文件/文件夹路径:";
    cin>>srcPath;
    if(isFile(srcPath))
    {
        string dir = splitPath(srcPath, 1);
        string name = splitPath(srcPath, 3);
        binary2csv(srcPath, dir + name + ".csv");
    }
    else if(isFolder(srcPath))
    {
        cout<<"请输入保存路径:";
        cin>> dstPath;
        cout<<"是否递归查找? 1:是 0:否";//加入文件判断逻辑跳过这里？
        cin>>flag;

        //initialize
        vector<string> pathList, mkList, dirList, nameList;
        fileList fList(srcPath);
        fList.setRecursion(flag);
        pathList = fList.exportList();
        for(int i = 0; i<pathList.size(); i ++) 
        {
            mkList.push_back(splitPath(srcPath,pathList.at(i),1));
            dirList.push_back(splitPath(srcPath,pathList.at(i),1));
            nameList.push_back(splitPath(srcPath,pathList.at(i),3));
        }

        //create dstdir
        //查重合并
        sort(mkList.begin(), mkList.end());
        vector<string>::iterator iter = unique(mkList.begin(), mkList.end());
        mkList.erase(iter, mkList.end());
        for(int i = 0; i<mkList.size(); i ++) 
        {
            string command = "mkdir " + srcPath + "\\processed\\" + mkList.at(i);
            system(command.c_str());
        }

        //convert
        for(int i = 0; i<pathList.size(); i ++) 
            binary2csv(pathList.at(i), dstPath  + "\\" + dirList.at(i) + "\\" + nameList.at(i) + ".csv");
        
    }

    return 0;
}