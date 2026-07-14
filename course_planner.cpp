#include "course_model.cpp"
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#define MAX 500

using namespace std;

class CoursePlanner {
private:
    unordered_map<int, Course> courseMap;
    int totalCourses = 0;
    vector<vector<int>> courseDirectedMatrix;

    enum VisitingStatus { VISITED,
                          UNVISITED,
                          VISITING };
    bool checkIfExist(int id) {
        try {
            courseMap.at(id);
            return true;
        } catch (const out_of_range &e) {
            return false;
        }
    }
    bool hasCircularDependency() {
        for (int i = 0; i < courseDirectedMatrix.size(); i++) {
            vector<VisitingStatus> visitingStats(courseDirectedMatrix.size(), UNVISITED);
            if (visitingStats[i] != VISITED) {
                if (hasCircularDependencyUtil(courseDirectedMatrix, i, visitingStats)) {
                    return true;
                }
            }
        }
        return false;
    }

    static bool hasCircularDependencyUtil(const vector<vector<int>> &graph, int src, vector<VisitingStatus> &visitingStats) {
        if (visitingStats[src] == VISITING) {
            return true;
        }

        if (visitingStats[src] == VISITED) {
            return false;
        }

        visitingStats[src] = VISITING;

        for (int i = 0; i < graph[src].size(); i++) {
            if (graph[src][i] == 1) {
                if (hasCircularDependencyUtil(graph, i, visitingStats)) {
                    return true;
                }
            }
        }
        visitingStats[src] = VISITED;
        return false;
    }

    vector<Course> findPreRequisites(int id) {
        vector<Course> courses;
        vector<bool> requiredCourses(courseDirectedMatrix.size(), false);
        queue<int> revQ;
        revQ.push(id);

        while (!revQ.empty()) {
            int curr = revQ.front();
            revQ.pop();
            for (int i = 0; i < totalCourses; i++) {
                //? requiredCourses.find(i) == requiredCourses.end() -> item not found
                if (courseDirectedMatrix[i][curr] == 1 && !requiredCourses[i]) {
                    requiredCourses[i] = true;
                    courses.push_back(courseMap.at(i));
                    revQ.push(i);
                }
            }
        }

        return courses;
    }
    vector<Course> findPostCourses(int id) {
        vector<Course> courses;
        vector<bool> requiredCourses(courseDirectedMatrix.size(), false);
        queue<int> revQ;
        revQ.push(id);

        while (!revQ.empty()) {
            int curr = revQ.front();
            revQ.pop();
            for (int i = 0; i < totalCourses; i++) {
                //? requiredCourses.find(i) == requiredCourses.end() -> item not found
                if (courseDirectedMatrix[curr][i] == 1 && !requiredCourses[i]) {
                    requiredCourses[i] = true;
                    courses.push_back(courseMap.at(i));
                    revQ.push(i);
                }
            }
        }

        return courses;
    }

public:
    Course addCourse(string subject_code, string subject_name) {
        Course course = Course(subject_name, subject_code);
        courseMap.insert({course.getSubjectId(), course});
        totalCourses++;
        return course;
    }

    /*
     * @throw std::invalid_argument exception
     */
    void defineDependency(vector<vector<int>> &dependencies) {
        courseDirectedMatrix.assign(totalCourses, vector<int>(totalCourses, 0));
        for (auto &chain : dependencies) {

            if (chain.size() < 2) {
                continue;
            }
            for (int i = 0; i < chain.size() - 1; i++) {
                int prereq = chain[i];
                int target = chain[i + 1];
                if (!checkIfExist(prereq) || !checkIfExist(target)) {
                    throw invalid_argument("Course ID not found");
                }

                courseDirectedMatrix[prereq][target] = 1;

                cout << "Rule Added: Must take ["
                     << courseMap.at(prereq).getSubjectCode() << "] "
                     << "before ["
                     << courseMap.at(target).getSubjectCode() << "]" << endl;
            }
        }

        if (hasCircularDependency()) {
            throw invalid_argument("Circular dependency exist! Impossible to continue this path.");
        }
    }
    /*
     * throw std:runtime_error exception
     */
    vector<Course> getCoursePrerequisites(int id) {
        if (courseDirectedMatrix.empty()) {
            throw runtime_error("Dependency graph not defined");
        }
        if (!checkIfExist(id)) {
            throw invalid_argument("This Course ID is not registered with us");
        }

        return findPreRequisites(id);
    }

    vector<Course> getCompleteCourseChain(int id) {
        auto preCourses = findPreRequisites(id);
        preCourses.push_back(courseMap.at(id));
        auto postCourses = findPostCourses(id);
        preCourses.insert(preCourses.end(), postCourses.begin(), postCourses.end());
        return preCourses;
    }
};
