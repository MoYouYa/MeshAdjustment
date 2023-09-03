#pragma once
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <functional>

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

	bool operator==(Vector2f v2) {
		return abs(x - v2.x) <= 1e-5 && abs(y - v2.y) <= 1e-5;
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

	bool operator==(Vector3f v3) {
		return abs(x - v3.x) <= 1e-5 && abs(y - v3.y) <= 1e-5 && abs(z - v3.z) <= 1e-5;
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

	inline bool hasTextureCoords() { return texIndices.size() > 0; }

	inline bool hasNormals() { return norIndices.size() > 0; }

	bool isTriangulated() {
		for (int i = 0; i < facePointNums.size(); i++) {
			if (facePointNums[i] != 3) {
				return false;
			}
		}
		return true;
	}

	void triangulateFace() {
		if (!isTriangulated()) {
			std::vector<unsigned int> newFacePointNums;
			std::vector<unsigned int> newPosIndices;
			std::vector<unsigned int> newTexIndices;
			std::vector<unsigned int> newNorIndices;
			for (int i = 0, j = 0; i < facePointNums.size(); i++) {
				if (facePointNums[i] == 3) {
					newPosIndices.emplace_back(posIndices[j]);
					newTexIndices.emplace_back(texIndices[j]);
					newNorIndices.emplace_back(norIndices[j]);
					newPosIndices.emplace_back(posIndices[j+1]);
					newTexIndices.emplace_back(texIndices[j+1]);
					newNorIndices.emplace_back(norIndices[j+1]);
					newPosIndices.emplace_back(posIndices[j+2]);
					newTexIndices.emplace_back(texIndices[j+2]);
					newNorIndices.emplace_back(norIndices[j+2]);
					newFacePointNums.emplace_back(3);
					j += 3;
				}
				else {
					for (int k = 0; k+2 < facePointNums[i]; k++) {
						newPosIndices.emplace_back(posIndices[j+k]);
						newTexIndices.emplace_back(texIndices[j+k]);
						newNorIndices.emplace_back(norIndices[j+k]);
						newPosIndices.emplace_back(posIndices[j + k+1]);
						newTexIndices.emplace_back(texIndices[j + k+1]);
						newNorIndices.emplace_back(norIndices[j + k+1]);
						newPosIndices.emplace_back(posIndices[j + k+2]);
						newTexIndices.emplace_back(texIndices[j + k+2]);
						newNorIndices.emplace_back(norIndices[j + k+2]);
						newFacePointNums.emplace_back(3);
					}
					j += facePointNums[i];
				}
			}
			facePointNums = move(newFacePointNums);
			posIndices = move(newPosIndices);
			texIndices = move(newTexIndices);
			norIndices = move(newNorIndices);
		}
	}
};

Vector3f& normalizeVector(Vector3f& v3);
Vector2f& normalizeVector(Vector2f& v2);
std::vector<std::string> splitStr(std::string& s, char splitChar=' ');
bool loadObjFile(std::string filePath,Mesh& mesh);
bool saveObjFile(Mesh& mesh, std::string filePath);
bool MeshSubdivision(Mesh& mesh,SubFunction subFunction);
void creatNewVertices_LoopSubdivision(std::vector<std::unordered_set<unsigned int>> sides, Mesh& mesh);
void adjustOldVertices_LoopSubdivision(int oldPointNum, Mesh& mesh);
void creatNewVertices_Catmull_ClarkSubdivision(std::vector<std::unordered_set<unsigned int>> sides, Mesh& mesh);
void adjustOldVertices_Catmull_ClarkSubdivision(int oldPointNum, Mesh& mesh);

Vector3f& normalizeVector(Vector3f& v3) {
	float len = v3.x * v3.x + v3.y * v3.y + v3.z * v3.z;
	len = sqrtf(len);
	v3.x /= len;
	v3.y /= len;
	v3.z /= len;
	return v3;
}

Vector2f& normalizeVector(Vector2f& v2) {
	float len = v2.x * v2.x + v2.y * v2.y;
	len = sqrtf(len);
	v2.x /= len;
	v2.y /= len;
	return v2;
}

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


bool loadObjFile(std::string filePath,Mesh& mesh) {
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
				for (int i = 0; i < strs.size() - 1; i++) {
					posIndices.emplace_back(std::stoi(splitStr(strs[1 + i], '/')[0]) - 1);
					if (!tCoords.empty()) {
						texIndices.emplace_back(std::stoi(splitStr(strs[1 + i], '/')[1]) - 1);
					}
					if (!tCoords.empty() && !normals.empty()) {
						norIndices.emplace_back(std::stoi(splitStr(strs[1 + i], '/')[2]) - 1);
					}
				}
				facePointNums.emplace_back(strs.size() - 1);
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
		for (int i = 0; i < mesh.posIndices.size()&&offset<mesh.facePointNums.size(); i+=mesh.facePointNums[offset],offset++){
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
		}
		file.close();
	}
	catch (std::exception e) {
		std::cout<<e.what();
		file.close();
		return false;
	}
	return true;
}

bool MeshSubdivision(Mesh& mesh,SubFunction subFunction) {
	int oldPointNum = mesh.positions.size();
	std::vector<std::unordered_set<unsigned int>> sides(oldPointNum);
	for (int i = 0,offset=0; i < mesh.posIndices.size()&&offset<mesh.facePointNums.size(); i+=mesh.facePointNums[offset],offset++) {
		for (int j = 0; j < mesh.facePointNums[offset];j++) {
			sides[mesh.posIndices[i + j]].insert(mesh.posIndices[i + (j < mesh.facePointNums[offset] - 1 ? j + 1 : 0)]);
			sides[mesh.posIndices[i + j]].insert(mesh.posIndices[i + (j > 0 ? j - 1 : mesh.facePointNums[offset] - 1)]);
		}
	}
	switch (subFunction) {
		case SubFunction::Loop_Subdivision:
		{
			if (!mesh.isTriangulated()) {
				return false;
			}
			creatNewVertices_LoopSubdivision(sides, mesh);
			adjustOldVertices_LoopSubdivision(oldPointNum, mesh);

		}break;
		case SubFunction::Catmull_Clark_Subdivision:
		{
			creatNewVertices_Catmull_ClarkSubdivision(sides, mesh);
			adjustOldVertices_Catmull_ClarkSubdivision(oldPointNum, mesh);
		}break;
	}
		
	return true;
}

void creatNewVertices_LoopSubdivision(std::vector<std::unordered_set<unsigned int>> sides,Mesh& mesh) {
	std::vector<unsigned int> newPosIndices;
	std::vector<std::unordered_map<unsigned int ,unsigned int>> midPosInices(sides.size());
	//std::vector<std::unordered_map<unsigned int, unsigned int>> midTexInices;
	//std::vector<std::unordered_map<unsigned int, unsigned int>> midNorInices;
	//if (mesh.hasTextureCoords()) {
	//	std::vector<std::unordered_map<unsigned int, unsigned int>> temp(sides.size());
	//	midTexInices= move(temp);
	//}
	//if (mesh.hasNormals()) {
	//	std::vector<std::unordered_map<unsigned int, unsigned int>> temp(sides.size());
	//	midNorInices = move(temp);
	//}

//#pragma region two function to find the index of vector<Vector3f || Vector2f>
//	std::function<int( std::vector<Vector3f>&,  Vector3f&)> findVector3fIndex = []( std::vector<Vector3f>& list,  Vector3f& v3) {
//		for (int i = 0; i < list.size(); i++) {
//			if (list[i] == v3) {
//				return i;
//			}
//		}
//		return -1;
//	};
//
//	std::function<int(std::vector<Vector2f>&, Vector2f&)> findVector2fIndex = [](std::vector<Vector2f>& list, Vector2f& v2) {
//		for (int i = 0; i < list.size(); i++) {
//			if (list[i] == v2) {
//				return i;
//			}
//		}
//		return -1;
//	};
//#pragma endregion


	for (int i = 0; i < sides.size(); i++) {
		for (auto j : sides[i]) {
			if (i<j) {
				unsigned int posIndex0, posIndex1;
				//unsigned int texIndex0, texIndex1;
				//unsigned int norIndex0, norIndex1;
				int pCount = 0;
				for (int k = 0; k < mesh.posIndices.size() && pCount<2; k += 3) {
					unsigned int p0 = mesh.posIndices[k + 0];
					unsigned int p1 = mesh.posIndices[k + 1];
					unsigned int p2 = mesh.posIndices[k + 2];
					if ((i == p0 || i == p1 || i == p2) && (j == p0 || j == p1 || j == p2)) {
						if (pCount == 0) {
							posIndex0 = p0 + p1 + p2 - i - j;
						}
						else {
							posIndex1 = p0 + p1 + p2 - i - j;
						}
						pCount++;
					}
				}
				if (pCount == 2) {
					Vector3f newPoint = mesh.positions[i] * 3.0f / 8.0f + mesh.positions[j] * 3.0f / 8.0f
						+ mesh.positions[posIndex0] / 8.0f + mesh.positions[posIndex1] / 8.0f;
					mesh.positions.emplace_back(newPoint);
					midPosInices[i].insert({j,mesh.positions.size()-1});
				}
			}
		}
	}
	for (int i = 0; i < mesh.posIndices.size(); i += 3) {
		unsigned int i0 = mesh.posIndices[i + 0];
		unsigned int i1 = mesh.posIndices[i + 1];
		unsigned int i2 = mesh.posIndices[i + 2];
		unsigned int i3 = midPosInices[i0 < i1 ? i0 : i1][i1 > i0 ? i1 : i0];
		unsigned int i4 = midPosInices[i1 < i2 ? i1 : i2][i2 > i1 ? i2 : i1];
		unsigned int i5 = midPosInices[i0 < i2 ? i0 : i2][i2 > i0 ? i2 : i0];

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

void creatNewVertices_Catmull_ClarkSubdivision(std::vector<std::unordered_set<unsigned int>> sides, Mesh& mesh) {
	std::vector<unsigned int> newPosIndices;
	std::vector<std::unordered_map<unsigned int, unsigned int>> midPosInices(sides.size());
	for (int i = 0; i < sides.size(); i++) {
		for (auto j : sides[i]) {
			if (i < j) {
				Vector3f center1(0,0,0), center2(0,0,0);
				int pCount = 0;
				for (int k = 0,offset=0; k < mesh.posIndices.size() && pCount < 2 && offset<mesh.facePointNums.size(); k += mesh.facePointNums[offset],offset++) {
					int same =0;
					for (int z = 0; z < mesh.facePointNums[offset]; z++) {
						if (i == mesh.posIndices[k + z] || j == mesh.posIndices[k + z]) {
							same++;
							if (same == 2) {
								break;
							}
						}
					}
					if (same == 2) {
						if (pCount == 0) {
							for (int p = 0; p < mesh.facePointNums[offset]; p++) {
								center1 += mesh.positions[mesh.posIndices[k + p]] / mesh.facePointNums[offset];
							}
							pCount++;
						}
						else if (pCount == 1) {
							for (int p = 0; p < mesh.facePointNums[offset]; p++) {
								center2 += mesh.positions[mesh.posIndices[k + p]] / mesh.facePointNums[offset];
							}
							pCount++;
						}
					}
					if (pCount == 2) {
						break;
					}
				}
				if (pCount == 2) {
					Vector3f newPoint = mesh.positions[i] / 4.0f + mesh.positions[j] / 4.0f
						+ center1 / 4.0f + center2 / 4.0f;
					mesh.positions.emplace_back(newPoint);
					midPosInices[i].insert({ j,mesh.positions.size() - 1 });
				}
			}
		}
	}
	for (int i = 0,offset=0; i < mesh.posIndices.size()&&offset<mesh.facePointNums.size(); i += mesh.facePointNums[offset],offset++) {
		Vector3f center(0,0,0);
		unsigned int centerIndex;
		for (int j = 0; j < mesh.facePointNums[offset]; j++) {
			center += mesh.positions[mesh.posIndices[i + j]] / mesh.facePointNums[offset];
		}
		mesh.positions.emplace_back(center);
		centerIndex = mesh.positions.size() - 1;
		for (int j = 0; j < mesh.facePointNums[offset]; j++) {
			unsigned int mid = mesh.posIndices[i + j];
			unsigned int right = mesh.posIndices[i + (j + 1) % mesh.facePointNums[offset]];
			unsigned int left = mesh.posIndices[i + (j + mesh.facePointNums[offset] - 1) % mesh.facePointNums[offset]];
			newPosIndices.emplace_back(mid);
			newPosIndices.emplace_back(midPosInices[mid < right ? mid : right][right > mid ? right : mid]);
			newPosIndices.emplace_back(centerIndex);
			newPosIndices.emplace_back(midPosInices[mid < left ? mid : left][left > mid ? left : mid]);
		}
	}
	std::vector<unsigned int> newFacePointNums(newPosIndices.size() / 4, 4);
	mesh.posIndices = move(newPosIndices);
	mesh.facePointNums = move(newFacePointNums);
}

void adjustOldVertices_Catmull_ClarkSubdivision(int oldPointNum, Mesh& mesh) {
	std::vector<std::unordered_set<unsigned int>> sides(oldPointNum);
	for (int i = 0, offset = 0; i < mesh.posIndices.size() && offset < mesh.facePointNums.size(); i += mesh.facePointNums[offset], offset++) {
		for (int j = 0; j < mesh.facePointNums[offset]; j++) {
			if (mesh.posIndices[i + j] < oldPointNum) {
				sides[mesh.posIndices[i + j]].insert(mesh.posIndices[i + (j < mesh.facePointNums[offset] - 1 ? j + 1 : 0)]);
				sides[mesh.posIndices[i + j]].insert(mesh.posIndices[i + (j > 0 ? j - 1 : mesh.facePointNums[offset] - 1)]);
			}
		}
	}
	for (int i = 0; i < oldPointNum; i++) {
		unsigned int degree = sides[i].size();
		mesh.positions[i] /= 4.0f;
		for (auto j : sides[i]) {
			mesh.positions[i] += (mesh.positions[j] * 2.0f / degree / 4.0f);
		}
		int pCount = 0;
		for (int k = 0, offset = 0; k < mesh.posIndices.size() && offset < mesh.facePointNums.size(); k += mesh.facePointNums[offset], offset++) {
			unsigned int same = 0;
			unsigned int index;
			for (int z = 0; z < mesh.facePointNums[offset]; z++) {
				if (sides[i].find(mesh.posIndices[k + z]) != sides[i].end()) {
					same++;
				}
				else {
					index = mesh.posIndices[k + z];
				}
			}
			if (same == 2) {
				mesh.positions[i] += (mesh.positions[index] / degree / 4.0f);
				pCount++;
				if (pCount >= degree) {
					break;
				}
			}
		}
	}
}