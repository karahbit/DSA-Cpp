// Name: George Koubbe
// Date of Submission: 2/20/2018
// Course: 16:332:573, Data Structures and Algorithms
// Homework 2. Problem Q4. Mergesort

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <fstream>
using std::ifstream;

#include <sstream>
using std::stringstream;

#include <string>
using std::getline;
using std::string;
using std::stoi;

#include <algorithm>
using std::min;

#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration;

// FUNCTION PROTOTYPES
int numberOfIntegers(string);
bool lessTD(int, int);
bool lessBU(int, int);
void show(int a[], int);
void mergeTD(int a[], int aux[], int lo, int mid, int hi);
void sortTD(int a[], int aux[], int lo, int hi);
void mergeBU(int a[], int aux[], int lo, int mid, int hi);
void sortBU(int a[], int aux[], int N);

// GLOBAL VARIABLES
long long counterTD = 0;            // count number of compares for top down
long long counterBU = 0;            // count number of compares for bottom up


// MAIN PROGRAM EXECUTION
int main() {
    // variable declaration and initialization
    int n;
    int i = 0;
    string element;
    string line;                      // Temp variable to store each file's line
    string::size_type sz;             // Pointer needed by function stoi
    string fileName;                  // Variable to store the name of the file
    
    // welcome message
    cout << "Mergesort Top-Down vs Bottom-Up" << endl;
    
    // request user to input file
    cout << "\nSelect file to open: " << endl;
    cin >> fileName;
    
    // calculate the number of integers in the file
    n = numberOfIntegers(fileName);
    int a[n]; int auxTD[n];
    int b[n]; int auxBU[n];
    
    // open existent file
    ifstream inFile(fileName);        // Change here to open a different file
    
    // check that the file was opened correctly
    if (!inFile) {
        cout << "Unable to open file" << endl;
        exit(1); // terminate with error
    }
    
    // read integers on file and put them in the array
    while(getline(inFile, line)) {
        stringstream ssin(line);
        if (ssin.good()){
            ssin >> element;
            a[i] = stoi (element, &sz);
            b[i] = a[i];
            i++;
        }
    }
    
    // Record start time
    auto start = high_resolution_clock::now();
    
    // sort the array
    sortTD(a,auxTD,0,n-1);
    //sortBU(b,auxBU,n);
    //show(a,n);
    
    // show the number of compares made
    cout << "\nThe number of comparisons for Mergesort Top-Down is: " << counterTD << endl;
    //cout << "The number of comparisons for Mergesort Bottom-Up is: " << counterBU << endl;
    
    // Record end time
    auto finish = high_resolution_clock::now();
    // Run time in seconds for the total algorithm
    duration<double> elapsed = finish - start;
    cout << "\nElapsed time: " << elapsed.count() << " s\n";
    
    inFile.close();                   // Close current file
    
    return 0;
}


// FUNCTION DEFINITIONS

// calculate number of integers in a file
int numberOfIntegers(string fileName) {
    int j;
    string line;
    ifstream inFile(fileName);
    for (j = 0; getline(inFile, line); ++j);
    inFile.close();
    return j;
}

// private helper function to compare two elements.
bool lessTD(int a, int b) {
    counterTD++;
    if (a < b) return 1; else return 0;
}

// private helper function to compare two elements.
bool lessBU(int a, int b) {
    counterBU++;
    if (a < b) return 1; else return 0;
}

// private helper function to print the array on a single vertical line.
void show(int a[], int N) {
    for (int i = 0; i < N; i++)
        cout << a[i] << endl;
}

void mergeTD(int a[], int aux[], int lo, int mid, int hi) {
    // Merge a[lo..mid] with a[mid+1..hi].
    int i = lo, j = mid+1;
    for (int k = lo; k <= hi; k++)  // Copy a[lo..hi] to aux[lo..hi].
        aux[k] = a[k];
    for (int k = lo; k <= hi; k++)  // Merge back to a[lo..hi].
        if      (i > mid)              a[k] = aux[j++];
        else if (j > hi )              a[k] = aux[i++];
        else if (lessTD(aux[j], aux[i])) a[k] = aux[j++];
        else                           a[k] = aux[i++];
}

// same merge as TD, but increments lessBU instead to count comparisons
void mergeBU(int a[], int aux[], int lo, int mid, int hi) {
    // Merge a[lo..mid] with a[mid+1..hi].
    int i = lo, j = mid+1;
    for (int k = lo; k <= hi; k++)  // Copy a[lo..hi] to aux[lo..hi].
        aux[k] = a[k];
    for (int k = lo; k <= hi; k++)  // Merge back to a[lo..hi].
        if      (i > mid)              a[k] = aux[j++];
        else if (j > hi )              a[k] = aux[i++];
        else if (lessBU(aux[j], aux[i])) a[k] = aux[j++];
        else                           a[k] = aux[i++];
}

void sortTD(int a[], int aux[], int lo, int hi) {
    // Sort a[lo..hi].
    if (hi <= lo) return;
    int mid = lo + (hi - lo)/2;
    sortTD(a, aux, lo, mid);       // Sort left half.
    sortTD(a, aux, mid+1, hi);     // Sort right half.
    mergeTD(a, aux, lo, mid, hi);  // Merge results.
}

void sortBU(int a[], int aux[], int N) {
    // Do lg N passes of pairwise merges.
    for (int sz = 1; sz < N; sz = sz+sz)            // sz: subarray size
        for (int lo = 0; lo < N-sz; lo += sz+sz)    // lo: subarray index
            mergeBU(a, aux, lo, lo+sz-1, min(lo+sz+sz-1, N-1));
}
