#include <iostream>
using namespace std;

int main() {
    int a, b;
    cin >> a >> b;

    float sum = a + b;
    cout << sum << ' ';
    printf("%.1f", sum / 2);
    return 0;
}