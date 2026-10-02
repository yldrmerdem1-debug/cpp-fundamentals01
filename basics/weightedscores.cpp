#include <iostream>
using namespace std;

int main() {
    int student[10][3];
    double totalgrade[10];
    double passingscore;
    double total = 0;

    for (int i = 0; i < 10; i++) {
        totalgrade[i] = 0;

        for (int j = 0; j < 3; j++) {
            if (j == 0) {
                cout << "Please enter midterm grade of student " << i + 1 << ": ";
                cin >> student[i][j];
                totalgrade[i] += student[i][j] * 0.30;
            }
            else if (j == 1) {
                cout << "Please enter project grade of student " << i + 1 << ": ";
                cin >> student[i][j];
                totalgrade[i] += student[i][j] * 0.30;
            }
            else if (j == 2) {
                cout << "Please enter final grade of student " << i + 1 << ": ";
                cin >> student[i][j];
                totalgrade[i] += student[i][j] * 0.40;
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        total += totalgrade[i];
    }

    passingscore = total / 10.0;

    cout << "\nPassing score is: " << passingscore << endl;

    for (int i = 0; i < 10; i++) {
        cout << "Student " << i + 1 << " final weighted score: " << totalgrade[i];

        if (totalgrade[i] >= passingscore)
            cout << " -> PASS" << endl;
        else
            cout << " -> FAIL" << endl;
    }

    return 0;
}