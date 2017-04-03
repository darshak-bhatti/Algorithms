#include <iostream>
#include <vector>
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <math.h>

using namespace std;

struct symbol_list{
    int hex=-1;
    int freq=1;
    int huffcode=-1;
    int left=-1;
    int right=-1;
} top;

struct huff_list{
    int code=-1;
    int hex=-1;
    int left=-1;
    int right=-1;
    int parent=-1;
};

vector<huff_list> h;

vector<int> bfs;

int counter = 256;

void min_heapify(vector<symbol_list> &a, int index){
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

    if (left < a.size() && a[left].freq < a[least].freq){
        //swap(a[index], a[left]);
        least = left;
    }

    if (mid < a.size() && a[mid].freq < a[least].freq){
        //swap(a[index], a[mid]);
        least = mid;
    }

    if (right < a.size() && a[right].freq < a[least].freq){
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

void perc_up(vector<symbol_list> &a, int index){
    if(index == 0){
        return ;
    }
    int parent = floor((index-1) /3);
    if(a[index].freq < a[parent].freq){
        swap(a[index], a[parent]);
        perc_up(a, parent);
    }
}

void insert(vector<symbol_list> &a, int sn, int f, int l, int r){
    a.push_back(symbol_list());
    int tmp = a.size() - 1;
    a[tmp].hex = sn;
    a[tmp].freq = f;
    a[tmp].left = l;
    a[tmp].right = r;

    perc_up(a, a.size()-1);
}

symbol_list removeMin(vector<symbol_list> &a, int flag){
    top = a[0];
    swap(a[0], a[a.size()-1]);
    a.pop_back();
    min_heapify(a, 0);

    h[top.hex].hex = top.hex;
    h[top.hex].code = flag;
    h[top.hex].left = top.left;
    h[top.hex].right = top.right;
    h[top.hex].parent = counter;

    return top;
}

void combine(vector<symbol_list> &a){
    symbol_list tmp1,tmp2;

    tmp1 = removeMin(a,0);
    tmp2 = removeMin(a,1);

    insert(a, counter++, tmp1.freq+tmp2.freq, tmp1.hex, tmp2.hex);
    h.push_back(huff_list());

}

void gen_huffman(int n){
    if(h[n].parent == h[h.size()-1].hex){
        printf("%d", h[n].code);
        return;
    }
    gen_huffman(h[n].parent);
    printf("%d", h[n].code);

}



int main() {
    //cout << "Hello, World!" << std::endl;
    vector<symbol_list> arr;
    /*
    insert(arr, 0, 27, -1, -1);
    insert(arr, 1, 9, -1, -1);
    insert(arr, 2, 8, -1, -1);
    insert(arr, 3, 12, -1, -1);
    insert(arr, 4, 28, -1, -1);
    insert(arr, 5, 4, -1, -1);
    insert(arr, 6, 5, -1, -1);
    insert(arr, 7, 7, -1, -1);
    */

    int a[256]={0}, tmp;
    char c;
    while((c = getchar()) != EOF){
        tmp = c;
        a[tmp]++;
    }

    for (int m = 0; m < 256; ++m) {
        a[m]++;
        insert(arr, m, a[m], -1,-1);
    }



    for (int j = 0; j < 256 ; ++j) {
        h.push_back(huff_list());
    }

    while (arr.size() > 1){
        combine(arr);
    }

    h[arr[0].hex].hex = arr[0].hex;
    h[arr[0].hex].left = arr[0].left;
    h[arr[0].hex].right = arr[0].right;

    bfs.push_back(arr[0].left);
    bfs.push_back(arr[0].right);

    int l =0;
    for (l = 0; l <32 ; ++l) {
        printf(" %.2X ",l);
        gen_huffman(l);
        cout<<endl;
    }
    printf(" %.2X ",l);
    gen_huffman(l);
    for (l = 33; l < 127; ++l) {
        printf("\n %c ",l);
        gen_huffman(l);
    }
    for (l = 127; l <256 ; ++l) {
        printf("\n %X ",l);
        gen_huffman(l);
    }

    cout<<endl;

    return 0;
}