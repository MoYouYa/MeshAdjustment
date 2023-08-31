#include"MeshAdjustment.h"


void main() {
	Mesh mesh;
	//loadObjFile("models/cube/cube.obj", mesh,LoadType::General);
	//MeshSubdivision(mesh, "cube_Catmull_Clark_Subdivision1.obj", SubFunction::Catmull_Clark_Subdivision);
	//MeshSubdivision(mesh, "cube_Catmull_Clark_Subdivision2.obj",SubFunction::Catmull_Clark_Subdivision);
	//MeshSubdivision(mesh, "cube_Catmull_Clark_Subdivision3.obj", SubFunction::Catmull_Clark_Subdivision);
	//MeshSubdivision(mesh, "cube_Catmull_Clark_Subdivision4.obj", SubFunction::Catmull_Clark_Subdivision);
	//loadObjFile("RecCube.obj", mesh, LoadType::General);
	//MeshSubdivision(mesh, "RecCube_Catmull_Clark_Subdivision_1.obj", SubFunction::Catmull_Clark_Subdivision);
	//MeshSubdivision(mesh, "RecCube_Catmull_Clark_Subdivision_2.obj", SubFunction::Catmull_Clark_Subdivision);
	//MeshSubdivision(mesh, "RecCube_Catmull_Clark_Subdivision_3.obj", SubFunction::Catmull_Clark_Subdivision);
	//MeshSubdivision(mesh, "RecCube_Catmull_Clark_Subdivision_4.obj", SubFunction::Catmull_Clark_Subdivision);
	//MeshSubdivision(mesh, "spot_triangulated.obj");
	//MeshSubdivision(mesh, "spot_triangulated.obj");
	loadObjFile("models/spot/spot_control_mesh.obj", mesh, LoadType::General);
	MeshSubdivision(mesh, "spot_control_mesh_Catmull_Clark_Subdivision_1.obj", SubFunction::Catmull_Clark_Subdivision);
	MeshSubdivision(mesh, "spot_control_mesh_Catmull_Clark_Subdivision_2.obj", SubFunction::Catmull_Clark_Subdivision);
	MeshSubdivision(mesh, "spot_control_mesh_Catmull_Clark_Subdivision_3.obj", SubFunction::Catmull_Clark_Subdivision);
	return;
}