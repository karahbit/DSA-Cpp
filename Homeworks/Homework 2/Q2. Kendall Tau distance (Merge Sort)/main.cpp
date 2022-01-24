// Name: George Koubbe
// Date of Submission: 2/20/2018
// Course: 16:332:573, Data Structures and Algorithms
// Homework 2. Problem Q2. Kendall Tau distance (Merge Sort)

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
int  _mergeSort(int arr[], int temp[], int left, int right);
int merge(int arr[], int temp[], int left, int mid, int right);
int mergeSort(int arr[], int array_size);


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
    cout << "Kendall Tau distance using Merge Sort" << endl;
    
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
    
    // show the number of inversions
    cout << "\nThe number of inversions are: " << mergeSort(a, n) << endl;
    
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

// THE BELOW FUNCTIONS WERE TAKEN FROM GEEKSFORGEEKS.ORG

/* This function sorts the input array and returns the
 number of inversions in the array */
int mergeSort(int arr[], int array_size)
{
    int *temp = (int *)malloc(sizeof(int)*array_size);
    return _mergeSort(arr, temp, 0, array_size - 1);
}

/* An auxiliary recursive function that sorts the input array and
 returns the number of inversions in the array. */
int _mergeSort(int arr[], int temp[], int left, int right)
{
    int mid, inv_count = 0;
    if (right > left)
    {
        /* Divide the array into two parts and call _mergeSortAndCountInv()
         for each of the parts */
        mid = (right + left)/2;
        
        /* Inversion count will be sum of inversions in left-part, right-part
         and number of inversions in merging */
        inv_count  = _mergeSort(arr, temp, left, mid);
        inv_count += _mergeSort(arr, temp, mid+1, right);
        
        /*Merge the two parts*/
        inv_count += merge(arr, temp, left, mid+1, right);
    }
    return inv_count;
}

/* This funt merges two sorted arrays and returns inversion count in
 the arrays.*/
int merge(int arr[], int temp[], int left, int mid, int right)
{
    int i, j, k;
    int inv_count = 0;
    
    i = left; /* i is index for left subarray*/
    j = mid;  /* j is index for right subarray*/
    k = left; /* k is index for resultant merged subarray*/
    while ((i <= mid - 1) && (j <= right))
    {
        if (arr[i] <= arr[j])
        {
            temp[k++] = arr[i++];
        }
        else
        {
            temp[k++] = arr[j++];
            
            inv_count = inv_count + (mid - i);
        }
    }
    
    /* Copy the remaining elements of left subarray
     (if there are any) to temp*/
    while (i <= mid - 1)
        temp[k++] = arr[i++];
    
    /* Copy the remaining elements of right subarray
     (if there are any) to temp*/
    while (j <= right)
        temp[k++] = arr[j++];
    
    /*Copy back the merged elements to original array*/
    for (i=left; i <= right; i++)
        arr[i] = temp[i];
    
    return inv_count;
}
