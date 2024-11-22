#include "functions.hpp"

// SEOクラスのコンストラクタ
SEO::SEO(int legCount)
    : Vd(0.0), Vn(0.0), Q(0.0), dQ(0.0), tunnel(0), V(legCount, 0.0) 
    {
        dE["up"] = 0.0;
        dE["down"] = 0.0;
        wt["up"] = 0.0;
        wt["down"] = 0.0;
        tunnel = "";
    }

// SEOクラスのパラメータ計算
void SEO::calculateParameters(double Cs, double Cjs) {
    Vd = Cs + Cjs;
    Vn = Cs - Cjs;
    for (size_t i = 0; i < V.size(); ++i) {
        V[i] = (Cs + i * Cjs) * 0.5; // 仮の計算例
    }
}

// SEOクラスの情報表示
void SEO::displayInfo() const {
    std::cout << "Vd: " << Vd << ", Vn: " << Vn << "\n";
    std::cout << "V: [";
    for (const auto& v : V) {
        std::cout << v << " ";
    }
    std::cout << "]\n";
}

// SEOGridクラスのコンストラクタ
SEOGrid::SEOGrid(int r, int c) : rows(r), columns(c) {
    grid.resize(rows, std::vector<std::shared_ptr<SEO>>(columns, nullptr));
}

// SEOGridクラスのSEO作成
void SEOGrid::createSEO(int row, int col, int legCount, double initialV) {
    if (row >= rows || col >= columns) {
        throw std::out_of_range("Invalid grid position");
    }
    grid[row][col] = std::make_shared<SEO>(legCount, initialV);
}

// SEOGridクラスのSEOアクセス
std::shared_ptr<SEO> SEOGrid::at(int row, int col) {
    if (row >= rows || col >= columns) {
        throw std::out_of_range("Invalid grid position");
    }
    return grid[row][col];
}

// SEOGridクラスの情報表示
void SEOGrid::displayInfo() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (grid[i][j]) {
                std::cout << "SEO[" << i << "][" << j << "]:\n";
                grid[i][j]->displayInfo();
                std::cout << "-----------------\n";
            } else {
                std::cout << "SEO[" << i << "][" << j << "] is empty.\n";
            }
        }
    }
}
