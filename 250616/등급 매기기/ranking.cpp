#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    cout << ((n < 60) ? 'F' : 'A' + (90 - (n / 90)));
    return 0;
}