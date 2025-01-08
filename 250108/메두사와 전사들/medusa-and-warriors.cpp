#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#define fastio ios::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

using namespace std;
using pii = pair<int, int>;

struct Knight {
    int y, x;
    bool rock = false;
};

int dy[] = {-1, 1, 0, 0};
int dx[] = {0, 0, -1, 1};
int sd[] = {-1, 0, 1}; // 시선 방향

int n, m;
pii medusa, park;
map<int, Knight> knight;             // 전사 정보
vector<vector<int>> vec_init;        // 초기화
vector<vector<int>> visited;         // 방문 노드
vector<vector<int>> MAP;             // 지도
vector<vector<int>> medusa_sight;    // 메두사 시선(-1: 전사에 의해 가려짐, 0: 시야각 밖, 1: 시야각 안)
vector<vector<set<int>>> knight_MAP; // 전사 위치

bool inRange(pii p) { return 0 <= p.first && p.first < n && 0 <= p.second && p.second < n; }

// (pos, p, dir) = (현재 탐색 위치, 전사 위치, 방향)
// 상한선, 하한선 구한 후 범위 내에 있는지 조사하기
bool inRange(pii pos, pii p, int dir) {
    if (!inRange(pos))
        return false;

    pii rangeY{0, n - 1}, rangeX{0, n - 1};
    if (dir >= 2) {
        if (p.first > medusa.first)
            rangeY.first = p.first;
        else if (p.first < medusa.first)
            rangeY.second = p.first;
        else
            rangeY = {p.first, p.first};
    } else {
        if (p.second > medusa.second)
            rangeX.first = p.second;
        else if (p.second < medusa.second)
            rangeX.second = p.second;
        else
            rangeX = {p.second, p.second};
    }
    return rangeY.first <= pos.first && pos.first <= rangeY.second && rangeX.first <= pos.second && pos.second <= rangeX.second;
}

int findDist(pii p1, pii p2) { return abs(p1.first - p2.first) + abs(p1.second - p2.second); }

void input() {
    cin >> n >> m;
    cin >> medusa.first >> medusa.second;
    cin >> park.first >> park.second;

    MAP.resize(n);
    for (auto &&i : MAP)
        i.resize(n, 0);
    vec_init = MAP;
    knight_MAP.resize(n);
    for (auto &&i : knight_MAP)
        i.resize(n);

    for (int i = 0; i < m; i++) {
        int y, x;
        cin >> y >> x;
        knight_MAP[y][x].insert(i);
        knight[i] = {y, x, false};
    }
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++)
            cin >> MAP[y][x];
}

vector<pii> findMedusaPath(pii start, pii end) {
    visited = vec_init;
    vector<vector<pii>> parent(n, vector<pii>(n, {-1, -1})); // 부모 노드
    queue<pii> q;                                            // BFS를 위한 큐
    q.push(start);
    visited[start.first][start.second] = 1;

    while (!q.empty()) {
        pii cur = q.front();
        q.pop();
        if (cur == end)
            break;

        for (int i = 0; i < 4; i++) {
            int ny = cur.first + dy[i];
            int nx = cur.second + dx[i];
            if (inRange({ny, nx}) && !visited[ny][nx] && MAP[ny][nx] == 0) {
                visited[ny][nx] = true;
                parent[ny][nx] = cur;
                q.push({ny, nx});
            }
        }
    }

    // 공원까지 방문하지 못했다면 path는 빈 배열
    vector<pii> path;
    if (!visited[end.first][end.second])
        return path;

    // 공원 좌표부터 역추적하며 부모 노드를 계속 삽입
    for (pii at = end; at != start; at = parent[at.first][at.second]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}

void setSight(pii pos, int dir, bool isKnight) {
    visited = vec_init;
    queue<pii> q;
    q.push(pos);
    visited[pos.first][pos.second] = 1;

    while (!q.empty()) {
        pii cur = q.front();
        q.pop();
        for (int i = 0; i < 3; i++) {
            int ny = cur.first + ((dir < 2) ? dy[dir] : sd[i]);
            int nx = cur.second + ((dir >= 2) ? dx[dir] : sd[i]);
            if (((!isKnight && inRange({ny, nx})) || (isKnight && inRange({ny, nx}, pos, dir))) && !visited[ny][nx]) {
                visited[ny][nx] = 1;
                medusa_sight[ny][nx] = (!isKnight) ? 1 : -1;
                q.push({ny, nx});
            }
        }
    }
}

void setCoverKnight(int dir) {
    switch (dir) {
    case 0:
        for (int y = medusa.first - 1; y >= 0; y--)
            for (int x = 0; x < n; x++)
                if (!knight_MAP[y][x].empty() && medusa_sight[y][x] == 1)
                    setSight({y, x}, dir, true);
        break;
    case 1:
        for (int y = medusa.first + 1; y < n; y++)
            for (int x = 0; x < n; x++)
                if (!knight_MAP[y][x].empty() && medusa_sight[y][x] == 1)
                    setSight({y, x}, dir, true);
        break;
    case 2:
        for (int x = medusa.second - 1; x >= 0; x--)
            for (int y = 0; y < n; y++)
                if (!knight_MAP[y][x].empty() && medusa_sight[y][x] == 1)
                    setSight({y, x}, dir, true);
        break;
    case 3:
        for (int x = medusa.second + 1; x < n; x++)
            for (int y = 0; y < n; y++)
                if (!knight_MAP[y][x].empty() && medusa_sight[y][x] == 1)
                    setSight({y, x}, dir, true);
        break;
    }
}

pii moveKnight(pii spos, bool isFirst, int &dist) {
    int minDist = findDist(spos, medusa);
    pii pos = spos;
    for (int i = 0; i < 4; i++) {
        int ny = spos.first + ((isFirst) ? dy[i] : dx[i]);
        int nx = spos.second + ((isFirst) ? dx[i] : dy[i]);
        int dist = findDist({ny, nx}, medusa);
        if (inRange({ny, nx}) && medusa_sight[ny][nx] != 1 && dist < minDist) {
            minDist = dist;
            pos = {ny, nx};
        }
    }

    dist += pos != spos;
    return pos;
}

int main() {
    fastio;
    freopen("input.txt", "r", stdin);
    input();

    vector<pii> path = findMedusaPath(medusa, park); // medusa의 최단거리로 이동하는 경로
    if (path.empty()) {
        cout << -1;
        return 0;
    }

    for (auto &&i : path) {
        // 전사 이동거리 합, 돌이 된 전사 수, 공격한 전사 수
        int knightDist = 0, turnRock = 0, attackMedusa = 0;
        int maxDir = 0; // 메두사가 돌로 만든 수가 가장 많은 방향

        // 공원에 도착했다면 0 출력 후 break
        if (i == park) {
            cout << 0;
            break;
        }

        // 메두사가 이동한 자리에 전사가 있다면 삭제
        medusa = i;
        for (auto &&j : knight_MAP[i.first][i.second])
            knight.erase(j);
        knight_MAP[i.first][i.second].clear();

        // 메두사가 네 방향을 바라보며 전사들을 돌로 만드는 최댓값 구하기
        for (int j = 0; j < 4; j++) {
            medusa_sight = vec_init;
            setSight(medusa, j, false);

            // 전사에 의해 가려지는 영역 구하기
            setCoverKnight(j);

            // 돌 되는 전사 수 구하기
            int cnt = 0;
            for (auto &&k : knight)
                cnt += medusa_sight[k.second.y][k.second.x] == 1;
            if (turnRock < cnt) {
                turnRock = max(turnRock, cnt);
                maxDir = j;
            }
        }

        // 메두사 기준 가장 가까이 있는 전사는 돌이 되어 이번 턴에 움직일 수 없다.
        // maxDir 기준으로 medusa_sight와 전사 정보를 업데이트
        medusa_sight = vec_init;
        setSight(medusa, maxDir, false);
        setCoverKnight(maxDir);

        vector<int> dieKnight;
        for (auto &&k : knight) {
            // 눈 마주친 전사를 돌로 만들기
            if (medusa_sight[k.second.y][k.second.x] == 1)
                k.second.rock = true;
            // 돌이 되지 않은 전사들의 이동 & 공격
            else {
                knight_MAP[k.second.y][k.second.x].erase(k.first);
                pii pos = moveKnight({k.second.y, k.second.x}, true, knightDist);
                pos = moveKnight(pos, false, knightDist);
                k.second.y = pos.first, k.second.x = pos.second;
                knight_MAP[k.second.y][k.second.x].insert(k.first);

                // medusa의 위치에 도달했다면 공격
                if (pos == medusa) {
                    attackMedusa++;
                    dieKnight.push_back(k.first);
                }
            }
        }

        // 공격한 전사는 소멸한다.
        for (auto &&j : dieKnight) {
            knight_MAP[knight[j].y][knight[j].x].erase(j);
            knight.erase(j);
        }

        // 출력 후 돌 상태 원복
        cout << knightDist << " " << turnRock << " " << attackMedusa << "\n";
        for (auto &&i : knight)
            i.second.rock = false;
    }
    return 0;
}