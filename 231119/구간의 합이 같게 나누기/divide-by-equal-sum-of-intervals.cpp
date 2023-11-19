#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
using namespace std;

int n, sum = 0;
vector<int> arr, L, R;
set<int> rslt_L, rslt_R;

void input() {
    cin >> n;
    arr = L = R = vector<int>(n, 0);
    for (auto &&i : arr) {
        cin >> i;
        sum += i;
    }
}

void LR_init() {
    // rslt_L: (총 합 / 4)가 나온 횟수, L: 총 합
    sum /= 4;
    int sumL = 0, sumR = 0;
    for (int i = 0; i < n; i++) {
        sumL += arr[i], sumR += arr[n - 1 - i];
        if (sumL == sum && i < n - 3)
            rslt_L.insert(i);
        if (sumR == sum && n - 1 - i >= 3)
            rslt_R.insert(n - 1 - i);
        L[i] = sumL, R[n - 1 - i] = sumR;
    }
}

int main() {
    input();
    if (sum % 4) {
        cout << 0;
        return 0;
    }
    LR_init();

    // 왼쪽 시작점 -> 오른쪽 시작점 선정
    // ans += 왼쪽 두번째 합 == 오른쪽 두번째 합 == sum * 2
    int ans = 0;
    for (auto &&i : rslt_L)
        for (auto &&j : rslt_R)
            for (int k = i + 1; k < j - 1; k++)
                ans += L[k] == R[n - k] && L[k] == sum * 2;
    cout << ans;
    return 0;
}