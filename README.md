# TDXConverter/ 通达信日线文件转换

Convert TongDaXin dayline file (\*.day) to csv format, with basic UI  
Written by Qt & C++  

## TDX Format (*.day Files)

.day files are stored under the TDX folder \vipdoc  
each .day file is named after market+code  
Reminder: OHLC in the data are all multiplied by 100 (Do this also apply to MOUTAI? I shall try it)  

```cpp
struct Data//32 Bytes each day
{
    int date;
    int open;
    int high;
    int low;
    int close;
    float amount;//unit: 100 millon CNY
    int volume;
    int unknown;//reserved
};
```

## Log

- 22.09.02  
调整文件/文件夹适配性，重构判据函数  
头文件&主函数尚处在不可用状态  
后续需要合理设计判断逻辑/次数，性能提升  
流程：判断文件/文件夹，文件则直接按name.suffix保存到目标文件夹下，文件夹则保留原目录结构，保存到目标文件夹下

## Todo List

- [x] Convert core
  - [x] Read Binary .day file
  - [x] Struct Data
  - [x] Export as .csv
  - [x] Command Line Version (v0.1.0)
  - [x] Encoding Conflict
  Default: ANSI for excel, Utf-8 for VSCode  
  Temporary solution：main.cpp-u8，dayfile.h-GB2312
  - [ ] Batch processing by command line
- [ ] Simple UI with Qt  
  - [ ] UI, select file/folders and run
  - [ ] UI, drag to import
  - [ ] Encapsulation improve
  - [ ] Encoding Conflict (convert functions based on std::ctype)
  - [ ] Import/Export module based on QFileDialog
  - [ ] Conversion core  
  - [ ] Batch-Singular by Qt
  - [ ] QSettings (setting:bool recursion)
  - [ ] Seperated functions & header by Makefile
- [ ] Tabs & Pages
  - [ ] Main console
    - [ ] Folder Selector
    - [ ] Run buttons
    - [ ] File table
    - [ ] Tasks table / Dynamic / Progress Bar
  - [ ] Candlestick preview
    - [ ] candlestick graph
    - [ ] prettier
    - [ ] float mouse event
    - [ ] b/s mark & virtual account management
    - [ ] basic indexes
  - [ ] Datatable preview
- [ ] Prettier
  - [ ] QSS/QtStyleSheet
  - [ ] Logo
- [ ] Python Version
  - [ ] Core
  - [ ] Batch support
  - [ ] UI
  - [ ] Matplotlib
- [ ] Online data processing / Crawling
- [ ] Other offline files, such as 5mins, month etc, and stat (from day to month)
- [ ] Add Lisence  
