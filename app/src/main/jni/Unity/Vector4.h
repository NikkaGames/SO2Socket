class Vector4 {
public:
    float x;
    float y;
    float z;
    Vector4()
            : x(0)
            , y(0)
            , z(0)
    {
    }
    Vector4(float x1, float y1, float z1)
            : x(x1)
            , y(y1)
            , z(z1)
    {
    }
    Vector4(const Vector4& v);
    ~Vector4();
};
Vector4::Vector4(const Vector4& v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
{
}
Vector4::~Vector4() {}
