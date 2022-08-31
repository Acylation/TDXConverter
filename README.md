# TDXConverter/ 通达信日线文件转换
Convert TongDaXin dayline file (\*.day) to csv format, with basic UI  
Written by Qt & C++  

### TDX Format
.day files are stored under the TDX folder \vipdoc  
each .day file is named after market+code  
Reminder: OHLC in the data are all multiplied by 100 (Do this also apply to MOUTAI? I shall try it)  
```
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
### Todo List: 
- Convert core √  
  - Read Binary .day file √ 
  - Struct Data √ 
  - Export as .csv  √  
  - Command Line Version (v0.1.0) √
  - Encoding Conflict √  
  Default: ANSI for excel, Utf-8 for VSCode  
  Temporary solution：main.cpp-u8，dayfile.h-GB2312
- Simple UI with Qt  
  - UI, select file/folders and run
  - UI, drag to import
  - Encapsulation improve
  - Encoding Conflict (convert functions based on std::ctype)
  - Import/Export module based on QFileDialog
  - Conversion core 
  - Batch-Singular
  - QSettings (setting:bool recursion)
  - Seperated functions & header by Makefile
- Tabs & Pages
  - Main console
    - Folder Selector
    - Run buttons
    - File table
    - Tasks table / Dynamic / Progress Bar
  - Candlestick preview
    - candlestick graph
    - prettier
    - float mouse event
    - b/s mark & virtual account management
    - basic indexes
  - Datatable preview
- Prettier
  - QSS/QtStyleSheet
  - Logo
- Python Version
- Online data processing

