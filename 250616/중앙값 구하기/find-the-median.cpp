#include <algorithm>
#include <iostream>
using namespace std;

int main() {
    int arr[3];
    for (int &&i : arr)
        cin >> i;
    sort(arr, arr + 3);
    cout << arr[1];
    return 0;
}