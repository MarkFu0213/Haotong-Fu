#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "avlbst.h" // Included my AVLTree implementation here
#include <set>

// This file implements AVL class to build an Exam Scheduler

using namespace std;
// a student struct to hold everything about this student, including a vector to store all its classes
struct Student {
    std::string name;
    std::vector<int> classes;
};

//function prototype
//this function adds key,value pair as classcode,slot number pair into the AVL tree container
bool scheduleExams(int Index, AVLTree<int, int>& classtree, const std::vector<Student>& students, int total_timeslot, const std::vector<int>& classCodes);

// a function checks whether a slot is valid: this student does not have another exam at the same time
bool valid_slot(int slot, int which_class, const AVLTree<int, int>& classtree, const std::vector<Student>& students);

// a helper function checks whether an element is inside a vector
bool isElementInVector(const std::vector<int>& vec, const int& element);


int main(int argc, char* argv[]) {
    //check if the command line argument is correct
    if (argc != 2) {
        std::cout << "Incorrect number of command-line arguments" << std::endl;
        return 1;
    }
    //check whether the file can be opened or not
    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cout << "Error opening file: " << argv[1] << std::endl;
        return 1;
    }
    //all the numbers are passed in
    int num_of_exams, num_of_students, num_of_timeslots;
    inputFile >> num_of_exams >> num_of_students >> num_of_timeslots;

    //student vector to store all student struct
    std::vector<Student> students;
    //storing all classcodes, no duplicate
    std::vector<int> uniqueclasscode;
    //skipping first line
    std::string skipped_line;
    std::getline(inputFile, skipped_line);

    //pass in each name and class code to each student struct
    std::string line;
    while (std::getline(inputFile, line)) {
        stringstream ss(line);
        Student student;
        ss >> student.name;
        int class_code;
        while (ss >> class_code) {
            student.classes.push_back(class_code);
            //check for duplicacity, if not, add the classcode into the vector
            if(!(isElementInVector(uniqueclasscode, class_code)))
            {
                uniqueclasscode.push_back(class_code);
            }
        }
        //push back the student struct into the vector
        students.push_back(student);
    }
    

    AVLTree<int, int> classtree;
    if (scheduleExams(0, classtree, students, num_of_timeslots, uniqueclasscode)) {
        // Output the schedule, iterate through all the class code
        for (int classCode : uniqueclasscode) {
            try {
                //find the classcode in the class avl tree, and output its key and value
                auto it = classtree.find(classCode);
                if (it != classtree.end()) {
                    std::cout << classCode << " " << it->second << std::endl;
                }
            } catch (const std::exception& e) {
                std::cout << "not found" << std::endl;
            }
        }
    } 
    //if did not find, output no solution
    else {
        std::cout << "No valid solution." << std::endl;
    }

    return 0;
}

//this function checks if an element is in an vector or not
bool isElementInVector(const std::vector<int>& vec, const int& element) {
    return std::find(vec.begin(), vec.end(), element) != vec.end();
}

bool scheduleExams(int Index, AVLTree<int, int>& classtree, const std::vector<Student>& students, int totalslots, const std::vector<int>& classCodes) {
   
    // base case, if the index is equal or larger than the size of classcode vector, then we have iterated all our codes
    if (Index >= static_cast<int>(classCodes.size())) {
        return true; 
    }
    
    int currentClassCode = classCodes[Index];

    for (int slot = 1; slot <= totalslots; ++slot) {
        //if this slot is valid, then we try to find the code in our tree
        if (valid_slot(slot, currentClassCode, classtree, students)) {
            try {
                // Find the iterator for the current class code
                auto it = classtree.find(currentClassCode);
                
                // Check if the class already exists in the tree
                if (it != classtree.end()) {
                    // If it exists, remove the existing node
                    classtree.remove(it->first); 
                }
            
                // then we insert or re-insert this key value pair into the tree
                classtree.insert(std::make_pair(currentClassCode, slot));

            } 
            //if the slot is unavaliable then we go to the next iteration
            catch (const std::exception& err) {
                std::cout << "Exception: " << err.what() << std::endl;
                // Skip this slot and continue with the next
                continue; 
            }
            //this is wheere the recursive call is, increment the class index one by one, if found return true
            if (scheduleExams(Index + 1, classtree, students, totalslots, classCodes)) {
                return true; 
            }
            //if not true, then we remove the current class code and go back
            classtree.remove(currentClassCode); 
        }
    }
    return false;
}

//this function checks if the slot if valid or not
bool valid_slot(int slot, int currentClassCode, const AVLTree<int, int>& classtree, const std::vector<Student>& students) {
    //iterate through all the student struct
    for (const auto& student : students) {
        //if this student takes this exam, then we perform the check
        if(isElementInVector(student.classes, currentClassCode))
        {
            //It skips over the currentClassCode itself since we are interested in other classes that might conflict
            for (int eachclass : student.classes) {
                if (eachclass == currentClassCode) {
                    continue;
                }
                // Check if the student is enrolled in another class that's scheduled in the same slot
                auto it = classtree.find(eachclass);
                if (it != classtree.end() && it->second == slot) {
                    // if yes, then we cannot use this slot
                    return false;
                }
            }
        }

    }
    //no conflict, this slot can be used
    return true; 
}

