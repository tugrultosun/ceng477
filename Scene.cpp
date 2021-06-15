#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Shape.h"
#include "tinyxml2.h"
#include "Image.h"

using namespace tinyxml2;

/* 
 * Must render the scene from each camera's viewpoint and create an image.
 * You can use the methods of the Image class to save the image as a PPM file. 
 */
float dotProductScene(Vector3f a, Vector3f b) {
	float result;
	result = (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
	return result;
}
Vector3f vectorDifferenceScene(Vector3f a, Vector3f b) {
	Vector3f result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return result;
}
Vector3f vectorSumScene(Vector3f a, Vector3f b) {
	Vector3f result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
Vector3f vectorDivScene(Vector3f a, float b) {
	Vector3f result;
	result.x = a.x/b;
	result.y = a.y/b;
	result.z = a.z/b;
	return result;
}
Vector3f vectorMulScene(Vector3f a, float b) {
	Vector3f result;
	result.x = a.x*b;
	result.y = a.y*b;
	result.z = a.z*b;
	return result;
}
float vectorLengthScene(Vector3f a){
	float res;
	res = a.x*a.x + a.y*a.y + a.z*a.z;
	res = sqrt(res);
	return res;
}

ReturnVal Scene::rayIntersect(Ray & ray, int i,int k){
	ReturnVal res,nearRes;
	Material* mat;
	float tmin=1000000;
	Shape* tri;
	nearRes.mat;
	nearRes.isInter=false;


	for(int o=0;o<objects.size();o++){
					tri = objects[o];
					res = tri->intersect(ray);

							
					if(res.isInter && res.t>shadowRayEps){
						if(res.t<tmin){
							mat = materials[objects[o]->matIndex-1];
							tmin = res.t;
							nearRes = res;
							nearRes.mat = mat->id;
							
						}
					}
				}

	return nearRes;
}


Vector3f Scene::rayTracer(Ray & ray, int count,int i, int k){
	ReturnVal res, shadowRes, nearRes,mirrorRes;
	Ray Raywr;
	Ray shadowRay;
	Shape* tri;
	Shape* shadowObject;
	Material* material;
	Material* material2;
	Vector3f contribution, wi,w0 , h,contribution2, wr,resultColor, mirrorColor;
	float cosine,cosine2;
	float red,green,blue;
	nearRes.isInter = false;
	shadowRes.isInter = false;
	mirrorRes.isInter = false;
	res.isInter=false;
	float tmin=1000000;
	float tmins=1;
	red=0;
	green=0;
	blue=0;
				
				res.isInter=false;
				nearRes.isInter = false;
				nearRes=rayIntersect(ray,i,k);
				
				tmins=1;
				if(nearRes.isInter && nearRes.t>0){
					material=materials[nearRes.mat-1];
					
					red = material->ambientRef.x * ambientLight.x;
					green = material->ambientRef.y * ambientLight.y;
					blue = material->ambientRef.z * ambientLight.z;

					
					if((material->mirrorRef.x!=0 || material->mirrorRef.y!=0 || material->mirrorRef.z!=0)&&count<=maxRecursionDepth){
						
						count++;
						tmin=10000000;
						w0 = vectorDifferenceScene(ray.origin,nearRes.pos);
						w0 = vectorDivScene(w0,vectorLengthScene(w0));
						wr = vectorDifferenceScene(vectorMulScene(vectorMulScene(nearRes.normal, dotProductScene(nearRes.normal,w0)),2),w0);
						wr = vectorDivScene(wr, vectorLengthScene(wr));
						Raywr= Ray(nearRes.pos, wr);
						mirrorColor = rayTracer(Raywr,count,i,k);
						res.isInter=false;
						red+=mirrorColor.x*material->mirrorRef.x;
						green+=mirrorColor.y*material->mirrorRef.y;
						blue+=mirrorColor.z*material->mirrorRef.z;
						for(int o=0;o<objects.size();o++){
							tri = objects[o];
							res = tri->intersect(Raywr);
							if(res.isInter && res.t>0){
								if(res.t<tmin){
									material2 = materials[objects[o]->matIndex-1];
									tmin = res.t;
									mirrorRes = res;

									
								}
							}
						}
					
						
					}
					tmins=1;



					for(int j=0 ; j<lights.size(); j++){
						
						shadowRay = Ray(lights[j]->position, vectorDifferenceScene(nearRes.pos,lights[j]->position));

						for(int o=0;o<objects.size();o++){
							shadowObject = objects[o];
							shadowRes = shadowObject->intersect(shadowRay);
							if(shadowRes.isInter && shadowRes.t>shadowRayEps){
								if(shadowRes.t<tmins){
									tmins = shadowRes.t;
								}
							}
						}

						
						
						if(tmins>=0.9998){

							contribution = lights[j]->computeLightContribution(nearRes.pos);
							contribution2 = lights[j]->computeLightContribution(nearRes.pos);
							wi = vectorDifferenceScene(lights[j]->position,nearRes.pos);
							wi = vectorDivScene(wi, vectorLengthScene(wi));
							w0 = vectorDifferenceScene(ray.origin,nearRes.pos);
							w0 = vectorDivScene(w0,vectorLengthScene(w0));
							h = vectorSumScene(wi,w0);
							h = vectorDivScene(h,vectorLengthScene(h));
							cosine = dotProductScene(wi, nearRes.normal)/(vectorLengthScene(nearRes.normal)*vectorLengthScene(wi));
							cosine2 = dotProductScene(h, nearRes.normal)/(vectorLengthScene(nearRes.normal)*vectorLengthScene(h));
							
							if(cosine<0){
								nearRes.normal.x *= -1;
								nearRes.normal.y *= -1;
								nearRes.normal.z *= -1;
								cosine = dotProductScene(nearRes.normal, wi)/(vectorLengthScene(nearRes.normal)*vectorLengthScene(wi));
								if(cosine2<0){
									cosine2 = dotProductScene(nearRes.normal, h)/(vectorLengthScene(nearRes.normal)*vectorLengthScene(h));
								}
							}
							
							if(material->phongExp>100){
								cosine2 = pow(cosine2, 0);
							}else{
								cosine2 = pow(cosine2, material->phongExp);
							}
							
							

			
							if(cosine>=0){
								contribution.x *= cosine;
								contribution.y *= cosine;
								contribution.z *= cosine;
							}else{
								contribution.x *= 0;
								contribution.y *= 0;
								contribution.z *= 0;
							}
							if(cosine2>=0){
								contribution2.x *= cosine2;
								contribution2.y *= cosine2;
								contribution2.z *= cosine2;
							}else{
								contribution2.x *= 0;
								contribution2.y *= 0;
								contribution2.z *= 0;
							}
							red+=material->diffuseRef.x * contribution.x;
							green+=material->diffuseRef.y * contribution.y;
							blue+=material->diffuseRef.z * contribution.z;

							red+=material->specularRef.x * contribution2.x;
							green+=material->specularRef.y * contribution2.y;
							blue+=material->specularRef.z * contribution2.z;

							
						}
						tmins=1;

						
						
						
					}
					if(red<255) resultColor.x=int(red);
					else resultColor.x=255;
					if(green<255) resultColor.y=int(green);
					else resultColor.y=255;
					if(blue<255) resultColor.z=int(blue);
					else resultColor.z=255;

					
				}else{
					resultColor.x=backgroundColor.x;
					resultColor.y=backgroundColor.y;
					resultColor.z=backgroundColor.z;
				}

				tmin=1000000;
				return resultColor;
			

}

void Scene::renderScene(void)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	Camera* cam;
	Image img =Image(0,0);; 
	Color color;
	Ray ray;
	Vector3f wr;

	for(int z=0;z<cameras.size();z++){
		
		cam = cameras[z];
		img= Image(cam->imgPlane.nx,cam->imgPlane.ny);
		
		for(int i=0;i<cam->imgPlane.nx;i++){
			for(int k=0;k<cam->imgPlane.ny;k++){
				ray = cam->getPrimaryRay(i, k);
				wr = rayTracer(ray,1,i,k);
				color.red=wr.x;
				color.grn=wr.y;
				color.blu=wr.z;
				img.setPixelValue(i,k,color);

			}
		}
		img.saveImage(cam->imageName);
	}
	
	
}

// Parses XML file. 
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLElement *pElement;

	maxRecursionDepth = 1;
	shadowRayEps = 0.001;

	eResult = xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	pElement = pRoot->FirstChildElement("MaxRecursionDepth");
	if(pElement != nullptr)
		pElement->QueryIntText(&maxRecursionDepth);

	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%f %f %f", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	pElement = pRoot->FirstChildElement("ShadowRayEpsilon");
	if(pElement != nullptr)
		pElement->QueryFloatText(&shadowRayEps);

	pElement = pRoot->FirstChildElement("IntersectionTestEpsilon");
	if(pElement != nullptr)
		eResult = pElement->QueryFloatText(&intTestEps);

	// Parse cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while(pCamera != nullptr)
	{
        int id;
        char imageName[64];
        Vector3f pos, gaze, up;
        ImagePlane imgPlane;

		eResult = pCamera->QueryIntAttribute("id", &id);
		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &pos.x, &pos.y, &pos.z);
		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &gaze.x, &gaze.y, &gaze.z);
		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &up.x, &up.y, &up.z);
		camElement = pCamera->FirstChildElement("NearPlane");
		str = camElement->GetText();
		sscanf(str, "%f %f %f %f", &imgPlane.left, &imgPlane.right, &imgPlane.bottom, &imgPlane.top);
		camElement = pCamera->FirstChildElement("NearDistance");
		eResult = camElement->QueryFloatText(&imgPlane.distance);
		camElement = pCamera->FirstChildElement("ImageResolution");	
		str = camElement->GetText();
		sscanf(str, "%d %d", &imgPlane.nx, &imgPlane.ny);
		camElement = pCamera->FirstChildElement("ImageName");
		str = camElement->GetText();
		strcpy(imageName, str);

		cameras.push_back(new Camera(id, imageName, pos, gaze, up, imgPlane));

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// Parse materals
	pElement = pRoot->FirstChildElement("Materials");
	XMLElement *pMaterial = pElement->FirstChildElement("Material");
	XMLElement *materialElement;
	while(pMaterial != nullptr)
	{
		materials.push_back(new Material());

		int curr = materials.size() - 1;
	
		eResult = pMaterial->QueryIntAttribute("id", &materials[curr]->id);
		materialElement = pMaterial->FirstChildElement("AmbientReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->ambientRef.r, &materials[curr]->ambientRef.g, &materials[curr]->ambientRef.b);
		materialElement = pMaterial->FirstChildElement("DiffuseReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->diffuseRef.r, &materials[curr]->diffuseRef.g, &materials[curr]->diffuseRef.b);
		materialElement = pMaterial->FirstChildElement("SpecularReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->specularRef.r, &materials[curr]->specularRef.g, &materials[curr]->specularRef.b);
		materialElement = pMaterial->FirstChildElement("MirrorReflectance");
		if(materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->mirrorRef.r, &materials[curr]->mirrorRef.g, &materials[curr]->mirrorRef.b);
		}
				else
		{
			materials[curr]->mirrorRef.r = 0.0;
			materials[curr]->mirrorRef.g = 0.0;
			materials[curr]->mirrorRef.b = 0.0;
		}
		materialElement = pMaterial->FirstChildElement("PhongExponent");
		if(materialElement != nullptr)
			materialElement->QueryIntText(&materials[curr]->phongExp);

		pMaterial = pMaterial->NextSiblingElement("Material");
	}

	// Parse vertex data
	pElement = pRoot->FirstChildElement("VertexData");
	int cursor = 0;
	Vector3f tmpPoint;
	str = pElement->GetText();
	while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
		cursor++;
	while(str[cursor] != '\0')
	{
		for(int cnt = 0 ; cnt < 3 ; cnt++)
		{
			if(cnt == 0)
				tmpPoint.x = atof(str + cursor);
			else if(cnt == 1)
				tmpPoint.y = atof(str + cursor);
			else
				tmpPoint.z = atof(str + cursor);
			while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
				cursor++; 
			while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
				cursor++;
		}
		vertices.push_back(tmpPoint);
	}

	// Parse objects
	pElement = pRoot->FirstChildElement("Objects");
	
	// Parse spheres
	XMLElement *pObject = pElement->FirstChildElement("Sphere");
	XMLElement *objElement;
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int cIndex;
		float R;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Center");
		eResult = objElement->QueryIntText(&cIndex);
		objElement = pObject->FirstChildElement("Radius");
		eResult = objElement->QueryFloatText(&R);

		objects.push_back(new Sphere(id, matIndex, cIndex, R, &vertices));

		pObject = pObject->NextSiblingElement("Sphere");
	}

	// Parse triangles
	pObject = pElement->FirstChildElement("Triangle");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Indices");
		str = objElement->GetText();
		sscanf(str, "%d %d %d", &p1Index, &p2Index, &p3Index);

		objects.push_back(new Triangle(id, matIndex, p1Index, p2Index, p3Index, &vertices));

		pObject = pObject->NextSiblingElement("Triangle");
	}

	// Parse meshes
	pObject = pElement->FirstChildElement("Mesh");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;
		int cursor = 0;
		int vertexOffset = 0;
		vector<Triangle> faces;
		vector<int> *meshIndices = new vector<int>;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Faces");
		objElement->QueryIntAttribute("vertexOffset", &vertexOffset);
		str = objElement->GetText();
		while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			cursor++;
		while(str[cursor] != '\0')
		{
			for(int cnt = 0 ; cnt < 3 ; cnt++)
			{
				if(cnt == 0)
					p1Index = atoi(str + cursor) + vertexOffset;
				else if(cnt == 1)
					p2Index = atoi(str + cursor) + vertexOffset;
				else
					p3Index = atoi(str + cursor) + vertexOffset;
				while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					cursor++; 
				while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					cursor++;
			}
			faces.push_back(*(new Triangle(-1, matIndex, p1Index, p2Index, p3Index, &vertices)));
			meshIndices->push_back(p1Index);
			meshIndices->push_back(p2Index);
			meshIndices->push_back(p3Index);
		}

		objects.push_back(new Mesh(id, matIndex, faces, meshIndices, &vertices));

		pObject = pObject->NextSiblingElement("Mesh");
	}

	// Parse lights
	int id;
	Vector3f position;
	Vector3f intensity;
	pElement = pRoot->FirstChildElement("Lights");

	XMLElement *pLight = pElement->FirstChildElement("AmbientLight");
	XMLElement *lightElement;
	str = pLight->GetText();
	sscanf(str, "%f %f %f", &ambientLight.r, &ambientLight.g, &ambientLight.b);

	pLight = pElement->FirstChildElement("PointLight");
	while(pLight != nullptr)
	{
		eResult = pLight->QueryIntAttribute("id", &id);
		lightElement = pLight->FirstChildElement("Position");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &position.x, &position.y, &position.z);
		lightElement = pLight->FirstChildElement("Intensity");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &intensity.r, &intensity.g, &intensity.b);

		lights.push_back(new PointLight(position, intensity));

		pLight = pLight->NextSiblingElement("PointLight");
	}
}

