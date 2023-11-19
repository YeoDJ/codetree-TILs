#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#define j n - 1 - i
#define iter set<int>::iterator
using namespace std;

int n, sum = 0, ans = 0;
vector<int> arr;
set<int> idx_L, idx_2L, idx_R, idx_2R;

void input() {
    cin >> n;
    arr = vector<int>(n, 0);
    for (auto &&i : arr) {
        cin >> i;
        sum += i;
    }
}

void LR_init() {
    sum /= 4;
    int sumL = 0, sumR = 0;
    for (int i = 0; i < n; i++) {
        sumL += arr[i], sumR += arr[j];
        if (i < n - 3 && sumL == sum)
            idx_L.insert(i);
        if (j >= 3 && sumR == sum)
            idx_R.insert(j);
        if (0 < i && i < n - 2 && sumL == sum * 2)
            idx_2L.insert(i);
        if (n - 1 > j && j >= 2 && sumR == sum * 2)
            idx_2R.insert(j);
    }
}

int main() {
    input();
    if (sum % 4) {
        cout << 0;
        return 0;
    }

    // 탐색 순서: idx_2L, idx_L, idx_2R, idx_R
    LR_init();
    for (auto &&i : idx_2L) {
        iter it1 = idx_L.lower_bound(i);
        it1--;
        iter it2 = idx_2R.upper_bound(i);
        ans += (distance(idx_L.begin(), it1) + 1) * distance(idx_R.upper_bound(*it2), idx_R.end());
    }
    cout << ans;
    return 0;
}