#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <map>
#include <queue>

using namespace std;

int m,n, minimum; //m = edges, n = vertices
        // minimum : variable to store the minimum amount of commodity which can be transfered via given path

queue<int> q;
vector<int> path;


/*
 * bfs arguments ::
 * aMat : adjacency matrix
 * p[] : parents array....p[i] has the value which represents index of the parent-node of i
 *     : p[j] = i... means i is the parent of node j
 * vis[] : keeps track of visited nodes in bfs traversal
 * v : BFS start node
 */
void bfs(int *aMat, int p[], int vis[], int v){
    q.push(v); //Insert start node (source) into the queue
    vis[v] = 1; //mark v as visisted
    int i,j;
    while(q.size() > 0){ //while queue is not empty
        i = q.front(); //get the first element from the queue
        q.pop(); //delete the first element from the queue...because q.front() gets the value only..does not remove the element
        for (j = 0; j < n; j++) {
            if( (*(aMat + (i*n) + j)) > 0 && vis[j] != 1){ //if there's an edge from i to j...and j is not marked visited then insert j into the queue
                q.push(j);
                p[j] = i; //set parent of j to i
                vis[j] = 1;
            }
        }
    }
}

/* get_path() : recursively gets path...traces back from sink to source vgoing through parent nodes
 * v : source node
 * aMat : adjacency matrix
 * p[] : parents array....p[i] has the value which represents index of the parent-node of i
 *     : p[j] = i... means i is the parent of node j
 */
void get_path(int v, int *aMat, int p[]){
    int parent;
    parent = p[v];
    path.push_back(parent); //insert parent to the path
    int d;
    d = *(aMat + (parent*n) + v);
    if(d<minimum){ //to get the minimum capacity inside the path
        minimum = d;
    }
    if(parent == 0){
        return;
    }
    else{
        get_path(parent, aMat, p);
    }
}


int main() {
    //cout << "Hello, World!" << endl;
    int i,j,k;
    string line,buf,tmp;
    getline(cin, line); //Scan number of Endpoints(n) and number of directed edges(m)
    stringstream ss(line);
    vector<string> tokens;
    while (ss >> buf){
        tokens.push_back(buf);
    }
    n = atoi(tokens[0].c_str()); // no. of vertices
    m = atoi(tokens[1].c_str()); // no. of edges


    int adjMat[n][n], parent[n], visited[n];
    /*
     * adjMat[n][n] : adjacency matrix
     * adjMat[i][j] > 0 : amount of commodity which can be transferred from i to j
     * adjMat[j][i] < 0 : Negative values are used for residual graph. they are actually not negative
     *                    but they are just to differentiate them from original graph capacity
     *
     *                    hence adjMat[i][j] = 3 tells that still commodity amount of '3' can be transferred from 'i' to 'j'
     *                    adjMat[j][i] = -7 tells that commodity of '7' can be transferred from 'i' to 'j' at this moment
     *                          and hence edge j to i should have 7 in the residual graph. -7 tells that
     *
     *                          so before any bfs traversal or edmonds-karp iterations, adjMat[i][j] would have been 10
     *
     */
    int ind1, ind2, w;
    int ip[m][2]; //keeps track of order in which edges were given to input...so that it helps printing path edges in order of input


    //initilise all matrices
    for(i=0; i<n; i++){
        parent[i] = -1;
        visited[i] = -1;
        for(j=0; j<n; j++){
            adjMat[i][j] = 0;
        }
    }

    //Scan edges and construct adjecancy matrix
    for(i=0; i<m; i++){
        getline(cin, line);
        stringstream ss(line);
        vector<string> tokens;
        while (ss >> buf){
            tokens.push_back(buf);
        }

        ind1 = atoi(tokens[0].c_str()); // v1
        ind2 = atoi(tokens[1].c_str()); // v2
        w = atoi(tokens[2].c_str()); // weight
        adjMat[ind1][ind2] = w;
        ip[i][0] = ind1; //this was the ith input
        ip[i][1] = ind2;
    }



    minimum = 999;
    bfs(&adjMat[0][0], parent, visited, 0); // first bfs call...

    while(visited[0] == 1 && visited[1] == 1){ //cwhile untill there's path from source to sink i.e. after bfs call sink-node gets visited...i check visited[0] just for extra robustness
        minimum = 999; //reset minimum
        path.clear(); //clear the path
        path.push_back(1); //Push "Sink" to the path
        get_path(1, &adjMat[0][0], parent); //Get path from 'sink' to 'source'
        //after above call to get_path, 'minimum' will have the minimum edge weight i.e. maximum commodity which can be transffered from source to sink via this path

        /*
         * 'path' is constructed in the following way :
         * sink- node1(parent of sink) - node2(parent of node 1)-...-node n-source(parent of node n)
         *
         * so we have to reduce capacity along this path i.e. edges (node1->sink, node2->node1,..., source->node n)
         */

        //reduce edge-weights of edges along this path and update residual graph
        for(i=path.size()-1; i>0; i--){ //we starting with last element in 'path' i.e. from the source...because source will be the last one to get pushed into 'path'
            ind1 = path[i];
            ind2 = path[i-1];
            adjMat[ind1][ind2] -= minimum; //remaining capacity
            adjMat[ind2][ind1] -= minimum; //residual graph entry...which is initially zero and hence it would become negative with every iteration
        }

        //reset parent[] and visite[] for next bfs traversal
        for(i=0; i<n; i++){
            parent[i] = -1;
            visited[i] = -1;
        }
        bfs(&adjMat[0][0], parent, visited, 0); //next bfs call
    }

    //to get the maximum flow value...sum up all 'outgoing' edge weights from sink in residual graph
    int flow=0;
    for (i = 0; i < n; i++) {
        flow += adjMat[1][i];
    }

    cout<<(-1*flow); // multiply by -1 ...because residual graph entries are negative and hence sum would be negative

    //print path edges..in order of input ...here we I use the 2D array ip[m][]
    for(i=0; i<m; i++){
        //here input edge is from ip[i][0]->ip[i][1]...but the commodity flowing through this edge is stored in adjMat in reverse order i.e. negative of edge ip[i][1]->ip[i][0]
        ind1 = ip[i][1]; //get the first edge from the input
        ind2 = ip[i][0];

        flow = (-1 * adjMat[ind1][ind2]); //because residual flow is negative and hence multiply by -1
        printf("\n%d %d %d",ind2, ind1, flow); //print
    }

    return 0;
}