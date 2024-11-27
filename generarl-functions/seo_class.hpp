#ifndef SEO_HPP
#define SEO_HPP

#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <stdexcept>


#define e 0.1602 // 電子の電荷

using namespace std;

class SEO {
private:
    static int idCounter;   // 静的IDカウンタ
    int id;                 // ノードのID
    double Q;               // ノード電荷
    double Vn;              // ノード電圧
    double Vd;              // バイアス電圧
    const double R;         // 抵抗
    const double Rj;        // トンネル抵抗
    const double Cj;        // 接合容量
    const double C;         // 接続容量
    const int legs;         // 足の数
    vector<double> V;       // 周囲のノード電圧
    vector<double> q;       // 足の数に対応した接続キャパシタの電荷
    map<string, double> dE; // エネルギー変化量(up, down)
    map<string, double> wt; // トンネル待時間(up, down)
    string tunnel;          // トンネルの有無("", up, down)
    vector<int> connection; // 接続されている素子のID

public:
    // コンストラクタ（パラメータの初期設定）
    SEO(double r, double rj, double cj, double c, double vd, int legscounts)
        : R(r), Rj(rj), Cj(cj), C(c), Vd(vd), Q(0.0), Vn(0.0), legs(legscounts), V(legscounts, 0.0), q(legscounts, 0.0),connection(legscounts, 0){
        id = (idCounter == 0) ? 0 : idCounter++;
        dE["up"] = 0.0;
        dE["down"] = 0.0;
        wt["up"] = 0.0;
        wt["down"] = 0.0;
        tunnel = "";
    }

    //-----------セッター------------//
    // バイアス電圧を設定
    void setVias(const double vd) {
        Vd = vd;
    }

    // 接続情報を設定
    void setConnections(const vector<int>& connections) {
        if (connections.size() != legs) {
            throw invalid_argument("The size of connections must match the number of legs.");
        }
        connection = connections;
    }

    // 周囲の電圧を設定
    void setSurroundingVoltages(const vector<SEO>& SEOs) {
        if (connection.size() != legs) {
            throw invalid_argument("The number of legs does not match the connection size.");
        }

        for (size_t i = 0; i < connection.size(); i++) {
            int connectedID = connection[i];  // 接続されている素子のID
            if (connectedID >= SEOs.size() || connectedID < 0) {
                throw out_of_range("Invalid connection ID.");
            }
            V[i] = SEOs[connectedID].getNodeVoltage();  // 接続先のVnを取得して設定
        }
    }

    //-----------ゲッター------------//
    // ノード電圧を取得
    double getNodeVoltage() const {
        return Vn;
    }

    // 電荷の更新
    void updateCharge(double dt) {
        Q += dt * (Vd - Vn) / R;
    }

    // ノード電圧の計算
    void calculateNodeVoltage() {
        double totalQ = Q;
        for (const auto& v : V) {
            totalQ += C * v;
        }
        Vn = totalQ / Cj;
    }

    // エネルギー変化量の計算
    double calculateEnergyChange(bool isUp) const {
        return isUp ? e * (-e + 2 * Q) / (2 * (4 * C + Cj))
                    : -e * (e + 2 * Q) / (2 * (4 * C + Cj));
    }

    // トンネル処理
    void tunnel(double dEup, double dEdown, int& sflag) {
        if (dEup > 0 && (dEdown <= 0 || dEup < dEdown)) {
            Q -= e;
            sflag = 1;
        } else if (dEdown > 0) {
            Q += e;
            sflag = 2;
        } else {
            sflag = 0;
        }
    }

    double getVoltage() const { return Vn; }
};

#endif // SEO_HPP
