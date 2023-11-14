#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

int n;
vector<int> arr, L, R;

void input() {
    cin >> n;
    arr = L = R = vector<int>(n, 0);
    for (auto &&i : arr)
        cin >> i;

    // 배열을 왼쪽(오른쪽)부터 계속 탐색하며 얻은 최대값
    L[0] = arr[0], R[n - 1] = arr[n - 1];
    for (int i = 1; i < n; i++) {
        L[i] = max(L[i - 1], arr[i]);
        R[n - 1 - i] = max(R[n - i], arr[n - 1 - i]);
    }
}

int main() {
    input();
    int ans = 0;
    for (int i = 2; i < n - 2; i++)
        ans = max(ans, L[i - 2] + arr[i] + R[i + 2]);
    cout << ans;
    return 0;
}