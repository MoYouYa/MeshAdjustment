#pragma once
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

enum LoadType {
	Trangle,
	General
};

enum SubFunction {
	Loop_Subdivision,
	Catmull_Clark_Subdivision
};

class Vector2f {
public:
	float x;
	float y;
	Vector2f(float _x = 0.0f, float _y = 0.0f) :x(_x), y(_y) {}

	Vector2f operator+(Vector2f v2) {
		return Vector2f(x + v2.x, y + v2.y);
	}

	Vector2f operator+=(Vector2f v2) {
		this->x += v2.x;
		this->y += v2.y;
		return *this;
	}

	Vector2f operator-(Vector2f v2) {
		return Vector2f(x - v2.x, y - v2.y);
	}

	Vector2f operator-=(Vector2f v2) {
		this->x -= v2.x;
		this->y -= v2.y;
		return *this;
	}

	Vector2f operator*(float n) {
		return Vector2f(x * n, y * n);
	}

	Vector2f operator*=(float n) {
		this->x *= n;
		this->y *= n;
		return *this;
	}

	Vector2f operator/(float n) {
		return Vector2f(x / n, y / n);
	}

	Vector2f operator/=(float n) {
		this->x /= n;
		this->y /= n;
		return *this;
	}
};

class Vector3f {
public:
	float x;
	float y;
	float z;
	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) :x(_x), y(_y), z(_z) {}

	Vector3f operator+(Vector3f v3) {
		return Vector3f(x + v3.x, y + v3.y, z + v3.z);
	}

	Vector3f operator+=(Vector3f v3) {
		this->x += v3.x;
		this->y += v3.y;
		this->z += v3.z;
		return *this;
	}

	Vector3f operator-(Vector3f v3) {
		return Vector3f(x - v3.x, y - v3.y, z - v3.z);
	}

	Vector3f operator-=(Vector3f v3) {
		this->x -= v3.x;
		this->y -= v3.y;
		this->z -= v3.z;
		return *this;
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator*=(float n) {
		this->x *= n;
		this->y *= n;
		this->z *= n;
		return *this;
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f operator/=(float n) {
		this->x /= n;
		this->y /= n;
		this->z /= n;
		return *this;
	}
};

class Mesh {
public:
	std::vector<Vector3f> positions;
	std::vector<Vector2f> textureCoordinates;
	std::vector<Vector3f> normals;
	std::vector<unsigned int> facePointNums;
	std::vector<unsigned int> posIndices;
	std::vector<unsigned int> texIndices;
	std::vector<unsigned int> norIndices;

	Mesh() {}
	Mesh(std::vector<Vector3f> _positions,std::vector<Vector2f> _textureCoordinates,std::vector<Vector3f> _normals
		,std::vector<unsigned int> _facePointNums, std::vector<unsigned int> _posIndices,std::vector<unsigned int> _texIndices,std::vector<unsigned int> _norIndices) {
		positions = _positions;
		textureCoordinates = _textureCoordinates;
		normals = _normals;
		facePointNums = _facePointNums;
		posIndices = _posIndices;
		texIndices = _texIndices;
		norIndices = _norIndices;
	}
};
std::vector<std::string> splitStr(std::string& s, char splitChar=' ');
bool loadObjFile(std::string filePath,Mesh& mesh,LoadType loadType);
bool saveObjFile(Mesh& mesh, std::string filePath);
bool MeshSubdivision(Mesh& mesh,std::string filePath, SubFunction subFunction);
void creatNewVertices_LoopSubdivision(std::vector<std::unordered_set<unsigned int>> sides, Mesh& mesh);
void adjustOldVertices_LoopSubdivision(int oldPointNum, Mesh& mesh);

std::vector<std::string> splitStr(std::string& s, char splitChar) {
	std::vector<std::string> res;
	std::string temp;
	for (char c : s) {
		if (c != splitChar) {
			temp.push_back(c);
		}
		else {
			if (!temp.empty()) {
				res.emplace_back(temp);
				temp = "";
			}
		}
	}
	if (!temp.empty()) {
		res.emplace_back(temp);
		temp = "";
	}
	return res;
}


bool loadObjFile(std::string filePath,Mesh& mesh,LoadType loadType) {
	if (filePath.substr(filePath.size() - 4, 4) != ".obj")
		return false;

	try {
		std::ifstream file(filePath);

		if (!file.is_open())
			return false;

		std::vector<Vector3f> positions;
		std::vector<Vector2f> tCoords;
		std::vector<Vector3f> normals;

		std::vector<unsigned int> facePointNums;
		std::vector<unsigned int> posIndices;
		std::vector<unsigned int> texIndices;
		std::vector<unsigned int> norIndices;

		std::string str;
		while (std::getline(file, str)) {
			if (str[0] == 'v') {
				std::vector<std::string> strs;
				strs = splitStr(str);
				if (str[1] == ' ') {
					positions.emplace_back(Vector3f(std::stof(strs[1]), std::stof(strs[2]), std::stof(strs[3])));
				}
				else if (str[1] == 't') {
					tCoords.emplace_back(Vector2f(std::stof(strs[1]), std::stof(strs[2])));
				}
				else if (str[1] == 'n') {
					normals.emplace_back(Vector3f(std::stof(strs[1]), std::stof(strs[2]), std::stof(strs[3])));
				}
			}
			else if (str[0] == 'f') {
				std::vector<std::string> strs;
				strs = splitStr(str);
				switch (loadType) {
					case LoadType::Trangle:
					{
						if (strs.size() == 4) {
							posIndices.emplace_back(std::stoi(splitStr(strs[1], '/')[0]) - 1);
							posIndices.emplace_back(std::stoi(splitStr(strs[2], '/')[0]) - 1);
							posIndices.emplace_back(std::stoi(splitStr(strs[3], '/')[0]) - 1);

							if (!tCoords.empty()) {
								texIndices.emplace_back(std::stoi(splitStr(strs[1], '/')[1]) - 1);
								texIndices.emplace_back(std::stoi(splitStr(strs[2], '/')[1]) - 1);
								texIndices.emplace_back(std::stoi(splitStr(strs[3], '/')[1]) - 1);
							}
							if (!tCoords.empty() && !normals.empty()) {
								norIndices.emplace_back(std::stoi(splitStr(strs[1], '/')[2]) - 1);
								norIndices.emplace_back(std::stoi(splitStr(strs[2], '/')[2]) - 1);
								norIndices.emplace_back(std::stoi(splitStr(strs[3], '/')[2]) - 1);
							}
							facePointNums.emplace_back(3);
						}
						else if (strs.size() > 4) {
							for (int i = 0; i < strs.size() - 1 - 2; i++) {
								posIndices.emplace_back(std::stoi(splitStr(strs[1], '/')[0]) - 1);
								posIndices.emplace_back(std::stoi(splitStr(strs[2+i], '/')[0]) - 1);
								posIndices.emplace_back(std::stoi(splitStr(strs[3+i], '/')[0]) - 1);

								if (!tCoords.empty()) {
									texIndices.emplace_back(std::stoi(splitStr(strs[1], '/')[1]) - 1);
									texIndices.emplace_back(std::stoi(splitStr(strs[2+i], '/')[1]) - 1);
									texIndices.emplace_back(std::stoi(splitStr(strs[3+i], '/')[1]) - 1);
								}
								if (!tCoords.empty() && !normals.empty()) {
									norIndices.emplace_back(std::stoi(splitStr(strs[1], '/')[2]) - 1);
									norIndices.emplace_back(std::stoi(splitStr(strs[2+i], '/')[2]) - 1);
									norIndices.emplace_back(std::stoi(splitStr(strs[3+i], '/')[2]) - 1);
								}
								facePointNums.emplace_back(3);
							}
						}
					}break;
					case LoadType::General:
					{
						// 
					}break;
				}
			}
		}
		mesh = std::move(Mesh(positions, tCoords, normals,facePointNums, posIndices, texIndices, norIndices));
		return true;
	}
	catch (std::exception e) {
		std::cout << e.what();
		return false;
	}
}

bool saveObjFile(Mesh& mesh,std::string filePath) {
	std::ofstream file;
	try {
		file.open(filePath, std::ios::out);
		if (!file.is_open()) {
			std::cout << "open file:" + filePath << " failed!";
			return false;
		}
		file << "# vertex count = " << mesh.positions.size() << "\n";
		file << "# face count = " << mesh.facePointNums.size()  << "\n";

		for (int i = 0; i < mesh.positions.size(); i++) {
			file << "v " << mesh.positions[i].x << " " << mesh.positions[i].y << " " << mesh.positions[i].z << "\n";
		}
		//if (!mesh.textureCoordinates.empty()) {
		//	for (int i = 0; i < mesh.textureCoordinates.size(); i++) {
		//		file << "vt " << mesh.textureCoordinates[i].x << " " << mesh.textureCoordinates[i].y << "\n";
		//	}
		//}
		//if (!mesh.normals.empty()) {
		//	for (int i = 0; i < mesh.normals.size(); i++) {
		//		file << "vn " << mesh.normals[i].x << " " << mesh.normals[i].y << " " << mesh.normals[i].z << "\n";
		//	}
		//}
		unsigned int offset = 0;
		for (int i = 0; i < mesh.posIndices.size()&&offset<mesh.facePointNums.size(); i+=mesh.facePointNums[offset-1]) {
			//if (!mesh.textureCoordinates.empty() && !mesh.normals.empty()) {
			//	file << "f " << mesh.posIndices[i+0] << "/" << mesh.texIndices[i + 0] << "/" << mesh.norIndices[i+0] << " " 
			//		<< mesh.posIndices[i + 1] << "/" << mesh.texIndices[i + 1] << "/" << mesh.norIndices[i + 1] << " " 
			//		<< mesh.posIndices[i + 2] << "/" << mesh.texIndices[i + 2] << "/" << mesh.norIndices[i + 2] << "\n";
			//}
			//if (!mesh.textureCoordinates.empty() && mesh.normals.empty()) {
			//	file << "f " << mesh.posIndices[i + 0] << "/" << mesh.texIndices[i+0] << " " 
			//		<< mesh.posIndices[i + 1] << "/" << mesh.texIndices[i + 1] << " " 
			//		<< mesh.posIndices[i + 2] << "/" << mesh.texIndices[i + 2] << "\n";
			//}
			//if (mesh.textureCoordinates.empty() && mesh.normals.empty()) {
				//file << "f " << mesh.posIndices[i + 0]+1 << " " << mesh.posIndices[i + 1]+1 <<  " " << mesh.posIndices[i + 2]+1 << "\n";
			//}


			file << "f";
			for (int j = 0; j < mesh.facePointNums[offset]; j++) {
				file << " " << mesh.posIndices[i + j] + 1;
			}
			file << "\n";
			offset++;
		}
	}
	catch (std::exception e) {
		std::cout<<e.what();
		return false;
	}
	return true;
}

bool MeshSubdivision(Mesh& mesh,std::string filePath,SubFunction subFunction) {
	switch (subFunction) {
		case SubFunction::Loop_Subdivision:
		{
			int oldPointNum = mesh.positions.size();
			std::vector<std::unordered_set<unsigned int>> sides(oldPointNum);
			for (int i = 0; i < mesh.posIndices.size(); i+=3) {
				sides[mesh.posIndices[i + 0]].insert(mesh.posIndices[i + 1]);
				sides[mesh.posIndices[i + 0]].insert(mesh.posIndices[i + 2]);

				sides[mesh.posIndices[i + 1]].insert(mesh.posIndices[i + 0]);
				sides[mesh.posIndices[i + 1]].insert(mesh.posIndices[i + 2]);

				sides[mesh.posIndices[i + 2]].insert(mesh.posIndices[i + 0]);
				sides[mesh.posIndices[i + 2]].insert(mesh.posIndices[i + 1]);
			}
			creatNewVertices_LoopSubdivision(sides, mesh);
			adjustOldVertices_LoopSubdivision(oldPointNum, mesh);

		}break;
		case SubFunction::Catmull_Clark_Subdivision:
		{

		}break;
	}
		
	return saveObjFile(mesh,filePath);
}

void creatNewVertices_LoopSubdivision(std::vector<std::unordered_set<unsigned int>> sides,Mesh& mesh) {
	std::vector<unsigned int> newPosIndices;
	std::vector<std::unordered_map<unsigned int ,unsigned int>> midPointInices(sides.size());
	for (int i = 0; i < sides.size(); i++) {
		for (auto j : sides[i]) {
			if (i<j) {
				unsigned int index0, index1;
				int pCount = 0;
				for (int k = 0; k < mesh.posIndices.size() && pCount<2; k += 3) {
					unsigned int p0 = mesh.posIndices[k + 0];
					unsigned int p1 = mesh.posIndices[k + 1];
					unsigned int p2 = mesh.posIndices[k + 2];
					if ((i == p0 || i == p1 || i == p2) && (j == p0 || j == p1 || j == p2)) {
						if (pCount == 0) {
							index0 = p0 + p1 + p2 - i - j;
						}
						else {
							index1 = p0 + p1 + p2 - i - j;
						}
						pCount++;
					}
				}
				if (pCount == 2) {
					Vector3f newPoint = mesh.positions[i] * 3.0f / 8.0f + mesh.positions[j] * 3.0f / 8.0f
						+ mesh.positions[index0] / 8.0f + mesh.positions[index1] / 8.0f;
					mesh.positions.emplace_back(newPoint);
					midPointInices[i].insert({j,mesh.positions.size()-1});
				}
			}
		}
	}
	for (int i = 0; i < mesh.posIndices.size(); i += 3) {
		unsigned int i0 = mesh.posIndices[i + 0];
		unsigned int i1 = mesh.posIndices[i + 1];
		unsigned int i2 = mesh.posIndices[i + 2];
		unsigned int i3 = midPointInices[i0 < i1 ? i0 : i1][i1 > i0 ? i1 : i0];
		unsigned int i4 = midPointInices[i1 < i2 ? i1 : i2][i2 > i1 ? i2 : i1];
		unsigned int i5 = midPointInices[i0 < i2 ? i0 : i2][i2 > i0 ? i2 : i0];

		newPosIndices.emplace_back(i0);
		newPosIndices.emplace_back(i3);
		newPosIndices.emplace_back(i5);

		newPosIndices.emplace_back(i1);
		newPosIndices.emplace_back(i3);
		newPosIndices.emplace_back(i4);

		newPosIndices.emplace_back(i2);
		newPosIndices.emplace_back(i4);
		newPosIndices.emplace_back(i5);

		newPosIndices.emplace_back(i3);
		newPosIndices.emplace_back(i4);
		newPosIndices.emplace_back(i5);
	}
	std::vector<unsigned int> newFacePointNums(newPosIndices.size()/3,3);
	mesh.posIndices = move(newPosIndices);
	mesh.facePointNums = move(newFacePointNums);
}

void adjustOldVertices_LoopSubdivision(int oldPointNum, Mesh& mesh) {
	std::vector<std::unordered_set<unsigned int>> sides(oldPointNum);
	for (int i = 0; i < mesh.posIndices.size(); i += 3) {
		unsigned int i0 = mesh.posIndices[i + 0];
		unsigned int i1 = mesh.posIndices[i + 1];
		unsigned int i2 = mesh.posIndices[i + 2];
		if (i0 < oldPointNum) {
			sides[i0].insert(i1);
			sides[i0].insert(i2);
		}
		if (i1 < oldPointNum) {
			sides[i1].insert(i0);
			sides[i1].insert(i2);
		}
		if (i2 < oldPointNum) {
			sides[i2].insert(i0);
			sides[i2].insert(i1);
		}
	}
	for (int i = 0; i < oldPointNum; i++) {
		unsigned int n = sides[i].size();
		float u;
		if (n == 3) {
			u = 3.0f / 16.0f;
		}
		else {
			u = 3.0f / 8.0f / n;
		}
		mesh.positions[i] *= (float)(1 - u * n);
		for (auto j : sides[i]) {
			mesh.positions[i] += mesh.positions[j] * u;
		}
	}
}