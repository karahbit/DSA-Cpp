// Name: George Koubbe
// Date of Submission: 2/20/2018
// Course: 16:332:573, Data Structures and Algorithms
// Homework 2. Problem Q1. Shellsort

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

// FUNCTION PROTOTYPES
int numberOfIntegers(string);
bool less(int, int);
void exch(int a[], int, int);
void show(int a[], int);
void shellSort(int a[], int);
void insertionSort(int arr[], int);

// GLOBAL VARIABLES
long long counterShell = 0;          // count number of compares for Shell Sort
long long counterIns = 0;            // count number of compares for Insertion Sort


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
    cout << "Shell Sort vs Insertion Sort" << endl;
    
    // request user to input file
    cout << "\nSelect file to open: " << endl;
    cin >> fileName;
    
    // calculate the number of integers in the file
    n = numberOfIntegers(fileName);
    int a[n];
    int b[n];
    
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
    
    // sort the array
    shellSort(a,n);
    insertionSort(b,n);
    //show(a,n);
    
    // show the number of compares made
    cout << "\nThe number of compares for Shell Sort is: " << counterShell << endl;
    cout << "The number of compares for Insertion Sort is: " << counterIns << endl;
    
    inFile.close();                   // Close current file
    
    cout << "\n";
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
bool less(int a, int b) {
    counterShell++;
    if (a < b) return 1; else return 0;
}

// private helper function to exchange two elements.
void exch(int a[], int i, int j) {
    int t = a[i]; a[i] = a[j]; a[j] = t;
}

// private helper function to print the array on a single vertical line.
void show(int a[], int N) {
    for (int i = 0; i < N; i++)
        cout << a[i] << endl;
}

// Function to sort an array using Shell Sort
void shellSort(int a[], int N) {
    // Sort a[] into increasing order.
    int h = 2;
    while (h < N/2) h = 2*h; // 1, 3, 7, 15, 31, 63, ...
    h--;
    while (h >= 1) {
        // h-sort the array.
        for (int i = h; i < N; i++) {
            // Insert a[i] among a[i-h], a[i-2*h], a[i-3*h]... .
            for (int j = i; j >= h && less(a[j], a[j-h]); j -= h)
                exch(a, j, j-h);
        }
        h = h/2;
    }
}

// Function to sort an array using Insertion Sort. Taken from geeksforgeeks.org
void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i-1;
        /* Move elements of arr[0..i-1], that are greater than key, to one
         position ahead of their current position */
        while (j >= 0 && arr[j] > key) {
            counterIns++;
            arr[j+1] = arr[j];
            j = j-1;
        }
        if (!(arr[j] > key)) counterIns++;
        arr[j+1] = key;
    }
}
