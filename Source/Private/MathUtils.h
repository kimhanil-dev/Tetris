#pragma once

union Vector3
{
	float v[3];
	
	struct {
		float x, y, z;
	};
};

union Vector4
{
	float v[4];
	
	struct {
		float x, y, z, w;
	};
};

union Matrix4
{
	float m[4][4];
	struct {
		float  _m11, _m12, _m13, _m14,
			_m21, _m22, _m23, _m24,
			_m31, _m32, _m33, _m34,
			_m41, _m42, _m43, _m44;

	};
	
	static void Identity(Matrix4& out)
	{
		out =	{ 1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1 };
	}

#define GET_MULTIPLIED_MATRIX_ELEMENT(lv,rv,i,j) lv[i][0] * rv[0][j] + lv[i][1] * rv[1][j] + lv[i][2] * rv[2][j] + lv[i][3] * rv[3][j]

	Matrix4 operator * (const Matrix4& _rv)
	{
		Matrix4 result;

		result._m11 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 0, 0);
		result._m12 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 0, 1);
		result._m13 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 0, 2);
		result._m14 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 0, 3);

		result._m21 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 1, 0);
		result._m22 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 1, 1);
		result._m23 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 1, 2);
		result._m24 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 1, 3);

		result._m31 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 2, 0);
		result._m32 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 2, 1);
		result._m33 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 2, 2);
		result._m34 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 2, 3);

		result._m41 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 3, 0);
		result._m42 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 3, 1);
		result._m43 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 3, 2);
		result._m44 = GET_MULTIPLIED_MATRIX_ELEMENT(m, _rv.m, 3, 3);
		
		return result;
	}

	Vector4 operator * (const Vector4& _rv)
	{
		Vector4 result;

		result.x = _rv.x * _m11 + _rv.y * _m21 + _rv.z * _m31 + _rv.w * _m41;
		result.y = _rv.x * _m12 + _rv.y * _m22 + _rv.z * _m32 + _rv.w * _m42;
		result.z = _rv.x * _m13 + _rv.y * _m23 + _rv.z * _m33 + _rv.w * _m43;
		result.w = _rv.x * _m14 + _rv.y * _m24 + _rv.z * _m34 + _rv.w * _m44;

		return result;
	}
};
