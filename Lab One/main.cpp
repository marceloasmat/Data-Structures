#include <iostream>
#include <cassert>
#include <algorithm>

// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using namespace std;

#define MIN_ARRAY_LEN 2
// example 5! = 1x2x3x4x5
// the base case will be whenever the number becomes 0 (factorial of 0 is 1)
// factorial 5 = 5 x factorial 4
// factorial 4 = 4 x factorial 3 ....
// factorial n = n x factorial n-1
int factorial(int n) {
    if (n == 0) {
        return 1; // this will return a integer "1" and stop the recursion
    }
    else {
        return n * factorial(n-1);
    }
    return 0; // this should never run, the else and if statement should run until we get to 1
}
// ask professor to how we should handle negative numbers (throw an exception?)
TEST_CASE("testing the factorial function") {
    CHECK(factorial(0) == 1);
    CHECK(factorial(3) == 6);
}


// A fibonacci sequance adds the previous 2 numbers to equal the next number, 0,1,1,2,3, ....
// the base case will be fibonacci 0 which is 0, fibonacci 1 = 1
// fibonacci 5 = fibonacci 4 + fibonacci 3   (3= 2+ 1)
// fibonacci 4 = fibonacci 3 + fibonacci 2   (2= 1+1)
// fibonacci 3 = fibonacci 2 + fibonacci 1... (1= 1+0)
// fibonacci n = fibonacci n-1 + fibonacci n-2


int fibonacci(int n) {
    if (n==0){
        return 0; // this will stop the recursion and just a value of 0 to our sum
    }
    else if (n==1){
        return 1;
    }
    else {
        return fibonacci(n-1) + fibonacci(n-2); //this is the method to add up the previous 2 methods
    }
    return 0; // should never run
}
// how do we want to index this? by user intuition or by how the array is organized? I,E. do we start at 0 or at 1?
TEST_CASE("testing the fibonacci function") {
    int actual = fibonacci(4);
    CHECK(actual == 3); 
}

  // moves n disks from original tower to destination tower using the extra tower.
  // returns number of moves needed to move n disks

int towersInternal(int n, char original, char destination, char extra){
    if (n==1){
        return 1;
    } else if (n == 0){
        return 0;
    }
   int numOfMovesForNMinusOne = towersInternal (n-1,original, extra, destination); 
   int numOfMovesForOneDisk = towersInternal (1, original, destination, extra);
   int numOfMovesForRest = towersInternal (n-1, extra, destination, original);
   return numOfMovesForNMinusOne + numOfMovesForOneDisk + numOfMovesForRest;
}

int towers(int n) {
    return towersInternal(n,'a','b','c');
}



TEST_CASE("testing the towers function") {
    CHECK(towers(3) == 7);
    CHECK(towers(4) == 15);
    CHECK(towers(10) == 1023);
    CHECK(towers(15) == 32767);
    CHECK(towers(20) == 1048575);
}



// Warning: Be sure to free the returned copy.
int* copyArray(const int arr[], int n) {
    int* r = new int[n];
    for (int i = 0; i < n; ++i) {
        r[i] = arr[i];
    }
    return r;
}

int chooseRandomPivotIndex(int start, int end) {
    int n = (end + 1) - start;
    return start + rand() % n;
}

// Partitions an array into segments s1, and s2 where s1 contains all the values less
// than or equal to the pivot value, and s2 contains the values greater or equal to the pivot.
//
// pivotIndex: Index whose value is used to partition the input array.
// start: The index in the array to start at.
// end: The inclusive index in the array to end at.
//
// Returns: Index where the pivotIndex has moved to and the input array arr partitioned.
int partition(int pivotIndex, int arr[], int start, int end) {
    int n = (end + 1) - start;
    assert(n != 0 && pivotIndex >= start && pivotIndex <= end);

    if (n == 1) {
        return pivotIndex;
    }
    int pivotValue = arr[pivotIndex];

    // Hints:
    //       * The pivot is not included in s1 or s2, although there can be other elements equal to the pivot in s1 or s2.
    //       * The pivot value determines what needs to move to s1 or s2;
    //       * If you change the location of the pivot value don't forget to return the new pivotIndex.
    //       * You might want to temporarily move the pivot value to aid in the process.
    //       * You can do this multiple ways; one pass, two passes, from center out, from ends in.
    //       * Use std::swap

    int i = start - 1;
    for (int j = start;j <= end;j++){ 
        int valueAtJ = arr[j];

        if (valueAtJ < pivotValue){
            // do swap
            i++;
            std::swap(arr[i], arr[j]);

        }
        else {
            //do nothing
        }
    }
    i++;
    std::swap(arr[i], arr[pivotIndex]);
    return i;
}

// Quickly finds the k-th smallest value without sorting the entire array.
//
// k: 0 based index k into array arr
// arr: mutable pointer to an array
// start: start index of the array
// end: end index of the array
int findKthSmallestValue(int k, int arr[], int start, int end) {
    int n = (end + 1) - start;
    if (n == 0 || k >= n) {
        throw runtime_error("Invalid input.");
    }

    // Return the single value in an array of length 1.
    if (n == 1) {
        return arr[start];
    }

    int chosenPivotIndex = chooseRandomPivotIndex(start, end);

    int pivotIndex = partition(chosenPivotIndex, arr, start, end);

    int s1 = pivotIndex - start;

    // Base case where s1 == k.
    if (s1 == k) {
        // TODO: Replace -1 with the value for the base case.
        return -1;
    }

    // Figure out which segment the kth smallest is in and recurse.
    // Note that the pivot is not included in s1 or s2. This guarantees that the problem reduces while recursing.
    // TODO: Replace true with a boolean expression that determines which segment to recurse into.
    if (true) {
        // TODO: Replace -1 with a recursive call to findKthSmallestValue.
        return -1;
    } else {
        // TODO: Replace -1 with a recursive call to findKthSmallestValue.
        return -1;
    }
}

// Used to test our results. Warning: Sorts the input array in place.
int findKthSmallestValueViaSorting(int k, int arr[], int start, int end) {
    int n = (end + 1) - start;
    if (n == 0 || k >= n) {
        throw runtime_error("Invalid input.");
    }

    if (n == 1) {
        return arr[start];
    }

    sort(arr, arr + n);

    return arr[k];
}

// Tests findKthSmallestValue for an array of length n.
void testFindKthSmallestValueForArraySizeN(int n) {
    if (n == 0 || n == 1) {
        throw runtime_error("Invalid input.");
    }

    int* arr = new int[n];

    // Populate the array with random numbers
    for (int i = 0; i < n; ++i)
    {
        int x = rand() % 100;
        arr[i] = x; // Random number between 0 and 99
    }

    for (int k = 0; k < n; ++k) {
        // Copying the array to ensure our input doesn't get sorted.
        int* copyArray1 = copyArray(arr, n);
        int expectedResult = findKthSmallestValueViaSorting(k, copyArray1, 0, n - 1);
        delete[] copyArray1;

        // Copying here to ensure the implementation doesn't mutate the array between different k iterations.
        int* copyArray2 = copyArray(arr, n);
        int result = findKthSmallestValue(k, copyArray2, 0, n - 1);
        delete[] copyArray2;

        if (result != expectedResult) {
            throw runtime_error("Test failed.");
        }
        else {
            cout << "Success for input array of size " << n << endl;
        }
    }

    delete[] arr;
}

void testFindKthSmallestValue(int repetitions, int maxArraySize) {
    if (maxArraySize < MIN_ARRAY_LEN) {
        throw runtime_error("Invalid input.");
    }

    for (int n = MIN_ARRAY_LEN; n <= maxArraySize; ++n) {
        for (int i = 0; i < repetitions; ++i) {
            testFindKthSmallestValueForArraySizeN(n);
        }
    }
}

TEST_CASE("test kth smallest value") {
    srand(0);
   // CHECK_NOTHROW(testFindKthSmallestValue(3, 5));
}

