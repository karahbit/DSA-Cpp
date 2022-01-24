// Name: George Koubbe
// Date of Submission: 4/24/2018
// Course: 16:332:573, Data Structures and Algorithms
// Homework 4. Problem Q5. DFS and BFS

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

#include <vector>

using std::vector;

#include <stack>

using std::stack;


// Class for an directed edge-weighted graph
class Graph
{

    int V;                    // No. of vertices
    int E;                    // No. of Edges
    // Pointer to an array containing adjacency lists
    list<pair<int,double>> *adj;
    bool isCyclicUtil(int v, bool visited[], int parent);
    void DFSUtil(int v, bool visited[]);    // A recursive function used by DFS

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
    void DFS(int v);          // DFS traversal of the vertices reachable from v
    void BFS(int s);          // prints BFS traversal from a given source s
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
    //adj[w].push_back(make_pair(v, wt)); // Add v to w’s list (for undirected)
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

void Graph::DFSUtil(int v, bool visited[])
{
    // Mark the current node as visited and print it
    visited[v] = true;
    cout << v << " ";

    // Recur for all the vertices adjacent to this vertex
    list<pair<int,double>>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[i->first])
            DFSUtil(i->first, visited);
}

/* Recursive version of DFS
// DFS traversal of the vertices reachable from v. It uses recursive DFSUtil()
void Graph::DFS(int v)
{
    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // Call the recursive helper function to print DFS traversal
    DFSUtil(v, visited);
}*/

// prints all not yet visited vertices reachable from s
void Graph::DFS(int v)
{
    // Initially mark all verices as not visited
    vector<bool> visited(V, false);

    // Create a stack for DFS
    stack<int> stack;

    // Push the current source node.
    stack.push(v);

    while (!stack.empty())
    {
        // Pop a vertex from stack and print it
        v = stack.top();
        stack.pop();

        // Stack may contain same vertex twice. So
        // we need to print the popped item only
        // if it is not visited.
        if (!visited[v])
        {
            cout << v << " ";
            visited[v] = true;
        }

        // Get all adjacent vertices of the popped vertex s
        // If a adjacent has not been visited, then puah it
        // to the stack.
        for (auto i = adj[v].begin(); i != adj[v].end(); ++i)
            if (!visited[i->first])
                stack.push(i->first);
    }
}

void Graph::BFS(int s)
{
    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    for(int i = 0; i < V; i++)
        visited[i] = false;

    // Create a queue for BFS
    list<int> queue;

    // Mark the current node as visited and enqueue it
    visited[s] = true;
    queue.push_back(s);

    // 'i' will be used to get all adjacent vertices of a vertex
    list<pair<int,double>>::iterator i;

    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        s = queue.front();
        cout << s << " ";
        queue.pop_front();

        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for (i = adj[s].begin(); i != adj[s].end(); ++i)
        {
            if (!visited[i->first])
            {
                visited[i->first] = true;
                queue.push_back(i->first);
            }
        }
    }
}


int main() {

    /*Graph g(4);
    g.addEdge(0, 1, 0.1);
    g.addEdge(0, 2, 0.1);
    g.addEdge(1, 2, 0.1);
    g.addEdge(2, 0, 0.1);
    g.addEdge(2, 3, 0.1);
    g.addEdge(3, 3, 0.1);

    cout << "Following is Depth First Traversal (starting from vertex 2) \n";
    g.DFS(2);
    cout << endl;

    cout << "Following is Breadth First Traversal (starting from vertex 2) \n";
    g.BFS(2);
    cout << endl;*/

    Graph g("NYC.txt");

    cout << "Following is Depth First Traversal (starting from vertex 2) \n";
    g.DFS(2);
    cout << endl;

    cout << "Following is Breadth First Traversal (starting from vertex 2) \n";
    g.BFS(2);
    cout << endl;

    cout << endl;
    return 0;
}
