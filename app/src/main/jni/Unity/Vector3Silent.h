#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>


struct Vector3S
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
};
        ///float data[3];
    };


    /**
     * Constructors.
     */
    inline Vector3S();
    inline Vector3S(float data[]);
    inline Vector3S(float value);
    inline Vector3S(float x, float y);
    inline Vector3S(float x, float y, float z);

    /**
     * Constants for common vectors.
     */
    static inline Vector3S zero();
    static inline Vector3S One();
    static inline Vector3S Right();
    static inline Vector3S Left();
    static inline Vector3S Up();
    static inline Vector3S Down();
    static inline Vector3S Forward();
    static inline Vector3S Backward();


    /**
     * Returns the angle between two vectors in radians.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A scalar value.
     */
    static inline float Angle(Vector3S a, Vector3S b);

    /**
     * Returns a vector with its magnitude clamped to maxLength.
     * @param vector: The target vector.
     * @param maxLength: The maximum length of the return vector.
     * @return: A new vector.
     */
    static inline Vector3S ClampMagnitude(Vector3S vector, float maxLength);

    /**
     * Retorna o componente de a na direção de b (projeção escalar).
     * @param a: O vetor de destino.
     * @param b: O vetor que está sendo comparado.
     * @return: Um valor escalar.
     */
    static inline float Component(Vector3S a, Vector3S b);

   /**
    * Retorna o produto vetorial de dois vetores.      
    * @param lhs: O lado esquerdo da multiplicação.     
    * @param rhs: O lado direito da multiplicação.     
    * @return: Um novo vetor.
    */
    static inline Vector3S Cross(Vector3S lhs, Vector3S rhs);

    /**
     * Returns the distance between a and b.
     * @param a: The first point.
     * @param b: The second point.
     * @return: A scalar value.
     */
    static inline float Distance(Vector3S a, Vector3S b);

    static inline char ToChar(Vector3S a);

    /**
     * Returns the dot product of two vectors.
     * @param lhs: The left side of the multiplication.
     * @param rhs: The right side of the multiplication.
     * @return: A scalar value.
     */
    static inline float Dot(Vector3S lhs, Vector3S rhs);

    /**
     * Converte uma representação esférica de um vetor em cartesiano
     * coordenadas.
     * Isso usa a convenção ISO (raio r, inclinação theta, azimute phi).
     * @param rad: A magnitude do vetor.
     * @param theta: O ângulo no plano xy do eixo x.
     * @param phi: O ângulo do eixo z positivo para o vetor.
     * @return: Um novo vetor.
     */
    static inline Vector3S FromSpherical(float rad, float theta, float phi);

    /**
     * Returns a vector linearly interpolated between a and b, moving along
     * a straight line. The vector is clamped to never go beyond the end points.
     * @param a: The starting point.
     * @param b: The ending point.
     * @param t: The interpolation value [0-1].
     * @return: A new vector.
     */
    static inline Vector3S Lerp(Vector3S a, Vector3S b, float t);

    /**
     * Returns a vector linearly interpolated between a and b, moving along
     * a straight line.
     * @param a: The starting point.
     * @param b: The ending point.
     * @param t: The interpolation value [0-1] (no actual bounds).
     * @return: A new vector.
     */
    static inline Vector3S LerpUnclamped(Vector3S a, Vector3S b, float t);

    /**
     * Returns the magnitude of a vector.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static inline float Magnitude(Vector3S v);

    /**
     * Returns a vector made from the largest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static inline Vector3S Max(Vector3S a, Vector3S b);

    /**
     * Returns a vector made from the smallest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static inline Vector3S Min(Vector3S a, Vector3S b);

    /**
     * Returns a vector "maxDistanceDelta" units closer to the target. This
     * interpolation is in a straight line, and will not overshoot.
     * @param current: The current position.
     * @param target: The destination position.
     * @param maxDistanceDelta: The maximum distance to move.
     * @return: A new vector.
     */
    static inline Vector3S MoveTowards(Vector3S current, Vector3S target,
                                      float maxDistanceDelta);

    /**
     * Returns a new vector with magnitude of one.
     * @param v: The vector in question.
     * @return: A new vector.
     */
    static inline Vector3S Normalized(Vector3S v);

    /**
     * Returns an arbitrary vector orthogonal to the input.
     * This vector is not normalized.
     * @param v: The input vector.
     * @return: A new vector.
     */
    static inline Vector3S Orthogonal(Vector3S v);

    /**
     * Creates a new coordinate system out of the three vectors.
     * Normalizes "normal", normalizes "tangent" and makes it orthogonal to
     * "normal" and normalizes "binormal" and makes it orthogonal to both
     * "normal" and "tangent".
     * @param normal: A reference to the first axis vector.
     * @param tangent: A reference to the second axis vector.
     * @param binormal: A reference to the third axis vector.
     */
    static inline void OrthoNormalize(Vector3S &normal, Vector3S &tangent,
                                      Vector3S &binormal);

    /**
     * Returns the vector projection of a onto b.
     * @param a: The target vector.
     * @param b: The vector being projected onto.
     * @return: A new vector.
     */
    static inline Vector3S Project(Vector3S a, Vector3S b);

    /**
     * Returns a vector projected onto a plane orthogonal to "planeNormal".
     * This can be visualized as the shadow of the vector onto the plane, if
     * the light source were in the direction of the plane normal.
     * @param vector: The vector to project.
     * @param planeNormal: The normal of the plane onto which to project.
     * @param: A new vector.
     */
    static inline Vector3S ProjectOnPlane(Vector3S vector, Vector3S planeNormal);

    /**
     * Returns a vector reflected off the plane orthogonal to the normal.
     * The input vector is pointed inward, at the plane, and the return vector
     * is pointed outward from the plane, like a beam of light hitting and then
     * reflecting off a mirror.
     * @param vector: The vector traveling inward at the plane.
     * @param planeNormal: The normal of the plane off of which to reflect.
     * @return: A new vector pointing outward from the plane.
     */
    static inline Vector3S Reflect(Vector3S vector, Vector3S planeNormal);

    /**
     * Returns the vector rejection of a on b.
     * @param a: The target vector.
     * @param b: The vector being projected onto.
     * @return: A new vector.
     */
    static inline Vector3S Reject(Vector3S a, Vector3S b);

    /**
     * Rotates vector "current" towards vector "target" by "maxRadiansDelta".
     * This treats the vectors as directions and will linearly interpolate
     * between their magnitudes by "maxMagnitudeDelta". This function does not
     * overshoot. If a negative delta is supplied, it will rotate away from
     * "target" until it is pointing the opposite direction, but will not
     * overshoot that either.
     * @param current: The starting direction.
     * @param target: The destination direction.
     * @param maxRadiansDelta: The maximum number of radians to rotate.
     * @param maxMagnitudeDelta: The maximum delta for magnitude interpolation.
     * @return: A new vector.
     */
    static inline Vector3S RotateTowards(Vector3S current, Vector3S target,
                                        float maxRadiansDelta,
                                        float maxMagnitudeDelta);

    /**
     * Multiplies two vectors element-wise.
     * @param a: The lhs of the multiplication.
     * @param b: The rhs of the multiplication.
     * @return: A new vector.
     */
    static inline Vector3S Scale(Vector3S a, Vector3S b);

    /**
     * Returns a vector rotated towards b from a by the percent t.
     * Since interpolation is done spherically, the vector moves at a constant
     * angular velocity. This rotation is clamped to 0 <= t <= 1.
     * @param a: The starting direction.
     * @param b: The ending direction.
     * @param t: The interpolation value [0-1].
     */
    static inline Vector3S Slerp(Vector3S a, Vector3S b, float t);

    /**
     * Returns a vector rotated towards b from a by the percent t.
     * Since interpolation is done spherically, the vector moves at a constant
     * angular velocity. This rotation is unclamped.
     * @param a: The starting direction.
     * @param b: The ending direction.
     * @param t: The interpolation value [0-1].
     */
    static inline Vector3S SlerpUnclamped(Vector3S a, Vector3S b, float t);

    /**
     * Returns the squared magnitude of a vector.
     * This is useful when comparing relative lengths, where the exact length
     * is not important, and much time can be saved by not calculating the
     * square root.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static inline float SqrMagnitude(Vector3S v);

    /**
     * Calculates the spherical coordinate space representation of a vector.
     * This uses the ISO convention (radius r, inclination theta, azimuth phi).
     * @param vector: The vector to convert.
     * @param rad: The magnitude of the vector.
     * @param theta: The angle in the xy plane from the x axis.
     * @param phi: The angle from the positive z axis to the vector.
     */
    static inline void ToSpherical(Vector3S vector, float &rad, float &theta,
                                   float &phi);


    /**
     * Operator overloading.
     */
    inline struct Vector3S& operator+=(const float rhs);
    inline struct Vector3S& operator-=(const float rhs);
    inline struct Vector3S& operator*=(const float rhs);
    inline struct Vector3S& operator/=(const float rhs);
    inline struct Vector3S& operator+=(const Vector3S rhs);
    inline struct Vector3S& operator-=(const Vector3S rhs);
};

inline Vector3S operator-(Vector3S rhs);
inline Vector3S operator+(Vector3S lhs, const float rhs);
inline Vector3S operator-(Vector3S lhs, const float rhs);
inline Vector3S operator*(Vector3S lhs, const float rhs);
inline Vector3S operator/(Vector3S lhs, const float rhs);
inline Vector3S operator+(const float lhs, Vector3S rhs);
inline Vector3S operator-(const float lhs, Vector3S rhs);
inline Vector3S operator*(const float lhs, Vector3S rhs);
inline Vector3S operator/(const float lhs, Vector3S rhs);
inline Vector3S operator+(Vector3S lhs, const Vector3S rhs);
inline Vector3S operator-(Vector3S lhs, const Vector3S rhs);
inline bool operator==(const Vector3S lhs, const Vector3S rhs);
inline bool operator!=(const Vector3S lhs, const Vector3S rhs);



/*******************************************************************************
 * Implementation
 */

Vector3S::Vector3S() : x(0), y(0), z(0) {}
Vector3S::Vector3S(float data[]) : x(data[0]), y(data[1]), z(data[2]) {}
Vector3S::Vector3S(float value) : x(value), y(value), z(value) {}
Vector3S::Vector3S(float x, float y) : x(x), y(y), z(0) {}
Vector3S::Vector3S(float x, float y, float z) : x(x), y(y), z(z) {}


Vector3S Vector3S::zero() { return Vector3S(0, 0, 0); }
Vector3S Vector3S::One() { return Vector3S(1, 1, 1); }
Vector3S Vector3S::Right() { return Vector3S(1, 0, 0); }
Vector3S Vector3S::Left() { return Vector3S(-1, 0, 0); }
Vector3S Vector3S::Up() { return Vector3S(0, 1, 0); }
Vector3S Vector3S::Down() { return Vector3S(0, -1, 0); }
Vector3S Vector3S::Forward() { return Vector3S(0, 0, 1); }
Vector3S Vector3S::Backward() { return Vector3S(0, 0, -1); }


float Vector3S::Angle(Vector3S a, Vector3S b)
{
    float v = Dot(a, b) / (Magnitude(a) * Magnitude(b));
    v = fmax(v, -1.0);
    v = fmin(v, 1.0);
    return acos(v);
}

Vector3S Vector3S::ClampMagnitude(Vector3S vector, float maxLength)
{
    float length = Magnitude(vector);
    if (length > maxLength)
        vector *= maxLength / length;
    return vector;
}

float Vector3S::Component(Vector3S a, Vector3S b)
{
    return Dot(a, b) / Magnitude(b);
}

Vector3S Vector3S::Cross(Vector3S lhs, Vector3S rhs)
{
    float x = lhs.y * rhs.z - lhs.z * rhs.y;
    float y = lhs.z * rhs.x - lhs.x * rhs.z;
    float z = lhs.x * rhs.y - lhs.y * rhs.x;
    return Vector3S(x, y, z);
}

float Vector3S::Distance(Vector3S a, Vector3S b)
{
    return Vector3S::Magnitude(a - b);
}

float Vector3S::Dot(Vector3S lhs, Vector3S rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vector3S Vector3S::FromSpherical(float rad, float theta, float phi)
{
    Vector3S v;
    v.x = rad * sin(theta) * cos(phi);
    v.y = rad * sin(theta) * sin(phi);
    v.z = rad * cos(theta);
    return v;
}

Vector3S Vector3S::Lerp(Vector3S a, Vector3S b, float t)
{
    if (t < 0) return a;
    else if (t > 1) return b;
    return LerpUnclamped(a, b, t);
}

Vector3S Vector3S::LerpUnclamped(Vector3S a, Vector3S b, float t)
{
    return (b - a) * t + a;
}

float Vector3S::Magnitude(Vector3S v)
{
    return sqrt(SqrMagnitude(v));
}

Vector3S Vector3S::Max(Vector3S a, Vector3S b)
{
    float x = a.x > b.x ? a.x : b.x;
    float y = a.y > b.y ? a.y : b.y;
    float z = a.z > b.z ? a.z : b.z;
    return Vector3S(x, y, z);
}

Vector3S Vector3S::Min(Vector3S a, Vector3S b)
{
    float x = a.x > b.x ? b.x : a.x;
    float y = a.y > b.y ? b.y : a.y;
    float z = a.z > b.z ? b.z : a.z;
    return Vector3S(x, y, z);
}

Vector3S Vector3S::MoveTowards(Vector3S current, Vector3S target,
                             float maxDistanceDelta)
{
    Vector3S d = target - current;
    float m = Magnitude(d);
    if (m < maxDistanceDelta || m == 0)
        return target;
    return current + (d * maxDistanceDelta / m);
}

Vector3S Vector3S::Normalized(Vector3S v)
{
    float mag = Magnitude(v);
    if (mag == 0)
        return Vector3S::zero();
    return v / mag;
}

Vector3S Vector3S::Orthogonal(Vector3S v)
{
    return v.z < v.x ? Vector3S(v.y, -v.x, 0) : Vector3S(0, -v.z, v.y);
}

void Vector3S::OrthoNormalize(Vector3S &normal, Vector3S &tangent,
                             Vector3S &binormal)
{
    normal = Normalized(normal);
    tangent = ProjectOnPlane(tangent, normal);
    tangent = Normalized(tangent);
    binormal = ProjectOnPlane(binormal, tangent);
    binormal = ProjectOnPlane(binormal, normal);
    binormal = Normalized(binormal);
}

Vector3S Vector3S::Project(Vector3S a, Vector3S b)
{
    float m = Magnitude(b);
    return Dot(a, b) / (m * m) * b;
}

Vector3S Vector3S::ProjectOnPlane(Vector3S vector, Vector3S planeNormal)
{
    return Reject(vector, planeNormal);
}

Vector3S Vector3S::Reflect(Vector3S vector, Vector3S planeNormal)
{
    return vector - 2 * Project(vector, planeNormal);
}

Vector3S Vector3S::Reject(Vector3S a, Vector3S b)
{
    return a - Project(a, b);
}

Vector3S Vector3S::RotateTowards(Vector3S current, Vector3S target,
                               float maxRadiansDelta,
                               float maxMagnitudeDelta)
{
    float magCur = Magnitude(current);
    float magTar = Magnitude(target);
    float newMag = magCur + maxMagnitudeDelta *
                            ((magTar > magCur) - (magCur > magTar));
    newMag = fmin(newMag, fmax(magCur, magTar));
    newMag = fmax(newMag, fmin(magCur, magTar));

    float totalAngle = Angle(current, target) - maxRadiansDelta;
    if (totalAngle <= 0)
        return Normalized(target) * newMag;
    else if (totalAngle >= M_PI)
        return Normalized(-target) * newMag;

    Vector3S axis = Cross(current, target);
    float magAxis = Magnitude(axis);
    if (magAxis == 0)
        axis = Normalized(Cross(current, current + Vector3S(3.95, 5.32, -4.24)));
    else
        axis /= magAxis;
    current = Normalized(current);
    Vector3S newVector = current * cos(maxRadiansDelta) +
                        Cross(axis, current) * sin(maxRadiansDelta);
    return newVector * newMag;
}

Vector3S Vector3S::Scale(Vector3S a, Vector3S b)
{
    return Vector3S(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vector3S Vector3S::Slerp(Vector3S a, Vector3S b, float t)
{
    if (t < 0) return a;
    else if (t > 1) return b;
    return SlerpUnclamped(a, b, t);
}

Vector3S Vector3S::SlerpUnclamped(Vector3S a, Vector3S b, float t)
{
    float magA = Magnitude(a);
    float magB = Magnitude(b);
    a /= magA;
    b /= magB;
    float dot = Dot(a, b);
    dot = fmax(dot, -1.0);
    dot = fmin(dot, 1.0);
    float theta = acos(dot) * t;
    Vector3S relativeVec = Normalized(b - a * dot);
    Vector3S newVec = a * cos(theta) + relativeVec * sin(theta);
    return newVec * (magA + (magB - magA) * t);
}

float Vector3S::SqrMagnitude(Vector3S v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

void Vector3S::ToSpherical(Vector3S vector, float &rad, float &theta,
                          float &phi)
{
    rad = Magnitude(vector);
    float v = vector.z / rad;
    v = fmax(v, -1.0);
    v = fmin(v, 1.0);
    theta = acos(v);
    phi = atan2(vector.y, vector.x);
}


struct Vector3S& Vector3S::operator+=(const float rhs)
{
    x += rhs;
    y += rhs;
    z += rhs;
    return *this;
}

struct Vector3S& Vector3S::operator-=(const float rhs)
{
    x -= rhs;
    y -= rhs;
    z -= rhs;
    return *this;
}

struct Vector3S& Vector3S::operator*=(const float rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}

struct Vector3S& Vector3S::operator/=(const float rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
}

struct Vector3S& Vector3S::operator+=(const Vector3S rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

struct Vector3S& Vector3S::operator-=(const Vector3S rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}



Vector3S operator-(Vector3S rhs) { return rhs * -1; }
Vector3S operator+(Vector3S lhs, const float rhs) { return lhs += rhs; }
Vector3S operator-(Vector3S lhs, const float rhs) { return lhs -= rhs; }
Vector3S operator*(Vector3S lhs, const float rhs) { return lhs *= rhs; }
Vector3S operator/(Vector3S lhs, const float rhs) { return lhs /= rhs; }
Vector3S operator+(const float lhs, Vector3S rhs) { return rhs += lhs; }
Vector3S operator-(const float lhs, Vector3S rhs) { return rhs -= lhs; }
Vector3S operator*(const float lhs, Vector3S rhs) { return rhs *= lhs; }
Vector3S operator/(const float lhs, Vector3S rhs) { return rhs /= lhs; }
Vector3S operator+(Vector3S lhs, const Vector3S rhs) { return lhs += rhs; }
Vector3S operator-(Vector3S lhs, const Vector3S rhs) { return lhs -= rhs; }

bool operator==(const Vector3S lhs, const Vector3S rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator!=(const Vector3S lhs, const Vector3S rhs)
{
    return !(lhs == rhs);
}
