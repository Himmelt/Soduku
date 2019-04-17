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
    cout << "----- finish matrix -----" << endl;
    show();
    cout << "-------------------------" << endl;
    puzzle puzzle(*this, hard);
    puzzle.build();
    cout << "------- the puzzle ------" << endl;
    puzzle.show();
    cout << "-------------------------" << endl;
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
