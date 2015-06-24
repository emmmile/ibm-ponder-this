#include <numeric>
#include <algorithm>
#include <array>
#include <cassert>
#include <initializer_list>
#include <iostream>
#include <complex>
#include <random>
#include <vector>
using namespace std;    


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

    average_t average () const {
        return average(elements);
    }

    // it needs at least one point
    deviation_t deviation ( ) const {
        return deviation(elements);
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
 





int main ( ) {
    random_device rd;
    default_random_engine generator(rd());
    uniform_real_distribution<> uniform(0, 1);

    int maximum = 0;
    points best;
    points current;

    for ( int i = 0; i < 1000000000; ++ i) {
        current = {{-100,0},{100,0}};
        //current = {{-100,0},{100,0}};

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
            cout << maximum << endl;
        }
    }

    cout << best << endl;
    cout << best.mathematica() << endl;
    cout << (long) best.convex_hull().area() << endl;

    return 0;
}

// (-100,0), (100,0), (110,279), (-448,22), (-128,-688), (454,-1082), (-1553,-811), (1669,1000), (-1461,2625), 
// N[ConvexHullArea[{{-100,0}, {100,0}, {110,279}, {-448,22}, {-128,-688}, {454,-1082}, {-1553,-811}, {1669,1000}, {-1461,2625}, }]]
// 7706009

// (-100,0), (100,0), (110,279), (-225,507), (59,956), (-70,1491), (-1630,168), (2060,445), (243,-2664), 
// N[ConvexHullArea[{{-100,0}, {100,0}, {110,279}, {-225,507}, {59,956}, {-70,1491}, {-1630,168}, {2060,445}, {243,-2664}, }]]
// 7709325