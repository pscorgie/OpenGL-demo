#include "MeshLoader.h"

#include <iostream>
#include <fstream>
#include "GL\freeglut.h" //Freeglut library
#include <vector>

using namespace std;

namespace MeshLoader
{
	void LoadVertices(ifstream& inFile, Mesh& mesh);
	void LoadNormals(ifstream& inFile, Mesh& mesh);
	void LoadtexCoords(ifstream& inFile, TexturedMesh& mesh);
	void LoadIndices(ifstream& inFile, Mesh& mesh);

	void LoadVertices(ifstream& inFile, Mesh& mesh)
	{
		inFile >> mesh.VertexCount;
		
		if(mesh.VertexCount > 0)
		{
			mesh.Vertices = new Vertex[mesh.VertexCount];
			for (int i = 0; i < mesh.VertexCount; i++)
			{
				inFile >> mesh.Vertices[i].x;
				inFile >> mesh.Vertices[i].y;
				inFile >> mesh.Vertices[i].z;
			}
		}
	}

	void LoadNormals(ifstream& inFile, Mesh& mesh)
	{
		inFile >> mesh.NormalCount;
		
		if(mesh.NormalCount > 0)
		{
			mesh.Normal = new Vector3[mesh.NormalCount];
			for (int i = 0; i < mesh.NormalCount; i++)
			{
				inFile >> mesh.Normal[i].x;
				inFile >> mesh.Normal[i].y;
				inFile >> mesh.Normal[i].z;
			}
		}
	}
	void LoadtexCoords(ifstream& inFile, TexturedMesh& mesh)
	{
		inFile >> mesh.TexCoordCount;
		if(mesh.TexCoordCount > 0)
		{
			mesh.TexCoords = new TexCoord[mesh.TexCoordCount];
			for (int i = 0; i < mesh.TexCoordCount; i++)
			{
				inFile >> mesh.TexCoords[i].u;
				inFile >> mesh.TexCoords[i].v;
			}
		}
	}
	void LoadIndices(ifstream& inFile, Mesh& mesh)
	{
		inFile >> mesh.IndexCount;
		
		if(mesh.IndexCount > 0)
		{
			mesh.Indices = new GLushort[mesh.IndexCount];
			for (int i = 0; i < mesh.IndexCount; i++)
			{
				inFile >> mesh.Indices[i];
			}
		}
	}

	Mesh* MeshLoader::Load(char* path)
	{
		Mesh* mesh = new Mesh();
		
		ifstream inFile;

		inFile.open(path);

		if(!inFile.good())
		{
			cerr << "Cant open texture file" << path << endl;
			return nullptr;
		}

		LoadVertices(inFile, *mesh);
		LoadNormals(inFile, *mesh);
		LoadIndices(inFile, *mesh);

		return mesh;
	}

	TexturedMesh* MeshLoader::LoadTextured(char* path)
	{
		TexturedMesh* mesh = new TexturedMesh();
		mesh->Mesh = new Mesh();

		ifstream inFile;

		inFile.open(path);

		if(!inFile.good())
		{
			cerr << "Cant open texture file" << path << endl;
			return nullptr;
		}

		LoadVertices(inFile, *mesh->Mesh);
		LoadtexCoords(inFile, *mesh);
		LoadNormals(inFile, *mesh->Mesh);
		LoadIndices(inFile, *mesh->Mesh);
		
		return mesh;
	}
	TexturedMesh* MeshLoader::LoadOBJ(char* path)
	{
	
		TexturedMesh* mesh = new TexturedMesh();
		mesh->Mesh = new Mesh();
		vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		vector< Vector3 > temp_vertices;
		vector< TexCoord > temp_uvs;
		vector< Vector3 > temp_normals;

		#pragma warning(disable : 4996)
		FILE * inFile = fopen(path, "r");

		if(inFile == NULL)
		{
			cerr << "Can't open object file" << path << endl;
			return nullptr;
		}

		while( 1 )
		{
			char lineHeader[128];
			// read the first word of the line
			int res = fscanf(inFile, "%s", lineHeader);
			if(res == EOF)
				break;

			if(strcmp(lineHeader, "v") == 0 )
			{
				Vector3 vertex;
				fscanf(inFile, "%f %f %f \n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}

			if(strcmp(lineHeader, "vt") == 0)
			{
				TexCoord uv;
				fscanf(inFile, "%f %f\n", &uv.u, &uv.v);
				temp_uvs.push_back(uv);
			}
			if(strcmp(lineHeader, "vn") == 0 )
			{
				Vector3 normal;
				fscanf(inFile, "%f %f %f \n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}

			else if (strcmp(lineHeader, "f") == 0)
			{
				string vertex1, vertex2, vertex3;
				int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(inFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
				if(matches != 9)
				{
					printf("File cant be read");
					return false;
				}
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);

			}	
		
		}

		for(unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			unsigned int vertexIndex = vertexIndices[i];
			Vector3 vertex = temp_vertices[vertexIndex - 1];
			mesh->Mesh->vertices.push_back(vertex);
		
		}
	

		for(unsigned int i = 0; i < uvIndices.size(); i++)
		{
			unsigned int uvIndex = uvIndices[i];
			TexCoord uv = temp_uvs[uvIndex - 1];
			mesh->Mesh->uvs.push_back(uv);
		}

		for(unsigned int i = 0; i < mesh->Mesh->uvs.size(); i++)
		{
			unsigned int normalIndex = normalIndices[i];
			Vector3 normal = temp_normals[normalIndex - 1];
			mesh->Mesh->normals.push_back(normal);
		}

	
		fclose(inFile);
		cout << path << " loaded." << endl;
		return mesh;
	}
	
}
