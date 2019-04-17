#include <random>
#include <cstring>
#include <iostream>
#include <vector>
#include <random>
#include <list>
#include <algorithm>
#include <ctime>
#include <chrono>
#include "Soduku.h"
#include "Puzzle.h"

using namespace std;

void clean();

void randLayout();

void showLayout();

vector<int> randQueue(int min, int max);

bool checkSetGrid(int index, int last);

list<int> randPossibles(int row, int col);

void geneSoduku();

void showSoduku(int cells[12][12]);

list<int> checkCellsPossibles(int row, int col, int theCells[12][12]);

bool checkSolution(int theCells[12][12], list<int> &selects, int &record);

int walkAllSolutions(int originCells[12][12], vector<int> selected);

bool checkPuzzle(int index, int last, vector<int> &selected, int theCells[12][12]);

void generatePuzzle(int amount);

bool solvePuzzle(int theCells[12][12], list<int> &selects);

void generateSolution();

int layout[4];
int cells[12][12];
static list<int> ONE2NINE;

int main() {
    ONE2NINE = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << "Hello, World!" << std::endl;
    clean();
    /*randLayout();
    showLayout();
    geneSoduku();
    showSoduku(cells);
    generatePuzzle(30);*/
    Soduku soduku;
    soduku.newGame();

    return 0;
}

void showLayout() {
    cout << "Layout: " << layout[0] << " " << layout[1] << " " << layout[2] << " " << layout[3] << endl;
}

void clean() {
    memset(layout, 0, 4 * sizeof(int));
    memset(cells, 0, 144 * sizeof(int));
}

void randLayout() {
    vector<int> list = randQueue(0, 3);
    memcpy(layout, &list[0], 4 * sizeof(int));
}

vector<int> randQueue(int min, int max) {
    vector<int> temp;
    for (int i = min; i <= max; i++) temp.push_back(i);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(temp.begin(), temp.end(), std::mt19937(seed));
    return temp;
}

bool checkSetGrid(int index, int last) {
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

list<int> randPossibles(int row, int col) {
    vector<int> temp = randQueue(1, 9);
    list<int> list;
    list.assign(temp.begin(), temp.end());
    for (int i = 0; i < 12; i++) {
        list.remove(cells[i][col]);
        list.remove(cells[row][i]);
    }
    int r2 = row / 3 * 3;
    int c2 = col / 3 * 3;
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) list.remove(cells[r2 + i][c2 + j]);
    return list;
}

void geneSoduku() {
    checkSetGrid(0, 143);
}

void showSoduku(int theCells[12][12]) {
    cout << "----------------------------------------" << endl;
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            if (layout[i / 3] == j / 3)cout << "+ ";
            else cout << theCells[i][j] << " ";
        }
        cout << endl;
    }
    cout << "----------------------------------------" << endl;
}

void generatePuzzle(int amount) {
    int copyCells[12][12];
    memcpy(copyCells, cells, 144 * sizeof(int));
    vector<int> selected;
    bool result = checkPuzzle(1, amount, selected, copyCells);
}

bool checkPuzzle(int index, int last, vector<int> &selected, int theCells[12][12]) {
    vector<int> temp = randQueue(0, 143);
    list<int> list;
    list.assign(temp.begin(), temp.end());
    for (int num : selected) list.remove(num);
    for (int grid : list) {
        int row = grid / 12, col = grid % 12;
        if (layout[row / 3] == col / 3) continue;
        selected.push_back(grid);
        if (index >= last) {
            int solutions = walkAllSolutions(theCells, selected);
            if (solutions == 1) {
                selected.pop_back();
                return true;
            } else if (solutions >= 2) {
                selected.pop_back();
                return false;
            }
        } else if (checkPuzzle(index + 1, last, selected, theCells)) {
            selected.pop_back();
            return true;
        }
        selected.pop_back();
    }
    return false;
}

int walkAllSolutions(int originCells[12][12], vector<int> selected) {
    int copyCells[12][12];
    memcpy(copyCells, originCells, 144 * sizeof(int));
    list<int> selects;
    selects.assign(selected.begin(), selected.end());
    for (int grid : selects) {
        int row = grid / 12, col = grid % 12;
        copyCells[row][col] = 0;
    }
    showSoduku(copyCells);
    int record = 0;
    checkSolution(copyCells, selects, record);
    return record;
}

bool checkSolution(int theCells[12][12], list<int> &selects, int &record) {
    if (selects.empty()) {
        record++;
        return true;
    }
    int min = 10;
    int best = -1;
    list<int> possibles;
    for (int grid : selects) {
        int row = grid / 12, col = grid % 12;
        list<int> pss = checkCellsPossibles(row, col, theCells);
        if (pss.size() < min) {
            min = pss.size();
            possibles = pss;
            best = grid;
        }
        if (min == 1) break;
    }
    if (min >= 1 && min <= 9 && best >= 0) {
        int row = best / 12, col = best % 12;
        selects.remove(best);
        for (int num : possibles) {
            theCells[row][col] = num;
            checkSolution(theCells, selects, record);
            theCells[row][col] = 0;
        }
        selects.push_back(best);
    } else {
        cout << "Invalid grids" << endl;
    }
    return false;
}

list<int> checkCellsPossibles(int row, int col, int theCells[12][12]) {
    list<int> list(ONE2NINE);
    for (int i = 0; i < 12; i++) {
        list.remove(theCells[i][col]);
        list.remove(theCells[row][i]);
    }
    int r2 = row / 3 * 3;
    int c2 = col / 3 * 3;
    for (int i = 0; i < 3; i++)for (int j = 0; j < 3; j++) list.remove(theCells[r2 + i][c2 + j]);
    return list;
}

void generateSolution() {
    list<int> selects;
    for (int grid = 0; grid < 144; grid++) {
        int row = grid / 12, col = grid % 12;
        if (layout[row / 3] == col / 3) continue;
        if (cells[row][col] == 0) selects.push_back(grid);
    }
    solvePuzzle(cells, selects);
}

bool solvePuzzle(int theCells[12][12], list<int> &selects) {
    if (selects.empty()) return true;
    int min = 9;
    int best = -1;
    list<int> possibles;
    for (int grid : selects) {
        int row = grid / 12, col = grid % 12;
        list<int> pss = checkCellsPossibles(row, col, theCells);
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
            theCells[row][col] = num;
            if (solvePuzzle(theCells, selects)) return true;
            theCells[row][col] = 0;
        }
        selects.push_back(best);
    } else {
        cout << "Invalid grids" << endl;
    }
    return false;
}

