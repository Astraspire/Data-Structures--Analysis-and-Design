# CS300 Portfolio Item Reflection - Danny Fetter

In my CS300 course: Data Structures and Analysis, I built a program to organize a course catalog with efficient algorithmic complexity and strong input validation. It reads course data from a file, parsed it into a structure, validated prerequisites, and allowed users to print a specific course or all courses alphabetically. I chose a Binary Search Tree to store courses by courseId, along with courseName and prerequisites, so searching and printing were smooth and efficient. I also added a feature to remove courses, ensuring the program handled all required tasks effectively.

My process began with parsing and validation, creating helpers like an isInteger function and trimming whitespace. I split logic into small, descriptive functions for loading files, inserting nodes, validating prerequisites, and traversing the tree. Feedback from my instructor led me to redesign prerequisite validation using separate pending and valid lists, which clarified the flow. Writing detailed pseudocode upfront helped me visualize control flow and adapt it into C++. Experimenting with AI tools outside of the project introduced me to how quickly detailed pseudocode can be translated into other languages, with line-by-line suggestions. I tested GitHub Copilot to adapt my pseudocode into Python as a test, and it worked exceptionally well so long as I reviewed each function as I added suggestions. I was fascinated by the speed of which I was able to translate my work, and it reinforced the value of well-written pseudocode. This project reinforced the value of clear, modular functions, and planning, leaving me with a stronger sense of how to write maintainable, readable, and adaptable code.

Overall, this project was a valuable learning experience that enhanced my skills in data structures, algorithm design, and code organization. It also highlighted the importance of adaptability and continuous learning in programming, as I explored new tools and techniques to improve my workflow. I am proud of the final product and the knowledge I gained throughout the process.

# Included Files:

- README.md

*Program Folder:*

- "CS 300 ABCU_Advising_Program_Input.csv" (input file containing course data)
- "ProjectTwo.cpp" (source code of the program)
- "ProjectTwo.exe" (runnable executable file of the program)

*Runtime Analysis and Evaluation folder:*

- RuntimeAnalysis_DannyFetter.docx (contains runtime analysis and evaluation of the program)

*Pseudocode Prototypes folder:*

- "BinarySearchTree_ABCU.txt" (pseudocode for the binary search tree implementation)
- "HashTable_ABCU.txt" (pseudocode for the hash table implementation - not used in final program)
- "Vector_ABCU.txt" (pseudocode for the vector implementation - not used in final program)
