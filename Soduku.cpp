#include "Soduku.h"

const list<int> Soduku::one2nine() {
    return list<int>{1, 2, 3, 4, 5, 6, 7, 8, 9};
}

void Soduku::show() {
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            if (layout[i / 3] == j / 3)cout << "+ ";
            else cout << cells[i][j] << " ";
        }
        cout << endl;
    }
}

Soduku::Soduku(int cells[12][12], int layout[4]) {
    memcpy(this->layout, layout, 4 * sizeof(int));
    memcpy(this->cells, cells, 144 * sizeof(int));
}
