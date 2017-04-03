#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <map>
#include <limits>

using namespace std;

int n;

vector<int> path; //to keep track of the path


void get_path(int *aMat, int *kMat,int i, int j){ //gets path in linear time
    int k;
    k = *(kMat + (i*n) + j); //get k-value from kMat
    if(k == -1){ //-1 : shortest path from i to j is direct edge only
        path.push_back(i); //add node to the path
        path.push_back(j); //add node to the path
        return; //that's the shortest path...return
    }
    if(k != -99){ //-99 represents no path from i to j...so if k!=99 and k!=-1 then there is path via some other node 'k'
        get_path(aMat, kMat, i, k); //Get shortest path from i to k
        //path.push_back(k);
        get_path(aMat, kMat, k, j); // get shortest path from j to k
    }
}



int main() {
    //cout << "Hello, World!" << endl;
    string line, buf, tmp;
    getline(cin, line); //Scan number of words
    n = atoi(line.c_str());
    int adjMat[n][n], kMat[n][n]; // adjMat[n][n] : Adjacency matrix;
    //kMat[n][n] : k matrix
    //kMat[i][j] = -1 : If shortest path from i to j is direct edge
    //kMat[i][j] = x : If shortest path from i to j goes from the node "x"
    //kMat[i][j] = -99 : If there is no path from i to j


    int i,j,k, len, flag1=0, w, m;

    map<string, int> word_map; // word_map : (word, index)...word to integer mapping ... index in the adjacency matrix... to keep track of the word in integer


    //Initialise matrices
    for(i = 0; i<n; i++){
        for(j=0; j<n; j++){
            adjMat[i][j] = 30000; //30000 represents infinite edge value
            kMat[i][j] = -99; //-99 represents no path from i to j
        }
    }

    vector<string> words;
    map<string, int>::iterator it;

    for (i=0; i<n; i++){
        getline(cin, line); //Scan words
        //line : newly scanned word

        //Scan and Construct Adjacency Matrix
        for (it = word_map.begin(); it != word_map.end(); ++it) { //traverse through all the previous words and check whether there can be an edge or not
            flag1 = 0;
            if(it->first.length() == line.length()){ //if length is not same, no need to check further
                len = line.length();
                tmp = it->first;
                j=0;

                while(tmp[j]==line[j] && j<len){ //go through the word till you find the first difference
                    j++;
                }

                if(j == len-1){ // check if only last character is different
                    w = abs(tmp[j] - line[j]);
                    flag1 = 1;
                    k = it->second;
                }

                if(j<len-1){ //check whether words have same letter after the single difference
                    w = abs(tmp[j] - line[j]);
                    j++;
                    while(tmp[j]==line[j] && j<len){
                        j++;
                    }
                    if(j==len){ //if only one difference then set the flag.
                        flag1 = 1;
                        k = it->second; //k : index of the word
                    }
                }
            }
            if(flag1 == 1){
                //cout<<endl<<tmp<<" & "<<line<<"   w:"<<w<<endl;
                adjMat[i][k] = w; //add edge to the adjacency matrix
                adjMat[k][i] = w; //because the graph is undirected graph
                kMat[i][k] = -1; // -1 represents direct edge
                kMat[k][i] = -1;
            }
        }
        word_map.insert(pair<string, int>(line, i)); //Insert to the list so that future inputs can be compared to
    }



    //set diagonals
    for(i=0; i<n; i++){
        adjMat[i][i] = 0;
        kMat[i][i] = i;
    }

    //Floyd-Warshall
    for(k=0; k<n; k++){
        for(i=0; i<n; i++){
            for(j=0; j<n; j++){

                if( (adjMat[i][k] + adjMat[k][j]) < adjMat[i][j] ){
                    adjMat[i][j] = adjMat[i][k] + adjMat[k][j];
                    adjMat[j][i] = adjMat[i][j];
                    kMat[i][j] = k;
                    kMat[j][i] = k;
                }
            }
        }
    }


    //compute average length of shortest path
    float sum=0;
    for(i=0; i<n; i++){
        for (j = 0; j<n; j++) {
            if(adjMat[i][j]!=30000){
                sum++;
            }
        }
    }
    printf("%.2f\n", sum/n); //Print average


    //----Queries----------
    int ind1, ind2;
    getline(cin, line); //Scan number of queries
    m = atoi(line.c_str());

    for(i=0; i<m; i++){
        getline(cin, line); //Scan words
        stringstream ss(line);
        vector<string> tokens;
        while (ss >> buf){
            tokens.push_back(buf);
        }


        for(it = word_map.begin(); it != word_map.end(); it++){ //go thorugh the word_map to get index for the given word
            if(it->first.compare(tokens[0]) == 0){ //for the first word in the query
                ind1 = it->second;
            }
            if(it->first.compare(tokens[1]) == 0){ //for the second word in the query
                ind2 = it->second;
            }
        }



        //Get shortest paths
        path.clear(); //clear path before finding path for the query
        if(adjMat[ind1][ind2] != 30000){ //30000 represents infinite edge and hence no path from i to j
            get_path(&adjMat[0][0], &kMat[0][0], ind1,ind2); //Get path from ind1 (word 1) to ind2 (word 2)

            vector<int> final_path; //To store path without duplicate entries...just a detail
                                    //because my implementation stores multiple entries of the same node in the variable "path"
                                    //It does not change runtime complexity of the algorithm because this step is linear time and floyd warshall is more expensive than that


            final_path.push_back(path[0]);
            for (int l = 1; l < path.size() ; ++l) {
                if(path[l] != path[l-1]){
                    final_path.push_back(path[l]);
                }
            }

            cout<<adjMat[ind1][ind2]; //print path cost
            for (int m = 0; m < final_path.size(); ++m) { //print the path-nodes
                for(it = word_map.begin(); it != word_map.end(); it++){ //traverse through word_map to get 'word' from integer index
                    if(it->second == final_path[m]){
                        cout<<" "<<it->first;
                    }
                }
            }
            cout<<endl;
        }
        else{
            cout<<tokens[0]<<" "<<tokens[1]<<" not reachable"<<endl; //if no path then print not reachable
        }

    }
    return 0;
}

/*
 * "kMat" keeps track of the nodes through which there is shortest path from i to j
 * so while getting shortest path using kMat, it results in linear time,
 * because at most we will go thorugh the number of vertices
 * unlike other recursive method in which we have to check every shortest path weight in adjecancy matrix to get the path
 */
