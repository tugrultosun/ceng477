#include "Light.h"
#include <math.h>

/* Constructor. Implemented for you. */
PointLight::PointLight(const Vector3f & position, const Vector3f & intensity)
    : position(position), intensity(intensity)
{
}

// Compute the contribution of light at point p using the
// inverse square law formula
Vector3f PointLight::computeLightContribution(const Vector3f& p)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	Vector3f res;
	float distance;
	distance = sqrt((p.x-position.x)*(p.x-position.x) + (p.y-position.y)*(p.y-position.y) + (p.z-position.z)*(p.z-position.z));
	res.x = intensity.x/(distance*distance);
	res.y = intensity.y/(distance*distance);
	res.z = intensity.z/(distance*distance);
	return res;
}
