#include <iostream>
using namespace std;

int main() {
    int grade;
    int freq[101] = {0};

    for (int i = 0; i < 100; i++) {
        cout << "Enter grade: ";
        cin >> grade;

        if (grade >= 0 && grade <= 100)
            freq[grade]++;
        else {
            cout << "Invalid grade!\n";
            i--; // tekrar al
        }
    }

    cout << "\nGrade Frequency:\n";

    for (int i = 0; i <= 100; i++) {
        if (freq[i] > 0)
            cout << "Grade " << i << ": " << freq[i] << endl;
    }

    return 0;
}