# TDXConverter
Convert TDX dayline file (\*.day) to csv format, with UI  
Written with Qt & C++  

### TDX Format
.day files are stored under the TDX folder \vipdoc  
each .day file is named after market+code  
A reminder: OHLC in the data are all multiplied by 100 (Do this also apply to MOUTAI? I shall try it)  
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
### Todo: 
- Convert core √  
  - Read Binary .day file √  
  - Export as .csv  √  
- Simple UI with QFileDialog  
- Candlestick preview  
