#include <iostream>
#include <string>

using namespace std;
typedef string String;

void debug( String program, int indent, String message ) {
    cerr << program << ": ";
    for ( int i = 0; i < indent; i++ )
        cerr << " ";
    cerr << message << endl;
}
