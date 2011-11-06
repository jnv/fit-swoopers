#include <cstdio>
#include <string>
#include <iostream>
#include "util.h"

GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile)
{
    GLuint shader = glCreateShader(eShaderType);
    const char *strFileData = strShaderFile.c_str();
    glShaderSource(shader, 1, &strFileData, NULL);

    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
	GLint infoLogLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

	GLchar *strInfoLog = new GLchar[infoLogLength + 1];
	glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

	const char *strShaderType = NULL;
	switch(eShaderType)
	{
	case GL_VERTEX_SHADER: strShaderType = "vertex";
	    break;
	case GL_FRAGMENT_SHADER: strShaderType = "fragment";
	    break;
	}

	fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
	delete[] strInfoLog;
	return 0;
    }

    return shader;
}

GLuint CreateShader(GLenum eShaderType, const char *filename)
{
    FILE *f = fopen(filename, "rb");
    if(!f)
    {
	std::cout << "Unable to open file " << filename << " for reading\n" << std::endl;
	return 0;
    }

    fseek(f, 0, SEEK_END);
    int length = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buffer = new char[length + 2];
    int length2 = fread(buffer, 1, length, f);
    fclose(f);
    buffer[length] = '\0';
    std::string str1((char*) buffer);
    //std::cout << str1 << "\n";

    return CreateShader(eShaderType, str1);
}

GLuint CreateProgram(const std::vector<GLuint> &shaderList)
{
    GLuint program = glCreateProgram();

    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
	glAttachShader(program, shaderList[iLoop]);

    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
	GLint infoLogLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

	GLchar *strInfoLog = new GLchar[infoLogLength + 1];
	glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
	fprintf(stderr, "Linker failure: %s\n", strInfoLog);
	delete[] strInfoLog;
    }

    return program;
}

/**
 * Converts integer to string
 */
string intToStr(const int x)
{
    stringstream ret;
    ret << x;
    return ret.str();
}

/**
 * Converts string to integer
 */
int strToInt(const string& s, bool& success)
{
    int ret;
    istringstream stream(s);

    success = (stream >> ret);
    return ret;
}