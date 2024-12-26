#include <algorithm>
#include <iostream>
#include <queue>
#define fastio ios::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)
using namespace std;

int n, m, centerPos, sum = 0;
int dy[] = {0, 1, 0, -1};
int dx[] = {1, 0, -1, 0};
vector<vector<int>> MAP;

bool inRange(int y, int x) { return 0 <= y && y < n && 0 <= x && x < n; }

void input() {
    cin >> n >> m;
    MAP = vector<vector<int>>(n, vector<int>(n, 0));
    centerPos = n / 2;
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++)
            cin >> MAP[y][x];
}

// 상하좌우 중 하나로 p만큼 공격
int attack(int d, int p) {
    int ans = 0;
    int ny = centerPos, nx = centerPos;
    for (int i = 0; i < p; i++) {
        ny += dy[d], nx += dx[d];
        if (!inRange(ny, nx) || MAP[ny][nx] == 0)
            break;
        ans += MAP[ny][nx];
        MAP[ny][nx] = 0;
    }
    return ans;
}

// 1차원 배열에다 빼내는 것과 다시 MAP에 넣는 것 모두 소용돌이 형태로 탐색한다.
// 탐색 방법: 가장 바깥쪽 (0, 0)에서 시계방향으로 중앙까지 들어오는 방향
vector<int> moveMAP() {
    vector<int> ans;
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    int ny = 0, nx = 0, nd = 0;

    // 1차원 배열에 0 빼고 다 저장한다.
    while (ny != centerPos || nx != centerPos) {
        visited[ny][nx] = true;
        if (MAP[ny][nx]) {
            ans.push_back(MAP[ny][nx]);
            MAP[ny][nx] = 0;
        }

        ny += dy[nd], nx += dx[nd];
        if (!inRange(ny, nx) || visited[ny][nx]) {
            ny -= dy[nd], nx -= dx[nd];
            nd = (nd == 3) ? 0 : nd + 1;
        }
    }
    return ans;
}

// 4개 이상 몬스터가 붙어 있다면 없애기
vector<int> removeMAP(vector<int> v) {
    int cnt = 1, sz = v.size();
    vector<int> ans;
    for (int i = 0; i < sz - 1; i++) {
        if (v[i + 1] != v[i]) {
            if (cnt < 4)
                for (int j = 0; j < cnt; j++)
                    ans.push_back(v[i]);
            else
                sum += cnt * v[i];
            cnt = 1;
        } else
            cnt++;
    }

    // 마지막 원소 처리
    if (sz > 1) {
        if (cnt < 4)
            for (int j = 0; j < cnt; j++)
                ans.push_back(v[sz - 1]);
        else
            sum += cnt * v[sz - 1];
    }
    return ans;
}

void inputMAP(vector<int> v) {
    int sz = v.size(), num;
    int ny = 0, nx = 0, nd = 0, cnt = 1;
    vector<int> tmp;
    vector<vector<bool>> visited(n, vector<bool>(n, false));

    // 마지막 짝짓기
    for (int i = sz - 1; i > 0; i--) {
        if (v[i - 1] != v[i]) {
            tmp.push_back(cnt);
            tmp.push_back(v[i]);
            cnt = 1;
        } else
            cnt++;
    }

    // 마지막 원소 처리
    if (sz > 1 && cnt == 1) {
        tmp.push_back(cnt);
        tmp.push_back(v[0]);
    }
    reverse(tmp.begin(), tmp.end());

    // 초기화
    sz = tmp.size();
    num = n * n - sz - 1;
    ny = nx = nd = cnt = 0;
    visited = vector<vector<bool>>(n, vector<bool>(n, false));

    // 다시 MAP에 집어넣기(방향은 moveMAP() 함수에서 한 것과 마찬가지로 진행)
    while (ny != centerPos || nx != centerPos) {
        visited[ny][nx] = true;
        if (cnt >= num)
            MAP[ny][nx] = tmp[cnt - num];

        ny += dy[nd], nx += dx[nd];
        if (!inRange(ny, nx) || visited[ny][nx]) {
            ny -= dy[nd], nx -= dx[nd];
            nd = (nd == 3) ? 0 : nd + 1;
        } else
            cnt++;
    }
}

int main() {
    fastio;
    input();

    for (int i = 0; i < m; i++) {
        int d, p;
        cin >> d >> p;
        sum += attack(d, p);

        // 중복이 없어질 때까지 반복
        vector<int> tmp1 = moveMAP();
        vector<int> tmp2;
        while (1) {
            // 모두 처리했다면 1차원 배열 재구성 후 다시 MAP에 삽입
            tmp2 = removeMAP(tmp1);
            if (tmp1 == tmp2) {
                inputMAP(tmp1);
                break;
            }
            tmp1 = tmp2;
        }
    }

    cout << sum;
    return 0;
}