#ifndef MATRIX4_CLASS_H
#define MATRIX4_CLASS_H

#include "matrix3.h"

#pragma intrinsic(sin, cos)

template <typename T>
struct matrix4
{
	typedef T element_type;
	typedef matrix4 &reference;
	typedef matrix4 const &const_reference;

	union
	{
		element_type m[4][4];
		element_type ma[16];
		struct
		{
			element_type xx, xy, xz, xw;
			element_type yx, yy, yz, yw;
			element_type zx, zy, zz, zw;
			element_type wx, wy, wz, ww;
		};
		struct
		{
			vec3<element_type> axis_x;	element_type pad_xw;
			vec3<element_type> axis_y;	element_type pad_yw;
			vec3<element_type> axis_z;	element_type pad_zw;
			union
			{
				struct
				{
					vec3<element_type> axis_w;	element_type pad_ww;
				};
				struct
				{
					vec3<element_type> axis_pos;
				};
			};
		};
	};

	// Constructors
	matrix4(void) { }
	explicit matrix4(element_type n);
	matrix4(element_type m00, element_type m01, element_type m02, element_type m03,
			  element_type m10, element_type m11, element_type m12, element_type m13,
			  element_type m20, element_type m21, element_type m22, element_type m23,
			  element_type m30, element_type m31, element_type m32, element_type m33);

   inline T operator[](size_t index)
   {
	   return ma[index];
   }

   inline const T operator[](size_t index) const
   {
	   return ma[index];
   }

	template <typename MT>
	reference operator*=(const matrix4<MT> &rhs);

	template<typename MT>
	matrix4<element_type> operator*(const matrix4<MT> &matb);

	template <typename MT>
	reference rotation(const matrix3<MT> &m);

	matrix3<element_type> rotation(void) const;

	vec3<element_type> scale(void) const;

	reference make_zero(void);
	reference make_identity(void);

	// "this" BECOMES the specified rotation matrix
	// make_rotation_ does NOT rotate "this", it constructs
	// a rotation matrix of 'radians' on the specified axis.
	reference make_rotation_x(element_type radians);
	reference make_rotation_y(element_type radians);
	reference make_rotation_z(element_type radians);

	// "this" BECOMES the specified scale matrix
	// make_rotation_ does NOT scale "this", it constructs
	// a scale matrix.
	reference make_scale(element_type x, element_type y, element_type z);

	template <typename VT>
	reference make_scale(const vec3<VT> &v);

	// "this" BECOMES the specified translation matrix
	// make_rotation_ does NOT rotate "this", it constructs
	// a translation matrix.
	reference make_translation(element_type x, element_type y, element_type z);
	
   template <typename VT>
	reference make_translation(const vec3<VT> &v);

	// pre is rotation * mat
	reference rotate_x_pre(element_type radians);
	reference rotate_y_pre(element_type radians);
	reference rotate_z_pre(element_type radians);

	// post is mat * rotation
	reference rotate_x_post(element_type radians);
	reference rotate_y_post(element_type radians);
	reference rotate_z_post(element_type radians);

	// scale * this
	reference scale_pre(element_type x, element_type y, element_type z);
   template <typename VT>
	reference scale_pre(const vec3<VT> &v);

	// this * scale
	reference scale_post(element_type x, element_type y, element_type z);
	template <typename VT>
	reference scale_post(const vec3<VT> &v);

	// trans * this
	reference translate_pre(element_type x, element_type y, element_type z);
	template <typename VT>
	reference translate_pre(const vec3<VT> &v);

	// this * trans
	reference translate_post(element_type x, element_type y, element_type z);
	template <typename VT>
	reference translate_post(const vec3<VT> &v);

	reference invert(void);
	reference transpose(void);
	reference negate(void);

	// Transposes the 3x3 and inverts position.  (Must be orthonormal)
//	void fast_invert(void);

	// nVidia wrote this.  I do not know if this is working.
//	bool calcProjectionMatrixDeg(float fFOV_degrees, float fAspect, float Znear, float Zfar);

	// normalize each 3x1 vector of the 3x3 subset
	reference normalize3x3vectors(void);

   template <typename T>
   inline friend matrix4<T> interpolate( const matrix4<T> &MatrixA, const matrix4<T> &MatrixB, float lamda)
   {
		matrix4<T> iMat(MatrixA);
		matrix4<T> result(MatrixB);

		// Inverse of MatrixA
		iMat.invert();

		// Remove MatrixA's transformation from MatrixB
		result *= iMat;

		// iMat is now the intermediary transformation from MatrixA to MatrixB
		// ie: iMat * MatrixA = MatrixB
		iMat = result;

		// The trace of our matrix
		T trace = 1.0f + iMat.ma[0] + iMat.ma[5] + iMat.ma[10];

		T quatResult[4];

		// Calculate the quaternion of iMat
		// If trace is greater than 0, but consider small values that
		// might result in 0 when operated upon due to floating point error
		if( trace > 0.00000001 )
		{
			T S = sqrt(trace)*2;
			quatResult[0] = (iMat.ma[9] - iMat.ma[6]) / S;
			quatResult[1] = (iMat.ma[2] - iMat.ma[8]) / S;
			quatResult[2] = (iMat.ma[4] - iMat.ma[1]) / S;
			quatResult[3] = T( 0.25 ) * S;
		}
		else
		{
			if( iMat.ma[0] > iMat.ma[5] && iMat.ma[0] > iMat.ma[10] )
			{
				T S = sqrt( T( 1.0 ) + iMat.ma[0] - iMat.ma[5] - iMat.ma[10] ) * 2;
				quatResult[0] = T( 0.25 ) * S;
				quatResult[1] = (iMat.ma[4] + iMat.ma[1]) / S;
				quatResult[2] = (iMat.ma[2] + iMat.ma[8]) / S;
				quatResult[3] = (iMat.ma[9] - iMat.ma[6]) / S;
			}
			else if( iMat.ma[5] > iMat.ma[10] )
			{
				T S = sqrt( T( 1.0 ) + iMat.ma[5] - iMat.ma[0] - iMat.ma[10] ) * 2;
				quatResult[0] = (iMat.ma[4] + iMat.ma[1]) / S;
				quatResult[1] = T( 0.25 ) * S;
				quatResult[2] = (iMat.ma[9] + iMat.ma[6]) / S;
				quatResult[3] = (iMat.ma[2] - iMat.ma[8]) / S;
			}
			else
			{
				T S = sqrt( T( 1.0 ) + iMat.ma[10] - iMat.ma[0] - iMat.ma[5] ) * 2;
				quatResult[0] = (iMat.ma[2] + iMat.ma[8]) / S;
				quatResult[1] = (iMat.ma[9] + iMat.ma[6]) / S;
				quatResult[2] = T( 0.25 ) * S;
				quatResult[3] = (iMat.ma[4] - iMat.ma[1]) / S;
			}
		}

		// Get the magnitude of our quaternion
		T quatMagnitude = sqrt( quatResult[0]*quatResult[0] + quatResult[1]*quatResult[1] + quatResult[2]*quatResult[2] + quatResult[3]*quatResult[3] );

		// Normalize our quaternion
		T quatNormalized[4] = { quatResult[0]/quatMagnitude, quatResult[1]/quatMagnitude, quatResult[2]/quatMagnitude, quatResult[3]/quatMagnitude }; 

		// Calculate the angles relevant to our quaternion
		T cos_a = quatNormalized[3];
		T angle = acos( cos_a ) * 2;
		T sin_a = sqrt( T( 1.0 ) - cos_a * cos_a );
	
		// If there was no rotation between matrices, calculation
		// of the rotation matrix will end badly. So just do the linear
		// interpolation of the translation component and return
		if( angle == 0.0 )
		{
			result = MatrixA;

			result.m[3][0] = MatrixA.m[3][0] + ((MatrixB.m[3][0]-MatrixA.m[3][0])*lamda);
			result.m[3][1] = MatrixA.m[3][1] + ((MatrixB.m[3][1]-MatrixA.m[3][1])*lamda);
			result.m[3][2] = MatrixA.m[3][2] + ((MatrixB.m[3][2]-MatrixA.m[3][2])*lamda);

			return result;
		}


		// Our axis of abitrary rotation
		vec3<T> axis;

		if( fabs( sin_a ) < 0.0005 )
			sin_a = 1;

		axis.x = quatNormalized[0] / sin_a;
		axis.y = quatNormalized[1] / sin_a;
		axis.z = quatNormalized[2] / sin_a;

		// Get the portion of the angle to rotate by
		angle *= lamda;

		axis.normalize();

		// Calculate the quaternion for our new (partial) angle of rotation
		sin_a = sin( angle / 2 );
		cos_a = cos( angle / 2 );
		quatNormalized[0] = axis.x * sin_a;
		quatNormalized[1] = axis.y * sin_a;
		quatNormalized[2] = axis.z * sin_a;
		quatNormalized[3] = cos_a;

		quatMagnitude = sqrt( quatNormalized[0]*quatNormalized[0] + quatNormalized[1]*quatNormalized[1] + quatNormalized[2]*quatNormalized[2] + quatNormalized[3]*quatNormalized[3] );		
		quatNormalized[0] /= quatMagnitude;
		quatNormalized[1] /= quatMagnitude;
		quatNormalized[2] /= quatMagnitude;
		quatNormalized[3] /= quatMagnitude;

		// Calculate our partial rotation matrix
		T xx      = quatNormalized[0] * quatNormalized[0];
		T xy      = quatNormalized[0] * quatNormalized[1];
		T xz      = quatNormalized[0] * quatNormalized[2];
		T xw      = quatNormalized[0] * quatNormalized[3];
		T yy      = quatNormalized[1] * quatNormalized[1];
		T yz      = quatNormalized[1] * quatNormalized[2];
		T yw      = quatNormalized[1] * quatNormalized[3];
		T zz      = quatNormalized[2] * quatNormalized[2];
		T zw      = quatNormalized[2] * quatNormalized[3];

		result.ma[0]  = 1 - 2 * ( yy + zz );
		result.ma[1]  =     2 * ( xy - zw );
		result.ma[2]  =     2 * ( xz + yw );
		result.ma[4]  =     2 * ( xy + zw );
		result.ma[5]  = 1 - 2 * ( xx + zz );
		result.ma[6]  =     2 * ( yz - xw );
		result.ma[8]  =     2 * ( xz - yw );
		result.ma[9]  =     2 * ( yz + xw );
		result.ma[10] = 1 - 2 * ( xx + yy );
		result.ma[3]  = result.ma[7] = result.ma[11] = result.ma[12] = result.ma[13] = result.ma[14] = 0;
		result.ma[15] = 1;

		// Combine our partial rotation with MatrixA
		result *= MatrixA;

		// Linear interpolation of the translation components of the matrices
		result.m[3][0] = MatrixA.m[3][0] + ((MatrixB.m[3][0]-MatrixA.m[3][0])*lamda);
		result.m[3][1] = MatrixA.m[3][1] + ((MatrixB.m[3][1]-MatrixA.m[3][1])*lamda);
		result.m[3][2] = MatrixA.m[3][2] + ((MatrixB.m[3][2]-MatrixA.m[3][2])*lamda);

		return result;
   }
};

template <typename T>
matrix4<T>::matrix4(element_type n)
:	xx(n), xy(n), xz(n), xw(n),
	yx(n), yy(n), yz(n), yw(n),
	zx(n), zy(n), zz(n), zw(n),
	wx(n), wy(n), wz(n), ww(n)
{
}

template <typename T>
matrix4<T>::matrix4(element_type m00, element_type m01, element_type m02, element_type m03,
						  element_type m10, element_type m11, element_type m12, element_type m13,
						  element_type m20, element_type m21, element_type m22, element_type m23,
						  element_type m30, element_type m31, element_type m32, element_type m33)
:	xx(m00), xy(m01), xz(m02), xw(m03),
	yx(m10), yy(m11), yz(m12), yw(m13),
	zx(m20), zy(m21), zz(m22), zw(m23),
	wx(m30), wy(m31), wz(m32), ww(m33)
{
}

template <typename T>
template <typename MT>
inline typename matrix4<T>::reference matrix4<T>::rotation(const matrix3<MT> &m)
{
	xx = m.xx;	xy = m.xy;	xz = m.xz;
	yx = m.yx;	yy = m.yy;	yz = m.yz;
	zx = m.zx;	zy = m.zy;	zz = m.zz;
	return *this;
}

template <typename T>
matrix3<T> matrix4<T>::rotation(void) const
{
	return matrix3<T>(xx, xy, xz, yx, yy, yz, zx, zy, zz);
}


template <typename T>
vec3<T> matrix4<T>::scale(void) const
{
	return vec3<T>(xx, yy, zz);
}

template <typename T>
typename matrix4<T>::reference matrix4<T>::make_zero(void)
{
   // TODO: would memset be faster here?
	xx = 0;	xy = 0;	xz = 0;	xw = 0;
	yx = 0;	yy = 0;	yz = 0;	yw = 0;
	zx = 0;	zy = 0;	zz = 0;	zw = 0;
	wx = 0;	wy = 0;	ww = 0;	ww = 0;
	return *this;
}

template <typename T>
typename matrix4<T>::reference matrix4<T>::make_identity(void)
{
	xx = 1;	xy = 0;	xz = 0;	xw = 0;
	yx = 0;	yy = 1;	yz = 0;	yw = 0;
	zx = 0;	zy = 0;	zz = 1;	zw = 0;
	wx = 0;	wy = 0;	wz = 0;	ww = 1;
	return *this;
}

template <typename T>
typename matrix4<T>::reference matrix4<T>::negate(void)
{
	*this *= -1;
	return *this;
}

template <typename T>
typename matrix4<T>::reference matrix4<T>::make_rotation_x(element_type radians)
{
	element_type sin_angle, cos_angle;
	sin_angle = std::sin(radians);
	cos_angle = std::cos(radians);

	xx = 1;	xy = 0;				xz = 0;				xw = 0;
	yx = 0;	yy = cos_angle;	yz = sin_angle;	yw = 0;
	zx = 0;	zy = -sin_angle;	zz = cos_angle;	zw = 0;
	wx = 0;	wy = 0;				wz = 0;				ww = 1;
	return *this;
}

template <typename T>
typename matrix4<T>::reference matrix4<T>::make_rotation_y(element_type radians)
{
	element_type sin_angle, cos_angle;
	sin_angle = std::sin(radians);
	cos_angle = std::cos(radians);

	xx = cos_angle;	xy = 0;	xz = -sin_angle;	xw = 0;
	yx = 0;				yy = 1;	yz = 0;				yw = 0;
	zx = sin_angle;	zy = 0;	zz = cos_angle;	zw = 0;
	wx = 0;				wy = 0;	wz = 0;				ww = 1;
	return *this;
}

template <typename T>
typename matrix4<T>::reference matrix4<T>::make_rotation_z(element_type radians)
{
	element_type sin_angle, cos_angle;
	sin_angle = sin(radians);
	cos_angle = cos(radians);

	xx = cos_angle;	xy = sin_angle;	xz = 0;	xw = 0;
	yx = -sin_angle;	yy = cos_angle;	yz = 0;	yw = 0;
	zx = 0;				zy = 0;				zz = 1;	zw = 0;
	wx = 0;				wy = 0;				wz = 0;	ww = 1;
	return *this;
}

template <typename T>
typename matrix4<T>::reference
matrix4<T>::make_scale(element_type x, element_type y, element_type z)
{
	xx = x;	xy = 0;	xz = 0;	xw = 0;
	yx = 0;	yy = y;	yz = 0;	yw = 0;
	zx = 0;	zy = 0;	zz = z;	zw = 0;
	wx = 0;	wy = 0;	wz = 0;	ww = 1;
	return *this;
}

template <typename T>
template <typename VT>
typename matrix4<T>::reference matrix4<T>::make_scale(const vec3<VT> &v)
{
	xx = v.x;	xy = 0;		xz = 0;		xw = 0;
	yx = 0;		yy = v.y;	yz = 0;		yw = 0;
	zx = 0;		zy = 0;		zz = v.z;	zw = 0;
	wx = 0;		wy = 0;		wz = 0;		ww = 1;
	return *this;
}

template <typename T>
typename matrix4<T>::reference
matrix4<T>::make_translation(element_type x, element_type y, element_type z)
{
	xx = 1;	xy = 0;	xz = 0;	xw = 0;
	yx = 0;	yy = 1;	yz = 0;	yw = 0;
	zx = 0;	zy = 0;	zz = 1;	zw = 0;
	wx = x;	wy = y;	wz = z;	ww = 1;
	return *this;
}

template <typename T>
template <typename VT>
typename matrix4<T>::reference matrix4<T>::make_translation(const vec3<VT> &v)
{
	xx = 1;		xy = 0;		xz = 0;		xw = 0;
	yx = 0;		yy = 1;		yz = 0;		yw = 0;
	zx = 0;		zy = 0;		zz = 1;		zw = 0;
	wx = v.x;	wy = v.y;	wz = v.z;	ww = 1;
	return *this;
}

template <typename T>
typename matrix4<T>::reference matrix4<T>::transpose(void)
{
	std::swap(xy, yx);
	std::swap(xz, zx);
	std::swap(xw, wx);	
	std::swap(yz, zy);
	std::swap(yw, wy);
	std::swap(zw, wz);
	return *this;
}

template <typename T>
typename matrix4<T>::reference matrix4<T>::invert(void)
{
	*this = matrix4<T>(xx, yx, zx, 0,
							 xy, yy, zy, 0,
							 xz, yz, zz, 0,
							 -(wx * xx + wy * xy + wz * xz),
							 -(wx * yx + wy * yy + wz * yz),
							 -(wx * zx + wy * zy + wz * zz), 1);
	return *this;
}

template <typename T>
typename matrix4<T>::reference matrix4<T>::rotate_x_pre(element_type radians)
{
	*this = matrix4<T>().make_rotation_x(radians) * (*this);
	return *this;
}

template <typename T>
typename matrix4<T>::reference matrix4<T>::rotate_y_pre(element_type radians)
{
	*this = matrix4<T>().make_rotation_y(radians) * (*this);
	return *this;
}

template <typename T>
typename matrix4<T>::reference matrix4<T>::rotate_z_pre(element_type radians)
{
	*this = matrix4<T>().make_rotation_z(radians) * (*this);
	return *this;
}

template <typename T>
typename matrix4<T>::reference matrix4<T>::rotate_x_post(element_type radians)
{
	*this *= matrix4<T>().make_rotation_x(radians);
	return *this;
}

template <typename T>
typename matrix4<T>::reference matrix4<T>::rotate_y_post(element_type radians)
{
	*this *= matrix4<T>().make_rotation_y(radians);
	return *this;
}

template <typename T>
typename matrix4<T>::reference matrix4<T>::rotate_z_post(element_type radians)
{
	*this *= matrix4<T>().make_rotation_z(radians);
	return *this;
}

template <typename T>
typename matrix4<T>::reference
matrix4<T>::scale_pre(element_type x, element_type y, element_type z)
{
	*this = matrix4<T>().make_scale(x, y, z) * (*this);
	return *this;
}

template <typename T>
template <typename VT>
typename matrix4<T>::reference matrix4<T>::scale_pre(const vec3<VT> &v)
{
	*this = matrix4<T>().make_scale(v) * (*this);
	return *this;
}

template <typename T>
typename matrix4<T>::reference
matrix4<T>::scale_post(element_type x, element_type y, element_type z)
{
	*this *= matrix4<T>().make_scale(x, y, z);
	return *this;
}

template <typename T>
template <typename VT>
typename matrix4<T>::reference matrix4<T>::scale_post(const vec3<VT> &v)
{
	*this *= matrix4<T>().make_scale(v);
	return *this;
}

template <typename T>
typename matrix4<T>::reference
matrix4<T>::translate_pre(element_type x, element_type y, element_type z)
{
	*this = matrix4<T>().make_translation(x, y, z) * (*this);
	return *this;
}

template <typename T>
template <typename VT>
typename matrix4<T>::reference matrix4<T>::translate_pre(const vec3<VT> &v)
{
	*this = matrix4<T>().make_translation(v) * (*this);
	return *this;
}

template <typename T>
typename matrix4<T>::reference
matrix4<T>::translate_post(element_type x, element_type y, element_type z)
{
	*this *= matrix4<T>().make_translation(x, y, z);
	return *this;
}

template <typename T>
template <typename VT>
typename matrix4<T>::reference matrix4<T>::translate_post(const vec3<VT> &v)
{
	*this *= matrix4<T>().make_translation(v);
	return *this;
}

template <typename T>
typename matrix4<T>::reference matrix4<T>::normalize3x3vectors(void)
{
	axis_x.normalize();
	axis_y.normalize();
	axis_z.normalize();
    return *this;
}

template <typename T>
template <typename MT>
typename matrix4<T>::reference matrix4<T>::operator*=(const matrix4<MT> &rhs)
{
	matrix4<T> temp(xx * rhs.xx + yx * rhs.xy + zx * rhs.xz + wx * rhs.xw,
						 xy * rhs.xx + yy * rhs.xy + zy * rhs.xz + wy * rhs.xw,
						 xz * rhs.xx + yz * rhs.xy + zz * rhs.xz + wz * rhs.xw,
						 xw * rhs.xx + yw * rhs.xy + zw * rhs.xz + ww * rhs.xw,

						 xx * rhs.yx + yx * rhs.yy + zx * rhs.yz + wx * rhs.yw,
						 xy * rhs.yx + yy * rhs.yy + zy * rhs.yz + wy * rhs.yw,
						 xz * rhs.yx + yz * rhs.yy + zz * rhs.yz + wz * rhs.yw,
						 xw * rhs.yx + yw * rhs.yy + zw * rhs.yz + ww * rhs.yw,

						 xx * rhs.zx + yx * rhs.zy + zx * rhs.zz + wx * rhs.zw,
						 xy * rhs.zx + yy * rhs.zy + zy * rhs.zz + wy * rhs.zw,
						 xz * rhs.zx + yz * rhs.zy + zz * rhs.zz + wz * rhs.zw,
						 xw * rhs.zx + yw * rhs.zy + zw * rhs.zz + ww * rhs.zw,

						 xx * rhs.wx + yx * rhs.wy + zx * rhs.wz + wx * rhs.ww,
						 xy * rhs.wx + yy * rhs.wy + zy * rhs.wz + wy * rhs.ww,
						 xz * rhs.wx + yz * rhs.wy + zz * rhs.wz + wz * rhs.ww,
						 xw * rhs.wx + yw * rhs.wy + zw * rhs.wz + ww * rhs.ww);
	*this = temp;
	return *this;
}

template <typename T>
template <typename MT>
matrix4<T> matrix4<T>::operator *(const matrix4<MT> &rhs)
{
	matrix4<T> temp(rhs.xx * xx + rhs.yx * xy + rhs.zx * xz + rhs.wx * xw,
						 rhs.xy * xx + rhs.yy * xy + rhs.zy * xz + rhs.wy * xw,
						 rhs.xz * xx + rhs.yz * xy + rhs.zz * xz + rhs.wz * xw,
						 rhs.xw * xx + rhs.yw * xy + rhs.zw * xz + rhs.ww * xw,

						 rhs.xx * yx + rhs.yx * yy + rhs.zx * yz + rhs.wx * yw,
						 rhs.xy * yx + rhs.yy * yy + rhs.zy * yz + rhs.wy * yw,
						 rhs.xz * yx + rhs.yz * yy + rhs.zz * yz + rhs.wz * yw,
						 rhs.xw * yx + rhs.yw * yy + rhs.zw * yz + rhs.ww * yw,

						 rhs.xx * zx + rhs.yx * zy + rhs.zx * zz + rhs.wx * zw,
						 rhs.xy * zx + rhs.yy * zy + rhs.zy * zz + rhs.wy * zw,
						 rhs.xz * zx + rhs.yz * zy + rhs.zz * zz + rhs.wz * zw,
						 rhs.xw * zx + rhs.yw * zy + rhs.zw * zz + rhs.ww * zw,

						 rhs.xx * wx + rhs.yx * wy + rhs.zx * wz + rhs.wx * ww,
						 rhs.xy * wx + rhs.yy * wy + rhs.zy * wz + rhs.wy * ww,
						 rhs.xz * wx + rhs.yz * wy + rhs.zz * wz + rhs.wz * ww,
						 rhs.xw * wx + rhs.yw * wy + rhs.zw * wz + rhs.ww * ww);
	return temp;
}

#pragma function(sin, cos)

typedef matrix4<float> matrix4f;
typedef matrix4<double> matrix4d;
typedef matrix4f float4x4;
typedef matrix4d double4x4;

#endif
