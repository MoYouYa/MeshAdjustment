
#include"MeshAdjustment.h"

void main() {
	Mesh mesh;
	loadObjFile("models/cube/cube.obj", mesh,LoadType::Trangle);
	MeshSubdivision(mesh, "cube_1.obj",SubFunction::Loop_Subdivision);
	//MeshSubdivision(mesh, "spot_triangulated.obj");
	//MeshSubdivision(mesh, "spot_triangulated.obj");
	return;
}