// Name: George Koubbe
// Date of Submission: 4/24/2018
// Course: 16:332:573, Data Structures and Algorithms
// Homework 4. Problem Q1. Graphs

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


// Class for an undirected graph
class Graph
{

    int V;                    // No. of vertices
    int E;                    // No. of Edges
    // Pointer to an array containing adjacency lists
    list<pair<int,double>> *adj;
    bool isCyclicUtil(int v, bool visited[], int parent);

public:
    Graph(int V);                   // Constructor
    Graph(string fileName);
    int getV() const;
    int getE() const;
    void validateVertex(int v);
    void addEdge(int v, int w, double wt);     // to add an edge to graph
    list<pair<int,double>> getAdj(int v);
    int degree(int v);
    bool isCyclic();                // returns true if there is a cycle
    void printGraph() const;        // Print the graph
};

Graph::Graph(int V)
{
    if (V<0) throw invalid_argument("Number of vertices must be nonnegative");
    this->V = V;
    E = 0;
    adj = new list<pair<int,double>>[V];
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

int Graph::getV() const
{
    return V;
}

int Graph::getE() const
{
    return E;
}

// throw an IllegalArgumentException unless {@code 0 <= v < V}
void Graph::validateVertex(int v) {
    if (v < 0 || v >= V) {
        cout << "vertex " << v << " is not between 0 and " << V-1;
        throw invalid_argument(" Error!");
    }
}

void Graph::addEdge(int v, int w, double wt)
{
    validateVertex(v);
    validateVertex(w);
    E++;
    adj[v].push_back(make_pair(w, wt)); // Add w to v’s list.
    adj[w].push_back(make_pair(v, wt)); // Add v to w’s list.
}

// Returns the vertices adjacent to vertex v.
list<pair<int,double>> Graph::getAdj(int v)
{
    validateVertex(v);
    return adj[v];
}

// Returns the degree of vertex {@code v}.
int Graph::degree(int v) {
    validateVertex(v);
    return static_cast<int>(adj[v].size());
}

// A recursive function that uses visited[] and parent to detect
// cycle in subgraph reachable from vertex v.
bool Graph::isCyclicUtil(int v, bool visited[], int parent)
{
    // Mark the current node as visited
    visited[v] = true;

    // Recur for all the vertices adjacent to this vertex
    list<pair<int,double>>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
    {
        // If an adjacent is not visited, then recur for that adjacent
        if (!visited[i->first])
        {
            if (isCyclicUtil(i->first, visited, v))
                return true;
        }

        // If an adjacent is visited and not parent of current vertex,
        // then there is a cycle.
        else if (i->first != parent)
            return true;
    }
    return false;
}

// Returns true if the graph contains a cycle, else false.
bool Graph::isCyclic()
{
    // Mark all the vertices as not visited and not part of recursion stack
    bool *visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // Call the recursive helper function to detect cycle in different
    // DFS trees
    for (int u = 0; u < V; u++)
        if (!visited[u]) // Don't recur for u if it is already visited
            if (isCyclicUtil(u, visited, -1))
                return true;

    return false;
}

// Print the adjacency list representation of graph
void Graph::printGraph() const
{
    int w;
    double wt;
    for (int v = 0; v < V; v++)
    {
        cout << "Node " << v << " makes an edge with \n";
        list<pair<int,double>>::iterator it;
        for (it = adj[v].begin(); it!=adj[v].end(); it++)
        {
            w = it->first;
            wt = it->second;
            cout << "\tNode " << w << " with edge weight = " << wt << "\n";
        }
        cout << "\n";
    }
}


int main() {

    Graph g("mediumEWG.txt");

    g.isCyclic()? cout << "Graph contains cycle\n":
    cout << "Graph doesn't contain cycle\n";
    cout << endl;

    g.printGraph();


    cout << endl;
    return 0;
}
