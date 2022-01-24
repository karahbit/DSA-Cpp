// Name: George Koubbe
// Date of Submission: 2/6/2018
// Course: 16:332:573, Data Structures and Algorithms
// Homework 1. Problem Q4.

#include <iostream>
#include <iomanip>      // parameterized stream manipulators
#include <ctime>       /* time */

using std::cout;
using std::endl;
using std::fixed;
using std::setprecision;

int main() {
    
    // Seed for generating random numbers relative to the current time
    srand(static_cast<unsigned int>(time(0)));
    
    // Variable declaration
    int N = 20;
    double arr[N];
    double floor = -10;
    double ceiling = 10;
    double max = 0;
    double min =0;
    double distance;
    
    // initializing array with random numbers
    for (int i = 0; i < N; i++)
        arr[i] = ((double(rand()) / double(RAND_MAX)) * (ceiling - floor)) + floor;
    
    // print unsorted array
    for (int i = 0; i < N; i++)
        cout << setprecision(2) << fixed << arr[i] << " ";
    
    // find max and min of the array, and calculate distance
    for (int i = 0; i < N; i++) {
        if (arr[i] > max) max = arr[i];
    else
        if (arr[i] < min) min = arr[i];
    }
    
    distance = max-min;
    
    cout << "\nthe farthest pair is: " << max << " and " << min << endl;
    cout << "the distance between them is: " << distance << endl;
    
    cout << "\n";
    return 0;
}
