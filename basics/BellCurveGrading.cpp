#include <iostream>
#include <cmath>
using namespace std;

int main() {
    double grades[10];
    double sum = 0, mean = 0, stdDev = 0;

    for (int i = 0; i < 10; i++) {
        cout << "Enter " << i + 1 << ". student grade: ";
        cin >> grades[i];
        sum += grades[i];
    }

    mean = sum / 10;

    for (int i = 0; i < 10; i++) {
        stdDev += pow(grades[i] - mean, 2);
    }

    stdDev = sqrt(stdDev / 10);

    cout << "\nMean: " << mean << endl;
    cout << "Standard Deviation: " << stdDev << endl;

    cout << "\nLetter Grades:\n";
    for (int i = 0; i < 10; i++) {
        cout << "Student " << i + 1 << ": " << grades[i] << " -> ";

        if (grades[i] >= mean + 1.5 * stdDev)
            cout << "AA";
        else if (grades[i] >= mean + 1.0 * stdDev)
            cout << "BA";
        else if (grades[i] >= mean + 0.5 * stdDev)
            cout << "BB";
        else if (grades[i] >= mean)
            cout << "CB";
        else if (grades[i] >= mean - 0.5 * stdDev)
            cout << "CC";
        else if (grades[i] >= mean - 1.0 * stdDev)
            cout << "DC";
        else if (grades[i] >= mean - 1.5 * stdDev)
            cout << "DD";
        else
            cout << "FF";

        cout << endl;
    }

    return 0;
}