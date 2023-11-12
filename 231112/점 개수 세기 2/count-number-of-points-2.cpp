#include <iostream>
#include <map>
#include <set>
#define IT (*it).second
using namespace std;

typedef map<int, set<int>>::iterator iter;
int n, q;
// X 좌표에 따른 Y 좌표 값
map<int, set<int>> arr;
// X 좌표별 Y 좌표에 따른 Node 번호 부여
map<int, map<int, int>> MAP;

void input() {
    cin >> n >> q;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        arr[x].insert(y);
    }
    for (auto &&i : arr) {
        int idx = 1;
        for (auto &&j : i.second)
            MAP[i.first][j] = idx++;
    }
}

int main() {
    input();
    for (int i = 0; i < q; i++) {
        int x1, y1, x2, y2, cnt = 0;
        cin >> x1 >> y1 >> x2 >> y2;

        // x1 이상 x2 이하의 iter 구하기
        iter s_iter_x = arr.lower_bound(x1);
        iter e_iter_x = arr.lower_bound(x2);
        if ((*e_iter_x).first > x2)
            e_iter_x--;

        // y1 이상 y2 이하의 iter 구하기
        for (iter it = s_iter_x; it != arr.end(); it++) {
            auto s_iter_y = IT.lower_bound(y1);
            auto e_iter_y = IT.lower_bound(y2);
            if (*e_iter_y > y2)
                e_iter_y--;
            cnt += (s_iter_y != IT.end() && e_iter_y != IT.end()) ? MAP[(*it).first][*e_iter_y] - MAP[(*it).first][*s_iter_y] + 1 : 0;
            if (it == e_iter_x)
                break;
        }
        cout << cnt << endl;
    }

    return 0;
}