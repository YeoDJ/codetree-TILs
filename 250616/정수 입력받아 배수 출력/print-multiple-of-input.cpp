#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    for (int i = n; i <= n * 5; i += n)
        cout << i << ' ';
    return 0;
}