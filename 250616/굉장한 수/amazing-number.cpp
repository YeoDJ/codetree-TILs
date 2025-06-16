#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    cout << (((n & 1 && n % 3 == 0) || (!(n & 1) && n % 5 == 0)) ? "true" : "false");
    return 0;
}