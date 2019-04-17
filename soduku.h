#ifndef SODUKU_SODUKU_H
#define SODUKU_SODUKU_H

#include <iostream>
#include <list>
#include <cstring>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;

class soduku {
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
        vector<int> temp = randQueue(min, max);
        list<int> list;
        list.assign(temp.begin(), temp.end());
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

    bool checkSetGrid(int index, int last);

    list<int> checkPossibles(int row, int col);

public:

    int layout[4] = {0};
    int cells[12][12] = {0};

    soduku();

    soduku(const soduku & soduku);

    soduku(int cells[12][12], int layout[4]);

    void newGame(int hard);

    void show();

    void clean();

    void randLayout();

    list<int> randPossibles(int row, int col);
};

#endif //SODUKU_SODUKU_H
