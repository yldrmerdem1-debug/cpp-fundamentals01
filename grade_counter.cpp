#include <iostream>
using namespace std;
int main() {
    int below50;
    int above50;
    int grade;
    for (int i=1;i<=5;i++) {
        cout<<"enter grade of students: "<<endl;
        cin>>grade;
        if (grade<50)
            below50++;
        else
            above50++;
    }
    cout<<"\nNumber of student below50: "<<below50<<endl;
    cout<<"Number of student above50: "<<above50<<endl;
}