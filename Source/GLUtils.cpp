#include "GLUtils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <exception>
#include "Debug.h"

// https://github.com/opengl-tutorials/ogl

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vertex_shader_code;
	std::ifstream vertex_shader_stream(vertex_file_path, std::ios::in);
	if (vertex_shader_stream.is_open()) {
		std::stringstream stream;
		stream << vertex_shader_stream.rdbuf();
		vertex_shader_code = stream.str();
		vertex_shader_stream.close();
	} else {
		std::stringstream stream;
		stream << "Impossible to open" << vertex_file_path << "Are you in the right directory ? Don't forget to read the FAQ !\n";
		throw std::runtime_error(stream.str());
	}

	// Read the Fragment Shader code from the file
	std::string fragment_shader_code;
	std::ifstream fragment_shader_stream(fragment_file_path, std::ios::in);
	if (fragment_shader_stream.is_open()) {
		std::stringstream stream;
		stream << fragment_shader_stream.rdbuf();
		fragment_shader_code = stream.str();
		fragment_shader_stream.close();
	}

	GLint result = GL_FALSE;
	int info_log_length;

	// Compile Vertex Shader
	Print("Compiling shader:", vertex_file_path);
	char const* vertex_source_pointer = vertex_shader_code.c_str();
	glShaderSource(vertex_shader_id, 1, &vertex_source_pointer, NULL);
	glCompileShader(vertex_shader_id);

	// Check Vertex Shader
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
		std::vector<char> vertex_shader_err_msg(info_log_length + 1);
		glGetShaderInfoLog(vertex_shader_id, info_log_length, NULL, &vertex_shader_err_msg[0]);
		Print(&vertex_shader_err_msg[0]);
	}

	// Compile Fragment Shader
	Print("Compiling shader:", fragment_file_path);
	char const* fragment_source_pointer = fragment_shader_code.c_str();
	glShaderSource(fragment_shader_id, 1, &fragment_source_pointer, NULL);
	glCompileShader(fragment_shader_id);

	// Check Fragment Shader
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
		std::vector<char> fragment_shader_err_msg(info_log_length + 1);
		glGetShaderInfoLog(fragment_shader_id, info_log_length, NULL, &fragment_shader_err_msg[0]);
		Print(&fragment_shader_err_msg[0]);
	}

	// Link the program
	Print("Linking program");
	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);

	// Check the program
	glGetProgramiv(program_id, GL_LINK_STATUS, &result);
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
		std::vector<char> program_err_msg(info_log_length + 1);
		glGetProgramInfoLog(program_id, info_log_length, NULL, &program_err_msg[0]);
		Print(&program_err_msg[0]);
	}
	
	glDetachShader(program_id, vertex_shader_id);
	glDetachShader(program_id, fragment_shader_id);
	
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	return program_id;
}


#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

GLuint LoadBMP(const char* imagepath){

	Print("Reading image", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file){
		Print(imagepath, "could not be opened.");
		getchar();
		return 0;
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if ( fread(header, 1, 54, file)!=54 ){ 
		Print("Not a correct BMP file");
		fclose(file);
		return 0;
	}
	// A BMP files always begins with "BM"
	if ( header[0]!='B' || header[1]!='M' ){
		Print("Not a correct BMP file");
		fclose(file);
		return 0;
	}
	
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=3  )         {Print("Not a correct BMP file");    fclose(file); return 0;}
	if ( *(char*)&(header[0x1C])!=32 )         {Print("Not a correct BMP file");    fclose(file); return 0;}

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	// Everything is in memory now, the file can be closed.
	fclose (file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete [] data;

	// Nice filtering, or ...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... ugly trilinear filtering ...
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// ... which requires mipmaps. Generate them automatically.
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}

GLuint LoadDDS(const char* path){

	unsigned char header[124];

	FILE *fp; 
 
	/* try to open the file */ 
	fp = fopen(path, "rb"); 
	if (fp == NULL) {
		std::stringstream stream;
		stream << path << "could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n";
		throw std::runtime_error(stream.str());
	}
   
	/* verify the type of file */ 
	char filecode[4]; 
	fread(filecode, 1, 4, fp); 
	if (strncmp(filecode, "DDS ", 4) != 0) { 
		fclose(fp); 
		return 0; 
	}
	
	/* get the surface desc */ 
	fread(&header, 124, 1, fp); 

	unsigned int height      = *(unsigned int*)&(header[8 ]);
	unsigned int width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);

 
	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */ 
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize; 
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char)); 
	fread(buffer, 1, bufsize, fp); 
	/* close the file pointer */ 
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4; 
	unsigned int format;
	switch (fourCC) { 
		case FOURCC_DXT1: 
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; 
			break; 
		case FOURCC_DXT3: 
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; 
			break; 
		case FOURCC_DXT5: 
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; 
			break; 
		default: 
			free(buffer); 
			return 0; 
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);	
	
	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16; 
	unsigned int offset = 0;

	/* load the mipmaps */ 
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) 
	{ 
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize; 
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,  
			0, size, buffer + offset); 
	 
		offset += size; 
		width  /= 2; 
		height /= 2; 

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(width < 1) width = 1;
		if(height < 1) height = 1;

	} 

	free(buffer); 

	return textureID;


}

void PrintMat4(glm::mat4 mat) {
	for (uint y = 0; y < 4; y++) {
		for (uint x = 0; x < 4; x++) {
			Print(mat[x][y]);
		}
		std::cout << "\n";
	}
}