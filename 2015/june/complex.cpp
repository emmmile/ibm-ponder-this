#include <numeric>
#include <algorithm>
#include <array>
#include <cassert>
#include <initializer_list>
#include <iostream>
#include <complex>
#include <unordered_map>
#include <random>
#include <vector>
using namespace std;


unordered_map<int, std::vector<pair<complex<int>, double>>> memo;



struct points : public array<complex<int>, 10> {
    typedef complex<int> point_t; 
    typedef complex<double> average_t;
    typedef double deviation_t;

    int elements;

    points ( ) : elements(0) {}

    points ( const initializer_list<point_t>& args ) : elements(args.end() - args.begin()) {
        assert(elements <= 9);

        unsigned int i = 0;
        for ( auto j = args.begin(); i < 10 && j != args.end(); ++j )
            (*this)[i++] = *j;
    }

    int size() const { return elements; }

    void add ( const point_t& p ) {
        (*this)[elements++] = p;
    }

    void remove ( ) {
        (*this)[--elements] = 0;
    }

    average_t average () const {
        return average(elements);
    }

    // it needs at least one point
    deviation_t deviation ( ) const {
        return deviation(elements);
    }

    template<class T>
    struct deviation_order {
        bool operator ()(const T &a, const T& b) const {
            return a.second < b.second;
        }
    };

    // generate all the gaussian integers at distance d from (0,0)
    std::vector<pair<point_t, double>> circle ( double d ) {
        typedef pair<point_t, double> pp;
        vector<pp> output;

        for ( double angle = 0; angle <= M_PI / 2; angle += 1 / d ) {
            output.push_back(pp({int(d * cos(angle)), int(d * sin(angle))}, 0.0));
            output.push_back(pp({-int(d * cos(angle)), int(d * sin(angle))}, 0.0));
            output.push_back(pp({int(d * cos(angle)), -int(d * sin(angle))}, 0.0));
            output.push_back(pp({-int(d * cos(angle)), -int(d * sin(angle))}, 0.0));
        }

        for ( unsigned int i = 0; i < output.size(); ++i ) {
            this->add({int(average().real() + output[i].first.real()), 
                       int(average().imag() + output[i].first.imag())});
            output[i].second = deviation();
            this->remove();
        }

        sort(output.begin(), output.end(), deviation_order<pp>());

        return output;
    }

    // 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
    // Returns a positive value, if OAB makes a counter-clockwise turn,
    // negative for clockwise turn, and zero if the points are collinear.
    long cross(const point_t &O, const point_t &A, const point_t &B) const {
        return (long)(A.real() - O.real()) * (B.imag() - O.imag()) - (long)(A.imag() - O.imag()) * (B.real() - O.real());
    }


    struct order {
        bool operator ()(const point_t &a, const point_t& b) const {
            return a.real() < b.real() || (a.real() == b.real() && a.imag() < b.imag());
        }
    };

    // Returns a list of points on the convex hull in counter-clockwise order.
    // Note: the last point in the returned list is the same as the first one.
    points convex_hull() const {
        int k = 0;
        points T(*this);
        points H;

        // Sort points lexicographically
        sort(T.begin(), T.begin() + elements, order());
     
        // Build lower hull
        // H[0,k) contains the points found until now
        for ( int i = 0; i < elements; ++i) {
            // while L contains at least two points and the sequence of last two points
            // of L and the point P[i] does not make a counter-clockwise turn:
            // remove the last point from L
            while (k >= 2 && cross(H[k-2], H[k-1], T[i]) <= 0) k--;
            H[k++] = T[i];
        }
     
        // Build upper hull
        for ( int i = elements - 2, t = k+1; i >= 0; i--) {
            while (k >= t && cross(H[k-2], H[k-1], T[i]) <= 0) k--;
            H[k++] = T[i];
        }

        H.elements = k;
        return H;
    }

    // http://alienryderflex.com/polygon_area/
    // last point is expected te be equal to the first
    int area() const {
        int area = 0;

        for (int i=0;i < elements - 1;i++) {
            int j = i + 1;
            area += (*this)[i].real() * (*this)[j].imag();
            area -= (*this)[i].imag() * (*this)[j].real();
        }

        area /= 2;
        return(area < 0 ? -area : area);
    }

    friend ostream& operator << ( ostream& o, const points& p ) {
        for ( int i = 0; i < p.elements; ++i ) o << p[i] << ", ";
        return o;
    }

    string mathematica ( ) const {
        std::stringstream buf;
        buf << "N[ConvexHullArea[{";
        for ( int i = 0; i < elements; ++i ) buf << "{" << (*this)[i].real() << "," << (*this)[i].imag() << "}, ";
        buf << "}]]";

        return buf.str();
    }

private:
    // utility stuff
    average_t average ( int n ) const {
        average_t center {0,0};

        for ( int i = 0; i < n; i++ )
            center += (*this)[i];

        center = (1.0 / n) * center;
        return center;
    }

    // it needs at least one point
    deviation_t deviation ( int n ) const {
        average_t mean = average(n);
        deviation_t average = 0;

        for ( int i = 0; i < n; i++ ) {
            average_t diff {(*this)[i].real() - mean.real(), (*this)[i].imag() - mean.imag()};
            average += norm(diff);
        }

        return sqrt( average / n );
    }
};


unsigned limit = 32;
points explore ( points start ) {
    if ( start.size() == 8 ) return start;

    // get average and standard deviation of previous measurements
    points::average_t average = start.average();
    points::deviation_t deviation = start.deviation();
    //cout << start << endl;

    std::vector<pair<complex<int>, double>> circle;
    if ( memo.count( int(300 * deviation) ) )
        circle = memo[ int(300 * deviation) ];
    else {
        circle = start.circle( 3 * deviation );
        memo[int(300 * deviation)] = circle;
    }
    // for ( auto p : circle ) cout << p.first << " " << p.second << endl;

    int maximum = 0;
    points best;
    for ( unsigned int i = 0; i < limit; ++i ) {
        points::point_t newone(average.real() + circle[i].first.real(),
                               average.imag() + circle[i].first.imag());

        // cout << newone << endl;
        start.add(newone);
        // cout << start << endl;
        // getchar();
        points current = explore(start);
        start.remove();

        if ( current.deviation() > maximum ) {
            maximum = current.deviation();
            best = current;
            // cout << maximum << endl;
        }
    }

    return best;
}



points pass ( points start, int stop, points& best ) {
    random_device rd;
    default_random_engine generator(rd());
    uniform_real_distribution<> uniform(0, 1);

    int maximum = best.convex_hull().area();
    points current;

    while ( maximum < stop ) {
        current = start;

        while ( current.size() != 9 ) {
            // get average and standard deviation of previous measurements
            points::average_t average = current.average();
            points::deviation_t deviation = current.deviation();
            //cout << current << endl;

            double phase = uniform(generator) * 2 * M_PI;
            double amplitude = 3 * deviation;
            while (true) { 
                // generate a point at maximum distance from the average, that is 3 * deviation
                complex<double> newone = std::polar(amplitude, phase);

                // cast to integer
                complex<double> final { newone.real() + average.real(), newone.imag() + average.imag() };

                if( (abs(final-average) < 3 * deviation) ) {
                    current.add(final);
                    break;
                } else {
                    amplitude *= 0.9999999;
                }
            }
        }

        if ( current.convex_hull().area() > maximum ) {
            maximum = current.convex_hull().area();
            best = current;
            cout << maximum << ": " << best << endl;
        }
    }

    return best;
}



int main ( ) {
    points best;
    points start{{-100,0},{100,0}};
    cout << "pass 1, starting from " << start << endl;
    pass(start, 7700000, best); 
    start.add(best[start.size()]);
    cout << "pass 2, starting from " << start << endl;
    pass(start, 7708000, best); 
    start.add(best[start.size()]);
    cout << "pass 3, starting from " << start << endl;
    pass(start, 7716000, best); 
    start.add(best[start.size()]);
    cout << "pass 4, starting from " << start << endl;
    pass(start, 7720000, best); 

    cout << best << endl;
    cout << best.mathematica() << endl;
    cout << (long) best.convex_hull().area() << endl;

    return 0;
}

// 7710906: (-100,0), (100,0), (24,-299), (-12,-589), (326,-915), (-1019,-2), (-448,-1918), (805,1568), (2924,-1340)
// 7711077: (-100,0), (100,0), (24,-299), (-12,-589), (606,248), (37,1013), (1755,-73), (-1940,-310), (252,-3147)
// 7712823: (-100,0), (100,0), (24,-299), (-12,-589), (606,248), (37,1013), (-1500,-310), (2189,-87), (360,-3149) 
// 7715144: (-100,0), (100,0), (24,-299), (27,-589), (747,-8), (-979,-63), (64,-1809), (459,1867), (3180,-422) 
// 7715839: (-100,0), (100,0), (24,-299), (27,-589), (747,-8), (1301,-95), (159,-1806), (0,1890), (-2868,-240)
// 7716371: (-100,0), (100,0), (24,-299), (27,-589), (747,-8), (199,-1323), (1794,-89), (-1907,-157), (42,2844)
// 7716979: (-100,0), (100,0), (24,-299), (27,-589), (747,-8), (1304,-184), (70,-1808), (99,1890), (-2869,-108)
// 7717684: (-100,0), (100,0), (24,-299), (27,-589), (747,-8), (1304,-166), (94,-1809), (76,1890), (-2869,-135)
// 7717442: (-100,0), (100,0), (24,-299), (27,-589), (747,-8), (1304,-184), (94,1452), (67,-2246), (-2870,-205),