#include <iostream>
using namespace std;

int main() {
    int a, b, c;
    cin >> a >> b >> c;

    float sum = a + b + c;
    float avg = sum / 3;
    cout << sum << endl;
    cout << avg << endl;
    cout << sum - avg << endl;
    return 0;
}