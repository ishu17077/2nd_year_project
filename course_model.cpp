#include <iostream>

using namespace std;

class Course {
private:
    inline static int _id = 0;
    int id;
    string subject_name;
    string subject_code;
    friend class CoursePlanner;

protected:
    Course(const string &subject_name, const string &subject_code) : subject_code(subject_code), subject_name(subject_name) {
        id = _id++;
    }

public:
    int getSubjectId() const { return id; }
    string getSubjectName() const { return subject_name; }
    string getSubjectCode() const { return subject_code; }
};
