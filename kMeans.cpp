#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <set>
#include <iterator>

using namespace std;

struct Point {
    string name;
    int cluster;
    double x;
    double y;
};

double sim(Point a, Point b);
void init(vector<Point> &S, int k, vector<Point> &c);
void run(vector<Point> &S, int k, vector<Point> &c);
int argmin(int k, int i, vector<Point> S, vector<Point> c);
void init2(vector<Point> &S, int k, vector<Point> c);
vector<Point> copyVector(vector<Point> v);
void showVector(vector<Point> v);
void showClusters(vector<Point> S);
void populate(vector<Point> &S);

int main() {
    srand(time(NULL));
    vector<Point> S;
    vector<Point> A;
    vector<Point> c;
    int k =3;

    cout << "Round 1: Using A1, A7, A8 as initial clusters.\n";
    populate(S);
    c.push_back(S[0]);
    c.push_back(S[6]);
    c.push_back(S[7]);

    init2(S, k, c);
//    showVector(S);
//    cout << "\nrun\n";
    run(S, k, c);
//    showVector(S);
    showClusters(S);
    S.clear();
    c.clear();


    cout << "\nRound 2: Using A2, A6, A8 as initial clusters.\n";
    populate(S);
    c.push_back(S[1]);
    c.push_back(S[5]);
    c.push_back(S[7]);

    init2(S, k, c);
//    showVector(S);
//    cout << "\nrun\n";
    run(S, k, c);

//    showVector(S);
    showClusters(S);
    S.clear();
    c.clear();

    cout << "\nRound 3: Using A3, A5, A6 as initial clusters.\n";
    populate(S);
    c.push_back(S[2]);
    c.push_back(S[4]);
    c.push_back(S[5]);

    init2(S, k, c);
//    showVector(S);
//    cout << "\nrun\n";
    run(S, k, c);

//    showVector(S);
    showClusters(S);
    S.clear();
    c.clear();

    cout << "\nRound 4: Using A2, A3, A7 as initial clusters.\n";
    populate(S);
    c.push_back(S[1]);
    c.push_back(S[2]);
    c.push_back(S[6]);

    init2(S, k, c);
//    showVector(S);
//    cout << "\nrun\n";
    run(S, k, c);
//    showVector(S);
    showClusters(S);
    S.clear();
    c.clear();

    cout << "\nRound 5: Using (5,5), (7,4), (8,5) as initial clusters.\n";
    populate(S);
    c.push_back(Point());
    c[0].x = 5; c[0].y = 5;
    c.push_back(Point());
    c[1].x = 7; c[1].y = 4;
    c.push_back(Point());
    c[2].x = 8; c[2].y = 5;

    init2(S, k, c);
//    showVector(S);
//    cout << "\nrun\n";
    run(S, k, c);
//    showVector(S);
    showClusters(S);
    S.clear();
    c.clear();

    cout << "\nEND";
    return 0;
}

void run(vector<Point> &S, int k, vector<Point> &c) {
    bool change = true;
    bool flag = false; //to prevent dividing by zero if one cluster has no points
    int a;
    int numChanges = 0;
    do{
        if(change) {
            change = false;
            for(int i=0; i<k; i++) {
                Point mean; mean.x = 0; mean.y = 0;
                int counter = 0;
                    for(int j=0; j<S.size(); j++) {
                        if(S[j].cluster == i) {
                            mean.x += S[j].x;
//                            cout << "mean x: " << mean.x << ",  ";
                            mean.y += S[j].y;
//                            cout << "mean y: " << mean.y << ",  ";
                            counter++;
//                            cout << "count: " << counter << endl;
                            flag = true;
                        }
                    }
                    if(flag == false) break;
                    double xmean = mean.x/counter;
                    double ymean = mean.y/counter;
                    if(c[i].x != xmean || c[i].y != ymean) {
                        change = true;
                    }
                    c[i].x = mean.x/counter;
                    c[i].y = mean.y/counter;
//                    cout << "i: " << i << endl;
//                    cout << "x: " << c[i].x << ",   " << mean.x << "/" << counter << endl;
//                    cout << "y: " << c[i].y << ",   " << mean.y << "/" << counter << endl;
            }
        }
//        cout << "center 0: x = " << c[0].x << ", y = " << c[0].y << endl;
 //       cout << "center 1: x = " << c[1].x << ", y = " << c[1].y << endl;
   //     cout << "center 2: x = " << c[2].x << ", y = " << c[2].y << endl;


        for(int i=0; i<S.size(); i++) {
            a = argmin(k, i, S, c);
    //            cout << "a: " << a << endl;
            if(a != S[i].cluster) {
                S[i].cluster = a;
                numChanges++;

//                cout << "change" << endl;
            }
        }
        if(numChanges > 1000) break; //stops at 1000 iterations
    } while(change);
    if(numChanges == 1) {
        cout << "There was " << numChanges << " cluster swap.\n";
    }
    else if(numChanges == 0) {
        cout << "There were no cluster swaps.\n";
    }
    else {
        cout << "There were " << numChanges << " cluster swaps.\n";
    }
}

//Finds euclidean distance between two Points, returns a double
double sim(Point a, Point b) {
    double distance;
    distance = abs(a.x-b.x) + abs(a.y-b.y);
    return distance;
}


int argmin(int k, int i, vector<Point> S, vector<Point> c) {
    vector<double> distances;
    int minIndex = -1;
    for(int j=0; j<k; j++) {
        distances.push_back(sim(S[i], c[j]));
//        cout << "j: " << j << ", " << distances[j] << endl;
    }
    double minVal = distances[0];
    for(int p=0; p<distances.size(); p++) {
        if(distances[p] <= minVal) {
            minVal = distances[p];
            minIndex = p;
        }
    }
//    cout << "min: " << minIndex << endl;
    return minIndex;
}

void init2(vector<Point> &S, int k, vector<Point> c) {
    for(int i=0; i<S.size(); i++) {
        S[i].cluster = argmin(k, i, S, c);
//        cout << S[i].cluster << endl;
    }
}


void init(vector<Point> &S, int k, vector<Point> c) {
    vector<Point> SS = copyVector(S);
    set<int> jVals;
    for(int i=0; i<k; i++) {
        int j = rand() % S.size();
        while(jVals.count(j) != 0) {
            j = rand() % S.size();
        }
        jVals.insert(j);
        c[k].x = SS[j].x;
        c[k].y = SS[j].y;
        cout << "j: " << j << "\n";
        cout << "c[k]" << c[k].x << ", " << c[k].y << "\n";
        SS.erase(SS.begin()+j);
    }

    for(int i=0; i<S.size(); i++) {
        S[i].cluster = argmin(k, i, S, c);
//        cout << S[i].cluster << endl;
    }
}

//Creates and returns a copy of a vector of Points
vector<Point> copyVector(vector<Point> v) {
    vector<Point> newVect;
    for(int i=0; i<v.size(); i++) {
        newVect.push_back(v[i]);
    }
    return newVect;
}

//Utility function to print a 1D vector of Points
void showVector(vector<Point> v) {
    for(int i=0; i<v.size(); i++) {
//        cout << "x: " << v[i].x;
//        cout << ", y: " << v[i].y;
        cout << v[i].name;
        cout << ", " << v[i].cluster << "\n";
    }
}

//Prints each cluster and its contents
void showClusters(vector<Point> S) {
    for(int j=0; j<3; j++) {
        cout << "Cluster " << j << ": ";
        for(int i=0; i<S.size(); i++) {
            if(S[i].cluster == j) {
                cout << S[i].name << ", ";
            }
        }
        cout << endl;
    }
}

//Assigns values and names to clusters
void populate(vector<Point> &S) {
    S.push_back(Point());
    S[0].x = 2; S[0].y = 10; S[0].name = "A1";
    S.push_back(Point());
    S[1].x = 2; S[1].y = 5; S[1].name = "A2";
    S.push_back(Point());
    S[2].x = 8; S[2].y = 4; S[2].name = "A3";
    S.push_back(Point());
    S[3].x = 5; S[3].y = 8; S[3].name = "A4";
    S.push_back(Point());
    S[4].x = 7; S[4].y = 5; S[4].name = "A5";
    S.push_back(Point());
    S[5].x = 6; S[5].y = 4; S[5].name = "A6";
    S.push_back(Point());
    S[6].x = 1; S[6].y = 2; S[6].name = "A7";
    S.push_back(Point());
    S[7].x = 4; S[7].y = 9; S[7].name = "A8";
}
