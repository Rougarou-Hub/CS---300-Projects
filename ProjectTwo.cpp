//============================================================================
// Name: Hunter Prince
// Project: CS 300 Project Two
// Date: 10/16/2025
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <limits>        
#define NOMINMAX        
#include <Windows.h>

using namespace std;

const int GLOBAL_SLEEP_TIME = 2000; // default delay between screens (2 seconds)

// Struct to hold course information
struct Course {
    string courseId;
    string courseName;
    vector<string> preList;
};

// Binary Search Tree class for course organization
class BinarySearchTree {

private:
    struct Node {
        Course course;
        Node* left;
        Node* right;

        Node(Course aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
    };

    Node* root;
    int size;

    void inOrder(Node* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->course.courseId << ", " << node->course.courseName << endl;
        inOrder(node->right);
    }

public:
    BinarySearchTree() {
        root = nullptr;
        size = 0;
    }

    void InOrder() {
        inOrder(root);
    }

    void Insert(Course aCourse) {
        Node* newNode = new Node(aCourse);

        if (root == nullptr) {
            root = newNode;
        }
        else {
            Node* current = root;
            while (true) {
                if (aCourse.courseId < current->course.courseId) {
                    if (current->left == nullptr) {
                        current->left = newNode;
                        break;
                    }
                    else {
                        current = current->left;
                    }
                }
                else {
                    if (current->right == nullptr) {
                        current->right = newNode;
                        break;
                    }
                    else {
                        current = current->right;
                    }
                }
            }
        }
        size++;
    }

    Course Search(string courseId) {
        Node* current = root;

        while (current != nullptr) {
            if (current->course.courseId == courseId) {
                return current->course;
            }
            else if (courseId < current->course.courseId) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        return Course(); // Return empty if not found
    }

    int Size() {
        return size;
    }
};

//------------------------------------------------------------
// Helper Functions
//------------------------------------------------------------

// Split a CSV line into tokens
vector<string> Split(string line) {
    vector<string> tokens;
    string token;
    stringstream ss(line);

    while (getline(ss, token, ',')) {
        // Trim whitespace
        token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
        tokens.push_back(token);
    }
    return tokens;
}

// Convert string to uppercase (in place)
void convertCase(string& s) {
    for (char& c : s) c = toupper(c);
}

// Load courses from CSV file
void loadCourses(string csvPath, BinarySearchTree* courseList) {
    ifstream inFS(csvPath);

    if (!inFS.is_open()) {
        cout << "Error: Could not open file \"" << csvPath << "\"" << endl;
        return;
    }

    string line;
    while (getline(inFS, line)) {
        if (line.empty()) continue;

        vector<string> tokens = Split(line);

        if (tokens.size() < 2) {
            cout << "Warning: Skipping malformed line: " << line << endl;
            continue;
        }

        Course aCourse;
        aCourse.courseId = tokens.at(0);
        aCourse.courseName = tokens.at(1);

        for (size_t i = 2; i < tokens.size(); ++i) {
            aCourse.preList.push_back(tokens.at(i));
        }

        courseList->Insert(aCourse);
    }

    inFS.close();
    cout << "\n" << courseList->Size() << " courses loaded successfully." << endl;
}

// Display a single course
void displayCourse(Course aCourse) {
    cout << aCourse.courseId << ", " << aCourse.courseName << endl;
    cout << "Prerequisites: ";

    if (aCourse.preList.empty()) {
        cout << "none";
    }
    else {
        for (size_t i = 0; i < aCourse.preList.size(); ++i) {
            cout << aCourse.preList[i];
            if (i < aCourse.preList.size() - 1) cout << ", ";
        }
    }
    cout << endl;
}

//------------------------------------------------------------
// Main Program
//------------------------------------------------------------
int main(int argc, char* argv[]) {
    string csvPath = "CS 300 ABCU_Advising_Program_Input.csv"; // Default file name
    if (argc > 1) csvPath = argv[1];

    BinarySearchTree* courseList = new BinarySearchTree();
    Course course;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << "\n1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit." << endl;
        cout << "\nWhat would you like to do? ";

        cin >> choice;
        // fully qualify numeric_limits to be explicit
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
        case 1:
            loadCourses(csvPath, courseList);
            Sleep(GLOBAL_SLEEP_TIME);
            break;

        case 2:
            if (courseList->Size() == 0) {
                cout << "\nNo courses loaded. Please load data first." << endl;
                break;
            }
            cout << "\nHere is a sample schedule:\n" << endl;
            courseList->InOrder();
            Sleep(GLOBAL_SLEEP_TIME);
            break;

        case 3: {
            if (courseList->Size() == 0) {
                cout << "\nNo courses loaded. Please load data first." << endl;
                break;
            }

            cout << "\nWhat course do you want to know about? ";
            string courseKey;
            cin >> courseKey;
            convertCase(courseKey);

            course = courseList->Search(courseKey);
            cout << endl;

            if (!course.courseId.empty()) {
                displayCourse(course);
            }
            else {
                cout << "Course ID " << courseKey << " not found." << endl;
            }
            Sleep(GLOBAL_SLEEP_TIME);
            break;
        }

        case 9:
            cout << "\nThank you for using the course planner!" << endl;
            Sleep(GLOBAL_SLEEP_TIME);
            break;

        default:
            cout << "\n" << choice << " is not a valid option." << endl;
            Sleep(GLOBAL_SLEEP_TIME);
            break;
        }
    }

    delete courseList;
    return 0;
}
