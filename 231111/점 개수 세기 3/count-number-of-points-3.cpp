#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
using namespace std;

int n, q;
set<int> arr;
unordered_map<int, int> MAP;

void input() {
    cin >> n >> q;
    // treeset으로 주어진 Node 값 대입
    for (int i = 0; i < n; i++) {
        int num;
        cin >> num;
        arr.insert(num);
    }
    // 이 순서대로 hashmap에 Node 번호 부여(val, Node_num)
    int idx = 1;
    for (auto &&i : arr)
        MAP[i] = idx++;
}

int main() {
    input();
    for (int i = 0; i < q; i++) {
        int a, b;
        cin >> a >> b;
        // a 이상으로 시작하는 iter
        auto it_start = arr.lower_bound(a);
        if (*it_start != a)
            it_start++;
        // b 이하로 끝나는 iter
        auto it_end = arr.lower_bound(b);
        cout << MAP[*it_end] - MAP[*it_start] + 1 << endl;
    }

    return 0;
}