#include <iostream>
#include <queue>
#include <set>
#define fastio ios::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;
using pii = pair<int, int>;

struct ghostInfo {
    bool isEgg;
    int row, col, dir;
    int die_cnt; // 시체 소멸 몇번째 턴에 발생하는가(0이면 소멸 대상 아님)
};

int m, t;
int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dx[8] = {0, -1, -1, -1, 0, 1, 1, 1};

vector<vector<int>> deadBody(4, vector<int>(4, 0)); // 시체가 몇 개 있는가?
vector<ghostInfo> ghost;                            // 현재 ghost 정보
pair<int, int> pacman;                              // 팩맨 위치
vector<pii> path_tmp;
set<pii> visited;

bool inRange(int y, int x) { return 0 <= y && y < 4 && 0 <= x && x < 4; }

void input() {
    cin >> m >> t;
    int r, c, d;
    cin >> r >> c;
    pacman = {r - 1, c - 1};
    for (int i = 0; i < m; i++) {
        cin >> r >> c >> d;
        ghost.push_back({false, r - 1, c - 1, d - 1, 0});
    }
}

ghostInfo moveGhost(ghostInfo g) {
    for (int i = 0; i < 8; i++) {
        int ny = g.row + dy[g.dir];
        int nx = g.col + dx[g.dir];
        if (inRange(ny, nx) && !deadBody[ny][nx] && make_pair(ny, nx) != pacman) {
            g.row = ny, g.col = nx;
            break;
        }
        g.dir = (g.dir == 7) ? 0 : g.dir + 1;
    }
    return g;
}

void movePacman(pii spos, int lvl, vector<pii> &path, int &cnt) {
    if (lvl == 3) {
        // 알 상태가 아니며
        int tmp = 0;
        for (auto &&i : path_tmp)
            for (auto &&j : ghost)
                tmp += (!j.isEgg && j.die_cnt == 0 && i == make_pair(j.row, j.col));

        // 가장 많이 먹을 수 있는 경우라면 path 수정
        if (tmp > cnt) {
            path = path_tmp;
            cnt = tmp;
        }
        return;
    }

    for (int i = 0; i < 7; i += 2) {
        int ny = spos.first + dy[i];
        int nx = spos.second + dx[i];
        pii pos(ny, nx);
        if (inRange(ny, nx) && visited.find(pos) == visited.end()) {
            path_tmp.push_back(pos);
            visited.insert(pos);
            movePacman(pos, lvl + 1, path, cnt);
            path_tmp.pop_back();
            visited.erase(pos);
        }
    }
}

int main() {
    fastio;
    freopen("input.txt", "r", stdin);
    input();

    for (int i = 0; i < t; i++) {
        // 몬스터 복제 시도
        int sz = ghost.size();
        for (int j = 0; j < sz; j++)
            if (!ghost[j].die_cnt) {
                ghostInfo tmp = ghost[j];
                tmp.isEgg = true;
                ghost.push_back(tmp);
            }

        // 몬스터 이동
        for (auto &&j : ghost) {
            if (j.isEgg)
                break;
            if (!j.die_cnt)
                j = moveGhost(j);
        }

        // 팩맨 이동
        int cnt = 0;
        vector<pii> path;
        path_tmp.clear(), visited.clear();
        visited.insert(pacman);
        movePacman(pacman, 0, path, cnt);
        pacman = path[2];

        // 몬스터 먹어치우기 & 시체 소멸
        for (auto &&j : path)
            for (int k = 0; k < ghost.size(); k++)
                if (!ghost[k].isEgg && j == make_pair(ghost[k].row, ghost[k].col)) {
                    if (!ghost[k].die_cnt) {
                        ghost[k].die_cnt = i + 2;
                        deadBody[ghost[k].row][ghost[k].col]++;
                    } else if (i > ghost[k].die_cnt) {
                        ghost.erase(ghost.begin() + k);
                        deadBody[ghost[k].row][ghost[k].col]--;
                        k--;
                    }
                }

        // 몬스터 복제
        for (auto &&j : ghost)
            if (j.isEgg)
                j.isEgg = false;
    }

    int ans = 0;
    for (auto &&i : ghost)
        ans += !i.die_cnt;
    cout << ans;
    return 0;
}