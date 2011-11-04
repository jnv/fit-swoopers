#ifndef _MATRIX4_H
#define _MATRIX4_H

#include <iostream>

#include "Vec3.h"

/*!
 * \brief Template class for Matrix4x4 math
 *
 * Matrix is stored in 2D array using column-major order.
 * So m[c][r] accesses column c and row r.
 * If you cast Matrix4 to pointer to 16 floating point numbers,
 * you can write m[c * 4 + r].
 * Use Matrix4f and Matrix4d typedefs for float and double matrices.
 */
template<class T>
class Matrix4
{
public:
  /// undefined matrix
  Matrix4();
  /// copy constructor
  Matrix4(const Matrix4 &p);
  /// copy from array
  Matrix4(const T * f);

  /// \name Math operators
  /// @{

  Vec3<T> operator* (const Vec3<T>& v) const;
  Matrix4 operator* (const Matrix4 &p) const;
  /// A *= B; means A = A * B (right multiplication)
  Matrix4& operator*= (const Matrix4 &p);
  Matrix4& operator= (const Matrix4 &p);
  Matrix4 operator* (const T &p) const;
  /// @}

  /// \name Graphics operations
  /// @{

  /// this matrix is translated by tr
  void translate(const Vec3<T>& tr);
  /// this matrix is scaled by sc
  void scale(const Vec3<T>& sc);
  /** this matrix is rotated by angle a in axis av
   * \param a is angle in radians
   */
  void rotate(T a,const Vec3<T>& av);
  /// @}

  /// \name Constructors of new matrices
  /// @{

  /// new matrix with all elements set to zero
  static const Matrix4 Zeros();
  /// returns identity matrix
  static const Matrix4 Identity();

  /// generates new translation matrix from tr
  static const Matrix4 FromTranslation(const Vec3<T>& tr);
  /// generates new scale matrix from sc
  static const Matrix4 FromScale(const Vec3<T>& sc);
  /** generates new rotation matrix
   * \param a is rotation angle in radians
   * \param av is rotation vector
   */
  static const Matrix4 FromRotation(T a,const Vec3<T>& av);

  /** generates perspective projection matrix
   *
   * \f[ f = cotangent(fovy/2) \f]
   * The generated matrix is
   * \f[
       \left(
       \begin{array}{cccc}
       \frac{f}{aspect} & 0 & 0 & 0\\
          0 & f & 0 & 0\\
          0 & 0 &\frac{zFar+zNear}{zNear-zFar} & \frac{2*zFar*zNear}{zNear-zFar}\\
          0 & 0 & -1 & 0\\
       \end{array}
       \right)
     \f]
   * \param fovy is in radians
   */
  static const Matrix4 Perspective(T fovy, T aspect, T zNear, T zFar);

  /// @}

  /// \name Data access
  /// @{

  /// get the column major matrix
  T * mat() { return m; }
  /// get the column major matrix
  const T * mat() const { return m; }
  /// get the column major matrix
  operator T*();
  /// get the column major matrix
  operator const T*() const;
  /// @}

  /// print matrix
  void dump()const;

 private:
  T m[4][4];
};

/// float matrix
typedef Matrix4<float> Matrix4f;
/// double matrix
typedef Matrix4<double> Matrix4d;


template<class T>
Matrix4<T>::Matrix4()
{
    //Unit();
}

template<class T>
Matrix4<T>::Matrix4(const Matrix4<T> &p)
{
  for (unsigned short j=0; j<4; j++)
  {
    m[j][0] = p.m[j][0];
    m[j][1] = p.m[j][1];
    m[j][2] = p.m[j][2];
    m[j][3] = p.m[j][3];
  }
}

template<class T>
Matrix4<T>::Matrix4(const T * f)
{
  for (unsigned short j=0; j<4; j++)
    for (unsigned short i=0; i<4; i++)
      m[i][j] = f[j + 4*i];
}

template<class T>
Matrix4<T> & Matrix4<T>::operator= (const Matrix4<T>  &p)
{
  for (unsigned short j=0; j<4; j++)
  {
    m[j][0] = p.m[j][0];
    m[j][1] = p.m[j][1];
    m[j][2] = p.m[j][2];
    m[j][3] = p.m[j][3];
  }
  return *this;
}

template<class T>
const Matrix4<T> Matrix4<T>::Zeros()
{
  Matrix4<T> ret;
  for (unsigned short j=0; j<4; j++)
  {
    ret.m[j][0] = 0;
    ret.m[j][1] = 0;
    ret.m[j][2] = 0;
    ret.m[j][3] = 0;
  }
  return ret;
}

template<class T>
const Matrix4<T> Matrix4<T>::Identity()
{
  Matrix4<T> ret;
  ret.m[0][0] = (T)1; ret.m[0][1] = 0; ret.m[0][2] = 0; ret.m[0][3] = 0;
  ret.m[1][0] = 0; ret.m[1][1] = (T)1; ret.m[1][2] = 0; ret.m[1][3] = 0;
  ret.m[2][0] = 0; ret.m[2][1] = 0; ret.m[2][2] = (T)1; ret.m[2][3] = 0;
  ret.m[3][0] = 0; ret.m[3][1] = 0; ret.m[3][2] = 0; ret.m[3][3] = (T)1;
  return ret;
}

template<class T>
void Matrix4<T>::translate(const Vec3<T>& tr)
{
  *this *= FromTranslation(tr);
}

template<class T>
const Matrix4<T> Matrix4<T>::FromTranslation(const Vec3<T>& tr)
{
  Matrix4<T> t = Identity();
  t.m[3][0]=tr.x;
  t.m[3][1]=tr.y;
  t.m[3][2]=tr.z;
  return t;
}

template<class T>
void Matrix4<T>::scale(const Vec3<T>& sc)
{
  Matrix4<T> t = Zeros();
  t.m[0][0]=sc.x;
  t.m[1][1]=sc.y;
  t.m[2][2]=sc.z;
  t.m[3][3]=(T)1;
  *this *= t;
}

template<class T>
const Matrix4<T> Matrix4<T>::FromScale(const Vec3<T>& sc)
{
  Matrix4<T> t = Zeros();
  t.m[0][0]=sc.x;
  t.m[1][1]=sc.y;
  t.m[2][2]=sc.z;
  t.m[3][3]=(T)1;
  return t;
}

template<class T>
const Matrix4<T> Matrix4<T>::FromRotation(T a,const Vec3<T>& av)
{
  /*
      c = cos(angle),	s = sine(angle)
      ( xx(1-c)+u    xy(1-c)-zs  xz(1-c)+ys  0 )
      | yx(1-c)+zsi  yy(1-c)+c   yz(1-c)-xs  0 |
      | xz(1-c)-ys   yz(1-c)+xs  zz(1-c)+c   0 |
      (	0            0           0           1 )
  */
  T len = sqrt(av.x * av.x + av.y * av.y + av.z * av.z);
  Vec3<T> v = Vec3<T>(av.x / len, av.y / len, av.z / len);
  Matrix4<T> t;
  T c = (T)cos(a);
  T s = (T)sin(a);

  t.m[0][0] = v.x*v.x*(1-c)+c;
  t.m[1][0] = v.x*v.y*(1-c)-v.z*s;
  t.m[2][0] = v.x*v.z*(1-c)+v.y*s;
  t.m[0][1] = v.y*v.x*(1-c)+v.z*s;
  t.m[1][1] = v.y*v.y*(1-c)+c;
  t.m[2][1] = v.y*v.z*(1-c)-v.x*s;
  t.m[0][2] = v.x*v.z*(1-c)-v.y*s;
  t.m[1][2] = v.y*v.z*(1-c)+v.x*s;
  t.m[2][2] = v.z*v.z*(1-c)+c;
  t.m[3][0] = t.m[3][1] = t.m[3][2] = t.m[0][3] = t.m[1][3] = t.m[2][3] = 0;
  t.m[3][3] = (T)1;
  return t;
}

template<class T>
const Matrix4<T> Matrix4<T>::Perspective(T fovy, T aspect, T zNear, T zFar)
{
  T f = 1.0 / tan(fovy * 0.5);

  Matrix4<T> ret;
  ret.m[0][0] = f / aspect;
  ret.m[1][0] = ret.m[2][0] = ret.m[3][0] = 0.0;
  ret.m[1][1] = f;
  ret.m[0][1] = ret.m[2][1] = ret.m[3][1] = 0.0;
  ret.m[2][2] = (zNear + zFar) / (zNear - zFar);
  ret.m[3][2] = (2 * zNear * zFar) / (zNear - zFar);
  ret.m[0][2] = ret.m[1][2] = 0.0;
  ret.m[2][3] = -1.0;
  ret.m[0][3] = ret.m[1][3] = ret.m[3][3] = 0.0;
  return ret;
}

template<class T>
void Matrix4<T>::rotate(T a,const Vec3<T>& av)
{
  *this  *= FromRotation(a,av);
}

template<class T>
Vec3<T> Matrix4<T>::operator* (const Vec3<T>& v) const
{
  Vec3<T> o;
  o.x = m[0][0]*v.x + m[1][0]*v.y + m[2][0]*v.z + m[3][0];
  o.y = m[0][1]*v.x + m[1][1]*v.y + m[2][1]*v.z + m[3][1];
  o.z = m[0][2]*v.x + m[1][2]*v.y + m[2][2]*v.z + m[3][2];
  return o;
}

template<class T>
Matrix4<T> Matrix4<T>::operator* (const Matrix4& p) const
{
  Matrix4<T> o;
  o.m[0][0] = p.m[0][0]*m[0][0] + p.m[0][1]*m[1][0] + p.m[0][2]*m[2][0] + p.m[0][3]*m[3][0];
  o.m[0][1] = p.m[0][0]*m[0][1] + p.m[0][1]*m[1][1] + p.m[0][2]*m[2][1] + p.m[0][3]*m[3][1];
  o.m[0][2] = p.m[0][0]*m[0][2] + p.m[0][1]*m[1][2] + p.m[0][2]*m[2][2] + p.m[0][3]*m[3][2];
  o.m[0][3] = p.m[0][0]*m[0][3] + p.m[0][1]*m[1][3] + p.m[0][2]*m[2][3] + p.m[0][3]*m[3][3];

  o.m[1][0] = p.m[1][0]*m[0][0] + p.m[1][1]*m[1][0] + p.m[1][2]*m[2][0] + p.m[1][3]*m[3][0];
  o.m[1][1] = p.m[1][0]*m[0][1] + p.m[1][1]*m[1][1] + p.m[1][2]*m[2][1] + p.m[1][3]*m[3][1];
  o.m[1][2] = p.m[1][0]*m[0][2] + p.m[1][1]*m[1][2] + p.m[1][2]*m[2][2] + p.m[1][3]*m[3][2];
  o.m[1][3] = p.m[1][0]*m[0][3] + p.m[1][1]*m[1][3] + p.m[1][2]*m[2][3] + p.m[1][3]*m[3][3];

  o.m[2][0] = p.m[2][0]*m[0][0] + p.m[2][1]*m[1][0] + p.m[2][2]*m[2][0] + p.m[2][3]*m[3][0];
  o.m[2][1] = p.m[2][0]*m[0][1] + p.m[2][1]*m[1][1] + p.m[2][2]*m[2][1] + p.m[2][3]*m[3][1];
  o.m[2][2] = p.m[2][0]*m[0][2] + p.m[2][1]*m[1][2] + p.m[2][2]*m[2][2] + p.m[2][3]*m[3][2];
  o.m[2][3] = p.m[2][0]*m[0][3] + p.m[2][1]*m[1][3] + p.m[2][2]*m[2][3] + p.m[2][3]*m[3][3];

  o.m[3][0] = p.m[3][0]*m[0][0] + p.m[3][1]*m[1][0] + p.m[3][2]*m[2][0] + p.m[3][3]*m[3][0];
  o.m[3][1] = p.m[3][0]*m[0][1] + p.m[3][1]*m[1][1] + p.m[3][2]*m[2][1] + p.m[3][3]*m[3][1];
  o.m[3][2] = p.m[3][0]*m[0][2] + p.m[3][1]*m[1][2] + p.m[3][2]*m[2][2] + p.m[3][3]*m[3][2];
  o.m[3][3] = p.m[3][0]*m[0][3] + p.m[3][1]*m[1][3] + p.m[3][2]*m[2][3] + p.m[3][3]*m[3][3];

  return o;
}

template<class T>
Matrix4<T> Matrix4<T>::operator* (const T  &p) const
{
  Matrix4 o;
  for (unsigned short j=0; j<4; j++)
  {
    o.m[j][0] = m[j][0] * p;
    o.m[j][1] = m[j][1] * p;
    o.m[j][2] = m[j][2] * p;
    o.m[j][3] = m[j][3] * p;
  }
  return o;
}

template<class T>
Matrix4<T>& Matrix4<T>::operator*= (const Matrix4<T>& p)
{
  return *this = this->operator*(p);
}

template<class T>
Matrix4<T>::operator T*()
{
  return (T*)this;
}

template<class T>
Matrix4<T>::operator const T*() const
{
  return (T*)this;
}

template<class T>
void Matrix4<T>::dump()const
{
  for (unsigned short r=0; r<4; r++)
  {
    for (unsigned short c=0; c<4; c++)
      std::cout << m[c][r] << " ";
    std::cout << std::endl;
  }
}

#endif // _MATRIX4_H
