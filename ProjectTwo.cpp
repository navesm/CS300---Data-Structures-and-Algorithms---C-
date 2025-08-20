#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// Structure for representing a course with number, title and prereqs
struct Course
{
  string courseNumber;
  string courseTitle;
  vector<string> prerequisites;
};

// struture for BST treenodes
struct TreeNode
{
  Course course;
  TreeNode *left;
  TreeNode *right;
};

// Class to manage BST of courses
class BinarySearchTree
{
public:
  TreeNode *root;

  // constructor to initialize empty BST
  BinarySearchTree() : root(nullptr) {}

  // function for inserting courses into BST
  void insert(Course course)
  {
    if (root == nullptr)
    {
      root = new TreeNode({course, nullptr, nullptr});
    }
    else
    {
      insertNode(root, course);
    }
  }

  // helper function to insert courses
  void insertNode(TreeNode *node, Course course)
  {
    if (course.courseNumber < node->course.courseNumber)
    {
      if (node->left == nullptr)
      {
        node->left = new TreeNode({course, nullptr, nullptr});
      }
      else
      {
        insertNode(node->left, course);
      }
    }
    else
    {
      if (node->right == nullptr)
      {
        node->right = new TreeNode({course, nullptr, nullptr});
      }
      else
      {
        insertNode(node->right, course);
      }
    }
  }

  // function to search for a specific course in BST
  TreeNode *search(TreeNode *node, string courseNumber)
  {
    if (node == nullptr)
    {
      return nullptr;
    }
    if (node->course.courseNumber == courseNumber)
    {
      return node;
    }
    if (courseNumber < node->course.courseNumber)
    {
      return search(node->left, courseNumber);
    }
    else
    {
      return search(node->right, courseNumber);
    }
  }

  // Function to print ALL courses in BST in alphanumeric order
  void printInOrder(TreeNode *node)
  {
    if (node != nullptr)
    {
      printInOrder(node->left);
      cout << node->course.courseNumber << ": " << node->course.courseTitle << endl;
      printInOrder(node->right);
    }
  }
};

// Function to validate all prereq courses exist for any given course
void validateCourses(vector<Course> &courses)
{
  for (auto &course : courses)
  {
    for (auto &prerequisite : course.prerequisites)
    {
      bool prereqFound = false;
      for (auto &otherCourse : courses)
      {
        if (otherCourse.courseNumber == prerequisite)
        {
          prereqFound = true;
          break;
        }
      }
      if (!prereqFound)
      {
        cout << "Error: Prequisite " << prerequisite << " for course " << course.courseNumber << " does not exist" << endl;
      }
    }
  }
}

// function to load courses into BST for the main menu
void loadCourses(BinarySearchTree &courses, const string &filename)
{
  ifstream file(filename);
  if (!file.is_open())
  {
    cout << "Error: Cannot open file" << endl;
    return;
  }

  vector<Course> courseList;
  string line;
  while (getline(file, line))
  {
    istringstream iss(line);
    string part;
    vector<string> parts;
    while (getline(iss, part, ','))
    {
      parts.push_back(part);
    }

    if (parts.size() < 2)
    {
      cout << "Error: Line missing parameter" << endl;
      continue;
    }

    Course course;
    course.courseNumber = parts[0];
    course.courseTitle = parts[1];
    // loop through prereqs and add to courseList vector
    for (size_t i = 2; i < parts.size(); ++i)
    {
      course.prerequisites.push_back(parts[i]);
    }
    courseList.push_back(course);
  }
  file.close();

  validateCourses(courseList);

  // insert validated courses into BST
  for (auto &course : courseList)
  {
    courses.insert(course);
  }
}

// Function to search for course by number and print details
void searchCourse(BinarySearchTree &courses, const string &courseNumber)
{
  TreeNode *current = courses.search(courses.root, courseNumber);
  if (current != nullptr)
  {
    cout << current->course.courseNumber << ", " << current->course.courseTitle << endl;
    if (current->course.prerequisites.empty())
    {
      cout << "Prerequisites: None" << endl;
    }
    else
    {
      cout << "Prerequisites: ";
      for (size_t i = 0; i < current->course.prerequisites.size(); ++i)
      {
        cout << current->course.prerequisites[i];
        if (i < current->course.prerequisites.size() - 1)
        {
          cout << ", ";
        }
      }
      cout << endl;
    }
  }
  else
  {
    cout << "Course " << courseNumber << " not found" << endl;
  }
}

// function to print all courses in BST in alphanumeric order
void printAllCourses(BinarySearchTree &courses)
{
  courses.printInOrder(courses.root);
}

void displayMenu()
{
  cout << endl
       << endl;
  cout << "ABCU Course Management System" << endl;
  cout << "1. Load Data Structure" << endl;
  cout << "2. Print Course List" << endl;
  cout << "3. Print Course" << endl;
  cout << "9. Exit" << endl;
  cout << "Make your selection..." << endl;
}

// Main driver function to run course management system
int main()
{
  BinarySearchTree courses;
  int choice = 0;
  string courseNumber;
  string filename = "CS 300 ABCU_Advising_Program_Input.csv";
  bool dataLoaded = false;

  while (choice != 9)
  {
    displayMenu();
    cout << endl;
    cin >> choice;
    cout << endl;

    switch (choice)
    {
    case 1:
      loadCourses(courses, filename);
      dataLoaded = true;
      cout << "Courses Loaded successfully" << endl;
      break;
    case 2:
      if (!dataLoaded)
      {
        cout << "Please load courses first" << endl;
      }
      else
      {
        printAllCourses(courses);
      }
      break;
    case 3:
      if (!dataLoaded)
      {
        cout << "Please load courses first" << endl;
      }
      else
      {
        cout << "Enter course number: ";
        cin.ignore();
        getline(cin, courseNumber);
        searchCourse(courses, courseNumber);
      }
      break;
    case 9:
      cout << "Goodbye!" << endl;
      break;
    default:
      cout << "Invalid option. Please try again." << endl;
      break;
    }
  }

  return 0;
}