#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

int n, q;
vector<int> arr, L, R;

void input() {
    cin >> n >> q;
    arr = L = R = vector<int>(n, 0);
    for (auto &&i : arr)
        cin >> i;

    L[0] = arr[0], R[n - 1] = arr[n - 1];
    for (int i = 1; i < n; i++) {
        L[i] = max(L[i - 1], arr[i]);
        R[n - 1 - i] = max(R[n - i], arr[n - 1 - i]);
    }
}

int main() {
    input();
    for (int t = 0; t < q; t++) {
        int a, b, ans = 0;
        cin >> a >> b;
        a--, b--;
        ans = max(ans, max(L[a - 1], R[b + 1]));
        cout << ans << endl;
    }
    return 0;
}