#include <iostream>
#include <cstdio>
#include <cmath>
//#include <random>
#include <stdlib.h>
#include <fstream>
#include<stdio.h>
#include<time.h>

using namespace std;

struct event_list{
    int type; // 1:Arrival, 2:Retransmission, 3:Service Completion
    float val;
    int id;
    float ST, D;
    struct event_list *next,*prev;
}*head = NULL, *endd = NULL;

struct complete_list{
    int id;
    float ST,CT,D, T;
    struct complete_list *next, *prev;
}*chead, *cendd;

struct queue{
    int id;
    float ST;
    float D;
    struct queue *next, *prev;
}*qhead, *qendd;


FILE *ofile, *ofile2, *ofile3;
float mc = 0, omc;
float iat=6, mot=5, st=10, b=2, mct=100, idc = 0;
float mat, mrt;
int q=0, arr_counter = 0, complete_counter=0;
int buff[2];
float buff_val[2];
/* iat = inter arrival time, mot = mean orbitting time
 * st = service time, b = buffer size, mct = master clock terminate
 * q = current queue state */

void print();

void insert(int type, float v, int id, float ST, float D){

    struct event_list *tmp2;


    if(head == NULL){
        head = (struct event_list *)malloc(sizeof(struct event_list));
        head->type = type;
        head->val = v;
        head->id = id;
        head->ST = ST;
        head->D = D;
        head->next = NULL;
        endd = head;
        head->prev = NULL;
        return ;
    }

    struct event_list *tmp;
    tmp = (struct event_list *)malloc(sizeof(struct event_list));
    tmp->type = type;
    tmp->val = v;
    tmp->id = id;
    tmp->ST = ST;
    tmp->D = D;

    tmp2 = head;
    while((tmp2 != NULL) && (tmp2->val < v)){
        tmp2 = tmp2->next;
    }

    if(tmp2==NULL){
        endd->next = tmp;
        tmp->next = NULL;
        tmp->prev = endd;
        endd = tmp;
    }else{
        while((tmp2 != NULL) && (tmp2->val == v && tmp2->type <= type)){
            tmp2 = tmp2->next;
        }

        if (tmp2 == head){
            printf("\n HEAD HIT %.0f %d", idc, complete_counter);
            printf("\n HEAD id : %d, Type : %d, ST: %.2f, V: %.2f", head->id, head->type, head->ST, head->val);
            printf("\n Insert id : %d, Type : %d, ST: %.2f, V: %.2f", id, type, ST, v);
            head->prev = tmp;
            tmp->next = head;
            tmp->prev = NULL;
            head = tmp;
            return ;
        }

        if(tmp2 == NULL){
            endd->next = tmp;
            tmp->next = NULL;
            tmp->prev = endd;
            endd = tmp;
        }
        else{
            tmp2->prev->next = tmp;
            tmp->prev = tmp2->prev;
            tmp->next = tmp2;
            tmp2->prev = tmp;
        }
    }
}


void complete_insert(int id, float ST, float CT, float D){
    complete_counter++;
    if (chead == NULL){
        chead = (struct complete_list *)malloc(sizeof(struct complete_list));
        chead->id = id;
        chead->ST = ST;
        chead->CT = CT;
        chead->D = D;
        chead->T = CT - ST;
        chead->next = NULL;
        chead->prev = NULL;
        cendd = chead;
        return;
    }

    struct complete_list *tmp;
    tmp = (struct complete_list *)malloc(sizeof(struct complete_list));
    tmp->id = id;
    tmp->ST = ST;
    tmp->CT = CT;
    tmp->D = D;
    tmp->T = CT - ST;
    cendd->next = tmp;
    tmp->prev = cendd;
    cendd = tmp;
}

void queue_insert(int id, float ST, float D){
    if(qhead == NULL){
        qhead = (struct queue *)malloc(sizeof(struct queue));
        qhead->id = id;
        qhead->ST = ST;
        qhead->D = D;
        qhead->next = NULL;
        qhead->prev = NULL;
        qendd = qhead;
        return ;
    }

    struct queue *tmp;
    tmp  = (struct queue *)malloc(sizeof(struct queue));
    tmp->id = id;
    tmp->ST = ST;
    tmp->D = D;
    tmp->next = NULL;
    qendd->next = tmp;
    tmp->prev = qendd;
    qendd = tmp;
}

int get_q_size(){
    if(qhead == NULL){
        return 0;
    }
    int count=0;
    struct queue *tmp;
    tmp = qhead;
    while(tmp != NULL){
        count++;
        tmp= tmp->next;
    }
    return count;
}

void print_queue(){
    if(qhead == NULL){
        printf("\n QUEUE is empty");
        return ;
    }
    int count = 0;
    struct queue *tmp;
    tmp = qhead;
    while(tmp != NULL){
        printf("\nCount : %d, ID : %d, ST : %.2f, D : %.2f", count, tmp->id, tmp->ST, tmp->D);
        count++;
        tmp= tmp->next;
    }
}



void get_event(){

    if(head == NULL){
        printf("Something's not right %.0f %d", idc, complete_counter);
        return ;
    }
    else{
        print();
        mc = head->val;
        float r;
        r = ((double) rand() / (RAND_MAX));
        iat = (float) ((log(r)) * (-mat));
        mot = (float) ((log(r)) * (-mrt));
        //printf("\n r : %f, IAT : %f, MOT : %f", r, iat, mot);

        switch (head->type){
            case 1: //Customer arrived
                if(idc < 1001) {
                    insert(1, mc + iat, ++idc, mc + iat, 0); //add next arrival in event list
                    if (q == 0) {
                        q++;
                        queue_insert(head->id, head->ST, head->D);
                        insert(3, mc + st, head->id, head->ST, head->D); //add service completion(departure) in event list
                    } else if (q < b) { //Queue still has one place
                        queue_insert(head->id, head->ST, head->D);
                        q++; //add arrival in queue
                    } else {
                        insert(2, mc + mot, head->id, head->ST,
                               head->D + mot); //queue is full, add retransmission event
                    }
                }
                break;

            case 2: //retransmission arrived
                if(q == 0){
                    q++;
                    queue_insert(head->id, head->ST, head->D);
                    insert(3, mc+st, head->id, head->ST, head->D); //add service completion(departure) in event list
                }
                else if(q < b){
                    q++;
                    queue_insert(head->id, head->ST, head->D);
                }
                else{
                    insert(2, mc+mot, head->id, head->ST, head->D + mot); //queue is full, add retransmission event
                }
                break;

            case 3: //service completion
                q--;
                qhead = qhead->next;
                if (qhead != NULL){
                    qhead->prev = NULL;
                }
                complete_insert(head->id, head->ST, mc, head->D); //The guy departed
                if(q>0){
                    insert(3, mc+st, qhead->id, qhead->ST, qhead->D); //add next service completion(departure) in event list
                    // insert(3, mc+st, buff[1], buff_val[1], 0);
                }
                break;

            default:
                printf("\nSomething's not right SWITCH");
                break;
        }

       // printf("\n %d, %d, %.2f, %.2f",buff[0],buff[1],buff_val[0], buff_val[1]);
       // printf("\n-------------------------");
       // print_queue();
       // printf("\n-------------------------");
        head = head->next;
        if(head != NULL){
            head->prev = NULL;
        }

    }

}

void print_complete(){
    struct complete_list *tmp;
    if (chead == NULL){
        printf("\nIt's empty");
        return ;
    }
    tmp = chead;
    while(tmp != NULL) {
        //printf("\n ID : %d, ST : %.2f, CT : %.2f, D : %.2f, T : %.2f", tmp->id, tmp->ST, tmp->CT, tmp->D, tmp->T);
        fprintf(ofile2, "\n%d, %.2f, %.2f, %.2f, %.2f", tmp->id, tmp->ST, tmp->CT, tmp->D, tmp->T);
        tmp = tmp->next;
    }
}

void traverse(){

    if(head == NULL){
        printf("List is empty");
        return ;
    }
    struct event_list *tmp;
    tmp = (struct event_list *)malloc(sizeof(struct event_list));
    tmp = head;
    while(tmp != NULL){
        printf("\nType : %d\t Time : %f", tmp->type, tmp->val);
        tmp = tmp->next;
    }
    return ;
}

void print(){
    float cla, cls;
    struct event_list *tmp;
    tmp = (struct event_list *)malloc(sizeof(struct event_list));
    tmp = head;
    if(tmp == NULL){
        printf("\nIt's empty down here");
        return ;
    }

    while(tmp != NULL && tmp->type != 1){
        tmp = tmp->next;
    }
    if(tmp == NULL){
        cla = -99;
    }
    else{
        cla = tmp->val;
    }

    tmp = head;
    while(tmp != NULL && tmp->type != 3){
        tmp = tmp->next;
    }
    if(tmp == NULL){
        cls = -99;
    }
    else{
        cls = tmp->val;
    }


    if(cls == -99){
        fprintf(ofile, "\n%.2f\t%.2f\tIDLE\t%d", mc, cla, q);

    }
    else{
        fprintf(ofile, "\n%.2f\t%.2f\t%.2f\t%d", mc, cla, cls, q);
    }

    tmp = head;

    while(tmp != NULL){
        if (tmp->type == 2){
            fprintf(ofile, "\t%.2f", tmp->val);
        }

        tmp = tmp->next;
    }
}




int main() {

    ofile = fopen("output.txt", "w");
    ofile2 = fopen("output_complete.txt", "w");
    ofile3 = fopen("output_p.txt", "w");

    time_t t;
    srand((unsigned) time(&t));


    printf("\nEnter Mean Arrival Time (1/lambda) : ");
    cin>>mat;
    printf("\nEnter Mean Retransmission Time (1/D) : ");
    cin>>mrt;
    printf("\nEnter Service Time : ");
    cin>>st;
    printf("\nEnter Buffer Size : ");
    cin>>b;

    /*
    mat = 17.98;
    mrt = 10;
    st = 16;
    b = 35;
    */

    //printf("\nEnter Master CLock Termination Time : ");
    //cin>>mct;

    printf("\nNote : It is assumed that first arrival occurs at MC=2, so the first line is initial condition\n");
    printf("\nOUTPUT : output.txt (Complete Simulation - 50 Iterations)");
    printf("\nOUTPUT : output_complete.txt (Customer Information :- Arrival Time, Departure Time, Total Time  for 50 Iterations)");
    printf("\nOUTPUT : output_p.txt (Total Simulation Time :- P  for 50 Iterations)");

    if(mc == 0){
        insert(1, 2, ++idc, 2, 0);
    }

    fprintf(ofile,"\nMC\tCLA\tCLS\tQueue\tCLR");
    fprintf(ofile2, "ID,ST, CT,D, T");

    while(complete_counter<1000){
        //printf("\nWHILE");
        get_event();
    }

    print_complete();
    //fprintf(ofile3, "\n%.2f", mc);

    int N = 0;
    for (N = 0; N<49; N++){
        fprintf(ofile, "%f\n\n", mc);
        fprintf(ofile2, "\n");
        fprintf(ofile3, "\n%.2f", mc);
        srand(((unsigned) time(&t)) + (N+1));
        idc = 0;
        mc = 0;
        complete_counter = 0;
        q=0;
        head = endd = NULL;
        qhead = qendd = NULL;
        chead = cendd = NULL;


        if(mc == 0){
            //printf("\n MC = 0, N = %d", N+2);
            insert(1, 2, ++idc, 2, 0);
        }

        fprintf(ofile,"\nMC\tCLA\tCLS\tQueue\tCLR");

        while(complete_counter<1000){
            //printf("\nWHILE");
            get_event();
        }

        print_complete();
    }


    fprintf(ofile, "%f\n\n", mc);
    fprintf(ofile2, "\n");
    fprintf(ofile3, "\n%.2f", mc);










    cout<<endl;
    return 0;
}
