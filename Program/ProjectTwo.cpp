/* ABCU - Binary Search Tree Course Catalog Program
 * C++ Code for CS300 Project Two - Danny Fetter
*/ 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
using namespace std;

struct COURSE {
    string courseId;
    string courseName;
    vector<string> coursePrerequisites;
};
struct PENDING_PREREQUISITE {
    string courseId;
    string missingPrereqId;
};
struct NODE {
    COURSE course;
    NODE* left;
    NODE* right;
}; 

// FUNCTION PROTOTYPES are:
void DisplayCourse(const COURSE& course);
string LTrimString(const string& s);
string RTrimString(const string& s);
string TrimString(const string& s);
void MainMenu();

// GLOBAL VARIABLES are:
vector<PENDING_PREREQUISITE> pendingPrerequisites; 

// GLOBAL CONSTANTS are:
const string INPUT_FILE = "CS 300 ABCU_Advising_Program_Input.csv";

class BinarySearchTree {
    private:
        NODE* root;
        size_t size;

        void AddNode(NODE* node, const COURSE& course) {
            if (course.courseId < node->course.courseId) {
                if (node->left == nullptr) {
                    NODE* newNode = new NODE;
                    newNode->course = course;
                    newNode->left = nullptr;
                    newNode->right = nullptr;
                    node->left = newNode;
                    SetBstSize(GetBstSize() + 1);
                } else {
                    AddNode(node->left, course);
                }
            } else {
                if (node->right == nullptr) {
                    NODE* newNode = new NODE;
                    newNode->course = course;
                    newNode->left = nullptr;
                    newNode->right = nullptr;
                    node->right = newNode;
                    SetBstSize(GetBstSize() + 1);
                } else {
                    AddNode(node->right, course);
                }
            }
        }

        void RemoveNode(NODE* node, const string& courseId) {
            if (node == nullptr) {
                return;
            }
            if (courseId < node->course.courseId) {
                RemoveNode(node->left, courseId);
            } else if (courseId > node->course.courseId) {
                RemoveNode(node->right, courseId);
            } else {
                if (node->left == nullptr && node->right == nullptr) {
                    delete node;
                    SetBstSize(GetBstSize() - 1);
                    node = nullptr;
                } else if (node->left != nullptr && node->right == nullptr) {
                    NODE* temp = node->left;
                    delete node;
                    SetBstSize(GetBstSize() - 1);
                    node = temp;
                } else if (node->left == nullptr && node->right != nullptr) {
                    NODE* temp = node->right;
                    delete node;
                    SetBstSize(GetBstSize() - 1);
                    node = temp;
                } else {
                    NODE* temp = node->right;
                    while (temp->left != nullptr) {
                        temp = temp->left;
                    }
                    node->course = temp->course;
                    RemoveNode(node->right, temp->course.courseId);
                }
            }
        }
    
        COURSE SearchNode(NODE* node, const string& courseId) {
            // if course not found, return
            if (node == nullptr) return COURSE();

            // search for course within catalog
            if (courseId < node->course.courseId) {
                return SearchNode(node->left, courseId);
            } else if (courseId > node->course.courseId) {
                return SearchNode(node->right, courseId);
            } else {
                return node->course;
            }
        }

        void InOrderPrint(NODE* node) {
            if (node == nullptr) return;
            InOrderPrint(node->left);
            DisplayCourse(node->course);
            InOrderPrint(node->right);
        }

        void PostOrderDelete(NODE* node) {
            if (node == nullptr) return;
            PostOrderDelete(node->left);
            PostOrderDelete(node->right);
            delete node;
        }

        void PreOrderValidation(NODE* node) {
            if (node == nullptr) return;
            PreOrderValidation(node->left);
            PreOrderValidation(node->right);
        }
       
    public:
        BinarySearchTree() {
            root = nullptr;
            size = 0;   
        }

        ~BinarySearchTree() {
            while (root != nullptr) {
                PostOrderDelete(root);
            }
            SetBstSize(0);
        }

        void InOrderPrintAll() {
            InOrderPrint(root);
        }

        void PreOrderValidationAll() {
            PreOrderValidation(root);
        }

        void InsertNode(const COURSE& course) {
            if (root == nullptr) {
                NODE* newNode = new NODE;
                newNode->course = course;
                newNode->left = nullptr;
                newNode->right = nullptr;
                root = newNode;
                SetBstSize(GetBstSize() + 1);
            } else {
                AddNode(root, course);
            }
        }

        void Remove(const string& courseId) {
            RemoveNode(root, courseId);
        }

        COURSE Search(const string& courseId) {
            return SearchNode(root, courseId);
        }
        
        void SetRoot(NODE* newRoot) {
            root = newRoot;
        }

        NODE* GetRoot() const {
            return root;
        }

        void SetBstSize(size_t newSize) {
            size = newSize;
        }

        size_t GetBstSize() const {
            return size;
        }
    
}; 

void DisplayCourse(const COURSE& course) {
    cout << "------------------------" << endl;
    cout << "Course ID: " << course.courseId << endl;
    cout << "Course Name: " << course.courseName << endl;
    if (course.coursePrerequisites.empty()) {
        cout << "------------------------" << endl;
        cout << endl;
        return;
    }
    cout << "Course Prerequisites: \n";
    for (const auto& prereq : course.coursePrerequisites) {
        cout << " - " << prereq << "\n";
    }
    cout << "------------------------" << endl;
    cout << endl;
};

string LTrimString(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    return (start == string::npos) ? "" : str.substr(start);
}

string RTrimString(const string& str) {
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return (end == string::npos) ? "" : str.substr(0, end + 1);
}

string TrimString(const string& str) {
    return LTrimString(RTrimString(str));
}

bool IsInteger(const string& str) {
    if (str.empty()) {
        return false;
    }

    for (char ch : str) {
        if (!isdigit(ch)) { 
            return false; 
        }
    }
    return true;
};

void ReadFile(const string& filename, BinarySearchTree& tree) {
    cout << "Opening file " << filename << endl;

    // return if error on open
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Could not open file" << endl;
        return;
    }

    cout << "File open. Reading and storing data..." << endl;
    
    string rawInput = "";

    try {
        // while there are lines to read, read them
        while (getline(file, rawInput)) {
            
            // string to hold the current field being processed
            string current = "";
            /* field indicators explaination:
             * 0 is for the courseId, 
             * 1 is for the name, 
            * 2+ are for prerequisites
            */
            int field = 0;

            COURSE newCourse;

            // parse the line per character
            for (char ch : rawInput) {
                // until there is a comma, which means the current field is done
                if (ch == ',') {
                    current = TrimString(current);
                    if (field == 0) {
                        // add courseId
                        newCourse.courseId = current;
                    } else if (field == 1) {
                        // add courseName
                        newCourse.courseName = current;
                    } else if (field > 1) {
                        // add prerequisites if they exist
                        COURSE searchOutput = tree.Search(current);
                        if (!searchOutput.courseId.empty()) {
                            newCourse.coursePrerequisites.push_back(current);

                        } else {
                            PENDING_PREREQUISITE pendingPrereq;
                            pendingPrereq.courseId = newCourse.courseId;
                            pendingPrereq.missingPrereqId = current;
                            pendingPrerequisites.push_back(pendingPrereq);
                        }
                    }
                    // reset current for next field
                    current = "";
                    // increment field indicator
                    field++;
                } else {
                    // build current field string
                    current += ch;
                }
            }

            // process the last field after the final comma
            if (!current.empty()) {
                current = TrimString(current);
                // process the last field after the final comma
                if (field == 1) {
                    newCourse.courseName = current;
                } else if (field > 1) {
                    COURSE searchOutput = tree.Search(current);
                    if (!searchOutput.courseId.empty()) {
                        newCourse.coursePrerequisites.push_back(current);
                    } else {
                        PENDING_PREREQUISITE pendingPrereq;
                        pendingPrereq.courseId = newCourse.courseId;
                        pendingPrereq.missingPrereqId = current;
                        pendingPrerequisites.push_back(pendingPrereq);
                    }
                }
                // reset current for next lineq
                current = "";
                tree.SetBstSize(tree.GetBstSize() + 1);
            }
            
            tree.InsertNode(newCourse);
        }

        if (file.eof()) {
            cout << "End of file reached." << endl;
            
        } else {
            cout << "Input failure before reaching end of file." << endl;
        }

        // validate prerequisites after all courses have been added
        tree.PreOrderValidationAll();

        cout << tree.GetBstSize() << " courses were added successfully!" << endl;
        cout << "Closing file: " << filename << endl; 
        file.close();
        return;
    } catch (const exception& e) {
        cout << "Error reading file " << filename << ": " << e.what() << endl;
    }
};

void MainMenu() {
    BinarySearchTree courseCatalog;
    int choice = 0;

    cout << "Press Enter to open the menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    do {
        cout << "MAIN MENU" << endl;
        cout << "'1' to read the file: " << INPUT_FILE << "." << endl;
        cout << "'2' to print the entire course catalog." << endl;
        cout << "'3' to search for and print a specific course." << endl;
        cout << "'4' to search for and remove a specific course." << endl;
        cout << "'0' to exit." << endl;
        
        string choiceLine;
        cout << "User choice: ";
        getline(cin , choiceLine);
        if (IsInteger(choiceLine)) {
            choice = stoi(choiceLine);
        } else {
            choice = -1; // invalid choice
        }

        switch (choice) {
            case 1:
                ReadFile(INPUT_FILE, courseCatalog);
                break;

            case 2:
                if (courseCatalog.GetBstSize() < 1) {
                    cout << "No courses currently in the catalog" << endl;
                } else {
                    courseCatalog.InOrderPrintAll();
                }
                break;
            
            case 3:
                if (courseCatalog.GetBstSize() < 1) {
                    cout << "No courses currently in the catalog" << endl;
                } else {
                    string searchTerm;
                    cout << "Search by course ID: ";
                    getline(cin, searchTerm);
                    COURSE searchResult = courseCatalog.Search(searchTerm);
                    if (searchResult.courseId.empty()) {
                        cout << "No results found for: " << searchTerm << endl;
                    } else {
                        DisplayCourse(searchResult);
                    }
                }
                break;   

            case 4:
                if (courseCatalog.GetBstSize() < 1) {
                    cout << "No courses currently in the catalog" << endl;
                } else {
                    string removeTerm;
                    cout << "Remove by course ID: ";
                    getline(cin, removeTerm);
                    COURSE removeResult = courseCatalog.Search(removeTerm);
                    if (removeResult.courseId.empty()) {
                        cout << "No results found for: " << removeTerm << endl;
                    } else {
                        courseCatalog.Remove(removeResult.courseId);
                        cout << "Course " << removeTerm << " removed." << endl;
                    }
                }
                break;

            default:
                if (choice != 0) {
                    cout << "Invalid entry. Please try again." << endl;
                }
                break;
     
        }
    } while (choice != 0);

    cout << "exiting..." << endl;
};

int main() {
    MainMenu();
    return 0;
}