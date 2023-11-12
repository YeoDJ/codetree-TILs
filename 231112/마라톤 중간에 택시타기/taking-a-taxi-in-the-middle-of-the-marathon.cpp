#include <algorithm>
#include <iostream>
#include <queue>
#define INIT vector<int>(n, 0)
using namespace std;

int n;
vector<int> x_arr, y_arr, x_sum[2], y_sum[2];

void input() {
    cin >> n;
    x_arr = y_arr = INIT;
    for (int i = 0; i < 2; i++)
        x_sum[i] = y_sum[i] = INIT;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        x_arr[i] = x, y_arr[i] = y;
    }
    // 0: Left, 1: Right
    for (int i = 1; i < n; i++) {
        x_sum[0][i] = x_sum[0][i - 1] + abs(x_arr[i] - x_arr[i - 1]);
        x_sum[1][n - 1 - i] = x_sum[1][n - i] + abs(x_arr[n - i] - x_arr[n - 1 - i]);
        y_sum[0][i] = y_sum[0][i - 1] + abs(y_arr[i] - y_arr[i - 1]);
        y_sum[1][n - 1 - i] = y_sum[1][n - i] + abs(y_arr[n - i] - y_arr[n - 1 - i]);
    }
}

int main() {
    input();
    int ans = INT32_MAX;

    // 빼먹을 지점
    for (int i = 1; i < n - 1; i++)
        ans = min(ans, x_sum[0][i - 1] + x_sum[1][i + 1] + abs(x_arr[i - 1] - x_arr[i + 1]) + y_sum[0][i - 1] + y_sum[1][i + 1] + abs(y_arr[i - 1] - y_arr[i + 1]));
    cout << ans;
    return 0;
}