
#include "sudokusolver.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

SudokuSolver::SudokuSolver() : root(static_cast<int>(std::sqrt(N))) {}

bool SudokuSolver::dfs(int cnt) {
    if (cnt == 0) return true;

    int min_avail = N + 1, x = -1, y = -1;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (str[i][j] == '.') {
                int k = i / root * root + j / root;
                cell_t avail = ~(hor[i] | ver[j] | square[k]);
                int avail_count = avail.count();
                if (avail_count < min_avail) {
                    min_avail = avail_count;
                    x = i; y = j;
                    if (min_avail == 1) break;
                }
            }
        }
        if (min_avail == 1) break;
    }

    if (x == -1 || y == -1 || min_avail == 0)
        return false;

    int k = x / root * root + y / root;
    cell_t avail = ~(hor[x] | ver[y] | square[k]);
    for (int i = 0; i < N; ++i) {
        if (avail[i]) {
            str[x][y] = '1' + i;
            hor[x][i] = ver[y][i] = square[k][i] = true;
            if (dfs(cnt - 1)) return true;
            str[x][y] = '.';
            hor[x][i] = ver[y][i] = square[k][i] = false;
        }
    }
    return false;
}

bool SudokuSolver::solve(const std::string& input, char output[N][N]) {
    if (input.length() != N*N)
        throw std::invalid_argument("输入字符串长度必须为81");

    std::copy(input.begin(), input.end(), &str[0][0]);
    int cnt = 0;

    for (int i = 0; i < N; ++i) {
        hor[i].reset(); ver[i].reset(); square[i].reset();
        for (int j = 0; j < N; ++j) {
            char c = str[i][j];
            if (c != '.') {
                if (c < '1' || c > '9')
                    throw std::invalid_argument("输入包含非法字符");
                int val = c - '1';
                if (hor[i][val])
                    throw std::invalid_argument("行冲突检测");
                hor[i][val] = true;
            }
        }
        cnt += N - hor[i].count();
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            char c = str[j][i];
            if (c != '.') {
                int val = c - '1';
                if (ver[i][val])
                    throw std::invalid_argument("列冲突检测");
                ver[i][val] = true;
            }
        }
    }

    for (int i = 0; i < N; i += root) {
        for (int j = 0; j < N; j += root) {
            int k = (i / root) * root + (j / root);
            for (int x = i; x < i + root; ++x) {
                for (int y = j; y < j + root; ++y) {
                    char c = str[x][y];
                    if (c != '.') {
                        int val = c - '1';
                        if (square[k][val])
                            throw std::invalid_argument("宫格冲突检测");
                        square[k][val] = true;
                    }
                }
            }
        }
    }

    if (dfs(cnt)) {
        std::copy(&str[0][0], &str[0][0] + N*N, &output[0][0]);
        return true;
    }
    return false;
}
