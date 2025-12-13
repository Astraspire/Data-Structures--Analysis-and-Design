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

struct BinarySearchTree {
    NODE* root;
    size_t size;
};

// Function prototypes
void SetBstSize(BinarySearchTree& tree, int newSize);
int GetBstSize(const BinarySearchTree& tree);
void AddNode(NODE* node, const COURSE& course);
NODE* RemoveNode(NODE* node, const std::string& courseId);
void InOrderPrint(NODE* node);
void PostOrderDelete(NODE* node);
void PreOrderValidation(NODE* node);

void Insert(BinarySearchTree& tree, const COURSE& course);
void Remove(BinarySearchTree& tree, const std::string& courseId);
COURSE Search(BinarySearchTree& tree, const std::string& courseId);

void ReadFile(const std::string& filename, BinarySearchTree& tree);
void MainMenu();

const std::string INPUT_FILE = "ABCU-CourseData.txt";
BinarySearchTree courseCatalog;
std::vector<PENDING_PREREQUISITE> pendingPrerequisites; 

//   GLOBAL CONSTANTS are:
const std::string INPUT_FILE = "CS 300 ABCU_Advising_Program_Input.csv";

class BinarySearchTree {
    private:
        NODE* root;
        size_t size;

    public:
        BinarySearchTree() {
            root = nullptr;
            size = 0;   
        }

        ~BinarySearchTree() {
            while (root != nullptr) {
                PostOrderDelete(root);
            }
            SetBstSize(*this, 0);
        }

        void InOrderPrintAll(BinarySearchTree& tree) {
            InOrderPrint(tree.root);
        }

        void PreOrderValidationAll(BinarySearchTree& tree) {
            PreOrderValidation(tree.root);
        }

        void InsertNode(BinarySearchTree& tree, const COURSE& course) {
            if (tree.root == nullptr) {
                NODE* newNode = new NODE;
                newNode->course = course;
                newNode->left = nullptr;
                newNode->right = nullptr;
                tree.root = newNode;
                SetBstSize(tree, GetBstSize() + 1);
            } else {
                AddNode(tree.root, course);
            }
        }

        void Remove(BinarySearchTree& tree, const std::string& courseId) {
            tree.root = RemoveNode(tree.root, courseId);
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

        void SetRoot(NODE* newRoot) {
            root = newRoot;
        }

        NODE* GetRoot() const {
            return root;
        }

        void SetBstSize(BinarySearchTree tree, size_t newSize) {
            tree.size = newSize;
        }

        size_t GetBstSize() const {
            return size;
        }
    
    private:
        void AddNode(NODE* node, const COURSE& course) {
            if (course.courseId < node->course.courseId) {
                if (node->left == nullptr) {
                    NODE* newNode = new NODE;
                    newNode->course = course;
                    newNode->left = nullptr;
                    newNode->right = nullptr;
                    node->left = newNode;
                    SetBstSize(*this, GetBstSize() + 1);
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
                    SetBstSize(*this, GetBstSize() + 1);
                } else {
                    AddNode(node->right, course);
                }
            }
        }

        NODE* RemoveNode(NODE* node, const std::string& courseId) {
            if (node == nullptr) return;
            if (courseId < node->course.courseId) {
                RemoveNode(node->left, courseId);
            } else if (courseId > node->course.courseId) {
                RemoveNode(node->right, courseId);
            } else {
                if (node->left == nullptr && node->right == nullptr) {
                    delete node;
                    SetBstSize(*this, GetBstSize() - 1);
                    node = nullptr;
                } else if (node->left != nullptr && node->right == nullptr) {
                    NODE* temp = node->left;
                    delete node;
                    SetBstSize(*this, GetBstSize() - 1);
                    node = temp;
                } else if (node->left == nullptr && node->right != nullptr) {
                    NODE* temp = node->right;
                    delete node;
                    SetBstSize(*this, GetBstSize() - 1);
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
                        COURSE searchOutput = tree.SearchNode(tree.root, current);
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
                    COURSE searchOutput = tree.SearchNode(tree.GetRoot(), current);
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
                tree.SetBstSize(tree, tree.GetBstSize() + 1);
            }
            
            tree.InsertNode(tree, newCourse);
        }

        if (file.eof()) {
            std::cout << "End of file reached." << std::endl;
            
        } else {
            std::cout << "Input failure before reaching end of file." << std::endl;
        }

        tree.PreOrderValidationAll(tree);

        std::cout << tree.GetBstSize() << " courses were added successfully!" << std::endl;
        std::cout << "Closing file: " << filename << std::endl; 
        file.close();
        return;
    } catch (const std::exception& e) {
        std::cout << "Error reading file " << filename << ": " << e.what() << std::endl;
    }
};

void MainMenu() {
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
                    courseCatalog.InOrderPrint(courseCatalog.GetRoot());
                }
                break;
            
            case 3:
                if (courseCatalog.GetBstSize() < 1) {
                    std::cout << "No courses currently in the catalog" << std::endl;
                } else {
                    std::string searchTerm;
                    std::cout << "Search by course name or ID: ";
                    std::getline(std::cin, searchTerm);
                    COURSE searchResult = courseCatalog.SearchNode(courseCatalog.GetRoot(), searchTerm);
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
                    COURSE removeResult = courseCatalog.SearchNode(courseCatalog.GetRoot(), removeTerm);
                    if (removeResult.courseId.empty()) {
                        std::cout << "No results found for: " << removeTerm << std::endl;
                    } else {
                        courseCatalog.RemoveNode(courseCatalog.GetRoot(), removeResult.courseId);
                        std::cout << "Course " << removeTerm << " removed." << std::endl;
                    }
                }
                break;

            
        }
};
//       IF choice equals 1
//         call ReadFile(INPUT_FILE, courseCatalog)

//       ELSE IF choice equals 2
//         IF the size of courseCatalog is less than 1
//           DISPLAY "No courses currently in the catalog"
//         ELSE
//           call InOrderPrint(courseCatalog)
//         END IF

//       ELSE IF choice equals 3
//         IF the size of courseCatalog is less than 1
//           DISPLAY "No courses currently in the catalog"
//         ELSE
//           SET searchTerm to empty text
//           DISPLAY "Search by course name or ID: "
//           read a full line into searchTerm
//           CREATE a new COURSE named searchResult
//           set searchResult to output of Search(courseCatalog, searchTerm)
//           IF searchResult.courseId is empty text
//             DISPLAY "No results found for: " and searchTerm
//           ELSE
//             CALL DisplayCourse(searchResult)
//           END IF
//         END IF
//       ELSE IF choice equals 4
//         IF the size of courseCatalog is less than 1
//           DISPLAY "No courses currently in the catalog"
//         ELSE
//           SET removeTerm to empty text
//           DISPLAY "Remove by course name or ID: "
//           read a full line into removeTerm
//           CREATE a new COURSE named removeResult
//           set removeResult to output of Search(courseCatalog, removeTerm)
//           IF removeResult.courseId is empty text
//             DISPLAY "No results found for: " and removeTerm
//           ELSE
//             CALL Remove(courseCatalog, removeResult.courseId)
//             DISPLAY "Course " and removeTerm and " removed."
//           END IF
//         END IF
//       ELSE IF choice equals 0
//         DISPLAY "exiting..."
//       ELSE
//         DISPLAY "Invalid entry!"
//       END IF


//   FUNCTION MainMenu()
//     SET choice to 0

//     DISPLAY "Press Enter to open the menu..."
//     wait for the user to press Enter

//     DO
//       DISPLAY "MAIN MENU"

}

//   FUNCTION RemoveNode(node, courseId)

//     IF node is null
//       RETURN node
//     ELSE IF courseId is less than node.course.courseId
//       set node.left to RemoveNode(node.left, courseId)
//     END IF

//     IF courseId equals node.course.courseId

//       IF node.left is null AND node.right is null
//         delete node
//         CALL SetBstSize(courseCatalog, GetBstSize(courseCatalog) - 1)
//         RETURN null

//       ELSE IF node.left is not null AND node.right is null
//         set temp to node.left
//         delete node
//         CALL SetBstSize(courseCatalog, GetBstSize(courseCatalog) - 1)
//         RETURN temp

//       ELSE IF node.left is null AND node.right is not null
//         set temp to node.right
//         delete node
//         CALL SetBstSize(courseCatalog, GetBstSize(courseCatalog) - 1)
//         RETURN temp

//       ELSE
//         set temp to node.right
//         WHILE temp.left is not null
//           set temp to temp.left
//         END WHILE
//         set node.course to temp.course
//         set node.right to RemoveNode(node.right, temp.course.courseId)
//         RETURN node

//       END IF

//     ELSE IF courseId is greater than node.course.courseId
//       set node.right to RemoveNode(node.right, courseId)
//       RETURN node
//     END IF

//     RETURN node

//   END FUNCTION

//   FUNCTION InOrderPrint(node)

//     IF node is not null
//       call InOrderPrint(node.left)
//       CALL DisplayCourse(node.course)
//       call InOrderPrint(node.right)
//     END IF

//   END FUNCTION

//   FUNCTION PostOrderDelete(node)

//     IF node is not null
//       call PostOrderDelete(node.left)
//       call PostOrderDelete(node.right)
//       CALL RemoveNode(node, node.course.courseId)
//     END IF

//   END FUNCTION

//   FUNCTION PreOrderValidation(node)

//     IF node is not null
//       IF pendingPrerequisites is not empty
//         FOR EACH pendingPrerequisite in pendingPrerequisites
//           searchOutput is a COURSE
//           set searchOutput to output of Search(courseCatalog, pendingPrerequisite.missingPrereqId)
//           IF searchOutput is empty
//             DISPLAY "Validation Error: Course " and node.course.courseId and " has missing prerequisite " and pendingPrerequisite.missingPrereqId
//           ELSE
//             set courseToUpdate to pendingPrerequisite.courseId
//             add pendingPrerequisite.missingPrereqId to searchOutput.coursePrerequisites
//             Remove(courseCatalog, courseToUpdate)
//             Insert(courseCatalog, searchOutput)
//             delete pendingPrerequisite from pendingPrerequisites
//           END IF
//         END FOR
//       END IF
//       call PreOrderValidation(node.left)
//       call PreOrderValidation(node.right)
//     END IF

//   END FUNCTION

//   FUNCTION InOrderPrintAll(tree)

//     call InOrderPrint(tree.root)

//   END FUNCTION

//   FUNCTION PostOrderDeleteAll(tree)

//     call PostOrderDelete(tree.root)

//   END FUNCTION

//   FUNCTION PreOrderValidationAll(tree)

//     call PreOrderValidation(tree.root)

//   END FUNCTION

//   FUNCTION Insert(tree, course)

//     CREATE a new NODE named newNode
//     set newNode.course to course
//     set newNode.left to null
//     set newNode.right to null

//     IF tree.root is null
//       set tree.root to newNode
//       CALL SetBstSize(courseCatalog, GetBstSize(courseCatalog) + 1)
//       RETURN
//     END IF

//     call AddNode(tree.root, course)

//   END FUNCTION

//   FUNCTION Remove(tree, courseId)

//     set tree.root to RemoveNode(tree.root, courseId)

//     IF tree.root is null
//       RETURN
//     END IF

//   END FUNCTION

//   FUNCTION Search(tree, courseId)

//     set currentNode to tree.root

//     WHILE currentNode is not null

//       IF currentNode.course.courseId equals courseId
//         RETURN currentNode.course
//       ELSE IF courseId is less than currentNode.course.courseId
//         set currentNode to currentNode.left
//       ELSE
//         set currentNode to currentNode.right
//       END IF

//     END WHILE

//     CREATE an empty course named course
//     RETURN course

//   END FUNCTION

//   FUNCTION ReadFile(fileName, courseCatalog)
//     DISPLAY "Opening file " and fileName
//     open file with name fileName for reading
//     IF the file is not open
//       DISPLAY "Could not open file"
//       RETURN 1
//     END IF

//     DISPLAY "File open. Reading and storing data..."

//     SET rawInput to empty text

//     TRY
//       WHILE there is another line to read from the file
//         read the next line into rawInput

//         SET current to empty text
//         SET field to 0 
//         * field indicators explaination:
//         * 0 is for the courseId, 
//         * 1 is for the name, 
//         * 2+ are for prerequisites
//         *
//         CREATE a new COURSE named newCourse

//         FOR EACH character ch in rawInput
//           IF ch is a comma
//             set current to TrimString(current)
//             IF field equals 0
//               set newCourse.courseId to current
//             ELSE IF field equals 1
//               set newCourse.name to current
//             ELSE IF field is greater than 1
//               searchOutput is a COURSE
//               set searchOutput to output of Search(courseCatalog, current)
//               IF searchOutput is not empty
//                 add current to newCourse.coursePrerequisites
//               ELSE
//                 CREATE a new PENDING PREREQUISITE named pendingPrereq
//                 set pendingPrereq.courseId to newCourse.courseId
//                 set pendingPrereq.missingPrereqId to current
//                 add pendingPrereq to pendingPrerequisites
//               END IF
//             set current to empty text
//             increase field by 1
//           ELSE
//             add ch to the end of current
//           END IF
//         END FOR

//         IF the length of current is at least 1
//           set current to TrimString(current)
//           IF field equals 1
//             set newCourse.name to current
//           ELSE IF field is greater than 1
//             searchOutput is a COURSE
//             set searchOutput to output of Search(courseCatalog, current)
//             IF searchOutput is not empty
//               add current to newCourse.coursePrerequisites
//             ELSE
//               CREATE a new PENDING PREREQUISITE named pendingPrereq
//               set pendingPrereq.courseId to newCourse.courseId
//               set pendingPrereq.missingPrereqId to current
//               add pendingPrereq to pendingPrerequisites
//             END IF
//           END IF
//           set current to empty text
//           increase field by 1
//         END IF

//         Insert(courseCatalog, newCourse)
//       END WHILE

//       IF the file did not finish at the end marker
//         DISPLAY "Input failure before reaching end of file."
//       END IF

//       * final validation pass to ensure all prerequisites are met *
//       call PreOrderValidation(courseCatalog)
//       DISPLAY the size of courseCatalog and " courses were added successfully!"
//       DISPLAY "Closing file: " and fileName

//       close the file

//       DISPLAY "File: " and fileName
//       DISPLAY "Import complete."

//       RETURN 0
//     CATCH exception
//       DISPLAY "Error reading file ", csvPath
//     END TRY
//   END FUNCTION

//   FUNCTION MainMenu()
//     SET choice to 0

//     DISPLAY "Press Enter to open the menu..."
//     wait for the user to press Enter

//     DO
//       DISPLAY "MAIN MENU"
//       DISPLAY "'1' to read the file."
//       DISPLAY "'2' to print the entire course catalog."
//       DISPLAY "'3' to search for and print a specific course."
//       DISPLAY "'4' to search for and remove a specific course."
//       DISPLAY "'0' to exit."
//       DISPLAY "User choice: "
//       read choice
//       clear any leftover input from that number entry

//       IF choice equals 1
//         call ReadFile(INPUT_FILE, courseCatalog)

//       ELSE IF choice equals 2
//         IF the size of courseCatalog is less than 1
//           DISPLAY "No courses currently in the catalog"
//         ELSE
//           call InOrderPrint(courseCatalog)
//         END IF

//       ELSE IF choice equals 3
//         IF the size of courseCatalog is less than 1
//           DISPLAY "No courses currently in the catalog"
//         ELSE
//           SET searchTerm to empty text
//           DISPLAY "Search by course name or ID: "
//           read a full line into searchTerm
//           CREATE a new COURSE named searchResult
//           set searchResult to output of Search(courseCatalog, searchTerm)
//           IF searchResult.courseId is empty text
//             DISPLAY "No results found for: " and searchTerm
//           ELSE
//             CALL DisplayCourse(searchResult)
//           END IF
//         END IF
//       ELSE IF choice equals 4
//         IF the size of courseCatalog is less than 1
//           DISPLAY "No courses currently in the catalog"
//         ELSE
//           SET removeTerm to empty text
//           DISPLAY "Remove by course name or ID: "
//           read a full line into removeTerm
//           CREATE a new COURSE named removeResult
//           set removeResult to output of Search(courseCatalog, removeTerm)
//           IF removeResult.courseId is empty text
//             DISPLAY "No results found for: " and removeTerm
//           ELSE
//             CALL Remove(courseCatalog, removeResult.courseId)
//             DISPLAY "Course " and removeTerm and " removed."
//           END IF
//         END IF
//       ELSE IF choice equals 0
//         DISPLAY "exiting..."
//       ELSE
//         DISPLAY "Invalid entry!"
//       END IF

//     WHILE choice is not equal to 0

//   END FUNCTION


}


// FUNCTIONS:

//   FUNCTION DisplayCourse(course)
//     IF course.courseId is empty text
//       DISPLAY "No course data to display."
//       RETURN
//     END IF

//     DISPLAY course.courseId and ": " and course.name
//     IF the size of course.coursePrerequisites is greater than 0
//       DISPLAY "  Prerequisites: "
//       FOR EACH prereqId in course.coursePrerequisites
//         DISPLAY new line and prereqId
//       END FOR
//     END IF
//   END FUNCTION

//   FUNCTION SetBstSize(tree, newSize)

//     set tree.size to newSize

//   END FUNCTION

//   FUNCTION GetBstSize(tree)

//     RETURN tree.size

//   END FUNCTION    

//   FUNCTION LTrimString(s)
//     WHILE the first character of s is a space character
//       remove the first character from s
//     END WHILE
//     RETURN s
//   END FUNCTION

//   FUNCTION RTrimString(s)
//     WHILE the last character of s is a space character
//       remove the last character from s
//     END WHILE
//     RETURN s
//   END FUNCTION

//   FUNCTION TrimString(s)
//     set s to RTrimString(s)
//     set s to LTrimString(s)
//     RETURN s
//   END FUNCTION

//   FUNCTION AddNode(node, course)

//     IF course.courseId is less than node.course.courseId

//       IF node.left is null
//         CREATE a new NODE named newNode
//         set newNode.course to course
//         set newNode.left to null
//         set newNode.right to null
//         set node.left to newNode
//         CALL SetBstSize(courseCatalog, GetBstSize(courseCatalog) + 1)
//       ELSE
//         call AddNode(node.left, course)
//       END IF
//     ELSE

//       IF node.right is null
//         CREATE a new NODE named newNode
//         set newNode.course to course
//         set newNode.left to null
//         set newNode.right to null
//         set node.right to newNode
//         CALL SetBstSize(courseCatalog, GetBstSize(courseCatalog) + 1)
//       ELSE
//         call AddNode(node.right, course)
//       END IF

//     END IF

//   END FUNCTION

//   FUNCTION RemoveNode(node, courseId)

//     IF node is null
//       RETURN node
//     ELSE IF courseId is less than node.course.courseId
//       set node.left to RemoveNode(node.left, courseId)
//     END IF

//     IF courseId equals node.course.courseId

//       IF node.left is null AND node.right is null
//         delete node
//         CALL SetBstSize(courseCatalog, GetBstSize(courseCatalog) - 1)
//         RETURN null

//       ELSE IF node.left is not null AND node.right is null
//         set temp to node.left
//         delete node
//         CALL SetBstSize(courseCatalog, GetBstSize(courseCatalog) - 1)
//         RETURN temp

//       ELSE IF node.left is null AND node.right is not null
//         set temp to node.right
//         delete node
//         CALL SetBstSize(courseCatalog, GetBstSize(courseCatalog) - 1)
//         RETURN temp

//       ELSE
//         set temp to node.right
//         WHILE temp.left is not null
//           set temp to temp.left
//         END WHILE
//         set node.course to temp.course
//         set node.right to RemoveNode(node.right, temp.course.courseId)
//         RETURN node

//       END IF

//     ELSE IF courseId is greater than node.course.courseId
//       set node.right to RemoveNode(node.right, courseId)
//       RETURN node
//     END IF

//     RETURN node

//   END FUNCTION

//   FUNCTION InOrderPrint(node)

//     IF node is not null
//       call InOrderPrint(node.left)
//       CALL DisplayCourse(node.course)
//       call InOrderPrint(node.right)
//     END IF

//   END FUNCTION

//   FUNCTION PostOrderDelete(node)

//     IF node is not null
//       call PostOrderDelete(node.left)
//       call PostOrderDelete(node.right)
//       CALL RemoveNode(node, node.course.courseId)
//     END IF

//   END FUNCTION

//   FUNCTION PreOrderValidation(node)

//     IF node is not null
//       IF pendingPrerequisites is not empty
//         FOR EACH pendingPrerequisite in pendingPrerequisites
//           searchOutput is a COURSE
//           set searchOutput to output of Search(courseCatalog, pendingPrerequisite.missingPrereqId)
//           IF searchOutput is empty
//             DISPLAY "Validation Error: Course " and node.course.courseId and " has missing prerequisite " and pendingPrerequisite.missingPrereqId
//           ELSE
//             set courseToUpdate to pendingPrerequisite.courseId
//             add pendingPrerequisite.missingPrereqId to searchOutput.coursePrerequisites
//             Remove(courseCatalog, courseToUpdate)
//             Insert(courseCatalog, searchOutput)
//             delete pendingPrerequisite from pendingPrerequisites
//           END IF
//         END FOR
//       END IF
//       call PreOrderValidation(node.left)
//       call PreOrderValidation(node.right)
//     END IF

//   END FUNCTION

//   FUNCTION InOrderPrintAll(tree)

//     call InOrderPrint(tree.root)

//   END FUNCTION

//   FUNCTION PostOrderDeleteAll(tree)

//     call PostOrderDelete(tree.root)

//   END FUNCTION

//   FUNCTION PreOrderValidationAll(tree)

//     call PreOrderValidation(tree.root)

//   END FUNCTION

//   FUNCTION Insert(tree, course)

//     CREATE a new NODE named newNode
//     set newNode.course to course
//     set newNode.left to null
//     set newNode.right to null

//     IF tree.root is null
//       set tree.root to newNode
//       CALL SetBstSize(courseCatalog, GetBstSize(courseCatalog) + 1)
//       RETURN
//     END IF

//     call AddNode(tree.root, course)

//   END FUNCTION

//   FUNCTION Remove(tree, courseId)

//     set tree.root to RemoveNode(tree.root, courseId)

//     IF tree.root is null
//       RETURN
//     END IF

//   END FUNCTION

//   FUNCTION Search(tree, courseId)

//     set currentNode to tree.root

//     WHILE currentNode is not null

//       IF currentNode.course.courseId equals courseId
//         RETURN currentNode.course
//       ELSE IF courseId is less than currentNode.course.courseId
//         set currentNode to currentNode.left
//       ELSE
//         set currentNode to currentNode.right
//       END IF

//     END WHILE

//     CREATE an empty course named course
//     RETURN course

//   END FUNCTION

//   FUNCTION ReadFile(fileName, courseCatalog)
//     DISPLAY "Opening file " and fileName
//     open file with name fileName for reading
//     IF the file is not open
//       DISPLAY "Could not open file"
//       RETURN 1
//     END IF

//     DISPLAY "File open. Reading and storing data..."

//     SET rawInput to empty text

//     TRY
//       WHILE there is another line to read from the file
//         read the next line into rawInput

//         SET current to empty text
//         SET field to 0 
//         * field indicators explaination:
//         * 0 is for the courseId, 
//         * 1 is for the name, 
//         * 2+ are for prerequisites
//         *
//         CREATE a new COURSE named newCourse

//         FOR EACH character ch in rawInput
//           IF ch is a comma
//             set current to TrimString(current)
//             IF field equals 0
//               set newCourse.courseId to current
//             ELSE IF field equals 1
//               set newCourse.name to current
//             ELSE IF field is greater than 1
//               searchOutput is a COURSE
//               set searchOutput to output of Search(courseCatalog, current)
//               IF searchOutput is not empty
//                 add current to newCourse.coursePrerequisites
//               ELSE
//                 CREATE a new PENDING PREREQUISITE named pendingPrereq
//                 set pendingPrereq.courseId to newCourse.courseId
//                 set pendingPrereq.missingPrereqId to current
//                 add pendingPrereq to pendingPrerequisites
//               END IF
//             set current to empty text
//             increase field by 1
//           ELSE
//             add ch to the end of current
//           END IF
//         END FOR

//         IF the length of current is at least 1
//           set current to TrimString(current)
//           IF field equals 1
//             set newCourse.name to current
//           ELSE IF field is greater than 1
//             searchOutput is a COURSE
//             set searchOutput to output of Search(courseCatalog, current)
//             IF searchOutput is not empty
//               add current to newCourse.coursePrerequisites
//             ELSE
//               CREATE a new PENDING PREREQUISITE named pendingPrereq
//               set pendingPrereq.courseId to newCourse.courseId
//               set pendingPrereq.missingPrereqId to current
//               add pendingPrereq to pendingPrerequisites
//             END IF
//           END IF
//           set current to empty text
//           increase field by 1
//         END IF

//         Insert(courseCatalog, newCourse)
//       END WHILE

//       IF the file did not finish at the end marker
//         DISPLAY "Input failure before reaching end of file."
//       END IF

//       * final validation pass to ensure all prerequisites are met *
//       call PreOrderValidation(courseCatalog)
//       DISPLAY the size of courseCatalog and " courses were added successfully!"
//       DISPLAY "Closing file: " and fileName

//       close the file

//       DISPLAY "File: " and fileName
//       DISPLAY "Import complete."

//       RETURN 0
//     CATCH exception
//       DISPLAY "Error reading file ", csvPath
//     END TRY
//   END FUNCTION

//   FUNCTION MainMenu()
//     SET choice to 0

//     DISPLAY "Press Enter to open the menu..."
//     wait for the user to press Enter

//     DO
//       DISPLAY "MAIN MENU"
//       DISPLAY "'1' to read the file."
//       DISPLAY "'2' to print the entire course catalog."
//       DISPLAY "'3' to search for and print a specific course."
//       DISPLAY "'4' to search for and remove a specific course."
//       DISPLAY "'0' to exit."
//       DISPLAY "User choice: "
//       read choice
//       clear any leftover input from that number entry

//       IF choice equals 1
//         call ReadFile(INPUT_FILE, courseCatalog)

//       ELSE IF choice equals 2
//         IF the size of courseCatalog is less than 1
//           DISPLAY "No courses currently in the catalog"
//         ELSE
//           call InOrderPrint(courseCatalog)
//         END IF

//       ELSE IF choice equals 3
//         IF the size of courseCatalog is less than 1
//           DISPLAY "No courses currently in the catalog"
//         ELSE
//           SET searchTerm to empty text
//           DISPLAY "Search by course name or ID: "
//           read a full line into searchTerm
//           CREATE a new COURSE named searchResult
//           set searchResult to output of Search(courseCatalog, searchTerm)
//           IF searchResult.courseId is empty text
//             DISPLAY "No results found for: " and searchTerm
//           ELSE
//             CALL DisplayCourse(searchResult)
//           END IF
//         END IF
//       ELSE IF choice equals 4
//         IF the size of courseCatalog is less than 1
//           DISPLAY "No courses currently in the catalog"
//         ELSE
//           SET removeTerm to empty text
//           DISPLAY "Remove by course name or ID: "
//           read a full line into removeTerm
//           CREATE a new COURSE named removeResult
//           set removeResult to output of Search(courseCatalog, removeTerm)
//           IF removeResult.courseId is empty text
//             DISPLAY "No results found for: " and removeTerm
//           ELSE
//             CALL Remove(courseCatalog, removeResult.courseId)
//             DISPLAY "Course " and removeTerm and " removed."
//           END IF
//         END IF
//       ELSE IF choice equals 0
//         DISPLAY "exiting..."
//       ELSE
//         DISPLAY "Invalid entry!"
//       END IF

//     WHILE choice is not equal to 0

//   END FUNCTION

// MAIN PROGRAM
//   call MainMenu()
//   END PROGRAM
