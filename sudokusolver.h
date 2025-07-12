
#pragma once
#include <bitset>
#include <string>

class SudokuSolver {
public:
    static const int N = 9;
    using cell_t = std::bitset<N>;
    
    SudokuSolver();
    bool solve(const std::string& input, char output[N][N]);

private:
    bool dfs(int cnt);
    
    int root;
    char str[N][N];
    cell_t hor[N], ver[N], square[N];
};
