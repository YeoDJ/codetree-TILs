#include <iostream>
#include <map>
#include <queue>
#include <set>
#define fastio ios::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL)

#define INPUT 100
#define CREATE_PRODUCT 200
#define CANCEL_PRODUCT 300
#define SELL_PRODUCT 400
#define CHANGE_DEPARTURE 500

#define INF INT32_MAX
using namespace std;
using pii = pair<int, int>;

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
map<int, set<pii>> MAP;  // 투어 맵 정보(출발지, {도착지, 가중치})
map<int, product> arr;   // 현재 존재하는 상품 정보
set<product, comp> nyam; // 정렬을 위한 상품 정보 변수

void input() {
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int v, u, dist;
        cin >> v >> u >> dist;
        MAP[v].insert({u, dist});
        MAP[u].insert({v, dist});
    }
}

void createProduct(int id, int revenue, int dest) {
    // 최단거리 구하기(dijkstra)
    vector<int> dist(n, INF);
    priority_queue<pii> pq; // {도착지, 가중치}

    // 자기 자신의 가중치는 0
    dist[s_place] = 0;
    pq.push({s_place, 0});

    while (!pq.empty()) {
        int min_node = pq.top().first;
        pq.pop();

        // i까지 거리 = 출발지에서 min_node까지 가는 거리 + min_node에서 i까지 가는 거리
        for (auto &&i : MAP[min_node]) {
            int alt = dist[min_node] + i.second;
            if (alt < dist[i.first]) {
                dist[i.first] = alt;
                pq.push({i.first, alt});
            }
        }
        // 도착지에 도착했다면 break
        if (min_node == dest)
            break;
    }

    product tmp = {id, revenue, dest, dist[dest]};
    arr[id] = tmp;
    nyam.insert(tmp);
}

void cancelProduct(int id) {
    nyam.erase(arr[id]);
    arr.erase(id);
}

int sellProduct() {
    int id = -1;
    for (auto &&i : nyam)
        if (i.dest != INF && i.revenue - i.cost >= 0) {
            id = i.id;
            arr.erase(i.id);
            nyam.erase(i);
            break;
        }
    return id;
}

void changeDeparture(int start_place) {
    s_place = start_place;
    for (auto &&i : arr) {
        nyam.erase(i.second);
        createProduct(i.first, i.second.revenue, i.second.dest);
    }
}

int main() {
    fastio;
    int q, cmd;
    int id, revenue, dest, s;
    cin >> q;

    for (int i = 0; i < q; i++) {
        cin >> cmd;
        switch (cmd) {
        case INPUT:
            input();
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