// Name: George Koubbe
// Date of Submission: 4/24/2018
// Course: 16:332:573, Data Structures and Algorithms
// Homework 4. Problem Q2. Prim's algorithm

// STL implementation of Prim's algorithm for MST
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

# define INF 0x3f3f3f3f

// iPair ==>  Integer Pair
typedef pair<int, double> iPair;

// This class represents a directed graph using
// adjacency list representation
class Graph
{
    int V;    // No. of vertices
    int E;                    // No. of Edges

    // In a weighted graph, we need to store vertex
    // and weight pair for every edge
    list< pair<int, double> > *adj;

public:
    Graph(int V);  // Constructor
    Graph(string fileName);

    // function to add an edge to graph
    void addEdge(int u, int v, double w);

    // Print MST using Prim's algorithm
    void primMST();
};

// Allocates memory for adjacency list
Graph::Graph(int V)
{
    this->V = V;
    E = 0;
    adj = new list<iPair> [V];
}

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

    if (V < 0) throw invalid_argument("Number of vertices must be nonnegative");
    adj = new list<pair<int,double>>[V];

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

void Graph::addEdge(int u, int v, double w)
{
    E++;
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
}

// Prints shortest paths from src to all other vertices
void Graph::primMST()
{
    // Create a priority queue to store vertices that
    // are being preinMST. This is weird syntax in C++.
    // Refer below link for details of this syntax
    // http://geeksquiz.com/implement-min-heap-using-stl/
    priority_queue< iPair, vector <iPair> , greater<iPair> > pq;

    int src = 0; // Taking vertex 0 as source

    // Create a vector for keys and initialize all
    // keys as infinite (INF)
    vector<int> key(V, INF);

    // To store parent array which in turn store MST
    vector<int> parent(V, -1);

    // To keep track of vertices included in MST
    vector<bool> inMST(V, false);

    // Insert source itself in priority queue and initialize
    // its key as 0.
    pq.push(make_pair(0, src));
    key[src] = 0;

    /* Looping till priority queue becomes empty */
    while (!pq.empty())
    {
        // The first vertex in pair is the minimum key
        // vertex, extract it from priority queue.
        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted key (key must be first item
        // in pair)
        int u = pq.top().second;
        pq.pop();

        inMST[u] = true;  // Include vertex in MST

        // 'i' is used to get all adjacent vertices of a vertex
        list< pair<int, double> >::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = (*i).first;
            int weight = (*i).second;

            //  If v is not in MST and weight of (u,v) is smaller
            // than current key of v
            if (inMST[v] == false && key[v] > weight)
            {
                // Updating key of v
                key[v] = weight;
                pq.push(make_pair(key[v], v));
                parent[v] = u;
            }
        }
    }

    // Print edges of MST using parent array
    for (int i = 1; i < V; ++i)
        printf("%d - %d\n", parent[i], i);
}

// Driver program to test methods of graph class
int main()
{

    // create the graph given in above fugure
    Graph g("mediumEWG.txt");

    // Record start time
    auto start = high_resolution_clock::now();

    g.primMST();

    // Record end time
    auto finish = high_resolution_clock::now();
    // Run time in seconds for the total algorithm
    duration<double> elapsed = finish - start;
    cout << "\nElapsed time: " << elapsed.count() << " s\n";


    return 0;
}
