#include <iostream>
#include <queue>
#include <map>
using namespace std;

int n, m;
vector<vector<int>> MAP;
vector<int> dist;

void input() {
    cin >> n >> m;
    MAP = vector<vector<int>>(n, vector<int>(n, 0));
    dist = vector<int>(n, INT32_MAX);
    dist[0] = 0;
    for (int i = 0; i < m; i++) {
        int s, e, d;
        cin >> s >> e >> d;
        MAP[s - 1][e - 1] = d;
    }
}

void dijkstra() {
    priority_queue<pair<int, int>> pq;
    pq.push({0, 0});

    while (!pq.empty()) {
        int min_node = pq.top().second;
        pq.pop();
        for (int i = 0; i < n; i++)
            if (i != min_node && MAP[min_node][i]) {
                int alt = dist[min_node] + MAP[min_node][i];
                if (alt < dist[i]) {
                    dist[i] = alt;
                    pq.push({dist[i], i});
                }
            }
    }
}

int main() {
    input();
    dijkstra();
    for (int i = 1; i < n; i++) {
        int rslt = (dist[i] == INT32_MAX) ? -1 : dist[i];
        cout << rslt << endl;
    }
    return 0;
}