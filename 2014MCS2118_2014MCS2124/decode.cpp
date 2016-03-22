#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>
#include <string>

using namespace std;


// true if it is satisfiable
bool isSat = false;
string isSatStr, metaData;
long sgSize;

vector<int> sgVertices;

int n, k;

void splitString (string message, string delimiter, string result[])
{
    int i = 0;
    int pos = 0;
    int length = 0;
    int temp;
    temp = message.find (delimiter.c_str (), pos);
    while (temp != -1)
    {
        length = temp - pos;
        //printf ("pos : %d length : %d\n",pos,length);
        result[i] = message.substr (pos, length);
        pos = temp + delimiter.size ();
        temp = message.find (delimiter.c_str (), pos);
        i++;
    }

    result[i] = message.substr (pos);
    i++;

}

main(int argc, char** argv) {

    ifstream myfile;
    myfile.open(argv[1], ios::in);

    getline( myfile, metaData );

    string temp[3];

    splitString( metaData, " ", temp);

    n = atol( temp[0].c_str() );
    k = atol( temp[2].c_str() );

    cin >> isSatStr;

    if ( !isSatStr.compare("UNSAT") ) {
        cout << "0";
        return 0;
    }

    for( int x = 1; x <= k; x++ ) {

        if( x != 1)
            cout << "\n";

        sgSize = 0;
        sgVertices.clear();

//        y(x,1) to y(x,n)
        for( int i = 1; i <= n; i++ ) {

            int temp;
            cin >> temp;


            if( temp > 0 ) {
                sgSize++;
                if( temp % n == 0 )
                    sgVertices.push_back( n );
                else
                    sgVertices.push_back( temp % n );
            }

        }

        cout << "#" << x << " " << sgSize << endl;

        for( int i = 0; i < sgSize; i++ ) {

            cout << sgVertices[i];

            if( i != sgSize-1)
                cout << " ";
        }

    }

}
