#include <iostream>
using namespace std;

class Time {
public:
    // Constructor
    Time() {
        hour = 0;
        minute = 0;
        second = 0;
    }

    // Set hour, minute and second
    void setTime(int h, int m, int s) {
        hour = (h >= 0 && h < 24) ? h : 0;
        minute = (m >= 0 && m < 60) ? m : 0;
        second = (s >= 0 && s < 60) ? s : 0;
    }

    // Print time in universal-time format
    void printUniversal() {
        cout << hour << ":" << minute << ":" << second;
    }

    // Print time in standard-time format
    void printStandard() {
        cout << ((hour == 0 || hour == 12) ? 12 : hour % 12) << ":"
             << minute << ":" << second
             << (hour < 12 ? " AM" : " PM");
    }

private:
    int hour;
    int minute;
    int second;
};

int main() {
    Time t; // constructor burada otomatik çalışır

    cout << "The initial universal time is ";
    t.printUniversal();

    cout << "\nThe initial standard time is ";
    t.printStandard();

    t.setTime(13, 27, 6);

    cout << "\n\nUniversal time after setTime is ";
    t.printUniversal();

    cout << "\nStandard time after setTime is ";
    t.printStandard();

    t.setTime(99, 99, 99);

    cout << "\n\nAfter attempting invalid settings:";
    cout << "\nUniversal time: ";
    t.printUniversal();

    cout << "\nStandard time: ";
    t.printStandard();

    cout << endl;

    return 0;
}