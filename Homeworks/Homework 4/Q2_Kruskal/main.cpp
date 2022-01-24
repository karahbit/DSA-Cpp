// Name: George Koubbe
// Date of Submission: 4/24/2018
// Course: 16:332:573, Data Structures and Algorithms
// Homework 4. Problem Q2. Kruskal's algorithm

#include <iostream>

using std::cout;
using std::endl;
using std::invalid_argument;

#include <list>

using std::list;

#include <string>

using std::string;
using std::stoi;
using std::stod;

#include <fstream>

using std::ifstream;

#include <sstream>

using std::stringstream;

#include <utility>

using std::pair;
using std::make_pair;

#include <queue>

using std::priority_queue;

#include <vector>

using std::vector;

#include <functional>

using std::greater;

#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::duration;

// Creating shortcut for an integer pair
typedef  pair<int, int> iPair;

// Structure to represent a graph
struct Graph
{
    int V, E;
    vector< pair<double, iPair> > edges;

    // Constructor
    Graph(int V, int E)
    {
        this->V = V;
        this->E = E;
    }

    Graph(string fileName);

    // Utility function to add an edge
    void addEdge(int u, int v, double w)
    {
        edges.push_back({w, {u, v}});
    }

    // Function to find MST using Kruskal's
    // MST algorithm
    double kruskalMST();
};

// To represent Disjoint Sets
struct DisjointSets
{
    int *parent, *rnk;
    int n;

    // Constructor.
    DisjointSets(int n)
    {
        // Allocate memory
        this->n = n;
        parent = new int[n+1];
        rnk = new int[n+1];

        // Initially, all vertices are in
        // different sets and have rank 0.
        for (int i = 0; i <= n; i++)
        {
            rnk[i] = 0;

            //every element is parent of itself
            parent[i] = i;
        }
    }

    // Find the parent of a node 'u'
    // Path Compression
    int find(int u)
    {
        /* Make the parent of the nodes in the path
           from u--> parent[u] point to parent[u] */
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    // Union by rank
    void merge(int x, int y)
    {
        x = find(x), y = find(y);

        /* Make tree with smaller height
           a subtree of the other tree  */
        if (rnk[x] > rnk[y])
            parent[y] = x;
        else // If rnk[x] <= rnk[y]
            parent[x] = y;

        if (rnk[x] == rnk[y])
            rnk[y]++;
    }
};

// Initializes a graph from the specified input stream.
Graph::Graph(string fileName)
{
    ifstream inFile(fileName);
    string line;                    // Temp variable to store each file's line
    string::size_type sz;           // Pointer needed by function stoi
    string element;
    int v = -1, w = -1;
    double wt = -1.0;

    // check that the file was opened correctly
    if (!inFile) {
        cout << "Unable to open file" << endl;
        exit(1); // terminate with error
    }

    // read No. of Vertices
    getline(inFile, line);
    stringstream ssin(line);
    if (ssin.good()){
        ssin >> element;
        V = stoi (element, &sz);
    }

    if (V<0) throw invalid_argument("Number of vertices must be nonnegative");

    // read No. of Edges
    getline(inFile, line);
    stringstream ssin2(line);
    if (ssin2.good()){
        ssin2 >> element;
        E = stoi (element, &sz);
    }

    if (E < 0) throw invalid_argument("number of edges in a Graph must be nonnegative");

    // read rest of file
    while(getline(inFile, line)) {
        stringstream ssin(line);
        if (ssin.good()){
            ssin >> element;
            v = stoi (element, &sz);
        }
        if (ssin.good()){
            ssin >> element;
            w = stoi (element, &sz);
        }
        if (ssin.good()){
            ssin >> element;
            wt = stod (element, &sz);
        }
        addEdge(v, w, wt);
    }

    inFile.close();

}

 /* Functions returns weight of the MST*/

double Graph::kruskalMST()
{
    double mst_wt = 0; // Initialize result

    // Sort edges in increasing order on basis of cost
    sort(edges.begin(), edges.end());

    // Create disjoint sets
    DisjointSets ds(V);

    // Iterate through all sorted edges
    vector< pair<double, iPair> >::iterator it;
    for (it=edges.begin(); it!=edges.end(); it++)
    {
        int u = it->second.first;
        int v = it->second.second;

        int set_u = ds.find(u);
        int set_v = ds.find(v);

        // Check if the selected edge is creating
        // a cycle or not (Cycle is created if u
        // and v belong to same set)
        if (set_u != set_v)
        {
            // Current edge will be in the MST
            // so print it
            cout << u << " - " << v << endl;

            // Update MST weight
            mst_wt += it->first;

            // Merge two sets
            ds.merge(set_u, set_v);
        }
    }

    return mst_wt;
}

// Driver program to test above functions
int main()
{

    // create the graph given in above fugure
    Graph g("mediumEWG.txt");

    // Record start time
    auto start = high_resolution_clock::now();

    cout << "Edges of MST are \n";
    double mst_wt = g.kruskalMST();

    //cout << "\nWeight of MST is " << mst_wt << endl;

    // Record end time
    auto finish = high_resolution_clock::now();
    // Run time in seconds for the total algorithm
    duration<double> elapsed = finish - start;
    cout << "\nElapsed time: " << elapsed.count() << " s\n";


    return 0;
}
