#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
#define fastio ios::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

#define INPUT 100
#define CREATE_PRODUCT 200
#define CANCEL_PRODUCT 300
#define SELL_PRODUCT 400
#define CHANGE_DEPARTURE 500
#define INF INT32_MAX

using namespace std;
using pii = pair<int, int>;
using mii = unordered_map<int, int>;

struct product {
    int id, revenue, dest, cost;
};

struct comp {
    bool operator()(const product &p1, const product &p2) const {
        if (p1.revenue - p1.cost != p2.revenue - p2.cost)
            return p1.revenue - p1.cost > p2.revenue - p2.cost;
        return p1.id < p2.id;
    }
};

int n, m, s_place = 0;
vector<int> dist;                // 출발지에서 해당 node까지 거리
unordered_map<int, mii> MAP;     // 투어 맵 정보(출발지, {도착지, 가중치})
unordered_map<int, product> arr; // 현재 존재하는 상품 정보
set<product, comp> nyam;         // 정렬을 위한 상품 정보 변수

void input() {
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int v, u, dist;
        cin >> v >> u >> dist;

        if (MAP[v].find(u) != MAP[v].end())
            dist = min(dist, MAP[v][u]);
        MAP[v][u] = dist;
        if (MAP[u].find(v) != MAP[u].end())
            dist = min(dist, MAP[u][v]);
        MAP[u][v] = dist;
    }
}

// 최단거리 구하기(dijkstra)
void dijkstra() {
    dist = vector<int>(n, INF);
    vector<bool> visited(n, false);                    // 한 번 방문한 node는 다시 연산하지 않는다.
    priority_queue<pii, vector<pii>, greater<pii>> pq; // {가중치, 도착지}

    // 자기 자신의 가중치는 0
    dist[s_place] = 0;
    pq.push({0, s_place});

    while (!pq.empty()) {
        int node = pq.top().second;
        pq.pop();
        if (visited[node])
            continue;
        visited[node] = true;

        // i까지 거리 = 출발지에서 node까지 가는 거리 + node에서 i까지 가는 거리
        // 출발점과 도착점이 다르고 구한 거리가 최소인 경우에 update
        for (auto &&i : MAP[node]) {
            int alt = dist[node] + i.second;
            if (i.first != node && alt < dist[i.first]) {
                dist[i.first] = alt;
                pq.push({alt, i.first});
            }
        }
    }
}

void createProduct(int id, int revenue, int dest) {
    product tmp = {id, revenue, dest, dist[dest]};
    arr[id] = tmp;
    nyam.insert(tmp);
}

void cancelProduct(int id) {
    nyam.erase(arr[id]);
    arr.erase(id);
}

// 가장 우선순위 상품 팔기(상품 목록이 없거나 목적지까지 갈 수 없거나 손해보는 장사라면 -1 반환)
int sellProduct() {
    int id = -1;
    auto it = nyam.begin();
    if (it != nyam.end() && (*it).dest != INF && (*it).revenue - (*it).cost >= 0) {
        id = (*it).id;
        arr.erase((*it).id);
        nyam.erase(it);
    }
    return id;
}

void changeDeparture(int start_place) {
    s_place = start_place;
    nyam.clear();
    dijkstra();
    for (auto &&i : arr)
        createProduct(i.first, i.second.revenue, i.second.dest);
}

int main() {
    fastio;
    freopen("input.txt", "r", stdin);
    int q, cmd;
    int id, revenue, dest, s;
    cin >> q;

    for (int i = 0; i < q; i++) {
        cin >> cmd;
        switch (cmd) {
        case INPUT:
            input();
            dijkstra();
            break;
        case CREATE_PRODUCT:
            cin >> id >> revenue >> dest;
            createProduct(id, revenue, dest);
            break;
        case CANCEL_PRODUCT:
            cin >> id;
            cancelProduct(id);
            break;
        case SELL_PRODUCT:
            cout << sellProduct() << '\n';
            break;
        case CHANGE_DEPARTURE:
            cin >> s;
            changeDeparture(s);
            break;
        default:
            break;
        }
    }

    return 0;
}