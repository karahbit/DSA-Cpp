// Name: George Koubbe
// Date of Submission: 2/20/2018
// Course: 16:332:573, Data Structures and Algorithms
// Homework 2. Problem Q5. Quicksort

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
void exch(int a[], int i, int j);
bool less(int, int);
void show(int a[], int);
void insertionSort(int a[], int lo, int hi);
int partition(int a[], int lo, int hi);
void sort(int a[], int lo, int hi);
int median3(int a[], int i, int j, int k);
void shuffle(int arr[], int n);

// GLOBAL VARIABLES
long long counter = 0;                  // count number of compares for Quicksort
const int CUTOFF = 7;                   // cutoff to insertion sort



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
    cout << "Quicksort" << endl;
    
    // request user to input file
    cout << "\nSelect file to open: " << endl;
    cin >> fileName;
    
    // calculate the number of integers in the file
    n = numberOfIntegers(fileName);
    int a[n];
    
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
            i++;
        }
    }
    
    // Record start time
    auto start = high_resolution_clock::now();
    
    // sort the array
    //shuffle(a,n);
    sort(a,0,n-1);
    //show(a,n);
    
    // show the number of compares made
    cout << "\nThe number of comparisons for QuickSort is: " << counter << endl;
    
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

// exchange a[i] and a[j]
void exch(int a[], int i, int j) {
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}

// private helper function to compare two elements.
bool less(int a, int b) {
    counter++;
    if (a < b) return 1; else return 0;
}

// private helper function to print the array on a single vertical line.
void show(int a[], int N) {
    for (int i = 0; i < N; i++)
        cout << a[i] << endl;
}

// sort from a[lo] to a[hi] using insertion sort
void insertionSort(int a[], int lo, int hi) {
    for (int i = lo; i <= hi; i++)
        for (int j = i; j > lo && less(a[j], a[j-1]); j--)
            exch(a, j, j-1);
}

// partition the subarray a[lo .. hi] by returning an index j
// so that a[lo .. j-1] <= a[j] <= a[j+1 .. hi]
int partition(int a[], int lo, int hi) {
    int i = lo;
    int j = hi + 1;
    int v = a[lo];
    while (true) {
        
        // find item on lo to swap
        while (less(a[++i], v))
            if (i == hi) break;
        
        // find item on hi to swap
        while (less(v, a[--j]))
            if (j == lo) break;      // redundant since a[lo] acts as sentinel
        
        // check if pointers cross
        if (i >= j) break;
        
        exch(a, i, j);
    }
    
    // put v = a[j] into position
    exch(a, lo, j);
    
    // with a[lo .. j-1] <= a[j] <= a[j+1 .. hi]
    return j;
}

// quicksort the subarray from a[lo] to a[hi]
void sort(int a[], int lo, int hi) {
    //if (hi <= lo) return;
    // cutoff to insertion sort
    int n = hi - lo + 1;
    if (n <= CUTOFF) {
        insertionSort(a, lo, hi);
        return;
    }
    
    int m = median3(a, lo, lo + n/2, hi);
    exch(a, m, lo);
    
    int j = partition(a, lo, hi);
    sort(a, lo, j-1);
    sort(a, j+1, hi);
}

// return the index of the median element among a[i], a[j], and a[k]
int median3(int a[], int i, int j, int k) {
    return (less(a[i], a[j]) ?
            (less(a[j], a[k]) ? j : less(a[i], a[k]) ? k : i) :
            (less(a[k], a[j]) ? j : less(a[k], a[i]) ? k : i));
}

// A function to generate a random permutation of arr[]
void shuffle(int arr[], int n) {
    // Use a different seed value so that we don't get same
    // result each time we run this program
    srand(static_cast<unsigned int>(time(0)));
    
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n-1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i+1);
        
        // Swap arr[i] with the element at random index
        exch(arr, i, j);
    }
}
