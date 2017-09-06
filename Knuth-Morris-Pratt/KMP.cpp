#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <map>
#include <queue>
#include <sys/time.h>

using namespace std;
int n,m;

void build_prefix(string p, int pi[]);

void naive(string t, string p){
    int i,j,k,flag=0;
    for(i=0;i<=n-m;i++){
        k=i;
        flag = 0;
        for(j=0;j<m; j++){
            if(p[j] != t[k]){
                flag = 1;
            }
            k++;
        }
        if(flag==0){
            cout<<endl<<"Naive Found match at: "<<i;
        }
    }
}

void kmp(string t, string p, int pi[]){
    build_prefix(p, pi);
    int q=-1;
    int i;
    for(i=0;i<=n-m;i++){
        while(q>=0 && p[q+1] != t[i]){
            q = pi[q];
        }

        if(p[q+1] == t[i]){
            q++;
        }

        if(q==(m-1)){
            cout<<endl<<"KMP Found match at: "<<i-m+1;
            q=pi[q];
        }
    }
}

void inbuilt(string t, string p){
    int k;
    k= t.find(p);
    cout<<endl<<"Inbuilt Found match at: "<<k;
}


void build_prefix(string p, int pi[]){
    pi[0] = -1;
    int k = -1;
    int i;
    for(i=1; i<m; i++){
        while(k>= 0 && p[k+1]!= p[i]){
            k = pi[k];
        }

        if(p[k+1] == p[i]){
            k++;
        }

        pi[i]=k;
    }
}

long getMilliseconds() {
    timeval tv;
    gettimeofday( &tv, NULL );
    long int ms = tv.tv_sec;
    ms = ms * 1000 + tv.tv_usec / 1000;
    return ms;
}

int main(int argc, char* argv[]) {
    std::cout << "Hello, World!" << std::endl;
    long t0,t1;
    string text, pattern;
    string random;
    char c;

    if(argc == 1){
        //generate random strings

        for (int i = 0; i < 9; ++i) {
            c = 97 + rand() % 26;
            random += c;
        }
	string tmp;
	tmp = random;

        for (int i = 0; i < 50; ++i) {
            random += tmp;
        }
        pattern = random;

        random = "a";
        for (int i = 0; i < 70000; ++i) {
            random += pattern.substr(0, pattern.size()-4);
            c = 97 + rand() % 26;
            random += c;
        }

	for (int i = 0; i < 70000; ++i) {
            c = 97 + rand() % 26;
            random += c;
        }

        for (int i = 0; i < 20000; ++i) {
            random += pattern.substr(0, pattern.size()-4);
            c = 97 + rand() % 26;
            random += c;
            random += c;
        }

        for (int i = 0; i < 2000000; ++i) {
            c = 97 + rand() % 26;
            random += c;
        }

        random += pattern;

        for (int i = 0; i < 50000; ++i) {
            c = 97 + rand() % 26;
            random += c;
        }



        text = random;
    }



    //text = "yaafqaafkdhaafqaaraafquqaafqaagaafsajaafqaaflazaafqakaafqaafaafqaafkaafqaayaazaaftaafqaafkaafqaaybaaf";
    //pattern = "aafqaafkaafqaayaazaaf";
    n = text.length();
    m = pattern.length();

    int pi[m],i,j,k;

    t0 = getMilliseconds();
    kmp(text,pattern,pi);
    t1 = getMilliseconds();
    cout<<endl<<"KMP Time : "<<(t1-t0);

    t0 = getMilliseconds();
    inbuilt(text, pattern);
    t1 = getMilliseconds();
    cout<<endl<<"Inbuilt Time : "<<(t1-t0);

    t0 = getMilliseconds();
    naive(text, pattern);
    t1 = getMilliseconds();
    cout<<endl<<"Naive Time : "<<(t1-t0);



    //cout<<endl<<random<<endl<<random.length();

    cout<<endl;
    return 0;
}
