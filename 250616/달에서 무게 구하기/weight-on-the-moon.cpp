#include <iostream>
using namespace std;

int main() {
    int weight = 13;
    float gravity_ratio = 0.165;
    printf("%d * %.6f = %.6f", weight, gravity_ratio, weight * gravity_ratio);
    return 0;
}