// Name: George Koubbe
// Date of Submission: 2/6/2018
// Course: 16:332:573, Data Structures and Algorithms
// Homework 1. Problem Q2.

#include <iostream>
#include <fstream>
#include <string>                     // std::string, std::stoi
#include <sstream>                    // for turning string into stream
#include <chrono>                     // for high_resolution_clock
#include <new>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::getline;
using std::string;
using std::stringstream;
using std::stoi;
using std::chrono::high_resolution_clock;
using std::chrono::duration;

// calculate number of integers in a file (2 integers per line)
int numberOfIntegers(string fileName) {
    int n = 0;
    string line;
    ifstream inFile(fileName);
    for (int j = 1; getline(inFile, line); ++j) n = j;
    n = 2*n;
    inFile.close();
    return n;
}

class QuickFindUF {
private:
    int *id;                        // id[i] = component identifier of i
    
    // validate that p is a valid index
    void validate(int p) {
        if (p < 1 || p > N) {
            cout << "index " << p << " is not between 1 " << " and " << N
                 << endl;
            exit(1); // terminate with error
        }
    }
    
public:
    int count;                      // number of components
    int j, N;
    
    /*
     * Initializes an empty union–find data structure with N sites
     * (from 0 to N-1). Each site is initially in its own component.
     */
    QuickFindUF(int n, int m) {
        N = m;                    // aux variable to store number of sites
        count = N;                // initially, #components = #sites
        j = n;                    // aux variable to store the size of the array
        id = new int[n];
        for (int i = 0; i < n; i++)
            id[i] = i;
    }
    
    /*
     * Returns true if the the two sites are in the same component.
     */
    bool connected(int p, int q) {
        validate(p);
        validate(q);
        return id[p] == id[q];
    }
    
    /*
     * Returns the component identifier for the component containing site p.
     */
    int find(int p) {
        validate(p);
        return id[p];
    }
    
    /*
     * Merges the component containing site p with the component containing
     * site q.
     */
    void Union(int p, int q) {
        validate(p);
        validate(q);
        int pID = id[p];   // needed for correctness
        int qID = id[q];   // to reduce the number of array accesses
        
        // p and q are already in the same component
        if (pID == qID) return;
        
        for (int i = 0; i < j; i++)
            if (id[i] == pID) id[i] = qID;
        count--;
    }
    
};

class QuickUnionUF {
private:
    int *parent;                        // id[i] = component identifier of i
    
    // validate that p is a valid index
    void validate(int p) {
        if (p < 1 || p > N) {
            cout << "index " << p << " is not between 1 " << " and " << N
            << endl;
            exit(1); // terminate with error
        }
    }
    
public:
    int count;                      // number of components
    int j, N;
    
    /*
     * Initializes an empty union–find data structure with N sites
     * (from 0 to N-1). Each site is initially in its own component.
     */
    QuickUnionUF(int n, int m) {
        N = m;                    // aux variable to store number of sites
        count = N;                // initially, #components = #sites
        j = n;                    // aux variable to store the size of the array
        parent = new int[n];
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }
    
    /*
     * Returns true if the the two sites are in the same component.
     */
    bool connected(int p, int q) {
        return find(p) == find(q);
    }
    
    /*
     * Returns the component identifier for the component containing site p.
     */
    int find(int p) {
        validate(p);
        while (p != parent[p])
            p = parent[p];
        return p;
    }
    
    /*
     * Merges the component containing site p with the component containing
     * site q.
     */
    void Union(int p, int q) {
        int rootP = find(p);
        int rootQ = find(q);
        if (rootP == rootQ) return;
        parent[rootP] = rootQ;
        count--;
    }
    
};

class WeightedQuickUnionUF {
private:
    int *parent;             // id[i] = component identifier of i
    int *size;               // size[i] = number of sites in subtree rooted at i
    
    // validate that p is a valid index
    void validate(int p) {
        if (p < 1 || p > N) {
            cout << "index " << p << " is not between 1 " << " and " << N
            << endl;
            exit(1); // terminate with error
        }
    }
    
public:
    int count;                      // number of components
    int j, N;
    
    /*
     * Initializes an empty union–find data structure with N sites
     * (from 0 to N-1). Each site is initially in its own component.
     */
    WeightedQuickUnionUF(int n, int m) {
        N = m;                    // aux variable to store number of sites
        count = N;                // initially, #components = #sites
        j = n;                    // aux variable to store the size of the array
        parent = new int[n];
        size = new int[n];
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            size[i] = 1;
        }
    }
    
    /*
     * Returns true if the the two sites are in the same component.
     */
    bool connected(int p, int q) {
        return find(p) == find(q);
    }
    
    /*
     * Returns the component identifier for the component containing site p.
     */
    int find(int p) {
        validate(p);
        while (p != parent[p])
            p = parent[p];
        return p;
    }
    
    /*
     * Merges the component containing site p with the component containing
     * site q.
     */
    void Union(int p, int q) {
        int rootP = find(p);
        int rootQ = find(q);
        if (rootP == rootQ) return;
        
        // make smaller root point to larger one
        if (size[rootP] < size[rootQ]) {
            parent[rootP] = rootQ;
            size[rootQ] += size[rootP];
        }
        else {
            parent[rootQ] = rootP;
            size[rootP] += size[rootQ];
        }
        count--;
    }
    
};

/*
 * Reads in a sequence of pairs of integers (between l and m) from standard
 * input, where each integer represents some site; if the sites are in different
 * components, merge the two components and print the pair to standard output.
 */
int main() {
    // variable declaration and initialization
    int n, N;
    string element;
    int p = 0;
    int q = 0;
    string line;                      // Temp variable to store each file's line
    string::size_type sz;             // Pointer needed by function stoi
    string fileName;                  // Variable to store the name of the file
    
    // request user to input file
    cout << "Select file to open: 8pair.txt, 32pair.txt, 128pair.txt" << endl;
    cout << "512pair.txt, 1024pair.txt, 4096pair.txt or 8192int.txt" << endl;
    cout << "\n" << "File: ";
    cin >> fileName;
    cout << "\n";
    
    cout << "Input highest data point label: ";
    cin >> N;
    cout << "\n";
    
    // calculate the number of integers in the file
    n = numberOfIntegers(fileName);
    
    // open existent file
    ifstream inFile(fileName);        // Change here to open a different file
    
    // check that the file was opened correctly
    if (!inFile) {
        cout << "Unable to open file" << endl;
        exit(1); // terminate with error
    }
    
    // Record start time
    auto start = high_resolution_clock::now();
    
    // create the data structure Union-Find
    //QuickFindUF uf(n, N);
    //QuickUnionUF uf(n, N);
    WeightedQuickUnionUF uf(n, N);

    /* Read integers on file and put them in p and q. If they are not connected,
    * perform the union and print the pair. Lastly, print the number of
    * remaining components.
    */
    while(getline(inFile, line)) {
        stringstream ssin(line);
        if (ssin.good()){
            ssin >> element;
            p = stoi (element, &sz);
        }
        if (ssin.good()){
            ssin >> element;
            q = stoi (element, &sz);
        }
        if (uf.connected(p, q)) continue;
        uf.Union(p, q);
        cout << p << " " << q << endl;
    }
    cout << uf.count << " components" << endl;
    
    // Record end time
    auto finish = high_resolution_clock::now();
    // Run time in seconds for the total algorithm
    duration<double> elapsed = finish - start;
    cout << "\nElapsed time: " << elapsed.count() << " s\n";
    
    inFile.close();                   // Close current file
    
    return 0;
}
