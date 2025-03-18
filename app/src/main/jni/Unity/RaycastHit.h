#include <Unity/Vector3.h>
#include <Unity/Vector2.h>

typedef struct _RaycastHit {
    Vector3 m_Point;
    Vector3 m_Normal;
    int m_FaceID;
    float m_Distance;
    Vector2 m_UV;
    void * collider;
} RaycastHit; 
