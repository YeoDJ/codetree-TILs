#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    cout << (char)((n < 60) ? 'F' : 'A' + (9 - (n / 90)));
    return 0;
}