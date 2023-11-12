#include <iostream>
#include <set>
#include <unordered_map>
using namespace std;

int n, q;
set<int> arr;
unordered_map<int, int> MAP;

void input() {
    cin >> n >> q;
    for (int i = 0; i < n; i++) {
        int num;
        cin >> num;
        arr.insert(num);
    }
    int idx = 1;
    for (auto &&i : arr)
        MAP[i] = idx++;
}

int solution(int a, int b) {
    auto sit = arr.lower_bound(a);
    if (sit == arr.end())
        return 0;
    auto eit = --arr.upper_bound(b);
    return (eit == arr.end() || b < *eit && *eit < *sit) ? 0 : MAP[*eit] - MAP[*sit] + 1;
}

int main() {
    input();
    for (int i = 0; i < q; i++) {
        int a, b;
        cin >> a >> b;
        cout << solution(a, b) << endl;
    }
    return 0;
}