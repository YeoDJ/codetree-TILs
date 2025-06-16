#include <algorithm>
#include <iostream>
using namespace std;

int main() {
    int arr[3];
    for (int i : arr)
        cin >> i;
    sort(arr.begin(), arr.end());
    cout << arr[1];
    return 0;
}