#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#define fastio ios::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;
using pii = pair<int, int>;

struct monsterInfo {
    int row, col, dir;
};

int m, t, idx = 0;
int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dx[] = {0, -1, -1, -1, 0, 1, 1, 1};

pii pacman;                    // 팩맨 현재 좌표
map<int, monsterInfo> monster; // idx별 몹 정보
set<int> MAP[4][4];            // 좌표 별 몹 정보(idx)
bool deadBody[4][4][3] = {0};  // 좌표 별 시체 존재 여부
vector<pii> path_tmp;          // 백트래킹을 위한 변수

bool inRange(int y, int x) { return 0 <= y && y < 4 && 0 <= x && x < 4; }

void input() {
    cin >> m >> t;
    cin >> pacman.first >> pacman.second;
    pacman.first--, pacman.second--;
    for (int i = 0; i < m; i++) {
        int r, c, d;
        cin >> r >> c >> d;
        monsterInfo tmp = {--r, --c, --d};
        monster[idx] = tmp;
        MAP[r][c].insert(idx++);
    }
}

// pos번째 몬스터 이동
void moveMonster(int pos, monsterInfo &g) {
    for (int i = 0; i < 8; i++) {
        int ny = g.row + dy[g.dir];
        int nx = g.col + dx[g.dir];
        bool flag = false;
        for (int j = 0; j < 3; j++)
            if (flag = deadBody[ny][nx][j])
                break;

        if (inRange(ny, nx) && !flag && make_pair(ny, nx) != pacman) {
            MAP[g.row][g.col].erase(pos);
            g.row = ny, g.col = nx;
            MAP[ny][nx].insert(pos);
            break;
        }
        g.dir = (g.dir == 7) ? 0 : g.dir + 1;
    }
}

// 팩맨 이동
void movePacman(pii spos, int lvl, vector<pii> &path, int &cnt) {
    if (lvl == 3) {
        int tmp = 0;
        vector<pii> visited;
        for (auto &&i : path_tmp)
            // 왔던 좌표에 다시 돌아왔다면 카운트하지 않는다.
            if (find(visited.begin(), visited.end(), i) == visited.end()) {
                tmp += MAP[i.first][i.second].size();
                visited.push_back(i);
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
        map<int, monsterInfo> egg = monster;

        // 몬스터 이동
        for (auto &&j : monster)
            moveMonster(j.first, j.second);

        // 팩맨 이동
        int cnt = 0;
        vector<pii> path;
        path_tmp.clear();
        movePacman(pacman, 0, path, cnt);
        pacman = path[2];

        // 시체 치우기
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 4; x++) {
                deadBody[y][x][2] = deadBody[y][x][1];
                deadBody[y][x][1] = deadBody[y][x][0];
            }

        // 팩맨 지나간 자리에 몬스터가 있는지 찾기
        vector<int> target;
        for (auto &&j : path) {
            deadBody[j.first][j.second][0] = !MAP[j.first][j.second].empty();
            for (auto &&k : MAP[j.first][j.second])
                target.push_back(k);
        }

        // 몬스터 먹기
        for (auto &&j : target) {
            monsterInfo tmp = monster[j];
            MAP[tmp.row][tmp.col].erase(j);
            monster.erase(j);
        }

        // 몬스터 복제(알 -> MAP으로 옮기기)
        for (auto &&j : egg) {
            monster[++idx] = j.second;
            MAP[j.second.row][j.second.col].insert(idx);
        }
    }

    cout << monster.size();
    return 0;
}