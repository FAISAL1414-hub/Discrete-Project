#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    const int maxStudents = 50;                // Maximum number of unique students
    string studentList[maxStudents];
    int friendCount[maxStudents] = {0};
    int studentIndex = 0;

    ifstream inputFile("D:\\Friends Dataset.csv");
    if (!inputFile.is_open()) {
        cout << "Error opening file!" << endl;
        return 1;
    }

    string fileLine;
    // Skip the header line
    getline(inputFile, fileLine);

    // Process each line
    while (getline(inputFile, fileLine)) {
        string friendName = "";
        int linePos = 0; // Position to traverse the line

        // Skip the first column (student's name)
        while (linePos < fileLine.length() && fileLine[linePos] != ',') {
            linePos++;
        }
        linePos++; // Move past the comma

        // Extract each friend's name
        while (linePos <= fileLine.length()) {
            if (linePos == fileLine.length() || fileLine[linePos] == ',') {
                // Add or update friend count
                bool isFound = false;
                for (int i = 0; i < studentIndex; i++) {
                    if (studentList[i] == friendName) {
                        friendCount[i]++;
                        isFound = true;
                        break;
                    }
                }
                if (!isFound && studentIndex < maxStudents && !friendName.empty()) {
                    studentList[studentIndex] = friendName;
                    friendCount[studentIndex] = 1;
                    studentIndex++;
                }
                friendName = ""; // Reset for the next name
            } else {
                friendName += fileLine[linePos]; // Append character to friend's name
            }
            linePos++;
        }
    }

    inputFile.close();

    // Find the maximum friend count
    int maxFriendCount = 0;
    for (int i = 0; i < studentIndex; i++) {
        if (friendCount[i] > maxFriendCount) {
            maxFriendCount = friendCount[i];
        }
    }

    // Print the most popular students
    cout << "\t\t\t\tMost Popular Student(s):" << endl;
    for (int i = 0; i < studentIndex; i++) {
        if (friendCount[i] == maxFriendCount) {
            cout << studentList[i] << " is the most popular student in the class based on the data provided in the file with " 
                 << friendCount[i] << " friends." << endl;
        }
    }
    return 0;
}
