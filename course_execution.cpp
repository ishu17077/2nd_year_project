#include "course_planner.cpp"
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;

void askCourse(CoursePlanner &cp) {
    string subjectName, subjectCode;
    cout << "Enter Subject name: ";
    cin >> subjectName;
    cout << "Enter Subject Code: ";
    cin >> subjectCode;
    Course c = cp.addCourse(subjectCode, subjectName);
    cout << "Subject ID: " << c.getSubjectId() << endl;
    cout << "--------------------------------------------" << endl;
}

void printCourseCodes(vector<Course> courses) {
    for (auto &c : courses) {
        cout << c.getSubjectCode() << endl;
    }
}

int main(int argc, char const *argv[]) {
    int n;
    CoursePlanner cp;
    cout << "Enter the number of courses, you want to enter: ";
    cin >> n;

    while (n-- > 0) {
        askCourse(cp);
    }

    int dependencies;
    vector<vector<int>> dependency;
    cout << "Enter the number of dependencies you want to add: ";
    cin >> dependencies;

    cout << "Enter dependencies(ID):" << endl;
    cin >> ws;

    while (dependencies-- > 0) {
        string line;
        getline(cin, line);

        stringstream ss(line);
        vector<int> currentDependency;
        int courseId;

        //? Extract each space-separated integer and put it in our inner vector
        while (ss >> courseId) {
            currentDependency.push_back(courseId);
        }

        dependency.push_back(currentDependency);
    }
    try {
        cp.defineDependency(dependency);
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << endl;
        return 400;
    }

    int id;
    cout << "Enter course id to get prerequisite and complete chain: ";
    cin >> id;
    cout << "Prerequisite to the course: " << endl;
    cout << "-------------------------------------\n";
    printCourseCodes(cp.getCoursePrerequisites(id));
    cout << "\n\n\n";
    cout
        << "Complete chain: " << endl;
         cout << "-------------------------------------\n";
    printCourseCodes(cp.getCompleteCourseChain(id));
   cout << endl;
    return 0;
}
