#include <random>
#include <vector>
#include <bitset>
#include <iostream>
#include <set>
#include <cassert>
#include <map>
#include <algorithm>    // std::random_shuffle
using namespace std;

int q[] = { 1, 1, 2, 3, 4, 5, 7, 8, 10, 12, 14, 16, 19, 21, 24, 27, 30, 
    33, 37, 40, 44, 48, 52, 56, 61, 65, 70, 75, 80, 85, 91, 96, 102, 108, 
    114, 120, 127, 133, 140, 147, 154, 161, 169, 176, 184, 192, 200, 208, 
    217, 225, 234, 243, 252, 261, 271, 280, 290, 300, 310, 320, 331, 341 };


class configuration {
public:
    typedef uint32_t configuration_type;
    typedef int size_type;
    struct face_type {
        size_type face [2];

        face_type( size_type n, size_type k ) {
            face[0] = n;
            face[1] = k;
        }

        size_type sum ( )   { return face[0]; }
        size_type empty ( ) { return face[1]; }
        size_type full ( )  { return 3 - face[1]; }
    };

    configuration_type numbers;

    configuration ( configuration_type n = 0xFFFFFFFF ) : numbers(n) {}

    size_type edge ( size_type e ) const {
        //cout << "edge " << e << endl;
        uint32_t y;
        uint32_t x = edge_value(e) + 1;
        asm ( "\tbsr %1, %0\n"
            : "=r"(y)
            : "r" (x)
        );
        return y;
    }

    void setedge ( size_type e, size_type v ) {
        v = (1 << v) - 1; // now v is encoded as a number of bits
        v &= (edge_masks[e] >> edge_offsets[e]); // to be sure;
        numbers &= ~edge_masks[e]; // clear
        numbers |= (v << edge_offsets[e]);
    }

    // returns the number of elements in a face, n
    // and the number of non-empty edges
    face_type face ( size_type f ) const {
        size_type n = 0;
        size_type k = 0;

        for ( size_type i = 0; i < 3; ++i ) {
            n +=  edge(face_edges[f][i]);
            k += (edge(face_edges[f][i]) == 0);
        }

        return face_type(n, k);
    }

    void facesetedge ( size_type f, size_type e, size_type v ) {
        setedge( face_edges[f][e], v );
    }

    size_type faceedge ( size_type f, size_type e ) const {
        return edge( face_edges[f][e] );
    }

    static size_type edges ( ) {
        return 6;
    }

    static size_type faces ( ) {
        return 4;
    }

    bool operator < ( const configuration& a ) const {
        if ( this->sum() == a.sum() )
            return this->numbers < a.numbers;
        else
            return this->sum() < a.sum();
    }

    bool operator != ( const configuration& a ) {
        return this->numbers != a.numbers;
    }

    friend ostream& operator << ( ostream& stream, const configuration& c ) {
        //for ( size_type i = 0; i < edges(); ++i )
        //    stream << c.edge(i) << " ";
        // order for the solution
        stream << c.edge(0) << " " << c.edge(5) << " " << c.edge(2) << " " 
               << c.edge(4) << " " << c.edge(3) << " " << c.edge(1);
        
        return stream;
    }

    size_type sum ( ) const {
        size_type total = 0;
        for ( size_type i = 0; i < edges(); ++i )
            total += edge(i);
        return total;
    }



    static bool move ( const configuration& from, const configuration& to ) {
        if ( from.numbers == to.numbers ) return false;
        for ( size_type i = 0; i < edges(); ++i )
            if ( to.edge(i) > from.edge(i) ) return false;

        configuration_type diff = from.numbers ^ to.numbers;
        return !(diff & ~face_mask(0)) || !(diff & ~face_mask(1)) || !(diff & ~face_mask(2)) || !(diff & ~face_mask(3));

        /*set<size_type> difference;
        for ( size_type i = 0; i < edges(); ++i ) {
            if ( to.edge(i) < from.edge(i) ) difference.insert(i);
            else if ( to.edge(i) > from.edge(i) ) return false;
        }

        for ( size_type i = 0; i < faces(); ++i ) {
            set<size_type> compare;
            compare.insert( face_edges[i], face_edges[i] + 3 );
            if ( std::includes( compare.begin(), compare.end(), difference.begin(), difference.end()) )
                return true;
        }

        return false;*/
    }
private:

    static configuration_type face_mask ( size_type f ) {
        return edge_masks[face_edges[f][0]] | edge_masks[face_edges[f][1]] | edge_masks[face_edges[f][2]];
    }

    size_type edge_value ( size_type e ) const {
        return (numbers & edge_masks[e]) >> edge_offsets[e];
    }

    static constexpr size_type face_edges [4][3] = {
        {0, 1, 5},
        {1, 2, 3},
        {0, 2, 4},
        {3, 4, 5}
    };

    static constexpr configuration_type edge_masks [6] = { 
        0x80000000,
        0x7FFC0000,
        0x00038000,
        0x00007F80,
        0x0000007C,
        0x00000003
    };

    static constexpr size_type edge_offsets [6] = { 
        31,
        18,
        15,
        7,
        2,
        0
    };
};


constexpr configuration::size_type configuration::face_edges [4][3];
constexpr configuration::configuration_type configuration::edge_masks [6];
constexpr configuration::size_type configuration::edge_offsets [6];





class winning {
    // winning and loosing configuration for the player that MOVES INTO that configuration
    set<configuration> loose;
    set<configuration> win;     // maybe it's redundant but I use both for error checking

    vector<configuration> configurations;

public:
    // generate all possibile configurations
    class configuration_iterator : public virtual std::iterator<output_iterator_tag, configuration, ptrdiff_t, configuration*, configuration&> {
        // state
        configuration current;
        configuration end;
        configuration::size_type overflow;
        configuration::size_type index;
        bool reset;
    public:

        configuration_iterator (const configuration& c, const configuration::size_type overflow = 0) 
            : current(c), end(configuration()), overflow(overflow), index(0) {
        }

        bool operator != ( const configuration_iterator& a ) {
            return (this->current != a.current) || (this->overflow != a.overflow);
        }

        configuration_iterator& operator++() {
            while ( index < configuration::edges() ) {
                if ( current.edge(index) < end.edge(index) ) {
                    current.setedge(index, current.edge(index) + 1);
                    if ( reset ) { index = 0; reset = false; }
                    return *this;
                }

                // find the first index where we can sum
                while ( current.edge(index) == end.edge(index) && index < configuration::edges() ) {
                    current.setedge(index, 0);
                    index++;
                    reset = true;
                }
            }

            overflow = 1;
            current = end;
            return *this;
        }

        configuration& operator*() {
           return current;
        }
    };

    static configuration_iterator cbegin() {// const configuration& c = configuration() ) {
        return configuration_iterator(configuration(0), 0);
    }

    static configuration_iterator cend() {
        return configuration_iterator(configuration(), 1);
    }

    winning ( ) {
        // put all configurations in the array, sorting them
        for( configuration_iterator i = cbegin(); i != cend(); ++i )
            configurations.push_back(*i);
        sort( configurations.begin(), configurations.end() );
        // the empty tetraedron is always loosing for the player that has to play
        loose.insert(configurations[0]);
    }

    void compute ( ) {
        size_t k = 0; // l'indice dell'ultimo scalino, exclusive

        for( size_t i = 1; i < configurations.size(); ++i ) {
            if ( configurations[i].sum() != configurations[k].sum())
                k = i;

            size_t total = 0;
            size_t winning = 0;
            size_t loosing = 0;
            for ( size_t j = 0; j < k; ++j ) {
                if ( configuration::move(configurations[i], configurations[j]) ) {
                    //cout << "move " << configurations[i] << " -> " << configurations[j] << endl;
                    total++;
                    if ( loose.count( configurations[j] ) ) loosing++;
                    if ( win.count( configurations[j] ) ) winning++;
                }
            }

            assert( loosing + winning == total );
            if ( loosing == total )
                win.insert(configurations[i]);
            else 
                loose.insert(configurations[i]);
        }
    }


    void check ( const configuration& from ) {

        size_t total = 0;
        size_t winning = 0;
        size_t loosing = 0;
        for( size_t i = 0; i < configurations.size(); ++i ) {
            if ( configurations[i].sum() == from.sum())
                break;

            if ( configuration::move(from, configurations[i]) ) {
                //cout << "move " << configurations[i] << " -> " << configurations[j] << endl;
                total++;

                if ( loose.count(configurations[i]) == 0 &&
                     win.count( configurations[i]) == 0 )
                    check( configurations[i]);

                if ( loose.count( configurations[i] ) ) loosing++;
                else if ( win.count( configurations[i] ) ) winning++;
            }
        }

        assert( loosing + winning == total );
        if ( loosing == total ) {
            win.insert(from);
        } else 
            loose.insert(from);
    }

    void results ( ) {
        size_t total = 0;
        configuration start;
        for( size_t i = 1; i < configurations.size(); ++i ) {
            if ( configuration::move(start, configurations[i]) ) {
                //cout << configurations[i] << endl;
                total++;
            }
        }

        cout << "Out of the " << total << " possible starting moves, this are the winning ones: " << endl;

        for( const configuration& c : win ) {
            cout << c << endl;
            if ( configuration::move(start, c) )
                cout << c << endl;
        }

        cout << "(the ones that, for any possible reply, leads to loosing configurations for the other player)" << endl;
        assert( configurations.size() == win.size() + loose.size() );


        cout << "Total winning configurations " << win.size() << endl;
    }
};

int main ( ) {
    configuration start;
    /*cout << "edge(0): " << start.edge(0) << endl;
    cout << "edge(1): " << start.edge(1) << endl;
    cout << "edge(2): " << start.edge(2) << endl;
    cout << "edge(3): " << start.edge(3) << endl;
    cout << "edge(4): " << start.edge(4) << endl;
    cout << "edge(5): " << start.edge(5) << endl;

    cout << "face(0): " << start.face(0).sum() << ", " << start.face(0).full() << endl;
    cout << "face(1): " << start.face(1).sum() << ", " << start.face(1).full() << endl;
    cout << "face(2): " << start.face(2).sum() << ", " << start.face(2).full() << endl;
    cout << "face(3): " << start.face(3).sum() << ", " << start.face(3).full() << endl;

    configuration start;
    configuration end;
    end.setedge(0, 0);
    cout << configuration::move(start, end) << endl;
    end.setedge(2, 2);
    cout << configuration::move(start, end) << endl;
    end.setedge(4, 1);
    cout << configuration::move(start, end) << endl;
    end.setedge(1, 12);
    cout << configuration::move(start, end) << endl;*/

    winning win;
    win.compute();
    win.results();
    win = winning();
    win.check(start);
    win.results();


    /* speed test
    size_t m = 0;
    for ( uint32_t i = 0; i < 0xFFFFFFFF; ++i ) {
        start = configuration(i);
        m += start.face(0).sum();
    }

    cout << m << endl;*/


    return 0;
}

/*
1, 2, 2, 5, 1, 5
1, 2, 3, 5, 0, 6
1, 2, 0, 5, 3, 7
1, 2, 1, 5, 2, 8
*/
