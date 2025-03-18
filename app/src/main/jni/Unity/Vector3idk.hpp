#pragma once

#define _USE_MATH_DEFINES
#include <math.h>


struct Vector3idk
{
  float x;
  float y;
  float z;

  Vector3idk();
  Vector3idk(float x, float y, float z);
  ~Vector3idk();


    /**
     * Constructors.
     */


    /**
     * Constants for common vectors.
     */
    static inline Vector3idk zero();
    static inline Vector3idk One();
    static inline Vector3idk Right();
    static inline Vector3idk Left();
    static inline Vector3idk Up();
    static inline Vector3idk Down();
    static inline Vector3idk Forward();
    static inline Vector3idk Backward();


    /**
     * Returns the angle between two vectors in radians.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A scalar value.
     */
    static inline float Angle(Vector3idk a, Vector3idk b);

    /**
     * Returns a vector with its magnitude clamped to maxLength.
     * @param vector: The target vector.
     * @param maxLength: The maximum length of the return vector.
     * @return: A new vector.
     */
    static inline Vector3idk ClampMagnitude(Vector3idk vector, float maxLength);

    /**
     * Returns the component of a in the direction of b (scalar projection).
     * @param a: The target vector.
     * @param b: The vector being compared against.
     * @return: A scalar value.
     */
    static inline float Component(Vector3idk a, Vector3idk b);

    /**
     * Returns the cross product of two vectors.
     * @param lhs: The left side of the multiplication.
     * @param rhs: The right side of the multiplication.
     * @return: A new vector.
     */
    static inline Vector3idk Cross(Vector3idk lhs, Vector3idk rhs);

    /**
     * Returns the distance between a and b.
     * @param a: The first point.
     * @param b: The second point.
     * @return: A scalar value.
     */
    static inline float Distance(Vector3idk a, Vector3idk b);

    static inline char ToChar(Vector3idk a);

    /**
     * Returns the dot product of two vectors.
     * @param lhs: The left side of the multiplication.
     * @param rhs: The right side of the multiplication.
     * @return: A scalar value.
     */
    static inline float Dot(Vector3idk lhs, Vector3idk rhs);

    /**
     * Converts a spherical representation of a vector into cartesian
     * coordinates.
     * This uses the ISO convention (radius r, inclination theta, azimuth phi).
     * @param rad: The magnitude of the vector.
     * @param theta: The angle in the xy plane from the x axis.
     * @param phi: The angle from the positive z axis to the vector.
     * @return: A new vector.
     */
    static inline Vector3idk FromSpherical(float rad, float theta, float phi);

    /**
     * Returns a vector linearly interpolated between a and b, moving along
     * a straight line. The vector is clamped to never go beyond the end points.
     * @param a: The starting point.
     * @param b: The ending point.
     * @param t: The interpolation value [0-1].
     * @return: A new vector.
     */
    static inline Vector3idk Lerp(Vector3idk a, Vector3idk b, float t);

    /**
     * Returns a vector linearly interpolated between a and b, moving along
     * a straight line.
     * @param a: The starting point.
     * @param b: The ending point.
     * @param t: The interpolation value [0-1] (no actual bounds).
     * @return: A new vector.
     */
    static inline Vector3idk LerpUnclamped(Vector3idk a, Vector3idk b, float t);

    /**
     * Returns the magnitude of a vector.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static inline float Magnitude(Vector3idk v);

    /**
     * Returns a vector made from the largest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static inline Vector3idk Max(Vector3idk a, Vector3idk b);

    /**
     * Returns a vector made from the smallest components of two other vectors.
     * @param a: The first vector.
     * @param b: The second vector.
     * @return: A new vector.
     */
    static inline Vector3idk Min(Vector3idk a, Vector3idk b);

    /**
     * Returns a vector "maxDistanceDelta" units closer to the target. This
     * interpolation is in a straight line, and will not overshoot.
     * @param current: The current position.
     * @param target: The destination position.
     * @param maxDistanceDelta: The maximum distance to move.
     * @return: A new vector.
     */
    static inline Vector3idk MoveTowards(Vector3idk current, Vector3idk target,
                                      float maxDistanceDelta);

    /**
     * Returns a new vector with magnitude of one.
     * @param v: The vector in question.
     * @return: A new vector.
     */
    static inline Vector3idk Normalized(Vector3idk v);

    /**
     * Returns an arbitrary vector orthogonal to the input.
     * This vector is not normalized.
     * @param v: The input vector.
     * @return: A new vector.
     */
    static inline Vector3idk Orthogonal(Vector3idk v);

    /**
     * Creates a new coordinate system out of the three vectors.
     * Normalizes "normal", normalizes "tangent" and makes it orthogonal to
     * "normal" and normalizes "binormal" and makes it orthogonal to both
     * "normal" and "tangent".
     * @param normal: A reference to the first axis vector.
     * @param tangent: A reference to the second axis vector.
     * @param binormal: A reference to the third axis vector.
     */
    static inline void OrthoNormalize(Vector3idk &normal, Vector3idk &tangent,
                                      Vector3idk &binormal);

    /**
     * Returns the vector projection of a onto b.
     * @param a: The target vector.
     * @param b: The vector being projected onto.
     * @return: A new vector.
     */
    static inline Vector3idk Project(Vector3idk a, Vector3idk b);

    /**
     * Returns a vector projected onto a plane orthogonal to "planeNormal".
     * This can be visualized as the shadow of the vector onto the plane, if
     * the light source were in the direction of the plane normal.
     * @param vector: The vector to project.
     * @param planeNormal: The normal of the plane onto which to project.
     * @param: A new vector.
     */
    static inline Vector3idk ProjectOnPlane(Vector3idk vector, Vector3idk planeNormal);

    /**
     * Returns a vector reflected off the plane orthogonal to the normal.
     * The input vector is pointed inward, at the plane, and the return vector
     * is pointed outward from the plane, like a beam of light hitting and then
     * reflecting off a mirror.
     * @param vector: The vector traveling inward at the plane.
     * @param planeNormal: The normal of the plane off of which to reflect.
     * @return: A new vector pointing outward from the plane.
     */
    static inline Vector3idk Reflect(Vector3idk vector, Vector3idk planeNormal);

    /**
     * Returns the vector rejection of a on b.
     * @param a: The target vector.
     * @param b: The vector being projected onto.
     * @return: A new vector.
     */
    static inline Vector3idk Reject(Vector3idk a, Vector3idk b);

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
    static inline Vector3idk RotateTowards(Vector3idk current, Vector3idk target,
                                        float maxRadiansDelta,
                                        float maxMagnitudeDelta);

    /**
     * Multiplies two vectors element-wise.
     * @param a: The lhs of the multiplication.
     * @param b: The rhs of the multiplication.
     * @return: A new vector.
     */
    static inline Vector3idk Scale(Vector3idk a, Vector3idk b);

    /**
     * Returns a vector rotated towards b from a by the percent t.
     * Since interpolation is done spherically, the vector moves at a constant
     * angular velocity. This rotation is clamped to 0 <= t <= 1.
     * @param a: The starting direction.
     * @param b: The ending direction.
     * @param t: The interpolation value [0-1].
     */
    static inline Vector3idk Slerp(Vector3idk a, Vector3idk b, float t);

    /**
     * Returns a vector rotated towards b from a by the percent t.
     * Since interpolation is done spherically, the vector moves at a constant
     * angular velocity. This rotation is unclamped.
     * @param a: The starting direction.
     * @param b: The ending direction.
     * @param t: The interpolation value [0-1].
     */
    static inline Vector3idk SlerpUnclamped(Vector3idk a, Vector3idk b, float t);

    /**
     * Returns the squared magnitude of a vector.
     * This is useful when comparing relative lengths, where the exact length
     * is not important, and much time can be saved by not calculating the
     * square root.
     * @param v: The vector in question.
     * @return: A scalar value.
     */
    static inline float SqrMagnitude(Vector3idk v);

    /**
     * Calculates the spherical coordinate space representation of a vector.
     * This uses the ISO convention (radius r, inclination theta, azimuth phi).
     * @param vector: The vector to convert.
     * @param rad: The magnitude of the vector.
     * @param theta: The angle in the xy plane from the x axis.
     * @param phi: The angle from the positive z axis to the vector.
     */
    static inline void ToSpherical(Vector3idk vector, float &rad, float &theta,
                                   float &phi);


    /**
     * Operator overloading.
     */
    inline struct Vector3idk& operator+=(const float rhs);
    inline struct Vector3idk& operator-=(const float rhs);
    inline struct Vector3idk& operator*=(const float rhs);
    inline struct Vector3idk& operator/=(const float rhs);
    inline struct Vector3idk& operator+=(const Vector3idk rhs);
    inline struct Vector3idk& operator-=(const Vector3idk rhs);
};

inline Vector3idk operator-(Vector3idk rhs);
inline Vector3idk operator+(Vector3idk lhs, const float rhs);
inline Vector3idk operator-(Vector3idk lhs, const float rhs);
inline Vector3idk operator*(Vector3idk lhs, const float rhs);
inline Vector3idk operator/(Vector3idk lhs, const float rhs);
inline Vector3idk operator+(const float lhs, Vector3idk rhs);
inline Vector3idk operator-(const float lhs, Vector3idk rhs);
inline Vector3idk operator*(const float lhs, Vector3idk rhs);
inline Vector3idk operator/(const float lhs, Vector3idk rhs);
inline Vector3idk operator+(Vector3idk lhs, const Vector3idk rhs);
inline Vector3idk operator-(Vector3idk lhs, const Vector3idk rhs);
inline bool operator==(const Vector3idk lhs, const Vector3idk rhs);
inline bool operator!=(const Vector3idk lhs, const Vector3idk rhs);



/*******************************************************************************
 * Implementation
 */

Vector3idk::Vector3idk() {}
Vector3idk::Vector3idk(float x, float y, float z) : x(x), y(y), z(z) {}
Vector3idk::~Vector3idk() {}


Vector3idk Vector3idk::zero() { return Vector3idk(0, 0, 0); }
Vector3idk Vector3idk::One() { return Vector3idk(1, 1, 1); }
Vector3idk Vector3idk::Right() { return Vector3idk(1, 0, 0); }
Vector3idk Vector3idk::Left() { return Vector3idk(-1, 0, 0); }
Vector3idk Vector3idk::Up() { return Vector3idk(0, 1, 0); }
Vector3idk Vector3idk::Down() { return Vector3idk(0, -1, 0); }
Vector3idk Vector3idk::Forward() { return Vector3idk(0, 0, 1); }
Vector3idk Vector3idk::Backward() { return Vector3idk(0, 0, -1); }


float Vector3idk::Angle(Vector3idk a, Vector3idk b)
{
    float v = Dot(a, b) / (Magnitude(a) * Magnitude(b));
    v = fmax(v, -1.0);
    v = fmin(v, 1.0);
    return acos(v);
}

Vector3idk Vector3idk::ClampMagnitude(Vector3idk vector, float maxLength)
{
    float length = Magnitude(vector);
    if (length > maxLength)
        vector *= maxLength / length;
    return vector;
}

float Vector3idk::Component(Vector3idk a, Vector3idk b)
{
    return Dot(a, b) / Magnitude(b);
}

Vector3idk Vector3idk::Cross(Vector3idk lhs, Vector3idk rhs)
{
    float x = lhs.y * rhs.z - lhs.z * rhs.y;
    float y = lhs.z * rhs.x - lhs.x * rhs.z;
    float z = lhs.x * rhs.y - lhs.y * rhs.x;
    return Vector3idk(x, y, z);
}

float Vector3idk::Distance(Vector3idk a, Vector3idk b)
{
    return Vector3idk::Magnitude(a - b);
}

float Vector3idk::Dot(Vector3idk lhs, Vector3idk rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vector3idk Vector3idk::FromSpherical(float rad, float theta, float phi)
{
    Vector3idk v;
    v.x = rad * sin(theta) * cos(phi);
    v.y = rad * sin(theta) * sin(phi);
    v.z = rad * cos(theta);
    return v;
}

Vector3idk Vector3idk::Lerp(Vector3idk a, Vector3idk b, float t)
{
    if (t < 0) return a;
    else if (t > 1) return b;
    return LerpUnclamped(a, b, t);
}

Vector3idk Vector3idk::LerpUnclamped(Vector3idk a, Vector3idk b, float t)
{
    return (b - a) * t + a;
}

float Vector3idk::Magnitude(Vector3idk v)
{
    return sqrt(SqrMagnitude(v));
}

Vector3idk Vector3idk::Max(Vector3idk a, Vector3idk b)
{
    float x = a.x > b.x ? a.x : b.x;
    float y = a.y > b.y ? a.y : b.y;
    float z = a.z > b.z ? a.z : b.z;
    return Vector3idk(x, y, z);
}

Vector3idk Vector3idk::Min(Vector3idk a, Vector3idk b)
{
    float x = a.x > b.x ? b.x : a.x;
    float y = a.y > b.y ? b.y : a.y;
    float z = a.z > b.z ? b.z : a.z;
    return Vector3idk(x, y, z);
}

Vector3idk Vector3idk::MoveTowards(Vector3idk current, Vector3idk target,
                             float maxDistanceDelta)
{
    Vector3idk d = target - current;
    float m = Magnitude(d);
    if (m < maxDistanceDelta || m == 0)
        return target;
    return current + (d * maxDistanceDelta / m);
}

Vector3idk Vector3idk::Normalized(Vector3idk v)
{
    float mag = Magnitude(v);
    if (mag == 0)
        return Vector3idk::zero();
    return v / mag;
}

Vector3idk Vector3idk::Orthogonal(Vector3idk v)
{
    return v.z < v.x ? Vector3idk(v.y, -v.x, 0) : Vector3idk(0, -v.z, v.y);
}

void Vector3idk::OrthoNormalize(Vector3idk &normal, Vector3idk &tangent,
                             Vector3idk &binormal)
{
    normal = Normalized(normal);
    tangent = ProjectOnPlane(tangent, normal);
    tangent = Normalized(tangent);
    binormal = ProjectOnPlane(binormal, tangent);
    binormal = ProjectOnPlane(binormal, normal);
    binormal = Normalized(binormal);
}

Vector3idk Vector3idk::Project(Vector3idk a, Vector3idk b)
{
    float m = Magnitude(b);
    return Dot(a, b) / (m * m) * b;
}

Vector3idk Vector3idk::ProjectOnPlane(Vector3idk vector, Vector3idk planeNormal)
{
    return Reject(vector, planeNormal);
}

Vector3idk Vector3idk::Reflect(Vector3idk vector, Vector3idk planeNormal)
{
    return vector - 2 * Project(vector, planeNormal);
}

Vector3idk Vector3idk::Reject(Vector3idk a, Vector3idk b)
{
    return a - Project(a, b);
}

Vector3idk Vector3idk::RotateTowards(Vector3idk current, Vector3idk target,
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

    Vector3idk axis = Cross(current, target);
    float magAxis = Magnitude(axis);
    if (magAxis == 0)
        axis = Normalized(Cross(current, current + Vector3idk(3.95, 5.32, -4.24)));
    else
        axis /= magAxis;
    current = Normalized(current);
    Vector3idk newVector = current * cos(maxRadiansDelta) +
                        Cross(axis, current) * sin(maxRadiansDelta);
    return newVector * newMag;
}

Vector3idk Vector3idk::Scale(Vector3idk a, Vector3idk b)
{
    return Vector3idk(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vector3idk Vector3idk::Slerp(Vector3idk a, Vector3idk b, float t)
{
    if (t < 0) return a;
    else if (t > 1) return b;
    return SlerpUnclamped(a, b, t);
}

Vector3idk Vector3idk::SlerpUnclamped(Vector3idk a, Vector3idk b, float t)
{
    float magA = Magnitude(a);
    float magB = Magnitude(b);
    a /= magA;
    b /= magB;
    float dot = Dot(a, b);
    dot = fmax(dot, -1.0);
    dot = fmin(dot, 1.0);
    float theta = acos(dot) * t;
    Vector3idk relativeVec = Normalized(b - a * dot);
    Vector3idk newVec = a * cos(theta) + relativeVec * sin(theta);
    return newVec * (magA + (magB - magA) * t);
}

float Vector3idk::SqrMagnitude(Vector3idk v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

void Vector3idk::ToSpherical(Vector3idk vector, float &rad, float &theta,
                          float &phi)
{
    rad = Magnitude(vector);
    float v = vector.z / rad;
    v = fmax(v, -1.0);
    v = fmin(v, 1.0);
    theta = acos(v);
    phi = atan2(vector.y, vector.x);
}


struct Vector3idk& Vector3idk::operator+=(const float rhs)
{
    x += rhs;
    y += rhs;
    z += rhs;
    return *this;
}

struct Vector3idk& Vector3idk::operator-=(const float rhs)
{
    x -= rhs;
    y -= rhs;
    z -= rhs;
    return *this;
}

struct Vector3idk& Vector3idk::operator*=(const float rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}

struct Vector3idk& Vector3idk::operator/=(const float rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
}

struct Vector3idk& Vector3idk::operator+=(const Vector3idk rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

struct Vector3idk& Vector3idk::operator-=(const Vector3idk rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}



Vector3idk operator-(Vector3idk rhs) { return rhs * -1; }
Vector3idk operator+(Vector3idk lhs, const float rhs) { return lhs += rhs; }
Vector3idk operator-(Vector3idk lhs, const float rhs) { return lhs -= rhs; }
Vector3idk operator*(Vector3idk lhs, const float rhs) { return lhs *= rhs; }
Vector3idk operator/(Vector3idk lhs, const float rhs) { return lhs /= rhs; }
Vector3idk operator+(const float lhs, Vector3idk rhs) { return rhs += lhs; }
Vector3idk operator-(const float lhs, Vector3idk rhs) { return rhs -= lhs; }
Vector3idk operator*(const float lhs, Vector3idk rhs) { return rhs *= lhs; }
Vector3idk operator/(const float lhs, Vector3idk rhs) { return rhs /= lhs; }
Vector3idk operator+(Vector3idk lhs, const Vector3idk rhs) { return lhs += rhs; }
Vector3idk operator-(Vector3idk lhs, const Vector3idk rhs) { return lhs -= rhs; }

bool operator==(const Vector3idk lhs, const Vector3idk rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator!=(const Vector3idk lhs, const Vector3idk rhs)
{
    return !(lhs == rhs);
}