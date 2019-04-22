#include "soduku.h"

using namespace std;

int main() {
    int hard = 1;
    cout << "请输入数独生成空格数(0 退出): ";
    cin >> hard;
    while (hard > 0) {
        soduku soduku;
        soduku.newGame(hard);
        cout << "请输入数独生成空格数(0 退出): ";
        cin >> hard;
    }
    return 0;
}
