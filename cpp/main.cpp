#include "soduku.h"

using namespace std;

int main() {
    int hard = 1;//难度
    cout << "请输入数独生成空格数(0 退出): ";
    cin >> hard;//输入
    // 循环
    while (hard > 0) {
        soduku soduku;//定义一个数独对象
        // 1. 生成终解（有效的数独）
        // 2. 挖空（生成一个题目）
        // 3. 验证题目有唯一解（已经进行求解，回溯）
        // 4. 求解
        soduku.newGame(hard);//新建游戏并求解
        cout << "请输入数独生成空格数(0 退出): ";
        cin >> hard;
    }
    return 0;//结束
}
