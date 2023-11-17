#include <iostream>
#include <queue>
#include <string>
using namespace std;

int n;
string str;
vector<int> C, W;

void input() {
    cin >> n >> str;
    for (int i = 0; i < 3; i++)
        C = W = vector<int>(n, 0);

    int sumL = 0, sumR = 0;
    for (int i = 0; i < n; i++) {
        sumL += str[i] == 'C';
        sumR += str[n - 1 - i] == 'W';
        for (int j = 0; j < 3; j++)
            C[i] = sumL, W[n - 1 - i] = sumR;
    }
}

int main() {
    input();
    if (str.length() < 3) {
        cout << 0;
        return 0;
    }

    unsigned long ans = 0;
    for (int i = 1; i < n - 1; i++)
        if (str[i] == 'O')
            ans += C[i - 1] * W[i + 1];
    cout << ans;
    return 0;
}