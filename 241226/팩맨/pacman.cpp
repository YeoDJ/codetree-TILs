#include <iostream>
#include <map>
#include <queue>
#define fastio ios::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;
using pii = pair<int, int>;

struct monsterInfo {
    int row, col, dir;
};

int m, t, idx = 0;
int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dx[] = {0, -1, -1, -1, 0, 1, 1, 1};

pii pacman;                   // 팩맨 현재 좌표
map<int, monsterInfo> ghost;  // idx별 몹 정보
map<int, int> MAP[4][4];      // 좌표 별 몹 정보(idx, 방향향)
map<int, int> deadBody[4][4]; // 좌표 별 시체 정보(idx, 시체가 있는 최대 턴)
vector<pii> path_tmp;         // 백트래킹을 위한 변수

bool inRange(int y, int x) { return 0 <= y && y < 4 && 0 <= x && x < 4; }

void input() {
    cin >> m >> t;
    cin >> pacman.first >> pacman.second;
    pacman.first--, pacman.second--;
    for (int i = 0; i < m; i++) {
        int r, c, d;
        cin >> r >> c >> d;
        monsterInfo tmp = {--r, --c, --d};
        ghost[idx] = tmp;
        MAP[r][c][idx++] = d;
    }
}

// pos번째 몬스터 이동
void moveMonster(int pos, monsterInfo &g) {
    for (int i = 0; i < 8; i++) {
        int ny = g.row + dy[g.dir];
        int nx = g.col + dx[g.dir];
        if (inRange(ny, nx) && deadBody[ny][nx].empty() && make_pair(ny, nx) != pacman) {
            MAP[g.row][g.col].erase(pos);
            g.row = ny, g.col = nx;
            MAP[ny][nx][pos] = g.dir;
            break;
        }
        g.dir = (g.dir == 7) ? 0 : g.dir + 1;
    }
}

// 팩맨 이동
void movePacman(pii spos, int lvl, vector<pii> &path, int &cnt) {
    if (lvl == 3) {
        int tmp = 0, pos = 0;
        for (auto &&i : path_tmp) {
            // 왔던 좌표에 다시 돌아왔다면 카운트하지 않는다.
            if (pos == 2 && i == *path_tmp.begin())
                break;
            tmp += MAP[i.first][i.second].size();
            pos++;
        }

        // 가장 많이 먹을 수 있는 경우라면 path 수정
        if (path.empty() || tmp > cnt) {
            path = path_tmp;
            cnt = tmp;
        }
        return;
    }

    for (int i = 0; i < 7; i += 2) {
        int ny = spos.first + dy[i];
        int nx = spos.second + dx[i];
        if (inRange(ny, nx)) {
            path_tmp.push_back({ny, nx});
            movePacman({ny, nx}, lvl + 1, path, cnt);
            path_tmp.pop_back();
        }
    }
}

int main() {
    fastio;
    freopen("input.txt", "r", stdin);
    input();

    for (int i = 0; i < t; i++) {
        // 몬스터 복제(알 상태)
        map<int, monsterInfo> egg;
        for (auto &&j : ghost)
            egg[idx++] = j.second;

        // 몬스터 이동
        for (auto &&j : ghost)
            moveMonster(j.first, j.second);

        // 팩맨 이동
        int cnt = 0;
        vector<pii> path;
        path_tmp.clear();
        movePacman(pacman, 0, path, cnt);
        pacman = path[2];

        // 몬스터 먹기
        for (auto &&j : path)
            for (auto &&k : MAP[j.first][j.second]) {
                deadBody[j.first][j.second][k.first] = i + 2;
                ghost.erase(k.first);
                MAP[j.first][j.second].erase(k.first);
            }

        // 시체 치우기
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 4; x++)
                for (auto &&j : deadBody[y][x])
                    if (i > j.second)
                        deadBody[y][x].erase(j.first);

        // 몬스터 복제(알 -> MAP으로 옮기기)
        for (auto &&j : egg) {
            ghost[j.first] = j.second;
            MAP[j.second.row][j.second.col][j.first] = j.second.dir;
        }
    }

    cout << ghost.size();
    return 0;
}