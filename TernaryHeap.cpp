#include <iostream>
#include <vector>
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <math.h>
#include <stdlib.h>

using namespace std;

void min_heapify(vector<int> &a, int index){
    int least,left,right,mid;

    left = 3*index +1;
    mid = 3*index +2;
    right = 3*index +3;

    //check for the left child
    if(index >= a.size()){
        return ;
    }
    least = index;
    //printf("\n left %d, mid %d, right %d", a[left], a[mid], a[right]);

    if (left < a.size() && a[left] < a[least]){
        //swap(a[index], a[left]);
        least = left;
    }

    if (mid < a.size() && a[mid] < a[least]){
        //swap(a[index], a[mid]);
        least = mid;
    }

    if (right < a.size() && a[right] < a[least]){
        //swap(a[index], a[right]);
        least = right;
    }
    if (least == index){
        //min_heapify(index+1);
        return;
    }
    else{
        swap(a[index], a[least]);
        min_heapify(a, least);
    }
}

void perc_up(vector<int> &a, int index){
    if(index == 0){
        return ;
    }
    int parent = floor((index-1) /3);
    if(a[index] < a[parent]){
        swap(a[index], a[parent]);
        perc_up(a, parent);
    }
}

void insert(vector<int> &a, int val){
    a.push_back(val);
    perc_up(a, a.size()-1);
}

int removeMin(vector<int> &a){
    int tmp;
    tmp = a[0];
    swap(a[0], a[a.size()-1]);
    a.pop_back();
    min_heapify(a, 0);
    return tmp;
}

int main() {

    string line;
    int i;
    string buf;
    int val;
    vector<int> arr;

    while (getline(cin, line)) {
        stringstream ss(line);
        vector<string> tokens;
        while (ss >> buf) {
            tokens.push_back(buf);
        }
        if (tokens[0].compare("add") == 0){
            //val = stoi(tokens[1]);
	    val = atoi(tokens[1].c_str());
            insert(arr, val);
        }

        if (tokens[0].compare("remove") == 0){
            cout<<removeMin(arr)<<endl;
            //swap(arr[0], arr[arr.size()-1]);
            //arr.pop_back();
            //min_heapify(arr, 0);
        }
    }





    return 0;
}
