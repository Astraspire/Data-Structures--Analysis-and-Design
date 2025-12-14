/* ABCU - Binary Search Tree Course Catalog Program
 * C++ Code for CS300 Project Two - Danny Fetter
*/ 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

struct COURSE {
    std::string courseId;
    std::string courseName;
    std::vector<std::string> coursePrerequisites;
};
struct PENDING_PREREQUISITE {
    std::string courseId;
    std::string missingPrereqId;
};
struct NODE {
    COURSE course;
    NODE* left;
    NODE* right;
}; 

// FUNCTION PROTOTYPES are:
void DisplayCourse(const COURSE& course);
std::string LTrimString(const std::string& s);
std::string RTrimString(const std::string& s);
std::string TrimString(const std::string& s);
void MainMenu();

// GLOBAL VARIABLES are:
std::vector<PENDING_PREREQUISITE> pendingPrerequisites; 

// GLOBAL CONSTANTS are:
const std::string INPUT_FILE = "CS 300 ABCU_Advising_Program_Input.csv";

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

        void RemoveNode(NODE* node, const std::string& courseId) {
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
    
        COURSE SearchNode(NODE* node, const std::string& courseId) {
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

        void Remove(const std::string& courseId) {
            RemoveNode(root, courseId);
        }

        COURSE Search(const std::string& courseId) {
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
    std::cout << "Course ID: " << course.courseId << std::endl;
    std::cout << "Course Name: " << course.courseName << std::endl;
    std::cout << "Course Prerequisites: ";
    for (const auto& prereq : course.coursePrerequisites) {
        std::cout << prereq << " ";
    }
    std::cout << std::endl;
};

std::string LTrimString(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : str.substr(start);
}

std::string RTrimString(const std::string& str) {
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

std::string TrimString(const std::string& str) {
    return LTrimString(RTrimString(str));
}

void ReadFile(const std::string& filename, BinarySearchTree& tree) {
    std::cout << "Opening file " << filename << std::endl;

    // return if error on open
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open file" << std::endl;
        return;
    }

    std::cout << "File open. Reading and storing data..." << std::endl;
//     SET rawInput to empty text
    std::string rawInput;

    
    try {
        while (std::getline(file, rawInput)) {
            std::cin >> rawInput;

            std::string current = "";
            int field = 0;
            /* field indicators explaination:
             * 0 is for the courseId, 
             * 1 is for the name, 
            * 2+ are for prerequisites
            */

            COURSE newCourse;

            for (char ch : rawInput) {
                if (ch == ',') {
                    current = TrimString(current);
                    if (field == 0) {
                        newCourse.courseId = current;
                    } else if (field == 1) {
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
                    current = "";
                    field++;
                } else {
                    current += ch;
                }
            }

            if (!current.empty()) {
                current = TrimString(current);
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
                current = "";
                tree.SetBstSize(tree.GetBstSize() + 1);
            }
            
            tree.InsertNode(newCourse);
        }

        if (file.eof()) {
            std::cout << "End of file reached." << std::endl;
            
        } else {
            std::cout << "Input failure before reaching end of file." << std::endl;
        }

        tree.PreOrderValidationAll();

        std::cout << tree.GetBstSize() << " courses were added successfully!" << std::endl;
        std::cout << "Closing file: " << filename << std::endl; 
        file.close();
        return;
    } catch (const std::exception& e) {
        std::cout << "Error reading file " << filename << ": " << e.what() << std::endl;
    }
};

void MainMenu() {
    BinarySearchTree courseCatalog;
    int choice = 0;

    std::cout << "Press Enter to open the menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    do {
        std::cout << "MAIN MENU" << std::endl;
        std::cout << "'1' to read the file." << std::endl;
        std::cout << "'2' to print the entire course catalog." << std::endl;
        std::cout << "'3' to search for and print a specific course." << std::endl;
        std::cout << "'4' to search for and remove a specific course." << std::endl;
        std::cout << "'0' to exit." << std::endl;
        std::cout << "User choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                ReadFile(INPUT_FILE, courseCatalog);
                break;

            case 2:
                if (courseCatalog.GetBstSize() < 1) {
                    std::cout << "No courses currently in the catalog" << std::endl;
                } else {
                    courseCatalog.InOrderPrintAll();
                }
                break;
            
            case 3:
                if (courseCatalog.GetBstSize() < 1) {
                    std::cout << "No courses currently in the catalog" << std::endl;
                } else {
                    std::string searchTerm;
                    std::cout << "Search by course name or ID: ";
                    std::getline(std::cin, searchTerm);
                    COURSE searchResult = courseCatalog.Search(searchTerm);
                    if (searchResult.courseId.empty()) {
                        std::cout << "No results found for: " << searchTerm << std::endl;
                    } else {
                        DisplayCourse(searchResult);
                    }
                }
                break;

            case 4:
                if (courseCatalog.GetBstSize() < 1) {
                    std::cout << "No courses currently in the catalog" << std::endl;
                } else {
                    std::string removeTerm;
                    std::cout << "Remove by course name or ID: ";
                    std::getline(std::cin, removeTerm);
                    COURSE removeResult = courseCatalog.Search(removeTerm);
                    if (removeResult.courseId.empty()) {
                        std::cout << "No results found for: " << removeTerm << std::endl;
                    } else {
                        courseCatalog.Remove(removeResult.courseId);
                        std::cout << "Course " << removeTerm << " removed." << std::endl;
                    }
                }
                break;

            
        }
    } while (choice != 0);

    std::cout << "exiting..." << std::endl;
};

int main() {
    MainMenu();
    return 0;
}