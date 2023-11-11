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
        int a, b, ans = 0;
        cin >> a >> b;
        // a 이상으로 시작하는 숫자 구하기
        auto it = arr.lower_bound(a);
        for (set<int>::iterator iter = it; iter != arr.end(); iter++) {
            if (*iter > b)
                break;
            ans++;
        }
        cout << ans << endl;
    }

    return 0;
}