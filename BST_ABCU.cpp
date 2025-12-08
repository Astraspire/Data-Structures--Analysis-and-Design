/* BCU University Course Catalog Software
 * By Danny Fetter
 * For CS300 DSA: Analysis and Design
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct COURSE {
    string courseId;
    string name;
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
struct BinarySearchTree {
    NODE* root;
    int size;
};

void DisplayCourse(const COURSE& course);
void SetBstSize(BinarySearchTree& tree, int newSize);
int GetBstSize(const BinarySearchTree& tree);
string LTrimString(const string& s);
string RTrimString(const string& s);
string TrimString(const string& s);
void AddNode(NODE* node, const COURSE& course);
NODE* RemoveNode(NODE* node, const string& courseId);
void InOrderPrint(NODE* node);
void PostOrderDelete(NODE* node);
void PreOrderValidation(NODE* node);
void ReadFile(const string& fileName, BinarySearchTree& courseCatalog);
void InOrderPrintAll(BinarySearchTree& tree);
void PostOrderDeleteAll(BinarySearchTree& tree);
void PreOrderValidationAll(BinarySearchTree& tree);
void Insert(BinarySearchTree& tree, const COURSE& course);
void Remove(BinarySearchTree& tree, const string& courseId);
COURSE Search(BinarySearchTree& tree, const string& courseId);
void MainMenu();

const string INPUT_FILE = "ABCU-CourseData.txt";
BinarySearchTree courseCatalog = {nullptr, 0};
vector<PENDING_PREREQUISITE> pendingPrerequisites;

void DisplayCourse(const COURSE& course) {
    if (course.courseId.empty()) {
        cout << "No course data to display." << endl;
        return;
    }
    cout << course.courseId << ": " << course.name << endl;
    if (course.coursePrerequisites.size() > 0) {
        cout << "  Prerequisites: " << endl;
        for (const auto& prereqId : course.coursePrerequisites) {
            cout << "    " << prereqId << endl;
        }
    }
}

void SetBstSize(BinarySearchTree& tree, int newSize) {
    tree.size = newSize;
}

int GetBstSize(const BinarySearchTree& tree) {
    return tree.size;
}

string LTrimString(const string& s) {
    size_t start = s.find_first_not_of(' ');
    return (start == string::npos) ? "" : s.substr(start);
}

string RTrimString(const string& s) {
    size_t end = s.find_last_not_of(' ');
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}

string TrimString(const string& s) {
    return RTrimString(LTrimString(s));
}

void AddNode(NODE* node, const COURSE& course) {
    if (course.courseId < node->course.courseId) {
        if (node->left == nullptr) {
            NODE* newNode = new NODE();
            newNode->course = course;
            newNode->left = nullptr;
            newNode->right = nullptr;
            node->left = newNode;
            SetBstSize(courseCatalog, GetBstSize(courseCatalog) + 1);
        } else {
            AddNode(node->left, course);
        }
    } else {
        if (node->right == nullptr) {
            NODE* newNode = new NODE();
            newNode->course = course;
            newNode->left = nullptr;
            newNode->right = nullptr;
            node->right = newNode;
            SetBstSize(courseCatalog, GetBstSize(courseCatalog) + 1);
        } else {
            AddNode(node->right, course);
        }
    }
}

NODE* RemoveNode(NODE* node, const string& courseId) {
    if (node == nullptr) {
        return node;
    } else if (courseId < node->course.courseId) {
        node->left = RemoveNode(node->left, courseId);
    }

    if (courseId == node->course.courseId) {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            SetBstSize(courseCatalog, GetBstSize(courseCatalog) - 1);
            return nullptr;
        } else if (node->left != nullptr && node->right == nullptr) {
            NODE* temp = node->left;
            delete node;
            SetBstSize(courseCatalog, GetBstSize(courseCatalog) - 1);
            return temp;
        } else if (node->left == nullptr && node->right != nullptr) {
            NODE* temp = node->right;
            delete node;
            SetBstSize(courseCatalog, GetBstSize(courseCatalog) - 1);
            return temp;
        } else {
            NODE* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->course = temp->course;
            node->right = RemoveNode(node->right, temp->course.courseId);
            return node;
        }
    } else if (courseId > node->course.courseId) {
        node->right = RemoveNode(node->right, courseId);
        return node;
    }
    return node;
}

void InOrderPrint(NODE* node) {
    if (node != nullptr) {
        InOrderPrint(node->left);
        DisplayCourse(node->course);
        InOrderPrint(node->right);
    }
}

void PostOrderDelete(NODE* node) {
    if (node != nullptr) {
        PostOrderDelete(node->left);
        PostOrderDelete(node->right);
        RemoveNode(node, node->course.courseId);
    }
}

void PreOrderValidation(NODE* node) {
    if (node != nullptr) {
        if (!pendingPrerequisites.empty()) {
            for (auto it = pendingPrerequisites.begin(); it != pendingPrerequisites.end(); ) {
                PENDING_PREREQUISITE& pendingPrerequisite = *it;
                COURSE searchOutput = Search(courseCatalog, pendingPrerequisite.missingPrereqId);
                if (searchOutput.courseId.empty()) {
                    cout << "Validation Error: Course " << node->course.courseId << " has missing prerequisite " << pendingPrerequisite.missingPrereqId << endl;
                    ++it;
                } else {
                    string courseToUpdate = pendingPrerequisite.courseId;
                    searchOutput.coursePrerequisites.push_back(pendingPrerequisite.missingPrereqId);
                    Remove(courseCatalog, courseToUpdate);
                    Insert(courseCatalog, searchOutput);
                    it = pendingPrerequisites.erase(it);
                }
            }
        }
        PreOrderValidation(node->left);
        PreOrderValidation(node->right);
    }
}

void ReadFile(const string& fileName, BinarySearchTree& courseCatalog) {
    cout << "Opening file " << fileName << endl;
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Could not open file" << endl;
        return;
    }
    cout << "File open. Reading and storing data..." << endl;
    string rawInput;
    try {
        while (getline(file, rawInput)) {
            string current;
            int field = 0; 
            // field indicators explanation:
            // 0 is for the courseId, 
            // 1 is for the name, 
            // 2+ are for prerequisites
            //
            COURSE newCourse;

            for (char ch : rawInput) {
                if (ch == ',') {
                    current = TrimString(current);
                    if (field == 0) {
                        newCourse.courseId = current;
                    } else if (field == 1) {
                        newCourse.name = current;
                    } else if (field > 1) {
                        COURSE searchOutput = Search(courseCatalog, current);
                        if (!searchOutput.courseId.empty()) {
                            newCourse.coursePrerequisites.push_back(current);
                        } else {
                            PENDING_PREREQUISITE pendingPrereq;
                            pendingPrereq.courseId = newCourse.courseId;
                            pendingPrereq.missingPrereqId = current;
                            pendingPrerequisites.push_back(pendingPrereq);
                        }
                    }
                    current.clear();
                    field++;
                } else {
                    current += ch;
                }
            }

            if (current.length() >= 1) {
                current = TrimString(current);
                if (field == 1) {
                    newCourse.name = current;
                } else if (field > 1) {
                    COURSE searchOutput = Search(courseCatalog, current);
                    if (!searchOutput.courseId.empty()) {
                        newCourse.coursePrerequisites.push_back(current);
                    } else {
                        PENDING_PREREQUISITE pendingPrereq;
                        pendingPrereq.courseId = newCourse.courseId;
                        pendingPrereq.missingPrereqId = current;
                        pendingPrerequisites.push_back(pendingPrereq);
                    }
                }
                current.clear();
                field++;
            }

            Insert(courseCatalog, newCourse);
        }

        if (!file.eof()) {
            cout << "Input failure before reaching end of file." << endl;
        }

        // final validation pass to ensure all prerequisites are met
        PreOrderValidation(courseCatalog.root);
        cout << GetBstSize(courseCatalog) << " courses were added successfully!" << endl;
        cout << "Closing file: " << fileName << endl;

        file.close();

        cout << "File: " << fileName << endl;
        cout
                  << "Import complete." << endl;
                } catch (const exception& e) {
        cout << "Error reading file " << fileName << endl;
    }
}

void InOrderPrintAll(BinarySearchTree& tree) {
    InOrderPrint(tree.root);
}

void PostOrderDeleteAll(BinarySearchTree& tree) {
    PostOrderDelete(tree.root);
}

void PreOrderValidationAll(BinarySearchTree& tree) {
    PreOrderValidation(tree.root);
}

void Insert(BinarySearchTree& tree, const COURSE& course) {
    NODE* newNode = new NODE();
    newNode->course = course;
    newNode->left = nullptr;
    newNode->right = nullptr;

    if (tree.root == nullptr) {
        tree.root = newNode;
        SetBstSize(courseCatalog, GetBstSize(courseCatalog) + 1);
        return;
    }

    AddNode(tree.root, course);
}

void Remove(BinarySearchTree& tree, const string& courseId) {
    tree.root = RemoveNode(tree.root, courseId);
    if (tree.root == nullptr) {
        return;
    }
}

COURSE Search(BinarySearchTree& tree, const string& courseId) {
    NODE* currentNode = tree.root;
    while (currentNode != nullptr) {
        if (currentNode->course.courseId == courseId) {
            return currentNode->course;
        } else if (courseId < currentNode->course.courseId) {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }
    }
    COURSE course;
    return course;
}

void MainMenu() {
    int choice = 0;
    cout << "Press Enter to open the menu..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    do {
        cout << "MAIN MENU" << endl;
        cout << "'1' to read the file." << endl;
        cout << "'2' to print the entire course catalog." << endl;
        cout << "'3' to search for and print a specific course." << endl;
        cout << "'4' to search for and remove a specific course." << endl;
        cout << "'0' to exit." << endl;
        cout << "User choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            ReadFile(INPUT_FILE, courseCatalog);
        } else if (choice == 2) {
            if (GetBstSize(courseCatalog) < 1) {
                cout << "No courses currently in the catalog" << endl;
            } else {
                InOrderPrintAll(courseCatalog);
            }
        } else if (choice == 3) {
            if (GetBstSize(courseCatalog) < 1) {
                cout << "No courses currently in the catalog" << endl;
            } else {
                string searchTerm;
                cout << "Search by course name or ID: ";
                getline(cin, searchTerm);
                COURSE searchResult = Search(courseCatalog, searchTerm);
                if (searchResult.courseId.empty()) {
                    cout << "No results found for: " << searchTerm << endl;
                } else {
                    DisplayCourse(searchResult);
                }
            }
        } else if (choice == 4) {
            if (GetBstSize(courseCatalog) < 1) {
                cout << "No courses currently in the catalog" << endl;
            } else {
                string removeTerm;
                cout << "Remove by course name or ID: ";
                getline(cin, removeTerm);
                COURSE removeResult = Search(courseCatalog, removeTerm);
                if (removeResult.courseId.empty()) {
                    cout << "No results found for: " << removeTerm << endl;
                } else {
                    Remove(courseCatalog, removeResult.courseId);
                    cout << "Course " << removeTerm << " removed." << endl;
                }
            }
        } else if (choice == 0) {
            cout << "exiting..." << endl;
        } else {
            cout << "Invalid entry!" << endl;
        }
    } while (choice != 0);
}

int main() {
    MainMenu();
    return 0;
}