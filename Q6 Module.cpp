#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm> // For find()

using namespace std;

const int MAX_ENTRIES = 100;

int main() {
    ifstream dataFile("D:\\Friends Dataset.csv");
    if (!dataFile.is_open()) {
        cout << "Error: Unable to open the file!" << endl;
        return 1;
    }

    string fileLine;
    getline(dataFile, fileLine); // Skip the header line if present

    vector<string> personNames;
    map<string, vector<string> > friendships; // To store the adjacency list

    // Parse the CSV file
    while (getline(dataFile, fileLine)) {
        stringstream lineStream(fileLine);
        string currentPerson;
        getline(lineStream, currentPerson, ',');

        string friendEntry;
        while (getline(lineStream, friendEntry, ',')) {
            if (!friendEntry.empty()) {
                friendships[currentPerson].push_back(friendEntry);
                friendships[friendEntry].push_back(currentPerson); // Bidirectional
            }
        }
    }

    dataFile.close();

    // Create the multiplicity graph
    map<pair<string, string>, int> weightedEdges;

    map<string, vector<string> >::iterator it1, it2;
    for (it1 = friendships.begin(); it1 != friendships.end(); ++it1) {
        for (it2 = friendships.begin(); it2 != friendships.end(); ++it2) {
            if (it1->first < it2->first) {
                vector<string> commonFriends;
                vector<string>& list1 = it1->second;
                vector<string>& list2 = it2->second;

                // Find common friends excluding direct connection
                for (size_t i = 0; i < list1.size(); ++i) {
                    const string& friend1 = list1[i];
                    if (friend1 != it2->first && 
                        find(list2.begin(), list2.end(), friend1) != list2.end()) {
                        commonFriends.push_back(friend1);
                    }
                }

                if (!commonFriends.empty()) {
                    weightedEdges[make_pair(it1->first, it2->first)] = commonFriends.size();
                }
            }
        }
    }

    // Input threshold a
    int alpha;
    cout << "Enter the threshold (a) for potential friendships: ";
    cin >> alpha;

    // Find potential friendships
    cout << "Potential Friendships (Not directly connected, Weight >= a):" << endl;
    map<pair<string, string>, int>::iterator edgeIt;
    for (edgeIt = weightedEdges.begin(); edgeIt != weightedEdges.end(); ++edgeIt) {
        string studentA = edgeIt->first.first;
        string studentB = edgeIt->first.second;
        int weight = edgeIt->second;

        // Check if they are not directly connected and weight >= a
        if (weight >= alpha &&
            find(friendships[studentA].begin(), friendships[studentA].end(), studentB) == friendships[studentA].end()) {
            cout << studentA << " - " << studentB << " : " << weight << " common friends" << endl;
        }
    }

    return 0;
}

