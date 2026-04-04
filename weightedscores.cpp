#include<iostream>
using namespace std;
int main() {
    int student[10][3];
    int totalgrade[10];
    double passingscore;
    int total;
    for(int i=0;i<10;i++) {
        for(int j=0;j<3;j++) {
            if (j==0)
                cout<<"Please enter midterm grade of student: ";
                cin>>student[i][j];
                totalgrade[i]=(student[i][j]*30)/100;
            if (j==1)
                cout<<"Please enter project grade of student: ";
                cin>>student[i][j];
                totalgrade[i]=(student[i][j]*30)/100;
            if (j==2)
                cout<<"Please enter final grade of student: ";
                cin>>student[i][j];
                totalgrade[i]=(student[i][j]*40)/100;
        }
    }
    for (int i=0;i<10;i++) {
        total=total+totalgrade[i];
    }
    passingscore=total/10;
    for (int i=0;i<10;i++) {
        if (totalgrade[i]=>passingscore)
            cout<<i<<".student is pass."<<endl;
        else
            cout<<i<<".student is not pass."<<endl;
    }
}