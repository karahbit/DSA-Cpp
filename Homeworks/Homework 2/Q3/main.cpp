// Name: George Koubbe
// Date of Submission: 2/20/2018
// Course: 16:332:573, Data Structures and Algorithms
// Homework 2. Problem Q3

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

// FUNCTION PROTOTYPES
void swap (int *a, int *b);
void shuffle ( int arr[], int n );
void printArray (int arr[], int n);


// MAIN PROGRAM EXECUTION
int main() {
    // variable declaration and initialization
    int i;
    int a[8192];
    int counter1 = 0;
    int counter11 = 0;
    int counter111 = 0;
    int counter1111 = 0;
    
    // create data set
    for (int i = 0; i < 1024; i++)
        a[i] = 1;
    for (int i = 1024; i < 3072; i++)
        a[i] = 11;
    for (int i = 3072; i < 7168; i++)
        a[i] = 111;
    for (int i = 7168; i < 8192; i++)
        a[i] = 1111;
    
    // shuffle the array
    shuffle(a,8192);
    
    // print the array
    printArray(a,8192);
    
    // count the number of each value that is in the array
    for (i = 0; i < 8192; i++){
        if (a[i] == 1) counter1++;
        if (a[i] == 11) counter11++;
        if (a[i] == 111) counter111++;
        if (a[i] == 1111) counter1111++;
    }
    
    // fill the array again, but this time ordered
    for (int i = 0; i < counter1; i++)
        a[i] = 1;
    for (int i = counter1; i < (counter1 + counter11); i++)
        a[i] = 11;
    for (int i = (counter1 + counter11); i < (counter1 + counter11 + counter111); i++)
        a[i] = 111;
    for (int i = (counter1 + counter11 + counter111); i < (counter1 + counter11 + counter111 + counter1111); i++)
        a[i] = 1111;
    
    // print the array
    printArray(a,8192);
    
    cout << "\n";
    return 0;
}

// FUNCTION DEFINITION

// A utility function to print an array
void printArray (int arr[], int n)
{
    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << "\n";
}

// A utility function to swap to integers
void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// A function to generate a random permutation of arr[]
void shuffle ( int arr[], int n )
{
    // Seed for generating random numbers relative to the current time
    srand(static_cast<unsigned int>(time(0)));
    
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n-1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i+1);
        
        // Swap arr[i] with the element at random index
        swap(&arr[i], &arr[j]);
    }
}
