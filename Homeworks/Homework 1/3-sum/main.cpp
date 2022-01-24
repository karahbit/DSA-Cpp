// Name: George Koubbe
// Date of Submission: 2/6/2018
// Course: 16:332:573, Data Structures and Algorithms
// Homework 1. Problem Q1.

#include <iostream>
#include <fstream>
#include <string>                     // std::string, std::stoi
#include <chrono>                     // for high_resolution_clock

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::getline;
using std::string;
using std::stoi;
using std::chrono::high_resolution_clock;
using std::chrono::duration;

// Function to calculate number of lines in a file
int numberOfLines(string fileName) {
    int N = 0;
    string line;
    ifstream inFile(fileName);
    for (int j = 1; getline(inFile, line); ++j) N = j;
    inFile.close();
    return N;
}

// Function to sort an array using insertion sort. Taken from geeksforgeeks.org
void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i-1;
        /* Move elements of arr[0..i-1], that are greater than key, to one
         position ahead of their current position */
        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j = j-1;
        }
        arr[j+1] = key;
    }
}

// Function to do binary search. Taken from textbook.
int binarySearch(int a[], int v, int l, int r) {
    while (r >= l)
    { int m = (l+r)/2;
        if (v == a[m]) return m;
        if (v < a[m]) r = m-1; else l = m+1;
    }
    return -1;
}

int main() {
    // Variable declaration and initialization
    int i = 0;
    string line;                      // Temp variable to store each file's line
    string::size_type sz;             // Pointer needed by function stoi
    string fileName;                  // Variable to store the name of the file
    int count = 0;                    // Variable to store the number of triples
                                      // whose sum equals to zero
    
    // Request user to input file
    cout << "Select file to open: 8int.txt, 32int.txt, 128int.txt" << endl;
    cout << "512int.txt, 1024int.txt, 4096int.txt, 4192int.txt or" << endl;
    cout << "8192int.txt" << "\n" << endl << "File: ";
    cin >> fileName;
    cout << "\n";
    
    // Calculate the number of lines in the file to create respective array
    int N = numberOfLines(fileName);
    int a[N];
    
    // Open existent file
    ifstream inFile(fileName);        // Change here to open a different file
    
    // Check that the file was opened correctly
    if (!inFile) {
        cout << "Unable to open file" << endl;
        exit(1); // terminate with error
    }
    
    // Read integers on file and put them in an array
    while(getline(inFile, line)) {
        a[i] = stoi (line, &sz);
        i++;
    }

    //************ 3-SUM O(N^3) AlGORITHM ***************
    
    /* Record start time
    auto start = high_resolution_clock::now();
    
    // Apply brute force 3-sum algorithm
    for (int j = 0; j < N; j++) {
        for (int k = j+1; k < N; k++) {
            for (int l = k+1; l < N; l++){
                if (a[j] + a[k] + a[l] == 0) count++;
            }
        }
     }
    
    // Print the number of triples tha sum exactly zero
    cout << "The number of triples that sum exactly zero is: " << count << endl;
    
    // Record end time
    auto finish = high_resolution_clock::now();
    // Run time in seconds for the total algorithm
    duration<double> elapsed = finish - start;
    cout << "\nElapsed time: " << elapsed.count() << " s\n"; */
    
    //*********** 3-SUM O(N^2logN) ALGORITHM **************
    
    // Record start time
    auto start = high_resolution_clock::now();
    
    // First step: sort the array. O(N^2)
    N = static_cast<int>(sizeof(a)/sizeof(a[0]));
    insertionSort(a, N);
    
    // Second step: binary search. O(N^2logN)
    for (int j=0; j<N-3; j++) {
        for (int k=j+1; k<N-2; k++) {
            int val = -(a[j]+a[k]);
            int res = binarySearch(a, val, k+1, N-1);
            if (res > -1) count++;
        }
    }
    
    // Print the number of triples tha sum exactly zero
    cout << "The number of triples that sum exactly zero is: " << count << endl;
    
    // Record end time
    auto finish = high_resolution_clock::now();
    // Run time in seconds for the total algorithm
    duration<double> elapsed = finish - start;
    cout << "\nElapsed time: " << elapsed.count() << " s\n";
    
    inFile.close();                   // Close current file
    
    return 0;
}
