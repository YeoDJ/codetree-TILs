#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    cout << ((n < 0) ? "ice" : (n >= 100) ? "vapor" : "water")
    return 0;
}