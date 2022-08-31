#include "dayfile-gb.h"

int main()
{
    string srcPath, dstPath;
    cout<<"请输入源文件路径:";
    cin>>srcPath;
    cout<<"请输入目标路径：";
    cin>>dstPath;
    binary2csv(srcPath, dstPath);
    return 0;
}
