#include "Shape.h"
#include "Scene.h"
#include <cstdio>

Shape::Shape(void)
{
}

Shape::Shape(int id, int matIndex)
    : id(id), matIndex(matIndex)
{
}

Sphere::Sphere(void)
{}

Vector3f crossProduct(Vector3f a, Vector3f b) {
	Vector3f result;
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;
	return result;
}
float dotProduct(Vector3f a, Vector3f b) {
	float result;
	result = (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
	return result;
}
Vector3f vectorDifference(Vector3f a, Vector3f b) {
	Vector3f result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return result;
}
float determinant(Vector3f a, Vector3f b, Vector3f c){
	float result =a.x*(b.y*c.z - c.y*b.z) + a.y*(c.x*b.z - c.z*b.x) + a.z*(b.x*c.y - b.y*c.x);
	return result;
}
float vectorLength(Vector3f a){
	float res;
	res = a.x*a.x + a.y*a.y + a.z*a.z;
	res = sqrt(res);
	return res;
}

/* Constructor for sphere. You will implement this. */
Sphere::Sphere(int id, int matIndex, int cIndex, float R, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    this->cIndex =cIndex;
    this->R=R;
    this->vertices = pVertices;
}

/* Sphere-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Sphere::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	
	ReturnVal res;
	float t1,t2,det,length;
	Vector3f in;
	Vector3f center = (*vertices)[cIndex-1];
	res.isInter=false;
	det = dotProduct(ray.direction,vectorDifference(ray.origin,center))*dotProduct(ray.direction,vectorDifference(ray.origin,center)) - dotProduct(ray.direction,ray.direction)*(dotProduct(vectorDifference(ray.origin,center),vectorDifference(ray.origin,center)) - R*R);
	if(det>0){
		res.isInter=true;
		t1 = (-1*dotProduct(ray.direction,vectorDifference(ray.origin,center)) + sqrt(det))/dotProduct(ray.direction,ray.direction);
		t2 = (-1*dotProduct(ray.direction,vectorDifference(ray.origin,center)) - sqrt(det))/dotProduct(ray.direction,ray.direction);

		if(t1>0 && t2>0){
			if(t1>t2){
				in.x = ray.origin.x + ray.direction.x*t2;
		        in.y = ray.origin.y + ray.direction.y*t2;
		        in.z = ray.origin.z + ray.direction.z*t2;
		        res.t=t2;
			}else{
				in.x = ray.origin.x + ray.direction.x*t1;
		        in.y = ray.origin.y + ray.direction.y*t1;
		        in.z = ray.origin.z + ray.direction.z*t1;
		        res.t=t1;
			}
			res.pos.x = in.x;
			res.pos.y = in.y;
			res.pos.z = in.z;
			res.normal.x=in.x-center.x;
			res.normal.y=in.y-center.y;
			res.normal.z=in.z-center.z;
			length = vectorLength(res.normal);
			res.normal.x/=length;
			res.normal.y/=length;
			res.normal.z/=length;

			
		}
		

	}
	//cout<<det<<" "<<ray.direction.x<<" "<<ray.direction.y<<" "<<ray.direction.z<<" "<<endl;
	return res;
}

Triangle::Triangle(void)
{}

/* Constructor for triangle. You will implement this. */
Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    this->p1Index = p1Index;
    this->p2Index = p2Index;
    this->p3Index = p3Index;
    this->vertices= pVertices;
}

/* Triangle-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Triangle::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    ReturnVal res;
    Vector3f a = (*vertices)[p1Index-1];
    Vector3f b = (*vertices)[p2Index-1];
    Vector3f c = (*vertices)[p3Index-1];
    Vector3f n, a2, b2 ,in, vp, vx;
    float t,eq, beta, alpha, A, length;
    res.isInter = false;
    
    a2.x = c.x-a.x;
    a2.y = c.y-a.y;
    a2.z = c.z-a.z;
    b2.x = b.x-a.x;
    b2.y = b.y-a.y;
    b2.z = b.z-a.z;
   

    n = crossProduct(a2, b2);
    length = vectorLength(n);
    n.x /= length;
    n.y /= length;
    n.z /= length;

   

    A = determinant(vectorDifference(a,b),vectorDifference(a,c),ray.direction);
    beta = determinant(vectorDifference(a,ray.origin),vectorDifference(a,c),ray.direction)/A;
	alpha = determinant(vectorDifference(a,b),vectorDifference(a,ray.origin),ray.direction)/A;
	t = determinant(vectorDifference(a,b),vectorDifference(a,c),vectorDifference(a,ray.origin))/A;
	if(beta>=0.0 && alpha>=0.0 && (beta+alpha)<=1.0 && t>0){
		res.isInter = true;
        res.normal = n;
        res.t = t;
        res.pos.x = ray.origin.x + ray.direction.x * t;
        res.pos.y = ray.origin.y + ray.direction.y * t;
        res.pos.z = ray.origin.z + ray.direction.z * t;
        res.a = a;
        res.b = b;
        res.c = c;
	}
    
    
    return res;
}

Mesh::Mesh()
{}

/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces, vector<int> *pIndices, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    this->faces = faces;
    this->pIndices = pIndices;
    this->vertices = pVertices;
}

/* Mesh-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Mesh::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	ReturnVal res, loopRes;
	res.isInter = false;
	float tmin= 100000000;
	for(int i = 0; i<faces.size(); i++){
		Triangle tri = faces[i];
		loopRes = tri.intersect(ray);
		if(loopRes.isInter && loopRes.t<tmin){
			tmin = loopRes.t;
			res = loopRes;

		}
	}
	return res;
}



