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

/*
"Events" are :
                A) When line's left endpoint intersects with sweep-line
                B) When line's right endpoint intersects with sweep-line
                C) When intersection point intersects with sweep-line

When Event occurs we are required to update "sweep_status" and check for intersection_set
"sweep_status" contains vertically sorted line segments which are intersecting with sweep-line
*/


int n; //n : total number of line segments
float sweep_x; //x-coordinate of sweep-line
vector<int> event_heap; //heap data structure to be used for events so that insert,delete complexity are of  'log n'
multimap<float,int> sweep_status; // sweep_status : (y-value, line_no) : stores the vertically sorted line segments
multimap<float, int>::iterator ssit; //iterator to traverse through the sweep_status
set<pair<int,int> > intersection_set; //stores all the pairs of lines which intersects so that if line-pair is already in the set we don't check for the intersection


/* Structures Description
  line_segs : Stores left and right and point of every line segment
              lx : x value of left end point
              ly : y value of left end point
              rx : x value of right end point
              ry : y value of right end point

event_aux : To store auxillary information associated with the events such as type of the event,
            point(x,y) where event occurs so that it can be used to update sweep_status
            l : which stores line number so that line-'l' can be inserted to vertically sorted list i.e. sweep_status

*/

struct line_segs{
    float lx,ly,rx,ry;
};

struct event_aux{
    int type; //1.left endpoint (insert), 2.right endpoint(remove)
    float x;
    float y;
    int l;
};

/* event_heap : This is a min-heap. And it has the x-coordinate as the key.
                So events such that occurence of left-endpoint, right-endpoint, intersection points
                are sorted by x-coordinate.

                If x-coordinate are same then y-coordinate is considered
*/

//min_heapify to be used for "event_heap"
void min_heapify(vector<event_aux> &eah, int index){
    int least, left, right;

    left = (2 * index) + 1;
    right = (2 * index) + 2;

    if(index >= event_heap.size()){
        return;
    }
    least = index;
    if(left < event_heap.size() && eah[event_heap[left]].x < eah[event_heap[least]].x){
        least = left;
    }
    if(right < event_heap.size() && eah[event_heap[right]].x < eah[event_heap[least]].x){
        least = right;
    }
    if(left < event_heap.size() && eah[event_heap[left]].x == eah[event_heap[least]].x){
        if(eah[event_heap[left]].y < eah[event_heap[least]].y){
            least = left;
        }
    }
    if(right < event_heap.size() && eah[event_heap[right]].x == eah[event_heap[least]].x){
        if(eah[event_heap[right]].y < eah[event_heap[least]].y){
            least = right;
        }
    }

    if(least == index){
        return ;
    }
    else{
        swap(event_heap[least], event_heap[index]);
        min_heapify(eah, least);
    }
}


//Get next event from the heap. It will always give the left-most event
int remove_min(vector<event_aux> &eah){
    int tmp;
    tmp = event_heap[0];
    swap(event_heap[0], event_heap[event_heap.size()-1]);
    event_heap.pop_back();
    min_heapify(eah, 0);
    return tmp;
}

//check for the intersection between 'line1' and 'line2'
void intersection(struct line_segs lsh[], int line1, int line2, vector<event_aux> &eah){


    // If line1 and line2 are already in the intersection list, dont check
    set<pair<int,int> >::iterator isit;
    isit = intersection_set.find(pair<int,int>(line1,line2));
    if(isit != intersection_set.end()){
        return;
    }

    isit = intersection_set.find(pair<int,int>(line2,line1));
    if(isit != intersection_set.end()){
        return;
    }

    //to find the intersection Point
    float a1,a2,b1,b2,c1,c2, det, ix,iy,t1,t2,f=0;
    a1 = lsh[line1].ry - lsh[line1].ly;
    b1 = lsh[line1].lx - lsh[line1].rx;
    c1 = (a1 * lsh[line1].lx) + (b1 * lsh[line1].ly);

    a2 = lsh[line2].ry - lsh[line2].ly;
    b2 = lsh[line2].lx - lsh[line2].rx;
    c2 = (a2 * lsh[line2].lx) + (b2 * lsh[line2].ly);

    det = (a1*b2) - (a2*b1);
    ix = ((b2*c1) - (b1*c2))/det;
    iy = ((a1*c2) - (a2*c1))/det;


    //check whether intersection point is on one of the linesegments or not
    if(min(lsh[line1].lx, lsh[line1].rx) <= ix && ix <= max(lsh[line1].lx, lsh[line1].rx)){
        if(min(lsh[line1].ly, lsh[line1].ry) <= iy && iy <= max(lsh[line1].ly, lsh[line1].ry)){
            if(min(lsh[line2].lx, lsh[line2].rx) <= ix && ix <= max(lsh[line2].lx, lsh[line2].rx)){
                if(min(lsh[line2].ly, lsh[line2].ry) <= iy && iy <= max(lsh[line2].ly, lsh[line2].ry)){

                    //if intersection point is on one of the line segment then print it on the screen
                    // also add these pair of lines to the intersection list so that we can avoid checking next time
                    printf("%.2f %.2f\n", ix,iy);
                    intersection_set.insert(pair<int,int>(line1,line2));
                    eah.push_back(event_aux());
                    int eat;
                    eat = eah.size()-1;
                    eah[eat].type = 3;
                    eah[eat].x = ix + 0.1; //Just the detail to swap the intersecting line in sweep_status after the intersection
                    eah[eat].y = iy;
                    eah[eat].l = -1;
                    event_heap.push_back(eat);
                }
            }
        }
    }
}


//traverse through sweep_status and check for intersection between two adjecent lines
void check_intersection(struct line_segs lsh[], vector<event_aux> &eah){
    if(sweep_status.size()<2){
        return;
    }
    for(ssit=sweep_status.begin(); ssit!=next(sweep_status.end(),-1); ssit++){
        intersection(lsh, ssit->second, next(ssit,1)->second, eah);
    }
}


//if new event occurs we are supposed to check vertical order of the lines
void update_sweep_status(struct line_segs lsh[]){
    float m,ny;
    int l;
    multimap<float,int> sweep_status_copy;
    sweep_status_copy = sweep_status;
    sweep_status.clear();
    for(ssit=sweep_status_copy.begin(); ssit!=sweep_status_copy.end(); ssit++){
      //sweep-line must have move ahead and y-coordinates of intersection of lines with sweep line might have changed
      //so we first update those intersecting coordinates so that we get updated vertically sorted line segments list
        l = ssit->second;
        m = (lsh[l].ry - lsh[l].ly)/(lsh[l].rx - lsh[l].lx);
        ny = (m * (sweep_x - lsh[l].lx)) + lsh[l].ly;

        sweep_status.insert(pair<float,int>(ny, l));
    }
}



int main() {
    //std::cout << "Hello, World!" << std::endl;
    int i,j,k, eat;
    string line, buf;
    getline(cin, line);
    n = atoi(line.c_str());
    struct line_segs ls[n];
    vector<event_aux> ea; //stores the auxillary information of event_heap

    multimap<float, int>::iterator tmp;
    float t1,t2;


    //Scanning of line segments endpoints
    for (i = 0, j=n; i < n; i++, j++) {
        getline(cin, line);
        stringstream ss(line);
        vector<string> tokens;
        while (ss >> buf){
            tokens.push_back(buf);
        }
        t1 = atoi(tokens[0].c_str());
        t2 = atoi(tokens[2].c_str());

        if(t1<t2){
            ls[i].lx = atoi(tokens[0].c_str());
            ls[i].ly = atoi(tokens[1].c_str());
            ls[i].rx = atoi(tokens[2].c_str());
            ls[i].ry = atoi(tokens[3].c_str());
        } else{
            ls[i].lx = atoi(tokens[2].c_str());
            ls[i].ly = atoi(tokens[3].c_str());
            ls[i].rx = atoi(tokens[0].c_str());
            ls[i].ry = atoi(tokens[1].c_str());
        }

        //each end point is an event so add it to the heap;
        ea.push_back(event_aux());
        eat = ea.size()-1;
        ea[eat].type = 1;
        ea[eat].x = ls[i].lx;
        ea[eat].y = ls[i].ly;
        ea[eat].l = i;
        event_heap.push_back(eat);

        ea.push_back(event_aux());
        eat = ea.size()-1;
        ea[eat].type = 2;
        ea[eat].x = ls[i].rx;
        ea[eat].y = ls[i].ry;
        ea[eat].l = i;
        event_heap.push_back(eat);


    }

    //construct the heap
    for(i = n; i>=0; i--){
        min_heapify(ea, i);
    }


    //get events from event_heap and act accordingly
    for(i=0; i<ea.size(); i++){
        k=remove_min(ea);

        //if left-endpoint occured, add line to the sweep_status
        if(ea[k].type == 1){
            sweep_x = ea[k].x;
            update_sweep_status(ls); //update the sweep_status first based on the current position of sweep-line
            sweep_status.insert(pair<float, int>(ls[ea[k].l].ly, ea[k].l));
            check_intersection(ls, ea); //check for intersections
        }

        //if right-endpoint occured, remove the line from the sweep_status, ea[k].l contains line number
        if(ea[k].type == 2){
            sweep_x = ea[k].x;
            update_sweep_status(ls); //update the sweep_status first based on the current position of sweep-line
            multimap<float,int> sweep_status2;
            sweep_status2 = sweep_status;
            for (ssit = sweep_status.begin(); ssit != sweep_status.end() ; ssit++) {
                if(ssit->second == ea[k].l){
                    tmp = ssit;
                    break;
                }
            }
            sweep_status.erase(tmp); //erase the line segment
            check_intersection(ls, ea); //check for the intersections
        }

        //if intersection occured, update the sweep_status based on the current position of sweep-line
        if(ea[k].type == 3){
            sweep_x = ea[k].x;
            update_sweep_status(ls);
            check_intersection(ls, ea); //and check for the intersections
        }
    }

    //cout<<endl;
    return 0;
}
