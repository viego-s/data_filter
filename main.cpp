#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip> 
using namespace std;

// 记录滑动平均窗口状态的结构体
typedef struct {
    vector<double> window;  // 存储滑动窗口数据
    int windowSize;         // 窗口大小N
    int currentIndex;       // 当前写入位置
    double sum;             // 当前窗口总和
    bool isWindowFull;      // 窗口是否已满
} TS_LatestMoveMeanFilter;

// 清空滑动平均窗口数据
void ClearLatestMoveMeanFilter(TS_LatestMoveMeanFilter* mf, int n) {
    mf->windowSize = n;
    mf->window.resize(n, 0.0);  // 预分配固定大小的窗口
    mf->currentIndex = 0;
    mf->sum = 0.0;
    mf->isWindowFull = false;
}

// 输入一个值到滑动窗口，返回当前滑动平均值
double GetDataFromMoveFilter(TS_LatestMoveMeanFilter* mf, double newData) {
    // 移除最早的数据对总和的贡献（如果窗口已满）
    if (mf->isWindowFull) {
        mf->sum -= mf->window[mf->currentIndex];
    }
    
    // 添加新数据到窗口并更新总和
    mf->window[mf->currentIndex] = newData;
    mf->sum += newData;
    
    // 更新索引，循环使用数组
    mf->currentIndex = (mf->currentIndex + 1) % mf->windowSize;
    
    // 检查窗口是否已满
    if (!mf->isWindowFull && mf->currentIndex == 0) {
        mf->isWindowFull = true;
    }
    
    // 计算平均值（数据不足N时，按实际数量计算）
    size_t count = mf->isWindowFull ? mf->windowSize : mf->currentIndex;
    return count > 0 ? mf->sum / count : 0.0;
}

int main() {
    // 1. 初始化滤波器（窗口大小设为4）
    TS_LatestMoveMeanFilter filter;
    ClearLatestMoveMeanFilter(&filter, 4);

    // 2. 打开文件：读原始数据(data1.csv)、写滤波结果(data1_filter.csv)
    ifstream inFile("data1.csv");
    ofstream outFile("data1_filter.csv");
    //设置输出精度
    outFile << fixed << setprecision(4);
    double data;
    
    // 3. 逐行处理数据
    while (inFile >> data) {
        double filtered = GetDataFromMoveFilter(&filter, data);
        outFile << data << "," << filtered << endl;
    }

    // 4. 关闭文件
    inFile.close();
    outFile.close();

    cout << "滤波完成！结果已保存至 data1_filter.csv" << endl;
    return 0;
}    