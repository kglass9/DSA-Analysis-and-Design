//============================================================================
// Name        : CS300ProjectTwo.cpp
// Author      : Kai Glass
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold courseData information
struct courseData {
    string courseNum; // unique identifier
    string name;
    vector <string> preReq;
};

// Internal structure for tree node
struct Node {
    courseData course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(courseData acourse) :
        Node() {
        this->course = acourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, courseData course);
    void printCourseList(Node* node);
    void printCourse(courseData course);
public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void printCourseList();
    void printCourse();
    void Insert(courseData course);
    courseData Search(string courseNum);
    
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // Initialize housekeeping variables
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::printCourseList() {
    // In order root
    // call inOrder fuction and pass root 
    this->printCourseList(root);
}

/* 
* Print desired course
*/
void BinarySearchTree::printCourse() {
    courseData course;
    // call printCourse and pass a course
    this->printCourse(course);
}

/**
* Print desired course info
*/
void BinarySearchTree::printCourse(courseData course) {
    // Define varaibles
    string courseNum;
    // Get desired course from user
    cout << "What course do you want to know about? " << endl;
    cin >> courseNum;
    // using transform() function and ::toupper in STL
    transform(courseNum.begin(), courseNum.end(), courseNum.begin(), ::toupper);
    // Look up the course they want
    course = Search(courseNum);
    // Display the courseNum and name
    vector <string> preReq = course.preReq;
    cout << course.courseNum << ", " << course.name << endl;
    // Display prerequisites
    cout << "Prerequisites: " << endl;
    for (int i = 0; i < preReq.size() - 1; i++) {
        cout << preReq[i] << ",";
    }
    for (int i = preReq.size() - 1; i < preReq.size(); i++) {
        cout << preReq[i];
    }
    cout << endl;
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(courseData course) {
    // Implement inserting a course into the tree
    // if root equarl to null ptr
    Node* node = new Node(course);
    if (root == nullptr) {
        // root is equal to new node course
        root = node;
    }
    // else
    else {
        // add Node root and course
        this->addNode(root, course);
    }
}

/**
 * Search for a course
 */
courseData BinarySearchTree::Search(string courseNum) {
    // Implement searching the tree for a course
    // set current node equal to root
    Node* current = root;
    // keep looping downwards until bottom reached or matching courseNum found
    while (current != nullptr) {
        // if match found, return current courseNum
        if (current->course.courseNum.compare(courseNum) == 0) {
            return current->course;
        }
        // if courseNum is smaller than current node then traverse left
        if (courseNum.compare(current->course.courseNum) < 0) {
            current = current->left;
        }
        // else larger so traverse right
        else {
            current = current->right;
        }
    }
    courseData course;
    return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param courseData course to be added
 */
void BinarySearchTree::addNode(Node* node, courseData course) {
    // Implement inserting a course into the tree
    // if node is larger then add to left
    if (node->course.courseNum.compare(course.courseNum) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            addNode(node->left, course);
        }
    }
    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        //else
        else {
            // recurse down the left node
            addNode(node->right, course);
        }
    }
}

void BinarySearchTree::printCourseList(Node* node) {
      //if node is not equal to null ptr
    if (node == nullptr) {
        return;
    }
    //InOrder not left
    printCourseList(node->left);
    //output courseNum and name
    cout << node->course.courseNum << ", " << node->course.name << endl;
    //InOder right
    printCourseList(node->right);
}

//============================================================================
// Static methods used for testing
//============================================================================

/*
* split string to list of strings on the basis of given delimiter
* 
* @Param a given string and a delemiter
* @Returnn a vector of string of the parsed content
*/
vector <string> parse(string courseString, string del = " ")
{
    vector <string> courseInfo;
    int start = 0;
    int end = courseString.find(del);
    while (end != -1) {
        courseInfo.push_back(courseString.substr(start, end - start));
        start = end + del.size();
        end = courseString.find(del, start);
    }
    courseInfo.push_back(courseString.substr(start, end - start));
    return courseInfo;
}
/*
* load file and store the details into list of courses
* 
* @Return a vector of courses
*/
vector <courseData> loadCourses(BinarySearchTree* bst)
{
    // Creating an object of iftsream class to open file
    cout << "hello" << endl;
    ifstream fin("ABCU_Advising_Program_Input.txt", ios::in);
    vector <courseData> courses;
    string line;
    // Infinite loop
    while (1) {
        getline(fin, line);
        // if end of file is reached then break the loop
        if (line == "-1") {
            break;
        }
        courseData course;
        // getting tokenized information which is separated by commas
        vector <string> info = parse(line, ",");
        // Store course number and name
        course.courseNum = info[0];
        course.name = info[1];
        // if there are prerequisites then storing them too
        for (int i = 2; i < info.size(); i++)
        {
            course.preReq[i].push_back(info[i]);
        }
        // appending the course into list of courses
        bst->Insert(course);
    }
    fin.close();
    return courses;
}
   

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    courseData course;
    vector <courseData> courses;

    int choice = 0;
    while (choice != 9) {
        cout << "Welcome to the course planner." << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Complete the method call to load the courses
            cout << "hello" << endl;
            courses = loadCourses(bst);
            cout << courses << endl;
            break;

        case 2:
            bst->printCourseList();
            break;

        case 3:
            bst->printCourse();
            break;
            
        default:
            if (choice != 9) {
                cout << choice << " is not a valid option" << endl;
                break;
            }
        }
    }

    cout << "Thank you for using the course planner!" << endl;

	return 0;
}
