#include <iostream>
#include <vector>
#include <cstdio>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <math.h>
#include <sys/time.h>
#include <cstring>

using namespace std;


int base;

int partition(vector<int> &arr, int a, int b);

long getMilliseconds() {
    timeval tv;
    gettimeofday( &tv, NULL );
    long int ms = tv.tv_sec;
    ms = ms * 1000 + tv.tv_usec / 1000;
    return ms;
}

void QuickSort(vector<int> &arr, int p, int q){
    // BUBBLE SORT
    if (q-p <= base-1){
        for (int k = p; k < q; ++k) {
            for (int l = k+1; l <= q; ++l) {
                if (arr[k] > arr[l]){
                    swap(arr[k], arr[l]);
                }
            }
        }
        return;
    }

    int z = partition(arr, p,q);
    QuickSort(arr, p, z-1);
    QuickSort(arr, z+1, q);
}

int partition(vector<int> &arr, int p, int q){
    if(q-p <= base-1){
        fprintf(stderr, "BASE VIOLATE");
    }
    int i,j,pivot;
    pivot = arr[q];
    i = p;
    for (j = p; j < q; ++j) {
        if (arr[j] <= pivot){
            swap(arr[i], arr[j]);
            i++;
        }
    }
    swap(arr[i], arr[q]);
    return i;
}

int main(int argc, char* argv[]) {
    //cout << "Hello, World!" << endl;
    string line;
    string buf; // Have a buffer string
    int val;
    vector<int> a;
    //printf("\n");

    if(argc != 2){
        fprintf(stderr, "\nInvalid command line arguments\n");
        exit(EXIT_FAILURE);
    }
    base = atoi(argv[1]);

    while (getline(cin, line)) {
        a.push_back(atoi(line.c_str()));
    }
    long t0 = getMilliseconds();
    QuickSort(a, 0, a.size()-1);
    long t1 = getMilliseconds();
    //printf("\n Command line %s, %d , count %d\n", argv[0], stoi(argv[1]), argc);


    for (int i = 0; i < a.size(); ++i) {
        printf("%d\n", a[i]);
    }

    fprintf( stderr, "Time consumed = %ld ms\n", t1 - t0 );

    return 0;
}
