#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
using namespace std;

int n, q;
unordered_map<int, int> MAP;

void input() {
    cin >> n >> q;
    set<int> arr;
    for (int i = 0; i < n; i++) {
        int num;
        cin >> num;
        arr.insert(num);
    }
    int idx = 1;
    for (auto &&i : arr)
        MAP[idx++] = i;
}

int main() {
    input();
    for (int i = 0; i < q; i++) {
        int a, b, ans = 0;
        cin >> a >> b;
        for (int j = 1; j <= n; j++)
            if (a <= MAP[j] && MAP[j] <= b)
                ans++;
        cout << ans << endl;
    }

    return 0;
}