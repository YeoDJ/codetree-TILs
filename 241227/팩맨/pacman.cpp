#include <cstring>
#include <iostream>
#include <queue>
#include <set>
#define fastio ios::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;
using pii = pair<int, int>;

int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dx[] = {0, -1, -1, -1, 0, 1, 1, 1};

int t, cnt;
pii pacman;
int monster[4][4][8] = {0};  // 좌표, 방향 별 몬스터 개수
int deadBody[4][4][3] = {0}; // 좌표 별 시체 개수(마지막 idx는 남은 턴 수)
vector<pii> path, path_tmp;  // 백트래킹 변수

bool inRange(int y, int x) { return 0 <= y && y < 4 && 0 <= x && x < 4; }

void input() {
    int m, r, c, d;
    cin >> m >> t;
    cin >> pacman.first >> pacman.second;
    pacman.first--, pacman.second--;
    for (int i = 0; i < m; i++) {
        cin >> r >> c >> d;
        r--, c--, d--;
        monster[r][c][d]++;
    }
}

void moveMonster(int y, int x, int dir, int nyam) {
    int ny = y, nx = x, nd = dir;
    for (int i = 0; i < 8; i++) {
        ny = y + dy[nd], nx = x + dx[nd];
        if (inRange(ny, nx) && !deadBody[ny][nx][0] && !deadBody[ny][nx][1] && make_pair(ny, nx) != pacman) {
            monster[y][x][dir] -= nyam;
            monster[ny][nx][nd] += nyam;
            break;
        }
        nd = (nd == 7) ? 0 : nd + 1;
    }
}

void movePacman(pii spos, int lvl) {
    if (lvl == 3) {
        int tmp = 0;      // 경로 상 몬스터 카운트
        set<pii> visited; // 한 번 들른 좌표는 다시 들르지 않는다.
        for (auto &&i : path_tmp)
            if (visited.find(i) == visited.end()) {
                for (int j = 0; j < 8; j++)
                    tmp += monster[i.first][i.second][j];
                visited.insert(i);
            }

        // 가장 많이 먹을 수 있는 경우 update
        if (path.empty() || tmp > cnt) {
            cnt = tmp;
            path = path_tmp;
        }
        return;
    }

    for (int i = 0; i <= 6; i += 2) {
        int ny = spos.first + dy[i];
        int nx = spos.second + dx[i];
        if (inRange(ny, nx)) {
            path_tmp.push_back({ny, nx});
            movePacman({ny, nx}, lvl + 1);
            path_tmp.pop_back();
        }
    }
}

int main() {
    fastio;
    freopen("input.txt", "r", stdin);
    input();

    for (int tc = 0; tc < t; tc++) {
        // 몬스터 복제(알 상태)
        int y, x, d;
        int egg[4][4][8] = {0};
        memcpy(egg, monster, sizeof(egg));

        // 몬스터 이동
        for (y = 0; y < 4; y++)
            for (x = 0; x < 4; x++)
                for (d = 0; d < 8; d++)
                    if (egg[y][x][d])
                        moveMonster(y, x, d, egg[y][x][d]);

        // 팩맨 이동
        cnt = 0;
        path.clear(), path_tmp.clear();
        movePacman(pacman, 0);
        pacman = path[2];

        // 몬스터 먹기
        for (auto &&i : path) {
            y = i.first, x = i.second;
            for (d = 0; d < 8; d++) {
                deadBody[y][x][2] += monster[y][x][d];
                monster[y][x][d] = 0;
            }
        }

        // 썩어가는 몬스터...
        for (y = 0; y < 4; y++)
            for (x = 0; x < 4; x++) {
                for (d = 0; d < 2; d++)
                    swap(deadBody[y][x][d], deadBody[y][x][d + 1]);
                deadBody[y][x][2] = 0;
            }

        // 몬스터 복제
        for (y = 0; y < 4; y++)
            for (x = 0; x < 4; x++)
                for (d = 0; d < 8; d++)
                    monster[y][x][d] += egg[y][x][d];
    }

    // 현재 몬스터 수
    int ans = 0;
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            for (int d = 0; d < 8; d++)
                ans += monster[y][x][d];
    cout << ans;
    return 0;
}
