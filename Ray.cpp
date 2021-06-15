#include "Ray.h"

Ray::Ray()
{
}

Ray::Ray(const Vector3f& origin, const Vector3f& direction)
    : origin(origin), direction(direction)
{
}

/* Takes a parameter t and returns the point accoring to t. t is the parametric variable in the ray equation o+t*d.*/
Vector3f Ray::getPoint(float t) const 
{
    Vector3f p;
    p.x = origin.x + t*direction.x;
    p.y = origin.y + t*direction.y;
    p.z = origin.z + t*direction.z;
    return p;
}

/* Takes a point p and returns the parameter t according to p such that p = o+t*d. */
float Ray::gett(const Vector3f & p) const
{
    float x1;
    x1 = p.x - origin.x;
    return x1/direction.x;
}