#include <iostream>
using namespace std;

int main() {
    int mathA, engA;
    int mathB, engB;
    cin >> mathA >> engA;
    cin >> mathB >> engB;
    
    cout << (mathA > mathB && engA > engB);

    return 0;
}