#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "cassert"
#include "Arithmetic.h"
#include "structure.h"

class Maths
{
public:

	static float Cot(float x);

	static Vector3 Add(const Vector3& v1, const Vector3& v2);

	static Vector3 Subtract(const Vector3& v1, const Vector3& v2);

	static Vector3 Multiply(float s, const Vector3& v);

	static Vector3 Forward(const Vector3& v, float s);

	static float Length(const Vector3& v);

	static Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

	static Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	static Matrix4x4 IdentityMatrix();

	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	static Matrix4x4 MakeRotateXMatrix(float theta);

	static Matrix4x4 MakeRotateYMatrix(float theta);

	static Matrix4x4 MakeRotateZMatrix(float theta);

	static Matrix4x4 MakeRotateMatrix(const Vector3& rotate);

	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	static Matrix4x4 AffineMatrix(const WorldTransform &affine);

	static Matrix4x4 STRAffineMatrix(const WorldTransform& affine);

	static Matrix4x4 Inverse(const Matrix4x4& m);

	static Matrix4x4 MakePerspectiveFovMatrix(float fovY,float aspectRatio,float nearClip,float farClip);

	static Matrix4x4 ViewportMatrix(float left, float top, float width,float height,float minDepth,float maxDepth);

	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

};

