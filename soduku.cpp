#include "soduku.h"
#include "puzzle.h"

const list<int> soduku::one2nine() {
    return list<int>{1, 2, 3, 4, 5, 6, 7, 8, 9};
}

void soduku::show() {
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            if (layout[i / 3] == j / 3) cout << "+ ";
            else cout << cells[i][j] << " ";
        }
        cout << endl;
    }
}

soduku::soduku(int cells[12][12], int layout[4]) {
    memcpy(this->layout, layout, 4 * sizeof(int));
    memcpy(this->cells, cells, 144 * sizeof(int));
}

void soduku::newGame(int hard) {
    clean();
    randLayout();
    checkSetGrid(0, 143);
    cout << "------- 终盘 ------" << endl;
    show();
    vector<puzzle> puzzles;
    puzzles.emplace_back(*this, hard);
    if (puzzles[0].build()){
        puzzles.emplace_back(puzzles[0]);
        puzzles.emplace_back(puzzles[0]);
        cout << "------- 题目 ------" << endl;
        puzzles[0].show();
        for (int i = 0; i < 3; ++i) {
            cout << "------ 方法 " << i << " 解-----" << endl;
            auto start = std::chrono::high_resolution_clock::now();
            puzzles[i].solve(i);
            auto stop = std::chrono::high_resolution_clock::now();
            puzzles[i].show();
            cout <<"开始时间: "<< start.time_since_epoch().count() << endl;
            cout << "结束时间: "<<stop.time_since_epoch().count() << endl;
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            cout << "----- 方法 " << i << " 历时: " << duration.count() << " 纳秒 -----" << endl;
        }
    }
}

bool soduku::checkSetGrid(int index, int last) {
    int row = index / 12, col = index % 12;
    if (layout[row / 3] == col / 3) return index == last || checkSetGrid(index + 1, last);
    list<int> possibles = randPossibles(row, col);
    for (int num : possibles) {
        cells[row][col] = num;
        if (index == last) return true;
        if (checkSetGrid(index + 1, last)) return true;
        cells[row][col] = 0;
    }
    return false;
}

list<int> soduku::checkPossibles(int row, int col) {
    list<int> list(one2nine());
    for (int i = 0; i < 12; i++) {
        list.remove(cells[i][col]);
        list.remove(cells[row][i]);
    }
    int r2 = row / 3 * 3;
    int c2 = col / 3 * 3;
    for (int i = 0; i < 3; i++)for (int j = 0; j < 3; j++) list.remove(cells[r2 + i][c2 + j]);
    return list;
}

soduku::soduku() {
    clean();
}

void soduku::clean() {
    memset(layout, 0, 4 * sizeof(int));
    memset(cells, 0, 144 * sizeof(int));
}

void soduku::randLayout() {
    vector<int> list = randQueue(0, 3);
    memcpy(layout, &list[0], 4 * sizeof(int));
}

list<int> soduku::randPossibles(int row, int col) {
    list<int> list = randList(1, 9);
    for (int i = 0; i < 12; i++) {
        list.remove(cells[i][col]);
        list.remove(cells[row][i]);
    }
    int r2 = row / 3 * 3;
    int c2 = col / 3 * 3;
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) list.remove(cells[r2 + i][c2 + j]);
    return list;
}

soduku::soduku(const soduku &soduku) {
    memcpy(layout, soduku.layout, 4 * sizeof(int));
    memcpy(cells, soduku.cells, 144 * sizeof(int));
}

void soduku::solve(int method) {
    list<int> selects;
    for (int grid = 0; grid < 144; grid++) {
        int row = grid / 12, col = grid % 12;
        if (layout[row / 3] == col / 3) continue;
        if (cells[row][col] == 0) selects.push_back(grid);
    }
    list<int> selected(selects);
    if (method == 1) solvePuzzle1(selected);
    else if (method == 2)solvePuzzle2(selected);
    else solvePuzzle0(selected);
}

bool soduku::solvePuzzle0(list<int> &selects) {
    if (selects.empty()) return true;
    int grid = selects.back();
    int row = grid / 12, col = grid % 12;
    list<int> possibles = checkThePossibles(cells, row, col);
    selects.remove(grid);
    for (int num : possibles) {
        cells[row][col] = num;
        if (solvePuzzle0(selects)) return true;
        cells[row][col] = 0;
    }
    selects.push_back(grid);
    return false;
}

bool soduku::solvePuzzle1(list<int> &selects) {
    if (selects.empty()) return true;
    int min = 9;
    int best = -1;
    list<int> possibles;
    for (int grid : selects) {
        int row = grid / 12, col = grid % 12;
        list<int> pss = checkThePossibles(cells, row, col);
        if (pss.size() < min) {
            min = pss.size();
            possibles = pss;
            best = grid;
        }
        if (min == 1) break;
    }
    if (min > 0 && min < 9 && best >= 0) {
        int row = best / 12, col = best % 12;
        selects.remove(best);
        for (int num : possibles) {
            cells[row][col] = num;
            if (solvePuzzle1(selects)) return true;
            cells[row][col] = 0;
        }
        selects.push_back(best);
    } else {
        cout << "无效局!!!" << endl;
    }
    return false;
}

bool soduku::solvePuzzle2(list<int> &selects) {
    for (int i = 0; i < 9; ++i) {
        int best = cycleBestLock();
        if (best != 0)scanLocked(best);
    }
    for (auto it = selects.begin(); it != selects.end();) {
        if (cells[*it / 12][*it % 12] != 0) {
            it = selects.erase(it);
        } else it++;
    }
    return solvePuzzle1(selects);
}

int soduku::cycleBestLock() {
    int best = 0;
    int max = 0;
    for (int i = 1; i <= 9; i++) {
        int count = checkNum(i);
        if (count > max && count < 12) {
            max = count;
            best = i;
        }
        if (max == 8)break;
    }
    return best;
}

bool soduku::scanLocked(int best) {
    list<int> list = sequenceList(0, 143);

    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            if (layout[i / 3] == j / 3) {
                list.remove(12 * i + j);
                continue;
            }
            if (cells[i][j] == best) {
                for (int k = 0; k < 12; ++k) {
                    list.remove(12 * i + k);
                    list.remove(12 * k + j);
                }
                int r2 = i / 3 * 3;
                int c2 = j / 3 * 3;
                for (int l = 0; l < 3; l++) for (int m = 0; m < 3; m++) list.remove(12 * (r2 + l) + c2 + m);
            } else if (cells[i][j] != 0) {
                list.remove(12 * i + j);
            }
        }
    }

    int amount = 0;
    // 行唯一 || 列唯一 || 宫唯一
    for (int grid:list) {
        if (testAlone(grid, list)) {
            cells[grid / 12][grid % 12] = best;
            amount++;
        }
    }

    return amount > 0;
}

int soduku::checkNum(int target) {
    int amount = 0;
    for (auto &cell:cells) for (int num:cell) if (num == target)amount++;
    return amount;
}

const bool soduku::testAlone(int grid, const list<int> &list) {
    int theRow = grid / 12, theCol = grid % 12;
    int rowHead = theRow / 3 * 3;
    int colHead = theCol / 3 * 3;
    bool sameRow = false, sameCol = false, sameMatrix = false;
    for (int num:list) {
        if (grid == num)continue;
        int row = num / 12, col = num % 12;
        if (theRow == row)sameRow = true;
        if (theCol == col)sameCol = true;
        if (row >= rowHead && row < rowHead + 3 && col >= colHead && col < colHead + 3)sameMatrix = true;
    }
    return !sameRow || !sameCol || !sameMatrix;
}
