#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <climits> // For INT_MAX

using namespace std;

const int MAX_ENTRIES = 100;
const int INF = INT_MAX;

struct Node {
    int index;
    int hopCount;
};

struct CompareNodes {
    bool operator()(const Node& a, const Node& b) {
        return a.hopCount > b.hopCount;
    }
};

int main() {
    ifstream dataFile("D:\\Friends Dataset.csv");
    if (!dataFile.is_open()) {
        cout << "Error: Unable to open the file!" << endl;
        return 1;
    }

    string fileLine;
    getline(dataFile, fileLine); // Skip the header line if present

    vector<string> personNames;
    int connectionMatrix[MAX_ENTRIES][MAX_ENTRIES];

    // Initialize the connection matrix with INF
    for (int i = 0; i < MAX_ENTRIES; i++) {
        for (int j = 0; j < MAX_ENTRIES; j++) {
            connectionMatrix[i][j] = (i == j) ? 0 : INF;
        }
    }

    // Parse the CSV file
    while (getline(dataFile, fileLine)) {
        stringstream lineStream(fileLine);
        string currentPerson, friendEntry;
        getline(lineStream, currentPerson, ',');

        int currentIndex = -1;
        for (int i = 0; i < personNames.size(); i++) {
            if (personNames[i] == currentPerson) {
                currentIndex = i;
                break;
            }
        }
        if (currentIndex == -1) {
            currentIndex = personNames.size();
            personNames.push_back(currentPerson);
        }

        while (getline(lineStream, friendEntry, ',')) {
            if (!friendEntry.empty()) {
                int friendIndex = -1;
                for (int i = 0; i < personNames.size(); i++) {
                    if (personNames[i] == friendEntry) {
                        friendIndex = i;
                        break;
                    }
                }
                if (friendIndex == -1) {
                    friendIndex = personNames.size();
                    personNames.push_back(friendEntry);
                }

                // Update the connection matrix
                connectionMatrix[currentIndex][friendIndex] = 1;
                connectionMatrix[friendIndex][currentIndex] = 1;
            }
        }
    }

    dataFile.close();

    // Input student A's name
    string studentA;
    cout << "Enter the name of student A: ";
    getline(cin, studentA); // Use getline to allow spaces in names

    int studentAIndex = -1;
    for (int i = 0; i < personNames.size(); i++) {
        if (personNames[i] == studentA) {
            studentAIndex = i;
            break;
        }
    }

    if (studentAIndex == -1) {
        cout << "Student A not found!" << endl;
        return 1;
    }

    // Initialize hop counts
    vector<int> hopCounts(personNames.size(), INF);
    hopCounts[studentAIndex] = 0;

    // Priority queue for BFS
    priority_queue<Node, vector<Node>, CompareNodes> pq;
    Node startNode = {studentAIndex, 0};
    pq.push(startNode);

    while (!pq.empty()) {
        Node currentNode = pq.top();
        pq.pop();

        for (int i = 0; i < personNames.size(); i++) {
            if (connectionMatrix[currentNode.index][i] == 1 && hopCounts[i] > currentNode.hopCount + 1) {
                hopCounts[i] = currentNode.hopCount + 1;
                Node nextNode = {i, hopCounts[i]};
                pq.push(nextNode);
            }
        }
    }

    // Categorize students based on hop counts
    vector<string> oneHop, twoHop, threeHop;
    for (int i = 0; i < personNames.size(); i++) {
        if (hopCounts[i] == 1) {
            oneHop.push_back(personNames[i]);
        } else if (hopCounts[i] == 2) {
            twoHop.push_back(personNames[i]);
        } else if (hopCounts[i] == 3) {
            threeHop.push_back(personNames[i]);
        }
    }

    // Output results
    cout << "Students 1-hop away from " << studentA << ":" << endl;
    if (oneHop.empty()) {
        cout << "None" << endl;
    } else {
        for (int i = 0; i < oneHop.size(); i++) {
            cout << oneHop[i] << endl;
        }
    }

    cout << "Students 2-hops away from " << studentA << ":" << endl;
    if (twoHop.empty()) {
        cout << "None" << endl;
    } else {
        for (int i = 0; i < twoHop.size(); i++) {
            cout << twoHop[i] << endl;
        }
    }

    cout << "Students 3-hops away from " << studentA << ":" << endl;
    if (threeHop.empty()) {
        cout << "None" << endl;
    } else {
        for (int i = 0; i < threeHop.size(); i++) {
            cout << threeHop[i] << endl;
        }
    }
    return 0;
}
