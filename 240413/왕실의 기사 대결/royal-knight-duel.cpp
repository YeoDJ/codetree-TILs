#include <iostream>
#include <queue>
using namespace std;

struct knight {
    pair<int, int> spos, epos; // 좌상단, 우하단 좌표
    int hp_init, hp;           // 초기 체력, 현재 체력
};
vector<knight> arr;

int dy[] = {-1, 0, 1, 0};
int dx[] = {0, 1, 0, -1};
int l, n, q;
vector<vector<int>> MAP;
vector<int> dmg;

bool inRange(pair<int, int> p) { return 0 <= p.first && p.first < l && 0 <= p.second && p.second < l; }

void input() {
    cin >> l >> n >> q;
    MAP = vector<vector<int>>(l, vector<int>(l));
    for (int i = 0; i < l; i++)
        for (int j = 0; j < l; j++)
            cin >> MAP[i][j];

    arr = vector<knight>(n);
    for (int i = 0; i < n; i++) {
        int r, c, h, w, k;
        cin >> r >> c >> h >> w >> k;
        arr[i] = {{r - 1, c - 1}, {h + r - 2, w + c - 2}, k, k};
    }
}

bool moveKnight(int idx, int dir) {
    dmg = vector<int>(n, 0);        // 받은 dmg 초기화
    vector<bool> isMoved(n, false); // 기사가 움직였는가?
    vector<knight> n_arr = arr;     // 다음 기사의 위치
    queue<int> q;                   // 기사의 queue

    q.push(idx);
    isMoved[idx] = true;

    while (!q.empty()) {
        int target = q.front();
        q.pop();

        n_arr[target].spos.first += dy[dir];
        n_arr[target].spos.second += dx[dir];
        n_arr[target].epos.first += dy[dir];
        n_arr[target].epos.second += dx[dir];

        if (!inRange(n_arr[target].spos) || !inRange(n_arr[target].epos))
            return false;

        // 환경 요소 충돌
        for (int i = n_arr[target].spos.first; i <= n_arr[target].epos.first; i++)
            for (int j = n_arr[target].spos.second; j <= n_arr[target].epos.second; j++) {
                // 함정인 경우
                if (MAP[i][j] == 1)
                    dmg[target]++;
                // 벽인 경우
                if (MAP[i][j] == 2)
                    return false;
            }

        // 다른 기사와 충돌
        for (int i = 0; i < n; i++) {
            // 이미 움직인 기사 or 체력 없는 기사
            if (isMoved[i] || n_arr[i].hp <= 0)
                continue;
            // 기사 충돌(두 기사의 좌표가 겹치는 경우)
            if (arr[i].spos.first > n_arr[i].epos.first || n_arr[i].spos.first > arr[i].epos.first)
                continue;
            if (arr[i].spos.second > n_arr[i].epos.second || n_arr[i].spos.second > arr[i].epos.second)
                continue;

            isMoved[i] = true;
            q.push(i);
        }
    }

    dmg[idx] = 0;
    arr = n_arr;
    return true;
}

int main() {
    input();

    for (int i = 0; i < q; i++) {
        int idx, dir;
        cin >> idx >> dir;
        if (arr[idx].hp > 0 && moveKnight(idx - 1, dir))
            for (int j = 0; j < n; j++)
                arr[j].hp -= dmg[j];
    }

    // '생존한' 기사의 damage 합 구하기
    int ans = 0;
    for (int i = 0; i < n; i++)
        if (arr[i].hp > 0)
            ans += arr[i].hp_init - arr[i].hp;
    cout << ans;
    return 0;
}