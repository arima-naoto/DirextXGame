#include "Maths.h"


float Maths::Cot(float x)
{
	return 1.0f / tanf(x);;
}

Vector2 Maths::Subtract(const Vector2& v1, const Vector2& v2)
{
	Vector2 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	return result;
}

Vector3 Maths::Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

Vector3 Maths::Subtract(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

Vector3 Maths::Multiply(float s, const Vector3& v)
{
	Vector3 result;
	result.x = s * v.x;
	result.y = s * v.y;
	result.z = s * v.z;
	return result;
}

Vector3 Maths::Forward(const Vector3& v, float s)
{
	Vector3 result;
	result.x = 1.0f / s * v.x;
	result.y = 1.0f / s * v.y;
	result.z = 1.0f / s * v.z;
	return result;
}

float Maths::Length(const Vector3& v)
{
	float result = sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));

	return result;
}

Matrix4x4 Maths::Add(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 resultAdd{};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			resultAdd.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}

	return resultAdd;
}

Matrix4x4 Maths::Subtract(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 subtract{};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			subtract.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return subtract;
}

Matrix4x4 Maths::Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 multiply{};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			multiply.m[i][j] = 0;
			for (int k = 0; k < 4; k++)
			{
				multiply.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	return multiply;
}

Matrix4x4 Maths::IdentityMatrix()
{
	Matrix4x4 result{};

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				result.m[i][j] = 1.0f;
			}
			else
			{
				result.m[i][j] = 0.0f;
			}
		}
	}

	return result;
}

Matrix4x4 Maths::MakeScaleMatrix(const Vector3& scale)
{
	return {
		scale.x,0.0f,0.0f,0.0f,
		0.0f,scale.y,0.0f,0.0f,
		0.0f,0.0f,scale.z,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}

Matrix4x4 Maths::MakeRotateXMatrix(float theta)
{
	float cosTheta = cosf(theta);
	float sinTheta = sinf(theta);

	return {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cosTheta,sinTheta,0.0f,
		0.0f,-sinTheta,cosTheta,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}

Matrix4x4 Maths::MakeRotateYMatrix(float theta)
{
	float cosTheta = cosf(theta);
	float sinTheta = sinf(theta);

	return {
		cosTheta,0.0f,-sinTheta,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		sinTheta,0.0f,cosTheta,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}

Matrix4x4 Maths::MakeRotateZMatrix(float theta)
{
	float cosTheta = cosf(theta);
	float sinTheta = sinf(theta);

	return {
		cosTheta,sinTheta,0.0f,0.0f,
		-sinTheta,cosTheta,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}

Matrix4x4 Maths::MakeRotateMatrix(const Vector3& rotate)
{
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

	Matrix4x4 rotateMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;

	return rotateMatrix;
}

Matrix4x4 Maths::MakeTranslateMatrix(const Vector3& translate)
{
	return {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		translate.x,translate.y,translate.z,1.0f
	};
}

Matrix4x4 Maths::AffineMatrix(const WorldTransform& affine)
{
	Matrix4x4 scaleMatrix = MakeScaleMatrix(affine.scale);
	Matrix4x4 rotateMatrix = MakeRotateMatrix(affine.rotate);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(affine.translate);

	Matrix4x4 affineMatrix = scaleMatrix * rotateMatrix * translateMatrix;
	return affineMatrix;
}

Matrix4x4 Maths::STRAffineMatrix(const WorldTransform& affine) {
	Matrix4x4 scaleMatrix = MakeScaleMatrix(affine.scale);
	Matrix4x4 rotateMatrix = MakeRotateMatrix(affine.rotate);
	Matrix4x4 translateMatrix = MakeTranslateMatrix(affine.translate);

	Matrix4x4 affineMatrix = scaleMatrix * translateMatrix * rotateMatrix;
	return affineMatrix;
}

Matrix4x4 Maths::Inverse(const Matrix4x4& m)
{
#pragma region //4x4行列の行列式Aを求める
	float MatrixA = 1.0f / (m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -
		m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -
		m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +
		m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +
		m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -
		m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -
		m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +
		m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);
#pragma endregion

	Matrix4x4 result;

#pragma region 0行目の逆行列を求める

	result.m[0][0] = MatrixA * (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2] -
		m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]);

	result.m[0][1] = MatrixA * (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2] +
		m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);

	result.m[0][2] = MatrixA * (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2] -
		m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);

	result.m[0][3] = MatrixA * (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2] +
		m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);

#pragma endregion

#pragma region 1行目の逆行列を求める

	result.m[1][0] = MatrixA * (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2] +
		m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);

	result.m[1][1] = MatrixA * (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2] -
		m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);

	result.m[1][2] = MatrixA * (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2] +
		m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);

	result.m[1][3] = MatrixA * (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2] -
		m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);

#pragma endregion

#pragma region 2行目の逆行列を求める

	result.m[2][0] = MatrixA * (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1] -
		m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]);

	result.m[2][1] = MatrixA * (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1] +
		m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);

	result.m[2][2] = MatrixA * (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1] -
		m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);

	result.m[2][3] = MatrixA * (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);

#pragma endregion

#pragma region 3行目の逆行列を求める

	result.m[3][0] = MatrixA * (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1] +
		m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);

	result.m[3][1] = MatrixA * (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1] -
		m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);

	result.m[3][2] = MatrixA * (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1] +
		m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);

	result.m[3][3] = MatrixA * (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);


#pragma endregion 

	return result;
}

/// <summary>
/// 転置行列
/// </summary>
/// <param name="m">行列</param>
/// <returns></returns>
Matrix4x4 Maths::Transpose(const Matrix4x4& m)
{
	Matrix4x4 result = {};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m.m[j][i];
		}
	}

	return result;

}

Matrix4x4 Maths::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	return {
		1.0f / aspectRatio * Cot(fovY / 2.0f),0.0f,0.0f,0.0f,
		0.0f,Cot(fovY / 2.0f),0.0f,0.0f,
		0.0f,0.0f,farClip / (farClip - nearClip),1.0f,
		0.0f,0.0f,-(nearClip * farClip) / (farClip - nearClip),0.0f
	};
}

Matrix4x4 Maths::MakePerspectiveFovLH(float fovY, float aspect, float nearZ, float farZ)
{
	Matrix4x4 m{};
	float f = 1.0f / tanf(fovY * 0.5f);

	m.m[0][0] = f / aspect;
	m.m[1][1] = f;
	m.m[2][2] = farZ / (farZ - nearZ);
	m.m[2][3] = 1.0f;
	m.m[3][2] = (-nearZ * farZ) / (farZ - nearZ);

	return m;
}

Matrix4x4 Maths::ViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	return {
		width / 2.0f,0.0f,0.0f,0.0f,
		0.0f,-height / 2.0f,0.0f,0.0f,
		0.0f,0.0f,maxDepth - minDepth,0.0f,
		(left + width) / 2.0f,(top + height) / 2.0f,minDepth,1.0f
	};
}

Vector3 Maths::Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}


