#include <array>
#include <ostream>
using namespace std;

typedef array<int, 3> vect;

vect operator + ( const vect& a, const vect& b ) {
    vect c;
    for ( size_t i = 0; i < a.size(); ++i ) c[i] = a[i] + b[i];
    return c;
}

vect operator - ( const vect& a, const vect& b ) {
    vect c;
    for ( size_t i = 0; i < a.size(); ++i ) c[i] = a[i] - b[i];
    return c;
}

ostream& operator << ( ostream& stream, vect& c ) {
    for ( auto i : c ) stream << i << " ";
    return stream;
}

