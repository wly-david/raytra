#pragma once

#include<cmath>
#include <iostream>
#include <cassert>

using std::istream;
using std::ostream;

class myVector
{
	double ijk_[3];
public:
	myVector(void) { }
	
    myVector (double i, double j, double k) {
        ijk_[0] = i; ijk_[1] = j; ijk_[2] = k;
    }

    void set (double i, double j, double k) {
        ijk_[0] = i; ijk_[1] = j; ijk_[2] = k;
    }

	myVector normalize(void);
	
    double operator[] (const int i) const;

	~myVector(void) { }
    // these two functions are used to allow us to read in
    // and print out (or save to a file). they are used by
    // the parser code.
    friend istream &operator>>(istream &is, myVector &v) {
        return is>>v.ijk_[0]>>v.ijk_[1]>>v.ijk_[2];
    }
    
    friend ostream &operator<<(ostream &os, myVector &v) {
        return os<<"<"<<v.ijk_[0]<<","<<v.ijk_[1]<<","<<v.ijk_[2]<<">";
    }
};

inline myVector crossProduct(const myVector &v1,const myVector &v2) {

	double s1, s2 ,s3;
	s1 = v1[1] * v2[2] - v1[2] * v2[1];
	s2 = v1[2] * v2[0] - v1[0] * v2[2];
	s3 = v1[0] * v2[1] - v1[1] * v2[0];
	return myVector(s1, s2, s3);	
	
};
inline myVector operator* (const double scalar, const myVector &v){
	return myVector(scalar * v[0], scalar * v[1], scalar * v[2]);
}

inline double operator* (const myVector &v1, const myVector &v2){
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}
inline myVector operator+ (const myVector &v1, const myVector &v2){
	return myVector(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

inline double myVector::operator[]
  (const int i) const
{
    assert(i >= 0 && i < 3);
    return ijk_[i];
}


inline myVector myVector::normalize ()
{
    assert (! (ijk_[0] == 0.0 && ijk_[1] == 0.0 && ijk_[2] == 0.0));
    
    double len = ijk_[0] * ijk_[0] + 
    ijk_[1] * ijk_[1] + 
    ijk_[2] * ijk_[2];
    
    len = sqrt (len);
    
    ijk_[0] /= len;
    ijk_[1] /= len;
    ijk_[2] /= len;
	return myVector(ijk_[0], ijk_[1], ijk_[2]);
}