#ifndef SODUKU_PUZZLE_H
#define SODUKU_PUZZLE_H

#include <iostream>
#include <list>
#include <cstring>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include "Soduku.h"

#define MAX_HARD 50

using namespace std;

class Puzzle : Soduku {
private:
    int hard = 0;
    list<int> selects;

    void randSelects() {
        if (hard > MAX_HARD || hard > 143) {
            cout << "Too many empty grids !" << endl;
            return;
        }
        selects.clear();
        list<int> list = randList(0, 143);
        for (int num:list) {
            int row = num / 12, col = num % 12;
            if (layout[row / 3] == col / 3) continue;
            selects.push_back(num);
            if (selects.size() >= hard)return;
        }
    }

    void selectNext(int repeat) {
        list<int> list = randList(0, 143);
        for (int num:selects)list.remove(num);
        selects.remove(repeat);
        for (int num:list) {
            int row = num / 12, col = num % 12;
            if (layout[row / 3] == col / 3) continue;
            selects.push_back(num);
            return;
        }
    }

    bool randPuzzle() {
        randSelects();
        int repeat;
        int count = 0;
        while ((repeat = checkRepeat()) != 0 && count < 100) {
            if (repeat < 0) {
                cout << "NO solution !" << endl;
                return false;
            }
            selectNext(repeat);
            count++;
        }
        return count <= 100;
    }

    // -1  失败
    //  0  唯一解
    // >1  重复值
    int checkRepeat() {
        // 置零布局
        int theCells[12][12];
        memcpy(theCells, cells, 144 * sizeof(int));
        for (int num : selects) theCells[num / 12][num % 12] = 0;
        // 回溯
        list<int> selected(selects);
        return checkSolution(theCells, selected);
    }

    int checkSolution(int theCells[12][12], list<int> &selected) {
        if (selected.empty())return 0;
        int grid = selected.back();
        int row = grid / 12, col = grid % 12;
        list<int> possibles = checkThePossibles(theCells, row, col);

        if (!possibles.empty()) {
            selected.remove(grid);
            int count = 0;
            for (int num : possibles) {
                cells[row][col] = num;
                int code = checkSolution(theCells, selected);
                if (code == 0)count++;
                else if (code >= 1 && code <= 144) return code;
                if (count >= 2) return grid + 1;
                cells[row][col] = 0;
            }
            selected.push_back(grid);
        }

        return -1;
    }

public:
    Puzzle(Soduku &sod, int difficult) {
        memcpy(layout, sod.layout, 4 * sizeof(int));
        memcpy(cells, sod.cells, 144 * sizeof(int));
        this->hard = difficult;
        selects.clear();
    }

    bool build() {
        if (randPuzzle()) {
            for (int num:selects) cells[num / 12][num % 12] = 0;
            return true;
        } else {
            cout << "Try 100 times, but no valid Puzzle !" << endl;
            return false;
        }
    }
};

#endif //SODUKU_PUZZLE_H
