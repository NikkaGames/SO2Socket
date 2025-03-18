class Vec3 {
public:
    float x;
    float y;
    float z;
    Vec3()
            : x(0)
            , y(0)
            , z(0)
    {
    }
    Vec3(float x1, float y1, float z1)
            : x(x1)
            , y(y1)
            , z(z1)
    {
    }
    Vec3(const Vec3& v);
    ~Vec3();
};
Vec3::Vec3(const Vec3& v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
{
}
Vec3::~Vec3() {}
