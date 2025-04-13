#include "Arithmetic.h"

Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
    return Maths::Add(v1, v2);
}

Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
    return Maths::Subtract(v1,v2);
}

Vector3 operator*(float s, const Vector3& v)
{
    return Maths::Multiply(s, v);
}

Vector3 operator*(const Vector3& v, float s)
{
    return s * v;
}

Vector3 operator/(const Vector3& v, float s)
{
    return Maths::Forward(v, s);
}

Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2)
{
    return Maths::Add(m1, m2);
}

Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2)
{
    return Maths::Subtract(m1, m2);
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2)
{
    return Maths::Multiply(m1, m2);
}

Vector3 operator-(const Vector3& v)
{
    return { -v.x, -v.y, -v.z };
}

