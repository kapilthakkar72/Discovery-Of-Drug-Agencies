#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int n, e, k;

set<int> discoveredVertices;

typedef set<int>::iterator setIntIterator;
typedef vector<int>::iterator vecIntIterator;
typedef vector< set<int> >::iterator vecSetIntIterator;


class vertex {
    public:

    int id;
    vector<int> adjList;
    vertex() {}
    vertex( int id ) {
        this->id = id;
    }

    void show() {
        cout << id << " - ";
        for_each( adjList.begin(), adjList.end(), [](int i){cout<<i<<" ";});
        cout << endl;
    }

    bool isAdjacent( int i ) {
        vecIntIterator vIt = find( adjList.begin(), adjList.end(), i );
        if( vIt != adjList.end() )
            return true;
        else
            return false;
    }

};

vector<vertex> vertices;

// returns the variable number given the indices of the variable y(x,i)
// number of these variables is - n*k
long cony( long x, long i ) {
    return (x-1)*n + i;
}

// returns the variable number given the indices of the variable w(x,i,j)
// number of variables is k*n*n
// kth subgraph with ith vertex and jth vertex
long conw( int x, int i, int j) {
    return n*k + (x-1)*n*n + (i-1)*n + j;
}

// returns the variable number given the indices of the variable z(p,q,i)
// number of variables is k*k*n
// pth subgraph and qth subgraph with ith vertex
long conz( int p, int q, int i) {
    return n*k + k*n*n + (p-1)*k*n + (q-1)*n + i;
}

// ith vector will contain the ith clause
vector< vector<int> > clauses;

main() {

    // since vertices are indexed with 1
    vertices.push_back( *(new vertex(0)) );

    cin >> n >> e >> k;

    for( int i = 1; i <= n; i++ ) {
        vertex temp = *(new vertex(i));
        vertices.push_back( temp );
    }

    for( int i = 1; i <= e; i++ ) {
        int v1, v2;
        cin >> v1 >> v2;
        vertices[v1].adjList.push_back( v2 );
        vertices[v2].adjList.push_back( v1 );
    }

    for( int i = 1; i <= n; i++ ) {
        sort( vertices[i].adjList.begin(), vertices[i].adjList.end() );
    }

//    for_each(vertices.begin(), vertices.end(), [](vertex v){v.show();});

//    RULE 1

//    there should be an edge between two vertices of same subgraph
    for( int x = 1; x <= k; x++ ) {
        for( int i = 1; i <= n; i++ ) {
            for( int j = 1; j <= n; j++ ) {
                if( i == j || vertices[i].isAdjacent(j) )
                    continue;
                vector<int> temp;
                temp.push_back( 0-cony(x,i) );
                temp.push_back( 0-cony(x,j) );

                clauses.push_back( temp );
            }
        }
    }


//    RULE 2

//    there must be atleast one subgraph including i and j if e(i,j) is true
    for( int i = 1; i <= n; i++ ) {
        for( int j = 1; j <= n; j++ ) {
            if( vertices[i].isAdjacent(j) ) {
                vector<int> temp;

                for( int x = 1; x <= k; x++ ) {
                    temp.push_back( conw(x, i, j) );
                }

                clauses.push_back( temp );
            }
        }
    }


//    RULE 3

//    each subgraph has atleast one vertex
    for( int x = 1; x <= k; x++ ) {

        vector<int> temp;
        for( int i = 1; i <= n; i++ ) {
            temp.push_back( cony(x,i) );
        }
        clauses.push_back( temp );

    }


//    RULE 4

//    checking for subgraph existence
    for( int p = 1; p <= k; p++ ) {
        for( int q = 1; q <= k; q++ ) {
            if( p == q )
                continue;

            vector<int> temp;
            for( int i = 1; i <= n; i++ ) {
                temp.push_back( conz(p,q,i) );
            }
            clauses.push_back( temp );

        }
    }

//    RULE 5

//    creating variables w(x,i,j) and z(p,q,i) which helped in making CNF form
//    (y(x,i) ^ y(x,j)) <-> w(x,i,j)
    for( int x = 1; x <= k; x++ ) {
        for( int i = 1; i <= n; i++ ) {
            for( int j = 1; j <= n; j++ ) {
                vector<int> temp;

                temp.push_back( 0-cony(x,i) );
                temp.push_back( 0-cony(x,j) );
                temp.push_back( conw(x,i,j) );

                clauses.push_back( temp );
                temp.clear();

                temp.push_back( 0-conw(x,i,j) );
                temp.push_back( cony(x,i) );

                clauses.push_back( temp );
                temp.clear();

                temp.push_back( 0-conw(x,i,j) );
                temp.push_back( cony(x,j) );

                clauses.push_back( temp );
                temp.clear();
            }
        }
    }

//    (y(p,i) ^ ~y(q,i)) <-> z(p,q,i)
    for( int p = 1; p <= k; p++ ) {
        for( int q = 1; q <= k; q++ ) {
            for( int i = 1; i <= n; i++ ) {
                vector<int> temp;

                temp.push_back( 0-cony(p,i) );
                temp.push_back( 0+cony(q,i) );
                temp.push_back( conz(p,q,i) );

                clauses.push_back( temp );
                temp.clear();

                temp.push_back( 0-conz(p,q,i) );
                temp.push_back( cony(p,i) );

                clauses.push_back( temp );
                temp.clear();

                temp.push_back( 0-conz(p,q,i) );
                temp.push_back( 0-cony(q,i) );

                clauses.push_back( temp );
                temp.clear();
            }
        }
    }


    long noOfVar = n*k + k*n*n + k*k*n;

    cout << "p cnf " << noOfVar << " " << clauses.size() << endl;

    for_each( clauses.begin(), clauses.end(), [](vector<int> v){
        for_each( v.begin(), v.end(), [](int i){ cout<<i<<" "; });
        cout << "0" << endl;
    });


    clauses.clear();

}
