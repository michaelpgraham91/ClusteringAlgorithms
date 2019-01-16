/*
Single Linkage Hierarchical Agglomeration Algorithm

Points are clustered based on shortest euclidean distance.
Program will output an array of all distances between points
followed by each cluster and its contents.
*/

#include <iostream>
#include <vector>
#include <math.h>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

/*
Data structure used to hold x and y coordinates,
name of point (for use when showing clusters),
a bool to tell if that point is part of another cluster,
a vector containing the other points in the same cluster
*/
struct Point {
    double x;
    double y;
    string name;
    bool inCluster = false;
    vector<Point> clust;
};

double sim(Point a, Point b);
double sldRow(int i, int m, int j, vector<vector<double>> C);
double sldCol(int i, int m, int j, vector<vector<double>> C);
void populate(vector<Point> &S);
void simplehac(vector<Point> &S, int k);
void show2dVector(vector<vector<Point>>);
Point closestPair(vector<vector<double>> C);

int main() {
    //Create a vector of points to add values too
    vector<Point> S;
    //Add values to vector
    populate(S);
    //Perform single linkage clustering
    simplehac(S, 2);

    cout << endl << endl;

    return 0;
}

//This returns the point on array C that has the smallest nonzero value
//where C is the array of distances between the points
Point closestPair(vector<vector<double>> C, vector<int> I) {
    double minimum = 100; //Could be any number larger than the greatest distance
    double tempMin;
    Point p;
    int x, y;
    for(int i=0; i<C.size()-1; i++) {
        for(int j=0; j<C[i].size(); j++) {
            tempMin = C[i][j];
            //The following statement resolves an issue where the distance between two pairs of
            //points is an extremely small number instead of zero, likely due to a
            //rounding error during floating point arithmetic
            if((tempMin - minimum < 0 && tempMin - minimum > -0.00001) ||
               (tempMin - minimum > 0 && tempMin - minimum < 0.00001)) break;
            if(tempMin < minimum && tempMin > 0 && I[i] != 0) {
                minimum = tempMin;
                x = i; y = j;
            }
        }
    }
    p.x = x; p.y = y;
    return p;
}

//Merges row in array C after that value has been added to a cluster
double sldRow(int i, int m, int j, vector<vector<double>> C) {
    double minimum = 100;
    double tempMin;

    if(C[i][j] == C[m][j]) return C[i][j];
    else if(C[i][j]>C[m][j]) {
        minimum = C[m][j];
    }
    else if(C[i][j]<C[m][j]) {
        minimum = C[i][j];
        }
    else minimum = 42;
    return minimum;
}

//Merges column in array C after that value has been added to a cluster
double sldCol(int i, int m, int j, vector<vector<double>> C) {
    double minimum = 100;
    double tempMin;

    if(C[j][i] == C[j][m]) return C[i][j];
    else if(C[j][i]>C[j][m]) {
        minimum = C[j][m];
    }
    else if(C[j][i]<C[j][m]) {
        minimum = C[j][i];
    }
    else minimum = 42;
    return minimum;
}


/*
Function to perform single linkage clustering
C is a 2d array of all of the distances between each point
Array C will be printed at runtime
I is an array that keeps track of which clusters have been made
(i.e. there can't be two cluster 1's)
S is the vector of points to be clustered
*/
void simplehac(vector<Point> &S, int k) {
    int N = S.size();
    int m = 0;
    vector<vector<double>> C(N, vector<double>(N));
    vector<int> I(N, 0);
    vector<vector<Point>> A(N-1); //there will be N-1 clusters total
    //Creates C 2d vector by finding distances between all points
    for(int o=0; o<N; o++) {
        for(int p=0; p<N; p++) {
            C[o][p] = sim(S[o],S[p]);
            cout << setprecision(3) << setw(7) << p << ", " << C[o][p];
        }
        cout << endl;
        I[o] = I[o] + 1;
    }
    int i=0;
    //Create n-1 clusters
    for(int k=0; k<N-1; k++) {
        Point coordinate = closestPair(C, I);
        i = coordinate.x;
        m = coordinate.y;
        if(i != m && I[i] == 1 || I[m] == 1) {
            if(S[i].inCluster == true) {
                for(int w=0; w<S[i].clust.size(); w++) {
                    A[k].push_back(S[i].clust[w]);
                }
            }
            if(S[m].inCluster == true) {
                for(int w=0; w<S[i].clust.size(); w++) {
                    A[k].push_back(S[m].clust[w]);
                }
            }
            else {
                S[i].inCluster = true;
                S[i].clust.push_back(S[m]);
                S[m].inCluster = true;
                S[m].clust.push_back(S[i]);
            }
            A[k].push_back(S[i]);
            A[k].push_back(S[m]);
        }
        for(int j=0; j<N; j++) {
            C[i][j] = sldRow(i, m, j, C);
            C[j][i] = sldCol(i, m, j, C);
        }
        I[m] = 0;
    }
    //uncomment this to see what the ending distance matrix looks like
    //rows that have been merged will not change, only the row they merge into will
/*    cout << "\n\n";
    for(int o=0; o<N; o++) {
//        cout << "i: "<< I[o];
        for(int p=0; p<N; p++) {
            cout << setprecision(3) << setw(5) << p << ", " << C[o][p];
       }
        cout << endl;
    }
*/
    show2dVector(A);
}

//Utility function to print a 2d vector of Point data structures
void show2dVector(vector<vector<Point>> S) {
    for(int i=0; i<S.size(); i++) {
        cout << "Cluster " << i << ": ";
        for(int j=0; j<S[i].size(); j++) {
            cout << S[i][j].name << ", ";
        }
        cout << endl;
    }
}

//Function to find the euclidean distance between two points
double sim(Point a, Point b) {
    double dist;
    dist = sqrt(pow(b.x-a.x, 2) + pow(b.y-a.y, 2));
    return dist;
}

//Function to populate the initial vector with values and names
void populate(vector<Point> &S) {
    S.push_back(Point());
    S[0].x = 2; S[0].y = 2; S[0].name = "A1";
    S.push_back(Point());
    S[1].x = 3.01; S[1].y = 2; S[1].name = "A2";
    S.push_back(Point());
    S[2].x = 4.02; S[2].y = 2; S[2].name = "A3";
    S.push_back(Point());
    S[3].x = 5.03; S[3].y = 2; S[3].name = "A4";
    S.push_back(Point());
    S[4].x = 6.04; S[4].y = 2; S[4].name = "A5";
    S.push_back(Point());
    S[5].x = 7.05; S[5].y = 2; S[5].name = "A6";
    S.push_back(Point());
    S[6].x = 2; S[6].y = 3.5; S[6].name = "A7";
    S.push_back(Point());
    S[7].x = 3.01; S[7].y = 3.5; S[7].name = "A8";
    S.push_back(Point());
    S[8].x = 4.02; S[8].y = 3.5; S[8].name = "A9";
    S.push_back(Point());
    S[9].x = 5.03; S[9].y = 3.5; S[9].name = "A10";
    S.push_back(Point());
    S[10].x = 6.04; S[10].y = 3.5; S[10].name = "A11";
    S.push_back(Point());
    S[11].x = 7.05; S[11].y = 3.5; S[11].name = "A12";
}
