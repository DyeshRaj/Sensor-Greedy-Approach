#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>

using namespace std;

// Plant graph represented as adjacency list
unordered_map<string, vector<string>> plant;
vector<string> allNodes;

// Add a bidirectional connection (pipeline) between two locations
void addEdge(string a, string b) {
    plant[a].push_back(b);
    plant[b].push_back(a);
}

// Add a node (location) to the plant
void addNode(string name) {
    if (plant.find(name) == plant.end()) {
        plant[name] = {};
        allNodes.push_back(name);
    }
}

//Greedy Algorithm for sensor placement
vector<string> greedySensorPlacement() {
    unordered_set<string> uncovered(allNodes.begin(), allNodes.end());
    vector<string> sensors;
    int step = 0;

    cout << "\n========================================\n";
    cout << "   RUNNING GREEDY ALGORITHM\n";
    cout << "========================================\n\n";

    while (!uncovered.empty()) {
        string bestNode = "";
        unordered_set<string> bestCoverage;

        // Find node that covers most uncovered neighbors
        for (const string& node : allNodes) {
            unordered_set<string> coverage;
            coverage.insert(node); // node covers itself
            for (const string& neighbor : plant[node]) {
                coverage.insert(neighbor);
            }

            // Find overlap with uncovered
            unordered_set<string> coveredNow;
            for (const string& n : coverage) {
                if (uncovered.count(n)) {
                    coveredNow.insert(n);
                }
            }

            if (coveredNow.size() > bestCoverage.size()) {
                bestNode = node;
                bestCoverage = coveredNow;
            }
        }

        // Place sensor at best node
        sensors.push_back(bestNode);
        for (const string& n : bestCoverage) {
            uncovered.erase(n);
        }

        step++;
        cout << "Step " << step << ":\n";
        cout << "  Best Node   -> \"" << bestNode << "\"";
        cout << " (covers " << bestCoverage.size() << " uncovered nodes)\n";

        cout << "  Covered     -> { ";
        for (const string& n : bestCoverage) cout << n << " ";
        cout << "}\n";

        cout << "  Remaining   -> { ";
        if (uncovered.empty()) cout << "none";
        else for (const string& n : uncovered) cout << n << " ";
        cout << "}\n\n";
    }

    return sensors;
}

//Display the plant map (adjacency list)
void displayPlantMap() {
    cout << "\n========================================\n";
    cout << "   PLANT MAP (Adjacency List)\n";
    cout << "========================================\n";
    for (const string& node : allNodes) {
        cout << "  " << node << " --> ";
        for (int i = 0; i < plant[node].size(); i++) {
            cout << plant[node][i];
            if (i != plant[node].size() - 1) cout << ", ";
        }
        cout << "\n";
    }
    cout << "\nTotal Locations : " << allNodes.size() << "\n";
}

//Taking input from user to build the plant graph
void takeInput() {
    int n, e;

    cout << "========================================\n";
    cout << "   GREEDY OPTIMAL SENSOR PLACEMENT\n";
    cout << "   Instrumentation & Control Engineering\n";
    cout << "========================================\n\n";

    cout << "Enter number of locations (nodes): ";
    cin >> n;

    cout << "Enter location names:\n";
    for (int i = 0; i < n; i++) {
        string name;
        cout << "  Location " << i + 1 << ": ";
        cin >> name;
        addNode(name);
    }

    cout << "\nEnter number of connections (edges/pipelines): ";
    cin >> e;

    cout << "Enter connections (format: LocationA LocationB):\n";
    for (int i = 0; i < e; i++) {
        string a, b;
        cout << "  Connection " << i + 1 << ": ";
        cin >> a >> b;
        addEdge(a, b);
    }
}


int main() {

    //Take input to build the plant graph
    takeInput();

    // Display the plant map
    displayPlantMap();

    // Run greedy algorithm
    vector<string> sensors = greedySensorPlacement();

    // Final result
    cout << "========================================\n";
    cout << "   FINAL RESULT\n";
    cout << "========================================\n";
    cout << "Total Locations : " << allNodes.size() << "\n";
    cout << "Sensors Placed  : " << sensors.size() << "\n";
    cout << "Sensor Locations: ";
    for (int i = 0; i < sensors.size(); i++) {
        cout << sensors[i];
        if (i != sensors.size() - 1) cout << ", ";
    }
    cout << "\nCoverage        : 100%\n";
    cout << "========================================\n\n";

    // Cost analysis
    int costPerSensor = 50000;
    cout << "COST ANALYSIS (at Rs." << costPerSensor << " per sensor):\n";
    cout << "  Without Greedy : Rs." << allNodes.size() * costPerSensor << " (" << allNodes.size() << " sensors)\n";
    cout << "  With Greedy    : Rs." << sensors.size() * costPerSensor << " (" << sensors.size() << " sensors)\n";
    cout << "  Money Saved    : Rs." << (allNodes.size() - sensors.size()) * costPerSensor << "\n";
    cout << "========================================\n";

    return 0;
}