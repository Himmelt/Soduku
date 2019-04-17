//
// Created by LocalAdmin on 2019-04-17.
//

#ifndef SODUKU_SODUKU_H
#define SODUKU_SODUKU_H

#include <iostream>
#include <list>
#include <cstring>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include "Puzzle.h"

using namespace std;

class Soduku {
protected:

    const static list<int> one2nine();

    const static vector<int> randQueue(int min, int max) {
        vector<int> temp;
        for (int i = min; i <= max; i++) temp.push_back(i);
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle(temp.begin(), temp.end(), std::mt19937(seed));
        return temp;
    }

    const static list<int> randList(int min, int max) {
        vector<int> temp = randQueue(1, 9);
        list<int> list;
        list.assign(temp.begin(), temp.end());
        return list;
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

    list<int> checkPossibles(int row, int col) {
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

    const static list<int> checkThePossibles(int theCells[12][12], int row, int col) {
        list<int> list(one2nine());
        for (int i = 0; i < 12; i++) {
            list.remove(theCells[i][col]);
            list.remove(theCells[row][i]);
        }
        int r2 = row / 3 * 3;
        int c2 = col / 3 * 3;
        for (int i = 0; i < 3; i++)for (int j = 0; j < 3; j++) list.remove(theCells[r2 + i][c2 + j]);
        return list;
    }

public:
    Soduku() {
        clean();
    }

    Soduku(int cells[12][12], int layout[4]);

    void newGame() {
        clean();
        randLayout();
        checkSetGrid(0, 143);
        show();
        Puzzle puzzle(this, 20);
        puzzle.build();
        puzzle.show();
    }

    void show();

    void clean() {
        memset(layout, 0, 4 * sizeof(int));
        memset(cells, 0, 144 * sizeof(int));
    }

    void randLayout() {
        vector<int> list = randQueue(0, 3);
        memcpy(layout, &list[0], 4 * sizeof(int));
    }

    list<int> randPossibles(int row, int col) {
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

    int layout[4] = {0};
    int cells[12][12] = {0};
};


#endif //SODUKU_SODUKU_H
