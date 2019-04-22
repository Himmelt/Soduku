#ifndef SODUKU_PUZZLE_H
#define SODUKU_PUZZLE_H

#include "soduku.h"

#define MAX_HARD 70

using namespace std;

class puzzle : public soduku {
private:
    int hard = 0;
    list<int> selects;

    bool randSelects();

    void selectNext(int repeat);

    bool randPuzzle();

    // -1  失败
    //  0  唯一解
    // >1  重复值
    int checkRepeat();

    int checkSolution(int theCells[12][12], list<int> &selected);

public:

    puzzle(const soduku &soduku, int hard);

    bool build();
};

#endif //SODUKU_PUZZLE_H
