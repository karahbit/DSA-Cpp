// Name: George Koubbe
// Date of Submission: 4/24/2018
// Course: 16:332:573, Data Structures and Algorithms
// Homework 4. Problem Q6. Djikstra

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

#include <functional>

using std::greater;

#include <queue>

using std::priority_queue;

#include <set>

using std::set;


// Program to find Dijkstra's shortest path using priority_queue in STL
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

    // prints shortest path from s
    void shortestPath(int s);

    void printGraph() const;
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
    //adj[v].push_back(make_pair(u, w));
}

// Prints shortest paths from src to all other vertices using priority_queue
void Graph::shortestPath(int src)
{
    // Create a priority queue to store vertices that
    // are being preprocessed. This is weird syntax in C++.
    // Refer below link for details of this syntax
    // http://geeksquiz.com/implement-min-heap-using-stl/
    priority_queue< iPair, vector <iPair> , greater<iPair> > pq;

    // Create a vector for distances and initialize all
    // distances as infinite (INF)
    vector<double> dist(V, INF);

    // Insert source itself in priority queue and initialize its distance as 0.
    pq.push(make_pair(0, src));
    dist[src] = 0;

    // Looping till priority queue becomes empty (or all distances are not
    // finalized
    while (!pq.empty())
    {
        // The first vertex in pair is the minimum distance
        // vertex, extract it from priority queue.
        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = pq.top().second;
        pq.pop();

        // 'i' is used to get all adjacent vertices of a vertex
        list< pair<int, double> >::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
        {
            // Get vertex label and weight of current adjacent of u.
            int v = (*i).first;
            double weight = (*i).second;

            //  If there is shorted path to v through u.
            if (dist[v] > dist[u] + weight)
            {
                // Updating distance of v
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    // Print shortest distances stored in dist[]
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < V; ++i)
        printf("%d \t\t %.2f\n", i, dist[i]);
}

/*// Prints shortest paths from src to all other vertices using set (NOT WORKING)
void Graph::shortestPath(int src)
{
    // Create a set to store vertices that are being
    // prerocessed
    set< pair<double, int> > setds;

    // Create a vector for distances and initialize all
    // distances as infinite (INF)
    vector<double> dist(V, INF);

    // Insert source itself in Set and initialize its
    // distance as 0.
    setds.insert(make_pair(0, src));
    dist[src] = 0;

    // Looping till all shortest distance are finalized then setds will become // empty
    while (!setds.empty())
    {
        // The first vertex in Set is the minimum distance
        // vertex, extract it from set.
        pair<double, int> tmp = *(setds.begin());
        setds.erase(setds.begin());

        // vertex label is stored in second of pair (it
        // has to be done this way to keep the vertices
        // sorted distance (distance must be first item
        // in pair)
        int u = tmp.second;

        // 'i' is used to get all adjacent vertices of a vertex
        list< pair<int, double> >::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
        {
            // Get vertex label and weight of current adjacent
            // of u.
            int v = (*i).first;
            int weight = (*i).second;

            //  If there is shorter path to v through u.
            if (dist[v] > dist[u] + weight)
            {
                //  If distance of v is not INF then it must be in
                //  our set, so removing it and inserting again
                //  with updated less distance.
                //  Note : We extract only those vertices from Set
                //  for which distance is finalized. So for them,
                //  we would never reach here.
                if (dist[v] != INF)
                    setds.erase(setds.find(make_pair(dist[v], v)));

                // Updating distance of v
                dist[v] = dist[u] + weight;
                setds.insert(make_pair(dist[v], v));
            }
        }
    }

    // Print shortest distances stored in dist[]
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < V; ++i)
        printf("%d \t\t %.2f\n", i, dist[i]);
}*/

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


// Driver program to test methods of graph class
int main()
{

    // create the graph given in above fugure
    Graph g(8);

    //  making above shown graph
    g.addEdge(0, 1, 4);
    g.addEdge(0, 7, 8);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 7, 11);
    g.addEdge(2, 3, 7);
    g.addEdge(2, 8, 2);
    g.addEdge(2, 5, 4);
    g.addEdge(3, 4, 9);
    g.addEdge(3, 5, 14);
    g.addEdge(4, 5, 10);
    g.addEdge(5, 6, 2);
    g.addEdge(6, 7, 1);
    g.addEdge(6, 8, 6);
    g.addEdge(7, 8, 7);

    g.shortestPath(0);

    Graph g2(4);
    g2.addEdge(0, 1, 1);
    g2.addEdge(0, 2, 0);
    g2.addEdge(0, 3, 99);
    g2.addEdge(1, 2, 1);
    g2.addEdge(3, 1, -300);
    g2.shortestPath(0);


    // create the graph given in above fugure
    Graph a("Q4a.txt");     // negative weights
    Graph b("Q4b.txt");     // negative cycles

    //a.printGraph();

    a.shortestPath(0);
    //b.shortestPath(0);


    cout << endl;
    return 0;
}
