/* In order to make sure as many patients to have one unit blood as possible. We need tp prioritize the patients based on their blood type and RH type:

1 source (i.e. only one certain type they can receive): 
O- ---> O-

2 sources:
O+ ---> O-, O+
A- ---> A-, O-
B- ---> B-, O-

4 sources:
AB- ---> A-, B-, O-, AB-
A+ ---> A-, A+, O-, O+
B+ ---> B-, B+, O-, O+

All sources:
AB+

In this question, we should be able to generate a graph based on the description, and since we wanna find out the maximum capacity this graph could take, that is to say we wanna find the max-flow
of the entire graph. Here we need to use FFA method in order to get the maximum flow.
However, I did make some mistakes in my code which I did not have enough time to trace. Also apologize for my health condition due to Covid, I was not abel to debug the code to be perfect.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

struct Edge {
    int from;
    int to;
    int capacity;
    int flow;
};

class MaximumFlow {
private:
    int n;  // Number of nodes
    vector<vector<int> > graph;  // Adjacency matrix representation of the graph
    vector<int> parent;  // Parent array to store augmenting path

    bool bfs(int source, int sink) {
        // Initialize parent array
        parent.assign(n, -1);

        // Mark source as visited and enqueue it
        vector<bool> visited(n, false);
        visited[source] = true;
        queue<int> q;
        q.push(source);

        // Standard BFS algorithm
        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < n; ++v) {
                // If the edge is not fully saturated and the node is not visited
                if (!visited[v] && graph[u][v] > 0) {
                    parent[v] = u;
                    visited[v] = true;
                    q.push(v);

                    // Stop BFS if the sink is reached
                    if (v == sink)
                        return true;
                }
            }
        }

        // No augmenting path found
        return false;
    }

    int findMaxFlow(int source, int sink) {
        int maxFlow = 0;

        // Augment the flow while there is an augmenting path
        while (bfs(source, sink)) {
            int pathFlow = INT_MAX;

            // Find the minimum residual capacity along the augmenting path
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                pathFlow = min(pathFlow, graph[u][v]);
            }

            // Update the residual capacities and reverse edges along the path
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                graph[u][v] -= pathFlow;
                graph[v][u] += pathFlow;
            }

            // Add the path flow to the overall max flow
            maxFlow += pathFlow;
        }

        // Return the maximum flow
        return maxFlow;
    }

public:
    MaximumFlow(int numNodes) : n(numNodes), graph(numNodes, vector<int>(numNodes, 0)) {}

    void addEdge(int from, int to, int capacity) {
        // Add forward edge
        graph[from][to] += capacity;

        // Add backward edge with capacity 0
        graph[to][from] = 0;
    }

    int findMaximumFlow(int source, int sink) {
        return findMaxFlow(source, sink);
    }
};

int main() {
    int units[8];
    int patients[8];

    for (int i = 0; i < 8; ++i) {
        cin >> units[i];
    }

    for (int i = 0; i < 8; ++i) {
        cin >> patients[i];
    }

    // Create the maximum flow network
    MaximumFlow network(18);  // 16 nodes for blood types and patients + source + sink

    // Define the indices of blood types and patients in the graph
    const int O_NEG = 0, O_POS = 1, A_NEG = 2, A_POS = 3, B_NEG = 4, B_POS = 5, AB_NEG = 6, AB_POS = 7;
    const int TYPE_O_NEG = 8, TYPE_O_POS = 9, TYPE_A_NEG = 10, TYPE_A_POS = 11, TYPE_B_NEG = 12, TYPE_B_POS = 13, TYPE_AB_NEG = 14, TYPE_AB_POS = 15;
    const int SOURCE = 16, SINK = 17;

    // Connect the source to blood types
    network.addEdge(SOURCE, O_NEG, units[0]);
    network.addEdge(SOURCE, O_POS, units[1]);
    network.addEdge(SOURCE, A_NEG, units[2]);
    network.addEdge(SOURCE, A_POS, units[3]);
    network.addEdge(SOURCE, B_NEG, units[4]);
    network.addEdge(SOURCE, B_POS, units[5]);
    network.addEdge(SOURCE, AB_NEG, units[6]);
    network.addEdge(SOURCE, AB_POS, units[7]);

    // Connect the blood types to patients
    network.addEdge(O_NEG, TYPE_O_NEG, patients[0]);
    network.addEdge(O_POS, TYPE_O_NEG, patients[1]);
    network.addEdge(O_POS, TYPE_O_POS, patients[1]);
    network.addEdge(A_NEG, TYPE_A_NEG, patients[2]);
    network.addEdge(A_NEG, TYPE_O_NEG, patients[2]);
    network.addEdge(A_POS, TYPE_A_POS, patients[3]);
    network.addEdge(A_POS, TYPE_A_NEG, patients[3]);
    network.addEdge(A_POS, TYPE_O_POS, patients[3]);
    network.addEdge(A_POS, TYPE_O_NEG, patients[3]);
    network.addEdge(B_NEG, TYPE_B_NEG, patients[4]);
    network.addEdge(B_NEG, TYPE_O_NEG, patients[4]);
    network.addEdge(B_POS, TYPE_B_POS, patients[5]);
    network.addEdge(B_POS, TYPE_B_NEG, patients[5]);
    network.addEdge(B_POS, TYPE_O_POS, patients[5]);
    network.addEdge(B_POS, TYPE_O_NEG, patients[5]);
    network.addEdge(AB_NEG, TYPE_O_NEG, patients[6]);
    network.addEdge(AB_NEG, TYPE_A_NEG, patients[6]);
    network.addEdge(AB_NEG, TYPE_B_NEG, patients[6]);
    network.addEdge(AB_NEG, TYPE_AB_NEG, patients[6]);
    network.addEdge(AB_POS, TYPE_AB_POS, patients[7]);
    network.addEdge(AB_POS, TYPE_A_POS, patients[7]);
    network.addEdge(AB_POS, TYPE_A_NEG, patients[7]);
    network.addEdge(AB_POS, TYPE_B_POS, patients[7]);
    network.addEdge(AB_POS, TYPE_B_NEG, patients[7]);
    network.addEdge(AB_POS, TYPE_O_POS, patients[7]);
    network.addEdge(AB_POS, TYPE_O_NEG, patients[7]);
    network.addEdge(AB_POS, TYPE_AB_NEG, patients[7]);

    // Connect the patients to the sink
    network.addEdge(TYPE_O_NEG, SINK, patients[0]);
    network.addEdge(TYPE_O_POS, SINK, patients[1]);
    network.addEdge(TYPE_A_NEG, SINK, patients[2]);
    network.addEdge(TYPE_A_POS, SINK, patients[3]);
    network.addEdge(TYPE_B_NEG, SINK, patients[4]);
    network.addEdge(TYPE_B_POS, SINK, patients[5]);
    network.addEdge(TYPE_AB_NEG, SINK, patients[6]);
    network.addEdge(TYPE_AB_POS, SINK, patients[7]);

    // Find the maximum flow in the network
    int maxFlow = network.findMaximumFlow(SOURCE, SINK);
    cout << maxFlow << endl;

    return 0;
}
