#pragma once
//procGen.h
#pragma once
#include "../ew/mesh.h"
namespace as {
	ew::MeshData createCube(float size); 
	ew::MeshData createSphere(float radius, int subdivisions);
	ew::MeshData createCylinder(float radius, float height, int subdivisions);
}


