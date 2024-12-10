#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

const int MAX_ENTITIES = 100; // Maximum number of individuals

int main() {
    string entityNames[MAX_ENTITIES]; // Array to store individual names
    bool relationshipMatrix[MAX_ENTITIES][MAX_ENTITIES] = {false}; // 2D array for relationships
    int entityCount = 0;

    // Open the CSV file
    ifstream dataFile("D:\\Friends Dataset.csv");
    if (!dataFile.is_open()) {
        cout << "Error: Unable to open the file!" << endl;
        return 1;
    }

    // Read the file
    string dataLine;
    getline(dataFile, dataLine); // Skip the header line
    while (getline(dataFile, dataLine)) {
        stringstream lineStream(dataLine);
        string currentEntity, associatedEntity;
        getline(lineStream, currentEntity, ','); // First column is the entity's name

        // Add the current entity to the list
        int currentEntityIndex = -1;
        for (int i = 0; i < entityCount; i++) {
            if (entityNames[i] == currentEntity) {
                currentEntityIndex = i;
                break;
            }
        }
        if (currentEntityIndex == -1) {
            currentEntityIndex = entityCount;
            entityNames[entityCount++] = currentEntity;
        }

        // Read associated entities
        while (getline(lineStream, associatedEntity, ',')) {
            if (!associatedEntity.empty()) {
                int associatedEntityIndex = -1;
                for (int i = 0; i < entityCount; i++) {
                    if (entityNames[i] == associatedEntity) {
                        associatedEntityIndex = i;
                        break;
                    }
                }
                if (associatedEntityIndex == -1) {
                    associatedEntityIndex = entityCount;
                    entityNames[entityCount++] = associatedEntity;
                }

                // Mark the relationship
                relationshipMatrix[currentEntityIndex][associatedEntityIndex] = true;
            }
        }
    }
    dataFile.close();

    // Find and display asymmetric relationships
    cout << "Asymmetric Relationships:" << endl;
    for (int i = 0; i < entityCount; i++) {
        for (int j = 0; j < entityCount; j++) {
            if (relationshipMatrix[i][j] && !relationshipMatrix[j][i]) {
                cout << entityNames[i] << " considers " << entityNames[j] 
                     << "  as a friend, but the feeling is not mutual." << endl;
            }
        }
    }
    return 0;
}
