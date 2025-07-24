#include <iostream>
using namespace std;

// Creating a union
union geek {
    int age;
    char grade;
    float GPA;
};

int main() {

    // Defining a union variable
    union geek student1;

    // Assigning values to data member of union geek and
    // printing the values of data members
    student1.age = 25;
    cout << student1.age << endl;
    student1.grade = 'B';
    cout << student1.grade << endl;
    student1.GPA = 4.5;
    cout <<  student1.GPA << endl;
  	student1.age = "23.5";
    cout <<  student1.age;
    return 0;
}