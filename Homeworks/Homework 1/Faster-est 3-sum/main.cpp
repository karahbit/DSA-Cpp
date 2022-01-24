// Name: George Koubbe
// Date of Submission: 2/6/2018
// Course: 16:332:573, Data Structures and Algorithms
// Homework 1. Problem Q5.

#include <iostream>
#include <ctime>       /* time */

using std::cout;
using std::endl;

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

int twoSumFaster(int array[], int N) {
    int count = 0;
    int start = 0;
    int end = N - 1;

    while(start < end){
        if(array[start] + array[end] == 0) {
            count++;
            start++;
            end--;
        }
        else if(array[start] + array[end] > 0) {
            end--;
        }
        else
            start++;
    }
    return count;
}

int threeSumFaster(int array[], int N) {
    int count = 0;
    
    for(int i = 0; i < N - 2; i++) {
        int start = i + 1;
        int end = N - 1;
        
        while(start < end){
            if(array[i] + array[start] + array[end] == 0){
                count++;
                end--;
                start++;
            }
            else if(array[i] + array[start] + array[end] > 0)
                end--;
            else
                start++;
            
        }
    }
    return count;
}

int main() {
    // Seed for generating random numbers relative to the current time
    srand(static_cast<unsigned int>(time(0)));
    
    // Variable declaration
    int N = 20;
    int arr[N];
    int floor = -100;
    int ceiling = 100;
    
    // initializing array with random numbers
    for (int i = 0; i < N; i++)
        arr[i] = (((rand()) / (RAND_MAX + 1.0)) * (ceiling - floor)) + floor;
    
    // print unsorted array
    cout << "unsorted array:" << endl;
    for (int i = 0; i < N; i++)
        cout << arr[i] << " ";
    
    // print sorted array
    cout << "\n\nsorted array" << endl;
    insertionSort(arr, N);
    for (int i = 0; i < N; i++)
      cout << arr[i] << " ";
    
    // Calculate the number of pairs that add up to zero
    cout << "\n\nthe number of pairs that add up to zero is: "
    << twoSumFaster(arr, N) << endl;
    
    // Calculate the number of triplets that add up to zero
    cout << "the number of triplets that add up to zero is: "
    << threeSumFaster(arr, N) << endl;
    
    cout << "\n";
    return 0;
}
