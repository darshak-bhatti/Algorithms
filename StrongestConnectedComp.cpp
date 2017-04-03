#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <map>

using namespace std;

vector<int> stack;
int c=0,n;

void dfs(int *aMat, int visited[], int ct[], int x, int v){
    visited[x] = v;
    //stack.push_back(x);
    int tmp, i;

    c++;
    tmp = x;
    //tmp = stack[stack.size() - 1];
    //stack.pop_back();


    for (i = 0; i < n; ++i) {
        if(*(aMat + (tmp*n) + i) == 1 && visited[i] == 0){
            visited[i] = v;
            dfs(aMat, visited, ct, i, v);
            //stack.push_back(i);
        }
    }
    ct[tmp] = c;
    c++;
}


int main() {
    //cout << "Hello, World!" << endl;
    int m,i,j,k,tmp;
    string line, buf;
    getline(cin, line); //Scan number of topics
    n = atoi(line.c_str());
    int adjMat[n][n], revAdjMat[n][n];
    int ct[n], visited[n], revVisited[n], rct[n];


    for (i = 0; i < n; ++i) {
        visited[i] = 0;
        revVisited[i] = 0;
        ct[i] = 0;
        rct[i] = 0;
        for (j = 0; j < n; ++j) {
            adjMat[i][j] = 0;
        }
    }


    vector<string> topics;
    int ind1,ind2;



    for (i=0; i<n; i++){
        getline(cin, line); //Scan topics
        topics.push_back(line);
    }

    getline(cin, line); //Scan number of dependencies
    m = atoi(line.c_str());


    for (i=0; i<m; i++){
        getline(cin, line); //Scan topics
        stringstream ss(line);
        vector<string> tokens;
        while (ss >> buf){
            tokens.push_back(buf);
        }
        ind1 = distance(topics.begin(), find(topics.begin(), topics.end(), tokens[0]));
        ind2 = distance(topics.begin(), find(topics.begin(), topics.end(), tokens[1]));
        adjMat[ind1][ind2] = 1;
    }

    for (int j1 = 0; j1 < n; ++j1) {
        //cout<<topics[j1];
    }

    //----------------Reverse edges matrix-------------
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            revAdjMat[j][i] = adjMat[i][j];
        }
    }

    // -------------------DFS-------------------
    dfs(&adjMat[0][0], visited, ct, 0, 1);
    for (i = 0; i < n; ++i) {
        if(visited[i] == 0){
            dfs(&adjMat[0][0], visited, ct, i, 1);
        }
    }

    map<int,int> tmap; //(completion time, node)
    map<int,int>::iterator it;

    for (i = 0; i < n; ++i) {
        tmap.insert(pair<int, int>(ct[i], i)); //collection of all completion time sorted by time
    }
/*
    for(it = tmap.begin(); it!= tmap.end(); it++){
        cout<<"\nTime  : "<<it->first<<" Node : "<<it->second;
    }
*/

    //------------------------DFS on reversed graph---------------
    i = 0;
    for (it = tmap.end(); it != tmap.begin();) {
        it--;
        if(revVisited[it->second] == 0){
            i++;
            dfs(&revAdjMat[0][0], revVisited, rct, it->second, i);
        }
    }


    //----------------get Strongly connected components------------
    vector<int> grps;
    int git;
    int done[i];
    int chck;

    for (int k1 = 0; k1 <= i; ++k1) {
        done[k1] = -1;
    }

    //cout<<endl;
    for (int l = 0; l <= n; ++l) {
        chck = revVisited[l];
        if (done[chck] == -1){
            done[chck] = 1;
            grps.clear();
            for (int i1 = 0; i1 < n; ++i1) {
                if(revVisited[i1] == chck){
                    grps.push_back(i1);
                }
            }
            if (grps.size() > 1){
                for(git = 0; git < grps.size()-1; git++){
                    cout<<topics[grps[git]]<<" ";
                }
                cout<<topics[grps[git]];
                printf("\n");
            }
        }
    }

    return 0;
}