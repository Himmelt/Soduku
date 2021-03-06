#include "puzzle.h"

puzzle::puzzle(const soduku &soduku, int hard) {
    memcpy(layout, soduku.layout, 4 * sizeof(int));
    memcpy(cells, soduku.cells, 144 * sizeof(int));
    this->hard = hard;
    selects.clear();
}

bool puzzle::build() {
    if (randPuzzle()) {
        for (int num:selects) cells[num / 12][num % 12] = 0;
        return true;
    } else {
        cout << "无效的题目!" << endl;
        return false;
    }
}

bool puzzle::randSelects() {
    if (hard > MAX_HARD || hard > 143) {
        cout << "空格数量太多，超过 " << MAX_HARD << endl;
        return false;
    }
    selects.clear();
    list<int> list = randList(0, 143);
    for (int num:list) {
        int row = num / 12, col = num % 12;
        if (layout[row / 3] == col / 3) continue;
        selects.push_back(num);
        if (selects.size() >= hard)return true;
    }
    return false;
}

void puzzle::selectNext(int repeat) {
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

bool puzzle::randPuzzle() {
    if (randSelects()) {
        int repeat;
        int count = 0;
        while ((repeat = checkRepeat()) != 0 && count < 100) {
            if (repeat < 0) {
                cout << "ERROR !! NO solution !" << endl;
                return false;
            }
            cout << "repeat:" << repeat - 1 << endl;
            selectNext(repeat - 1);
            count++;
        }
        return repeat == 0;
    }
    return false;
}

int puzzle::checkRepeat() {
    // 置零布局
    int theCells[12][12];
    memcpy(theCells, cells, 144 * sizeof(int));
    for (int num : selects) theCells[num / 12][num % 12] = 0;
    // 回溯
    list<int> selected(selects);
    return checkSolution(theCells, selected);
}

int puzzle::checkSolution(int theCells[12][12], list<int> &selected) {
    if (selected.empty())return 0;
    int grid = selected.back();
    int row = grid / 12, col = grid % 12;
    list<int> possibles = checkThePossibles(theCells, row, col);

    if (!possibles.empty()) {
        selected.remove(grid);
        int count = 0;
        for (int num : possibles) {
            theCells[row][col] = num;
            int code = checkSolution(theCells, selected);
            if (code == 0)count++;
            else if (code >= 1 && code <= 144) return code;
            if (count >= 2) return grid + 1;
            theCells[row][col] = 0;
        }
        selected.push_back(grid);
        if (count == 1)return 0;
    }
    return -1;
}
