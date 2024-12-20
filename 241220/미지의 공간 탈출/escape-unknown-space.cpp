#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <tuple>
#define fastio ios::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;
using three_dim = tuple<int, int, int>;
using two_dim = pair<int, int>;

struct Spread {
    int row, col, dir, v;
};
int dy[] = {0, 0, 1, -1};
int dx[] = {1, -1, 0, 0};

int n, m, f;
int z1, _y1_, x1, y2, x2;
vector<vector<vector<int>>> space, dist1; // 벽면의 정보와 거리
vector<vector<int>> MAP, dist2;           // 바닥면의 정보와 거리
vector<Spread> arr;                       // 확산 정보, 확산되었다면 해당 좌표의 MAP 값을 1로 설정
map<int, vector<int>> set_time;           // 확산할 시간대 별 확산 idx

three_dim start_pos(-1, -1, -1); // 시작점
three_dim wall_end(-1, -1, -1);  // 벽 종점
two_dim bottom_start(-1, -1);    // 바닥의 시작점
two_dim end_pos(-1, -1);         // 도착점

bool inRange(int sz, int y, int x) { return 0 <= y && y < sz && 0 <= x && x < sz; }

// 입력 & 각 지점의 좌표 구하기기
void input() {
    cin >> n >> m >> f;
    space = vector<vector<vector<int>>>(5, vector<vector<int>>(m, vector<int>(m, 0)));
    MAP = vector<vector<int>>(n, vector<int>(n, 0));
    arr.resize(f);

    // 바닥면에 대한 정보
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++)
            cin >> MAP[y][x];

    // 벽면에 대한 정보
    for (int z = 0; z < 5; z++)
        for (int y = 0; y < m; y++)
            for (int x = 0; x < m; x++)
                cin >> space[z][y][x];

    // 확산에 대한 정보(확산 시간대도 추출)
    for (int i = 0; i < f; i++) {
        cin >> arr[i].row >> arr[i].col >> arr[i].dir >> arr[i].v;
        set_time[0].push_back(i);
        int ny = arr[i].row, nx = arr[i].col;
        for (int j = 0; j < n; j++) {
            ny += dy[arr[i].dir], nx += dx[arr[i].dir];
            int nv = arr[i].v * (j + 1);
            if (!inRange(n, ny, nx) || MAP[ny][nx] > 0 || nv >= 5 * m * m + n * n - 2)
                break;
            set_time[nv].push_back(i);
        }
    }
}

void findPos() {
    int ny, nny, nx, nnx;
    two_dim pos(-1, -1); // 시간의 벽 좌표의 좌상단 좌표

    // 바닥의 시작점과 도착점 구하기
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++) {
            if (MAP[y][x] == 0)
                for (int i = 0; i < 4; i++) {
                    ny = y + dy[i], nx = x + dx[i];
                    if (inRange(n, ny, nx) && MAP[ny][nx] == 3) {
                        bottom_start = make_pair(y, x);
                        break;
                    }
                }
            if (pos.first == -1 && MAP[y][x] == 3)
                pos = make_pair(y, x);
            if (MAP[y][x] == 4)
                end_pos = make_pair(y, x);
        }

    // 시작점과 벽 종점 구하기
    // 벽면의 끝점 구하기 -> x 좌표와 벽의 방향에 따라 바닥면의 좌표 조건을 결정한다.
    // (바닥면 기준)
    // 동쪽: y좌표 감소, x좌표 우측 끝
    // 서쪽: y좌표 증가, x좌표 좌측 끝
    // 남쪽: x좌표 증가, y좌표 하단 끝
    // 북쪽: x좌표 감소, y좌표 상단 끝
    for (int z = 0; z < 5; z++)
        for (int y = 0; y < m; y++)
            for (int x = 0; x < m; x++) {
                int tmpY[5] = {pos.first + m - 1 - x, pos.first + x, pos.first + m - 1, pos.first, -1};
                int tmpX[5] = {pos.second + m - 1, pos.second, pos.second + x, pos.second + m - 1 - x, -1};
                ny = tmpY[z], nx = tmpX[z];
                if (z != 4 && y == m - 1 && space[z][y][x] == 0 && inRange(n, ny, nx) && MAP[ny][nx] == 3) {
                    for (int i = 0; i < 4; i++) {
                        nny = ny + dy[i], nnx = nx + dx[i];
                        if (inRange(n, nny, nnx) && MAP[nny][nnx] == 0) {
                            wall_end = make_tuple(z, y, x);
                            break;
                        }
                    }
                }
                if (z == 4 && space[z][y][x] == 2)
                    start_pos = make_tuple(z, y, x);
            }
}

// 윗면 -> 벽 출구(윗면과 벽 / 벽과 벽)
// 방향과 어떤 면인지에 따라 진행 방향이 달라짐
// 시계 방향: 0, 2, 1, 3
void three_dim_bfs(three_dim spos, three_dim epos) {
    int z, y, x;
    queue<three_dim> q;
    vector<vector<vector<bool>>> visited(5, vector<vector<bool>>(m, vector<bool>(m, false)));

    // 배열 범위를 벗어났을 때 해당 두 배열을 참고한다.(단, 벽 기준 남쪽으로 향한다면 넘어가자)
    // idx 정보: (현재 위치하고 있는 면, 진행 방향) -> 값: 다음에 위치할 면
    int n_side[5][4] = {{3, 2, -1, 4}, {2, 3, -1, 4}, {0, 1, -1, 4}, {1, 0, -1, 4}, {0, 1, 2, 3}};

    dist1 = vector<vector<vector<int>>>(5, vector<vector<int>>(m, vector<int>(m, 0)));
    q.push(spos);
    tie(z, y, x) = spos;
    visited[z][y][x] = true;

    while (!q.empty()) {
        three_dim cur = q.front();
        tie(z, y, x) = cur;
        q.pop();

        for (int i = 0; i < 4; i++) {
            int nz = z;
            int ny = y + dy[i];
            int nx = x + dx[i];
            if (!inRange(m, ny, nx)) {
                nz = n_side[z][i];
                if (nz == -1)
                    continue;

                // idx 정보: (현재 면, 다음 면) -> 값: 좌표 값
                int nextY[5][5] = {{-1, -1, ny, ny, m - 1 - nx}, {-1, -1, ny, ny, nx}, {ny, ny, -1, -1, m - 1}, {ny, ny, -1, -1, 0}, {0, 0, 0, 0, -1}};
                int nextX[5][5] = {{-1, -1, m - 1, 0, m - 1}, {-1, -1, 0, m - 1, 0}, {0, m - 1, -1, -1, nx}, {m - 1, 0, -1, -1, m - 1 - nx}, {m - 1 - ny, ny, nx, m - 1 - nx, -1}};
                ny = nextY[z][nz], nx = nextX[z][nz];
                if (ny == -1 || nx == -1)
                    continue;
            }
            if (space[nz][ny][nx] == 0 && !visited[nz][ny][nx]) {
                visited[nz][ny][nx] = true;
                dist1[nz][ny][nx] += dist1[z][y][x] + 1;
                q.push(make_tuple(nz, ny, nx));
            }
            if (make_tuple(nz, ny, nx) == epos)
                return;
        }
    }
}

// 벽과 바닥의 경계 -> 도착점
void two_dim_bfs(two_dim spos, two_dim epos) {
    queue<two_dim> q;
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    dist2 = vector<vector<int>>(n, vector<int>(n, 0));

    q.push(spos);
    visited[spos.first][spos.second] = true;
    tie(z1, _y1_, x1) = wall_end;
    tie(y2, x2) = spos;
    dist2[y2][x2] = dist1[z1][_y1_][x1] + 1;

    while (!q.empty()) {
        two_dim cur = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            int ny = cur.first + dy[i];
            int nx = cur.second + dx[i];
            if (inRange(n, ny, nx) && (MAP[ny][nx] == 0 || MAP[ny][nx] == 4) && !visited[ny][nx]) {
                visited[ny][nx] = true;
                dist2[ny][nx] += dist2[cur.first][cur.second] + 1;
                q.push({ny, nx});
            }
            if (make_pair(ny, nx) == epos)
                return;
        }
    }
}

int main() {
    fastio;
    input();
    findPos();

    // 각 지점의 좌표가 제대로 나왔는지 확인
    if (get<0>(start_pos) == -1 || get<0>(wall_end) == -1 || bottom_start.first == -1 || end_pos.first == -1) {
        cout << -1;
        return 0;
    }

    int ans = -1; // 이전 sum의 값으로도 수행함
    // 윗면 -> 벽면 bfs 진행 후 진행 거리 update(3차원까지 확산하지 않음)
    three_dim_bfs(start_pos, wall_end);
    // 만약 3차원에서도 이미 진행을 못한다면 return
    tie(z1, _y1_, x1) = wall_end;
    if (!dist1[z1][_y1_][x1]) {
        cout << -1;
        return 0;
    }

    // 확산하며 2차원 bfs 진행
    for (auto &&i : set_time) {
        // 확산
        for (auto &&j : i.second) {
            MAP[arr[j].row][arr[j].col] = 1;
            arr[j].row += dy[arr[j].dir];
            arr[j].col += dx[arr[j].dir];
        }

        // 만약 확산 시간대가 현재 답보다 크다면 빠져나오기
        if (ans != -1 && i.first > ans)
            break;

        // 벽면 -> 종점 bfs 진행
        two_dim_bfs(bottom_start, end_pos);
        int sum = dist2[end_pos.first][end_pos.second];

        // 확산 결과 갈 수 없게 되었다면 빠져나오기
        if (!sum) {
            // 현재 답이 확산 시간보다 크다면 진행을 못하는 것임
            if (ans > i.first)
                ans = -1;
            break;
        }
        ans = sum;
    }

    cout << ans;
    return 0;
}