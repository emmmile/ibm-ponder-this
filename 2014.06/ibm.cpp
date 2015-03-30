#include <random>
#include <vector>
#include <bitset>
#include <iostream>
#include <algorithm>    // std::random_shuffle
using namespace std;

/*int source [31];


template<size_t N, size_t T>
struct cube {
	int faces;
    cube ( int faces = 0 ) : faces( faces ) {}

    cube ( int* beg, int* end ) {
        faces = 0;

        for ( int* i = beg; i != end; ++i ) {
            int mask = (1 << *i);
            faces |= mask;
        }
    }

    // true if they share only one number
    bool intersection ( cube& another ) {
        unsigned int x = another.faces & faces;
        return x && !(x & (x - 1));
    }

	friend ostream& operator << ( ostream& stream, cube& c ) {
        for ( unsigned int i = 0; i <= N; ++i ) {
            int mask = (1 << i);
            if ( (c.faces & mask) != 0 )
                stream << i+1 << " ";
		} 
		
		return stream;
	}

    static void combinations ( ) {
        int c [T+3];
        int j;
        int x;

        for ( int i = 1; i <= T; ++i ) c[i] = i - 1;
        c[T+1] = N;
        c[T+2] = 0;
        j = T;

        while (true) {
            cube visit(c + 1, c + 1 + T);
            cout << visit << endl;

            if ( j > 0 ) {
                x = j;
            } else {
                if ( c[1] + 1 < c[2] ) {
                    c[1] = c[1] + 1;
                    continue;
                } else {
                    j = 2;
                }

                while ( true ) {
                    c[j-1] = j-2;
                    x = c[j] + 1;
                    if ( x == c[j+1] ) j++;
                    else break;
                }

                if ( j > T ) break;
            }

            c[j] = x;
            j = j -1;
        }
    }
};*/



template<size_t N, size_t T>
class combinations {
    typedef bitset<N> C;
public:
    class iterator : public virtual std::iterator<output_iterator_tag, C, ptrdiff_t, C*, C&> {
        int c [T+3];
        int j;
        int x;
        bool finished;
        C current;

        void update(int* beg, int* end) {
            current.reset();
            for ( int* i = beg; i != end; ++i )
                current[*i] = true;
        }

        iterator () {
            finished = false;
            for ( int i = 1; i <= T; ++i ) c[i] = i - 1;
            c[T+1] = N;
            c[T+2] = 0;
            j = T;
            update(c + 1, c + 1 + T);
        }
    public:
        static iterator begin() {
            return iterator();
        }

        static iterator end() {
            // lo voglio statico!!!
            iterator out;
            out.finished = true;
            return out;
        }

        bool operator != ( const iterator& a ) {
            return a.finished != finished;
        }

        iterator& operator++() {
            while (!finished) {
                if ( j > 0 ) {
                    x = j;
                } else {
                    if ( c[1] + 1 < c[2] ) {
                        c[1] = c[1] + 1;
                        update(c + 1, c + 1 + T);
                        break;
                    } else {
                        j = 2;
                    }

                    while ( true ) {
                        c[j-1] = j-2;
                        x = c[j] + 1;
                        if ( x == c[j+1] ) j++;
                        else break;
                    }

                    if ( j > T ) finished = true;
                }

                c[j] = x;
                j = j -1;

                update(c + 1, c + 1 + T);
                break;
            }

            return *this;
        }

        C& operator*() {
           return current;
        }
    };


    static iterator begin() {
        return iterator::begin();
    }

     static iterator end() {
        return iterator::end();
    }
};


/*int main ( ) {
    //cube<31,5>::combinations();
    typedef typename cubes<31,5>::cube_iterator iter;

    iter it = iter();

    while ( it != iter::end() ) {
        cout << *it << endl;
        ++it;
    }

    cubes<31,4>::fill();
}

typedef cube<31,5> fastcube;

fastcube single( ) {
	random_shuffle(source, source + 31);
    return fastcube(source, source + 6);
}

void next(fastcube& face, vector<fastcube> faces) {
	while(true) {
        face = single( );

        unsigned int i = 0;
		for ( ; i < faces.size(); ++i ) {
            if ( !face.intersection(faces[i]) ) break;
		}

		if (i == faces.size()) return;
	}
}

void compute() {
    vector<fastcube> cubes;
	while( cubes.size() < 25 ) {
        fastcube newcube;
		next(newcube, cubes);
		cout << newcube << endl;
		cubes.push_back(newcube);
	}
}

int main ( ) {
	random_device rd;
	srandom( rd() );
	
	for (int i = 0; i < 31; ++i )
		source[i] = i + 1;

	compute();
}*/



int main ( ) {
    combinations<31,5>::iterator it = combinations<31,5>::begin(); 
    while ( it != combinations<31,5>::end() ) {
        cout << *it << endl;
        ++it;
    }

    return 0;
}
