#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    cout << ((n >= 3000) ? "book" : (n >= 1000) ? "mask" : "no");
    return 0;
}