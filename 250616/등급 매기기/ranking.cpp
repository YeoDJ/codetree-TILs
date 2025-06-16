#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    cout << (char)((n < 60) ? 'F' : (n == 100) ? 'A' : 'A' + (9 - (n / 10)));
    return 0;
}