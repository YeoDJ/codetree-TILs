#include <algorithm>
#include <iostream>
#include <queue>
#define fastio ios::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;
using pii = pair<int, int>;

// [MAP 정보] - 0: 빈칸, 1: 길, 2: 출구, 3: 골렘 위치
// [golem_idx 정보] - 골렘 들어올 때마다 변경경
class Golem {
  public:
    int row, col;
    vector<vector<int>> MAP;
    vector<vector<int>> golem_idx;

  private:
    int dir_arr[3][3] = {
        {0, 1, 2},
        {1, 2, 3},
        {2, 3, 0},
    };
    // 북동남서 & 중앙
    int dy[5] = {-1, 0, 1, 0, 0};
    int dx[5] = {0, 1, 0, -1, 0};

    // 십자가이므로 십자가에 맞게 범위 설정
    bool inRange(int y, int x) { return y < row && 0 <= x && x < col; }

    // 골렘 이동시킬 수 있는가?(빈 칸이 3칸이 나오는가?)
    bool isMove(int y, int x, int grav) {
        for (int i = 0; i < 3; i++) {
            int ny = y + dy[dir_arr[grav - 1][i]] + dy[grav];
            int nx = x + dx[dir_arr[grav - 1][i]] + dx[grav];
            bool flag = inRange(ny, nx);
            if (!inRange(ny, nx) || MAP[ny][nx])
                return false;
        }
        return true;
    }

    // 최종 골렘 이동시키기
    void moveGolem(int y, int x, int &dir, int grav, int idx, bool isSpin) {
        // 옮기기 전 다 0으로 초기화 후
        int ny, nx;
        for (int i = 0; i < 5; i++) {
            ny = y + dy[i];
            nx = x + dx[i];
            MAP[ny][nx] = golem_idx[ny][nx] = 0;
        }

        // 옮겨질 부분에 다시 MAP 채워넣기
        for (int i = 0; i < 5; i++) {
            ny = y + dy[i] + dy[grav];
            nx = x + dx[i] + dx[grav];
            MAP[ny][nx] = (i == dir) ? 2 : 1;
            golem_idx[ny][nx] = idx;
        }

        // 출구 회전하기(단, 아래로 떨어지는 경우는 제외외)
        if (isSpin) {
            int next_dir = 2;
            if (grav == 3)
                next_dir = (dir == 0) ? 3 : dir - 1;
            if (grav == 1)
                next_dir = (dir == 3) ? 0 : dir + 1;
            swap(MAP[ny + dy[dir]][nx + dx[dir]], MAP[ny + dy[next_dir]][nx + dx[next_dir]]);
            dir = next_dir;
        }
    }

  public:
    Golem(int r, int c) {
        // 십자가가 위로 삐져 나오는 부분까지 고려
        row = r + 3, col = c;
        MAP.resize(row);
        for (auto &&i : MAP)
            i.resize(col, 0);
        golem_idx = MAP;
    }

    // 골렘 떨어뜨리기(r, c는 골렘의 중심 좌표) -> 최종 좌표 반환
    pii gravity(int c, int dir, int idx) {
        // 처음 골렘 위치시키기
        int r = 1;
        for (int i = 0; i < 5; i++) {
            MAP[r + dy[i]][c + dx[i]] = (i == dir) ? 2 : 1;
            golem_idx[r + dy[i]][c + dx[i]] = idx;
        }

        // 아래(2) -> 좌측(3) -> 우측(1) 순서대로 판단
        // 그럼에도 떨어질 곳이 없다면 끝내기
        while (1) {
            if (isMove(r, c, 2)) {
                moveGolem(r++, c, dir, 2, idx, false);
                continue;
            }
            if (isMove(r, c, 3)) {
                moveGolem(r, c--, dir, 3, idx, false);
                if (!isMove(r, c, 2))
                    moveGolem(r, c++, dir, 1, idx, false);
                else {
                    moveGolem(r++, c, dir, 2, idx, true);
                    continue;
                }
            }
            if (isMove(r, c, 1)) {
                moveGolem(r, c++, dir, 1, idx, false);
                if (!isMove(r, c, 2))
                    moveGolem(r, c--, dir, 3, idx, false);
                else {
                    moveGolem(r++, c, dir, 2, idx, true);
                    continue;
                }
            }
            break;
        }
        return {r, c};
    }

    // 움직일 수 있는 경우의 수
    // 1. 서로 갈은 골렘 idx에서 1 또는 2로 가는 경우
    // 2. 서로 다른 골렘 idx에서 2->1로 가는 경우
    int findCol(pii pos) {
        int ans = 0;
        vector<vector<bool>> used(row + 3, vector<bool>(col, false));
        queue<pii> q;

        used[pos.first][pos.second] = true;
        q.push(pos);

        while (!q.empty()) {
            pii cur = q.front();
            q.pop();
            for (int i = 0; i < 4; i++) {
                int ny = cur.first + dy[i];
                int nx = cur.second + dx[i];
                if (inRange(ny, nx) && !used[ny][nx] && MAP[ny][nx] > 0 &&
                    (golem_idx[cur.first][cur.second] == golem_idx[ny][nx] || (golem_idx[cur.first][cur.second] != golem_idx[ny][nx] && MAP[cur.first][cur.second] == 2))) {
                    used[ny][nx] = true;
                    q.push({ny, nx});
                    ans = max(ans, ny - 2);
                }
            }
        }
        return ans;
    }
};

int main() {
    fastio;
    int r, c, k;
    int ans = 0;
    cin >> r >> c >> k;
    Golem golem(r, c);

    for (int tc = 1; tc <= k; tc++) {
        int col, dir;
        cin >> col >> dir;
        pii pos = golem.gravity(col - 1, dir, tc);
        if (pos.first >= 3)
            ans += golem.findCol(pos); // 최대 열
        else
            golem = Golem(r, c); // 초기화
    }

    cout << ans;
    return 0;
}