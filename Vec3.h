#ifndef _VEC3_H
#define _VEC3_H

#include <cmath>

/// Simple three dimensional vector class
template<class T>
struct Vec3
{
  T x,y,z;

  Vec3(const T &xx = (T)0, const T &yy = (T)0, const T &zz = (T)0) : x(xx), y(yy), z(zz) {}
  Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z) {}
  Vec3(const T * arr) : x(arr[0]), y(arr[1]), z(arr[2]) {}

  operator T*();
  operator const T*() const;
};

typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;
typedef Vec3<unsigned char> Vec3ub;

template<class T>
Vec3<T>::operator T*()
{
  return (T*)this;
}

template<class T>
Vec3<T>::operator const T*() const
{
  return (T*)this;
}

#endif // _VEC3_H
