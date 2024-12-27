#include <iostream>
#include <map>
#include <queue>
#include <set>
#define fastio ios::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;
using pii = pair<int, int>;
int n, m, h, k;

// 술래 정보
pii Me;
int me_dir = 0, me_cnt = 0, max_cnt = 1;
bool spiral = true; // true: 안 -> 밖

vector<vector<set<int>>> MAP; // 도망자 MAP 정보(좌표 별 idx)
map<int, pii> arr;            // 도망자 정보(idx, 좌표)
map<int, int> arr_d;          // 도망자 정보(idx, 방향)
set<pii> tree;                // 나무 정보

bool inRange(int y, int x) { return 0 <= y && y < n && 0 <= x && x < n; }

int dist(pii p1, pii p2) { return abs(p1.first - p2.first) + abs(p1.second - p2.second); }

void input() {
    int r, c, d;
    cin >> n >> m >> h >> k;
    Me = {n / 2, n / 2};
    MAP.resize(n);
    for (auto &&i : MAP)
        i.resize(n);

    // 도망자
    for (int i = 0; i < m; i++) {
        cin >> r >> c >> d;
        r--, c--;
        arr[i] = {r, c};
        arr_d[i] = (d == 1) ? 0 : 2;
        MAP[r][c].insert(i);
    }

    // 나무
    for (int i = 0; i < h; i++) {
        cin >> r >> c;
        tree.insert({--r, --c});
    }
}

void runPerson(int idx) {
    int dy[] = {0, 0, 1, -1};
    int dx[] = {1, -1, 0, 0};
    for (int i = 0; i < 2; i++) {
        int ny = arr[idx].first + dy[arr_d[idx]];
        int nx = arr[idx].second + dx[arr_d[idx]];
        if (inRange(ny, nx)) {
            if (make_pair(ny, nx) != Me) {
                MAP[arr[idx].first][arr[idx].second].erase(idx);
                arr[idx] = {ny, nx};
                MAP[ny][nx].insert(idx);
            }
            break;
        }
        arr_d[idx] += (arr_d[idx] % 2) ? -1 : 1;
    }
}

int runMe(int turn) {
    int dy[] = {-1, 0, 1, 0};
    int dx[] = {0, 1, 0, -1};
    me_cnt++;
    Me = {Me.first + dy[me_dir], Me.second + dx[me_dir]};

    // 꺾을 시점일 때
    if (me_cnt == max_cnt || (max_cnt == n && me_cnt == max_cnt - 1)) {
        me_cnt = 0;
        // (0, 0)이면 안쪽으로 돌리기
        if (Me == make_pair(0, 0)) {
            spiral = false;
            max_cnt = n;
            me_dir = 2;
        }
        // 정중앙이면 바깥쪽으로 돌리기
        else if (Me == make_pair(n / 2, n / 2)) {
            spiral = true;
            max_cnt = 1;
            me_dir = 0;
        }

        // 바깥쪽으로 돌아갈 때
        else if (spiral) {
            if (me_dir % 2)
                max_cnt++;
            me_dir = (me_dir == 3) ? 0 : me_dir + 1;
        }
        // 안쪽으로 돌아갈 때
        else if (!spiral) {
            if (!(me_dir % 2))
                max_cnt--;
            me_dir = (me_dir == 0) ? 3 : me_dir - 1;
        }
    }

    // 도망자 잡기
    int ny = Me.first, nx = Me.second, ans = 0;
    for (int i = 0; i < 3; i++) {
        if (inRange(ny, nx) && tree.find({ny, nx}) == tree.end()) {
            ans += turn * MAP[ny][nx].size();
            for (auto &&j : MAP[ny][nx]) {
                arr.erase(j);
                arr_d.erase(j);
            }
            MAP[ny][nx].clear();
        }
        ny += dy[me_dir], nx += dx[me_dir];
    }
    return ans;
}

int main() {
    fastio;
    freopen("input.txt", "r", stdin);
    int ans = 0;
    input();

    for (int tc = 0; tc < k; tc++) {
        // 도망자가 아무도 없다면 break
        if (arr.empty())
            break;

        // 도망가기
        for (auto &&i : arr)
            if (dist(Me, i.second) <= 3)
                runPerson(i.first);

        // 술래 움직인 후 도망자 잡기
        ans += runMe(tc + 1);
    }

    cout << ans;
    return 0;
}