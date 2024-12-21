#include <iostream>
#include <map>
#include <queue>
#define fastio ios::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;
using pii = pair<int, int>;

int n;
map<int, int> MAP;

bool bfs(int idx) {
    queue<int> q;
    vector<bool> used(n + 1, false);
    q.push(idx);
    used[idx] = true;

    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        if (MAP.find(cur) == MAP.end())
            break;
        if (used[MAP[cur]])
            return false;
        used[MAP[cur]] = true;
        q.push(MAP[cur]);
    }
    return true;
}

int main() {
    fastio;
    cin >> n;

    for (int i = 1; i <= n; i++) {
        int num;
        cin >> num;
        if (num)
            MAP[i] = num;
    }

    int ans = 0;
    for (int i = 1; i <= n; i++) {
        ans += bfs(i);
    }

    cout << ans;
    return 0;
}