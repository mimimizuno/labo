#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>


using namespace std;

class SEO {
private:
    // クラス共通のidカウンタ
    static int idCounter;
    // id
    int id;
    // 抵抗
    const double R;
    // トンネル抵抗
    const double Rj;
    // トンネル容量
    const double Cj;
    // 接合容量
    const double C;
    // 足の数
    const int legs;
    // バイアス電圧
    double Vd;
    // ノード電圧
    double Vn;
    // 足の数に対応する周囲のノード電圧
    vector<double> V;
    // 足の数に対応した接続キャパシタの電荷
    vector<double> q;
    // ノードの電荷
    double Q;
    // 電荷の変化量
    double dQ;
    // エネルギー変化量(up, down)
    map<string, double> dE;
    // トンネル待時間(up, down)
    map<string, double> wt;
    // トンネルの有無("", up, down)
    string tunnel;

public:
    SEO(int legCount);
    void calculateParameters(double Cs, double Cjs);
    void displayInfo() const;
};

class SEOGrid {
private:
    int rows;
    int columns;
    std::vector<std::vector<std::shared_ptr<SEO>>> grid;

public:
    SEOGrid(int r, int c);
    void createSEO(int row, int col, int legCount, double initialV = 0.0);
    std::shared_ptr<SEO> at(int row, int col);
    void displayInfo() const;
};

#endif // FUNCTIONS_HPP
