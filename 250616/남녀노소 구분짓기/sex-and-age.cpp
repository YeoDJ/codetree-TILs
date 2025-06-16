#include <iostream>
using namespace std;

int main() {
    int gender, age;
    cin >> gender >> age;

    if (gender)
        cout << ((age >= 19) ? "WOMAN" : "GIRL");
    else
        cout << ((age >= 19) ? "MAN" : "BOY");
    return 0;
}