#include "Texture2D.h"
#include <iostream>
#include <fstream>

using namespace std;

Texture2D::Texture2D(void)
{
}


Texture2D::~Texture2D(void)
{
}

bool Texture2D::LoadRAW(char* path, int width, int height)
{
	char* tempTextureData; 
	int fileSize; 
	ifstream inFile;
	_width = width; _height = height;
	inFile.open(path, ios::binary);

	if(!inFile.good())
	{
		cerr << "Can't open texture file " << path << endl;
		return false;
	}

	inFile.seekg(0, ios::end); // seek to the end of file
	fileSize = (int)inFile.tellg(); //Get current position in file - The end, this gives us total file size
	tempTextureData = new char [fileSize];
	inFile.seekg(0, ios::beg); //Seek back to beginning of the file
	inFile.read(tempTextureData, fileSize); // Read in all the data in one go
	inFile.close(); //close the file

	cout<< path << " loaded." << endl;

	glGenTextures(1, &_ID); // Get next texture ID
	glBindTexture(GL_TEXTURE_2D, _ID); //Bind the texture to the ID
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, tempTextureData);
	delete [] tempTextureData; //Clear up the data - We dont need this anymore
	return true;
}

bool Texture2D::LoadTGA(const char* textureFileName)
{
	
	char* tempHeaderData = new char[18]; //18 Bytes is TGA Header Size
	char* tempTextureData;  
	int fileSize;
	char type, pixelDepth, mode;

	ifstream inFile;

	inFile.open(textureFileName, ios::binary);
	if (!inFile.good())  
	{
		cerr  << "Can't open texture file " << textureFileName << endl;
		return -1;
	}

	//18 Bytes is the size of a TGA Header
	inFile.seekg (0, ios::beg); //Seek back to beginning of file
	inFile.read (tempHeaderData, 18); //Read in all the data in one go

	inFile.seekg (0, ios::end); //Seek to end of file
	fileSize = (int)inFile.tellg() - 18; //Get current position in file - The End, this gives us total file size
	tempTextureData = new char [fileSize]; //Create an new aray to store data
	inFile.seekg (18, ios::beg); //Seek back to beginning of file + 18
	inFile.read (tempTextureData, fileSize); //Read in all the data in one go
	inFile.close(); //Close the file

	type = tempHeaderData[2]; //Get TGA Type out of Header - Must be RGB for this to work
	_width = ((unsigned char)tempHeaderData[13] << 8u) + (unsigned char)tempHeaderData[12]; // Find the width (Combines two bytes into a short)
	_height = ((unsigned char)tempHeaderData[15] << 8u) + (unsigned char)tempHeaderData[14]; //Find the height
	pixelDepth = tempHeaderData[16]; // Find the pixel depth (24/32bpp)

	bool flipped = false;
	if ((int)((tempHeaderData[11] << 8) + tempHeaderData[10]) == 0)
		flipped = true;

	//We only support RGB type
	if (type == 2)
	{
		cout << textureFileName << " loaded." << endl;
		
		glGenTextures(1, &_ID); //Get next Texture ID
		glBindTexture(GL_TEXTURE_2D, _ID); //Bind the texture to the ID

		mode = pixelDepth / 8;

		//Note that TGA files are stored as BGR(A) - So we need to specify the format as GL_BGR(A)_EXT
		if (mode == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, tempTextureData);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, tempTextureData);
		}		
	}

	delete [] tempHeaderData; //We don't need the header memory anymore
	delete [] tempTextureData; //Clear up the data - We don't need this any more


	return _ID;
}

bool Texture2D::LoadBMP(const char * imagepath)
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned char * data;

	#pragma warning(disable : 4996)
	FILE * file = fopen(imagepath, "rb");

	if (!file)                              
	{
		printf("Image could not be opened\n"); 
		return 0;
	}

	if(fread(header, 1, 54, file) != 54)
	{
		printf("Not a correct BMP file\n");
		return false;
	}

	if(header[0] != 'B' || header[1] != 'M')
	{
		printf("Not a correct BMP file \n");
	}

	// read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	_width = *(int*)&(header[0x12]);
	_height = *(int*)&(header[0x16]);

	if(imageSize == 0)
		imageSize = _width*_height*3;
	if(dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data,1,imageSize,file);
	fclose(file);

	cout<< imagepath << " loaded." << endl;

	glGenTextures(1, &_ID);
	glBindTexture(GL_TEXTURE_2D, _ID);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, _width, _height, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	delete [] data;

	return true;
}
