//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Jason Johnson
// Description : ABCU Advising Assistance Program
//============================================================================

#include <iostream>
#include <fstream>
#include <cctype>   // for functions like toupper and isspace
#include <string>
#include <vector>
#include <set>      // for tracking unique course IDs
#include <limits>   // for numeric_limits in input validation

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

struct Course {
    string courseId;    // unique ID
    string courseName;
    vector<string> preReqs;
};

struct Node {
    Course course; 
    Node* right;
	Node* left;

	Node() {        // default constructor initializes pointers to nullptr
        right = nullptr;
        left = nullptr;
    }
	Node(const Course& aCourse) : // constructor initializes course and pointers
            Node() {
        course = aCourse;
	}
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/** 
* This class provides methods to insert courses, search for courses by ID,
* and display courses in order. It also handles memory management for the tree nodes.
*/
class CourseBST {
// Private members and helper functions
private:
    Node* root;

    void addNode(Node* node, const Course& course);
    void inOrder(Node* node);
	void deleteTree(Node* node);

// Public interface
public:
    CourseBST();
    virtual ~CourseBST();
    void Insert(const Course& course);
	void InOrder();
	const Course* Search(const string& courseId) const;
};

/**
* Default constructor initializes the root of the tree to nullptr.
*/
CourseBST::CourseBST() {
    root = nullptr;
}

/**
* Destructor calls a helper function to delete all nodes in the tree and free memory.
*/
CourseBST::~CourseBST() {
    deleteTree(root);
    root = nullptr;
}

/**
* Inserts a course into the binary search tree. 
*/
void CourseBST::Insert(const Course& course) {
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        addNode(root, course);
    }
}

/**
* Displays all courses in the tree in order (sorted by course ID).
*/
void CourseBST::InOrder() {
    inOrder(root);
}

/**
* Searches for a course by its ID. Returns a pointer to the course if found, or nullptr if not found.
*/
const Course* CourseBST::Search(const string& courseId) const {
	Node* currentNode = root;

	
    while (currentNode != nullptr) {
        if (currentNode->course.courseId == courseId) {
            return &(currentNode->course);
        }
        else if (courseId < currentNode->course.courseId) {
            currentNode = currentNode->left;
        }
        else {
            currentNode = currentNode->right;
        }
    }
	return nullptr;
}

//=============================================================================
// Helper functions for CourseBST and file processing
//=============================================================================

/**
* This function recursively adds a course to the correct position in the binary search tree based 
* on the course ID.
*/
void CourseBST::addNode(Node* node, const Course& course) {
	// If the course ID already exists in the tree, do not add it again
	if (course.courseId == node->course.courseId) {
        return;
    }
	// If the course ID is less than the current node's course ID, go left; otherwise, go right
    if (course.courseId < node->course.courseId) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        //Otherwise, continue traversing left
        else {
            addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            addNode(node->right, course);
        }
    }
}

/**
* This function performs an in-order traversal of the binary search tree, 
* printing each course's ID and name.
*/
void CourseBST::inOrder(Node* node) {
    if (node != nullptr) {
		inOrder(node->left);   
		cout << node->course.courseId << ", " << node->course.courseName << endl;
		inOrder(node->right);   
    }
}

/**
* This function recursively(post order) deletes all nodes in the binary search tree to free memory.
*/
void CourseBST::deleteTree(Node* node) {
    if (node != nullptr) {
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
}

//=============================================================================
// Helper functions for string processing and file loading
//=============================================================================

/**
* This function trims leading and trailing whitespace from a string. 
*/
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r"); // skip leading whitespace
    size_t end = str.find_last_not_of(" \t\n\r");    // skip trailing whitespace

    if (start == string::npos) {
        return "";
    }
	return str.substr(start, end - start + 1);
}

/**
* This function normalizes a course ID by trimming whitespace, removing internal spaces, 
* and converting to uppercase.
*/
string normalizeCourseId(const string& s) { // avoids copying string additional time when calling method, since we are copying it in trim
    string trimmed = trim(s);

    // remove internal spaces
    string cleaned;
    cleaned.reserve(trimmed.size()); // allocates space upfront
    for (char ch : trimmed) {
        unsigned char uch = static_cast<unsigned char>(ch); // only call cast once then pass as var
        if (!isspace(uch)) {
			cleaned += static_cast<char>(toupper(uch));  // convert to uppercase
        }
    }
    return cleaned;
}

/**
* This function splits a string into tokens based on a specified delimiter. 
*/
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;  
	string token;           

	// Iterate through each character in the input string
    for (char ch : s) {
        if (ch == delimiter) {
			tokens.push_back(trim(token));  // once delimiter is reached, add trimmed token to vector
			token.clear();      // reset token for next one
        }
        else {
			token += ch;    
        }
    }
	tokens.push_back(trim(token));  // add the last token after the loop ends
    
    return tokens;
}

/**
* This function parses a CSV line into tokens and validates that it has at least 2 fields.
*/
// Being super nit-picky here and pulling this code out since it is repeated twice (DRY principles)
bool parseLine(const string& line, vector<string>& tokens) {
    tokens = split(line, ',');
    if (tokens.size() < 2) {
        cerr << "Invalid line format: " << line << endl;
        return false;
    }
    return true;
}

/**
* This function validates the CSV file format and collects all course IDs into a set.
* It ensures each line has a course ID and name, no duplicates exist, and IDs are non-empty.
*/
bool validateCourseFile(const string& csvPath, set<string>& courseNumbers) {
    ifstream file(csvPath);

    if (!file.is_open()) {
        cerr << "Error opening file: " << csvPath << endl;
        return false;
    }

    string line;

    while (getline(file, line)) {
        vector<string> tokens;
        if (!parseLine(line, tokens)) return false;

        // Normalize the course ID and extract the course name
        string courseId = normalizeCourseId(tokens[0]);
        string courseName = tokens[1];

        // Validate that the course ID and name are not empty
        if (courseId.empty() || courseName.empty()) {
            cerr << "Missing course ID or name: " << line << endl;
            return false;
        }

        // Check for duplicate course IDs
        if (courseNumbers.find(courseId) != courseNumbers.end()) {
            cerr << "Duplicate course ID found: " << courseId << endl;
            return false;
        }

        courseNumbers.insert(courseId);
    }

    return true;
}

/**
* This function builds the CourseBST from a validated CSV file using the set of known course IDs.
*/
bool buildCourseTree(const string& csvPath, const set<string>& courseNumbers, CourseBST& outBst) {
    ifstream file(csvPath);

    if (!file.is_open()) {
        cerr << "Error opening file: " << csvPath << endl;
        return false;
    }

    string line;

    while (getline(file, line)) {
        vector<string> tokens;
        if (!parseLine(line, tokens)) return false;

        Course newCourse;
        newCourse.courseId = normalizeCourseId(tokens[0]);
        newCourse.courseName = tokens[1];

        // Process prereqs starting from the third token (index 2)
        for (size_t i = 2; i < tokens.size(); i++) {
            string preReq = normalizeCourseId(tokens[i]);

            // Validate that the prereq is not empty and exists in the set of course IDs
            if (preReq.empty()) {
                continue;
            }

            if (courseNumbers.find(preReq) == courseNumbers.end()) {
                cerr << "Prerequisite course not found: " << preReq
                    << " for course: " << newCourse.courseId << endl;
                return false;
            }

            newCourse.preReqs.push_back(preReq);
        }

        outBst.Insert(newCourse);
    }

    return true;
}

/**
* This function loads courses from a CSV file into the provided CourseBST.
* It performs validation to ensure that:
* - Each line has at least a course ID and name
* - Course IDs are unique
* - Prereqs reference valid course IDs
*/
bool loadCourses(const string& csvPath, CourseBST& outBst) {
    set<string> courseNumbers;
    if (!validateCourseFile(csvPath, courseNumbers)) return false; // Pulled logic for validating into one method
    return buildCourseTree(csvPath, courseNumbers, outBst); // And then build courseTree into antoher method
}

/**
* This function prints the details of a course, including its ID, name, and prereqs.
*/
void printCourse(const Course& course) {
    cout << course.courseId << ", " << course.courseName << endl;
    cout << "Prerequisites: ";

    if (course.preReqs.empty()) {
        cout << "None" << endl;
    }
	
    else {
        for (size_t i = 0; i < course.preReqs.size(); ++i) {
            if (i > 0) {
                cout << ", "; // just *slightly* easier to read
            }
            cout << course.preReqs[i];
        }
        cout << endl;
    }
    cout << endl;
}

/**
* This function looks up a course by ID in the CourseBST and prints its information if found.
*/
void printCourseInfo(const CourseBST& bst, const string& courseId) {
    const Course* course = bst.Search(courseId);

	// Checks if course is found
    if (course == nullptr) {
        cout << "Course not found: " << courseId << endl;
        return;
    }
    
	printCourse(*course);
}

/**
* Main entry point of the ABCU Advising Assistance program. 
* It provides a menu-driven interface for the user.
*/
int main() {
	string filePath;
	CourseBST bst;
	bool loaded = false;    // flag to track whether courses have loaded successfully

	// Main loop
    while (true) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

		// Input validation to ensure the user enters a valid integer choice
        int choice;
        if (!(cin >> choice)) {
			cin.clear();    // clear error state caused by invalid input
			cin.ignore(numeric_limits<streamsize>::max(), '\n');    // discard invalid input from buffer
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

		// Process the user's menu choice
        switch (choice) {
        case 1:
			// Prevent loading courses multiple times without restarting the program
            if (loaded) {
                cout << "Courses already loaded. Restart program to load a different file.\n";
                break;
            }

            cout << "Enter file name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, filePath);

			// Load courses from the specified file path and update the loaded flag accordingly
            if (loadCourses(filePath, bst)) {
                cout << "Courses loaded successfully from " << filePath << endl;
				loaded = true;
            }
            else {
                loaded = false;
				cout << "Failed to load courses from " << filePath << endl;
            }
            break;

        case 2:
			// Check if courses have been loaded before attempting to display them
            if (!loaded) {
                cout << "Please load courses first." << endl;
            }
            else {
				bst.InOrder();
            }
            break;

        case 3:
            if (!loaded) {
                cout << "Please load courses first." << endl;
            }
			// Look up and display course information for normalized user course ID
            else {
                string courseId;
                cout << "Enter course ID: ";
                cin >> courseId;
				courseId = normalizeCourseId(courseId);
                printCourseInfo(bst, courseId);
            }
            break;

        case 9:
            cout << "See you next time." << endl;
            return 0;

        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

