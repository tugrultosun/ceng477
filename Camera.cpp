#include "Camera.h"

Camera::Camera(int id,                      // Id of the camera
               const char* imageName,       // Name of the output PPM file 
               const Vector3f& pos,         // Camera position
               const Vector3f& gaze,        // Camera gaze direction
               const Vector3f& up,          // Camera up direction
               const ImagePlane& imgPlane)  // Image plane parameters
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
     this->id = id;
     strcpy(this->imageName , imageName);
     this->pos = pos;
     this->gaze = gaze;
     this->up = up;
     this->imgPlane = imgPlane;
}

/* Takes coordinate of an image pixel as row and col, and
 * returns the ray going through that pixel. 
 */
float vectorLengthCam(Vector3f a){
     float res;
     res = a.x*a.x + a.y*a.y + a.z*a.z;
     res = sqrt(res);
     return res;
}
Vector3f vectorDivCam(Vector3f a, float b) {
     Vector3f result;
     result.x = a.x/b;
     result.y = a.y/b;
     result.z = a.z/b;
     return result;
}

Ray Camera::getPrimaryRay(int col, int row) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

     Vector3f direction, m, q, s ;
     Ray result;
     m.x = pos.x;
     m.y = pos.y;
     m.z = pos.z - imgPlane.distance;
     q.x = m.x + imgPlane.left;
     q.y = m.y + imgPlane.top;
     q.z = m.z;
     float su = (col+0.5)*((imgPlane.right-imgPlane.left)/imgPlane.nx);
     float sv = (row+0.5)*((imgPlane.top-imgPlane.bottom)/imgPlane.ny);
     s.x = q.x + su;
     s.y = q.y - sv;
     s.z = q.z;
     direction.x = (s.x-pos.x);
     direction.y = (s.y-pos.y);
     direction.z = (s.z-pos.z);
     direction= vectorDivCam(direction, vectorLengthCam(direction));
     result.origin = pos;
     result.direction = direction;
     return result;

}

