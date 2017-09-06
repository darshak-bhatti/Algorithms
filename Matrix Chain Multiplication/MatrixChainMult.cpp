s#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int n=7;
string ans;

void print(int i, int j, int *km){
    char tmp[50];
    if( i == j ){
        //printf("M%d ", i+1);
        sprintf(tmp, "M%d ", i+1);
        ans.append(tmp);
        return ;
    }
    int k;
    //k = km[i][j];
    k = *(km + (i*n) + j);

    if (i == 0 && j == n-1){
        print(i, k, km);
        //printf("* ");
        ans.append("* ");

        print(k+1, j, km);
    }else {
        //printf("( ");
        ans.append("( ");
        print(i, k, km);
        //printf("* ");
        ans.append("* ");
        print(k + 1, j, km);
        //printf(") ");
        ans.append(") ");
    }
}


int main() {
    //cout << "Hello, World!" << endl;
    string line;
    getline(cin,line);
    n=atoi(line.c_str());

    int m[n][n], km[n][n];
    int p[n+1];
    int i,j,k,c, min=0, min_k, tmp;

    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            m[i][j] = 0;
            km[i][j] = 0;
        }
    }
    i = 0;
    while(getline(cin, line)){
        p[i] = atoi(line.c_str());
        i++;
    }


    c=1;
    for(i=n-1; i>0; i--){
        for(j=0; j<i; j++){
            //printf("\n K %d-%d : ", j, j+c);
            k = j;
            min = -99;
            for(k=j; k<j+c; k++){
                //printf(" %d ", k);
                tmp = m[j][k] + m[k+1][j+c] + p[j]*p[k+1]*p[j+c+1];
                if (tmp < min || min == -99){
                    min = tmp;
                    min_k = k;
                }
            }
            m[j][j+c] = min;
            km[j][j+c] = min_k;
        }
        c++;
    }


    print(0,n-1, &km[0][0]);
    ans.pop_back();
    cout<<ans;
    cout<<endl;
    return 0;
}
