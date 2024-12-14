#include <algorithm>
#include <iostream>
#include <queue>
#define fastio ios::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;
using pii = pair<int, int>;

int k, m, ans = 0, min_deg = 4;
vector<vector<int>> MAP, max_MAP;
int used[5][5] = {0};
queue<int> arr;
vector<pii> max_pos;

bool inRange(int y, int x) { return 0 <= y && y < 5 && 0 <= x && x < 5; }

void input() {
    cin >> k >> m;
    MAP = vector<vector<int>>(5, vector<int>(5, 0));
    max_MAP = MAP;

    for (int y = 0; y < 5; y++)
        for (int x = 0; x < 5; x++)
            cin >> MAP[y][x];
    for (int i = 0; i < m; i++) {
        int num;
        cin >> num;
        arr.push(num);
    }
}

// 시계방향 90도 돌리기기
void rotateMap(int ny, int nx) {
    vector<vector<int>> tmp1(3, vector<int>(3, 0));
    for (int y = -1; y <= 1; y++)
        for (int x = -1; x <= 1; x++)
            tmp1[y + 1][x + 1] = MAP[ny + y][nx + x];

    vector<vector<int>> tmp2 = tmp1;
    for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++)
            tmp1[x][2 - y] = tmp2[y][x];

    for (int y = -1; y <= 1; y++)
        for (int x = -1; x <= 1; x++)
            MAP[y + ny][x + nx] = tmp1[y + 1][x + 1];
}

vector<pii> bfs(int ny, int nx) {
    int target = MAP[ny][nx];
    int dy[] = {-1, 1, 0, 0};
    int dx[] = {0, 0, -1, 1};
    vector<pii> ans_arr;
    queue<pii> q;

    used[ny][nx] = 1;
    q.push({ny, nx});
    ans_arr.push_back({ny, nx});

    while (!q.empty()) {
        pii cur = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            ny = cur.first + dy[i];
            nx = cur.second + dx[i];
            if (inRange(ny, nx) && MAP[ny][nx] == target && !used[ny][nx]) {
                used[ny][nx] = 1;
                q.push({ny, nx});
                ans_arr.push_back({ny, nx});
            }
        }
    }
    return ans_arr;
}

int main() {
    fastio;
    input();

    for (int tc = 0; tc < k; tc++) {
        ans = 0, min_deg = 4, max_pos.clear();
        for (int y = 1; y <= 3; y++)
            for (int x = 1; x <= 3; x++)
                for (int i = 0; i < 4; i++) {
                    // 돌리고
                    rotateMap(y, x);

                    if (i < 3) {
                        // 같은 영역의 위치 및 접근 여부 초기 설정
                        vector<pii> pos;
                        memset(used, 0, sizeof(used));

                        // bfs로 좌표 하나하나 보면서 같은 번호의 유적 찾기
                        // 단, 3개 이상인 경우만
                        for (int ny = 0; ny < 5; ny++)
                            for (int nx = 0; nx < 5; nx++) {
                                if (MAP[ny][nx] && !used[ny][nx]) {
                                    vector<pii> tmp = bfs(ny, nx);
                                    if (tmp.size() >= 3)
                                        for (auto &&j : tmp)
                                            pos.push_back(j);
                                }
                            }

                        // 만약 찾은 유적의 개수가 최대 값이라면
                        // 그 최대값이 될 수 있는 유적의 위치 Update
                        int sz1 = max_pos.size(), sz2 = pos.size();
                        if (!pos.empty() && (sz1 < sz2 || (sz1 == sz2 && i < min_deg))) {
                            max_pos = pos;
                            max_MAP = MAP;
                            min_deg = i;
                        }
                    }
                }

        if (max_pos.empty())
            break;

        MAP = max_MAP;
        ans += max_pos.size();
        for (auto &&i : max_pos)
            MAP[i.first][i.second] = 0;
        max_pos.clear();

        // 새로운 유물 채우기
        for (int x = 0; x < 5; x++)
            for (int y = 4; y >= 0; y--) {
                if (arr.empty())
                    break;
                if (!MAP[y][x]) {
                    int cur = arr.front();
                    MAP[y][x] = cur;
                    arr.pop();
                }
            }

        // 유물 연쇄 작용
        // bfs 수행
        while (1) {
            memset(used, 0, sizeof(used));
            for (int y = 0; y < 5; y++)
                for (int x = 0; x < 5; x++) {
                    if (MAP[y][x] && !used[y][x]) {
                        vector<pii> tmp = bfs(y, x);
                        if (tmp.size() >= 3)
                            for (auto &&j : tmp)
                                max_pos.push_back(j);
                    }
                }

            // 없앨 유물 없으면 break
            if (max_pos.empty())
                break;

            // 또 없애
            ans += max_pos.size();
            for (auto &&i : max_pos)
                MAP[i.first][i.second] = 0;
            max_pos.clear();

            // 또 채워
            for (int x = 0; x < 5; x++)
                for (int y = 4; y >= 0; y--) {
                    if (arr.empty())
                        break;
                    if (!MAP[y][x]) {
                        int cur = arr.front();
                        MAP[y][x] = cur;
                        arr.pop();
                    }
                }
        }
        cout << ans << ' ';
    }

    return 0;
}