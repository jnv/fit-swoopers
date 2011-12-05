#include "util.h"

#ifdef _UNICODE
#error "please compile without _UNICODE support (project properties -> Configuration Properties -> General -> Character Set -> Not Set"
#error "we have to disable wchar_t support due to file name errors"
#endif

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

GLuint CreateTexture(const char * filename)
{
    /*
    ILuint img_id;
    ilGenImages(1, &img_id); // generate one image ID (name)
    ilBindImage(img_id); // bind that generated id

    // set origin to LOWER LEFT corner (the orientation which OpenGL uses)
    ilEnable(IL_ORIGIN_SET);
    ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);

    // this will load image data to the currently bound image
    if(ilLoadImage(filename) == IL_FALSE)
    {
	ilDeleteImages(1, &img_id);
	printf("cannot load image \"%s\"\n", filename);
	return 0;
    }

    // if the image was correctly loaded, we can obtain some informatins about our image
    ILint w = ilGetInteger(IL_IMAGE_WIDTH);
    ILint h = ilGetInteger(IL_IMAGE_HEIGHT);
    ILenum f = ilGetInteger(IL_IMAGE_FORMAT);
    //printf("loaded %i x %i image\n", w, h);
    // there are many possible image formats and data types
    // we will convert all image types to RGB or RGBA format, with one byte per channel
    unsigned Bpp = (f == IL_RGBA || f == IL_BGRA ? 4 : 3);
    unsigned char * data = new unsigned char[w * h * Bpp];
    // thiw will convert image to RGB or RGBA, one byte per channel and store data to our array
    ilCopyPixels(0, 0, 0, w, h, 1, Bpp == 4 ? IL_RGBA : IL_RGB, IL_UNSIGNED_BYTE, data);

    // bogus ATI drivers may require this call to work with mipmaps
    //glEnable(GL_TEXTURE_2D);
     */

    TextureStruct * texture = LoadTextureData(filename);

    if(texture == NULL)
    {
	return 0;
    }

    GLuint texID;
    // generate and bind one texture
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    // set linear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // upload our image data to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->w, texture->h, 0, texture->Bpp == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, texture->data);
    // create mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // free our data (they were copied to OpenGL)
    delete texture;
    //ilDeleteImages(1, &img_id);

    return texID;
}

TextureStruct * LoadTextureData(const char * filename)
{

    ILuint img_id;
    ilGenImages(1, &img_id); // generate one image ID (name)
    ilBindImage(img_id); // bind that generated id

    // set origin to LOWER LEFT corner (the orientation which OpenGL uses)
    ilEnable(IL_ORIGIN_SET);
    ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);

    // this will load image data to the currently bound image
    if(ilLoadImage(filename) == IL_FALSE)
    {
	ilDeleteImages(1, &img_id);
	printf("cannot load image \"%s\"\n", filename);
	return 0;
    }

    // if the image was correctly loaded, we can obtain some informatins about our image
    ILint w = ilGetInteger(IL_IMAGE_WIDTH);
    ILint h = ilGetInteger(IL_IMAGE_HEIGHT);
    ILenum f = ilGetInteger(IL_IMAGE_FORMAT);
    //printf("loaded %i x %i image\n", w, h);
    // there are many possible image formats and data types
    // we will convert all image types to RGB or RGBA format, with one byte per channel
    unsigned Bpp = (f == IL_RGBA || f == IL_BGRA ? 4 : 3);
    unsigned char * data = new unsigned char[w * h * Bpp];
    // thiw will convert image to RGB or RGBA, one byte per channel and store data to our array
    ilCopyPixels(0, 0, 0, w, h, 1, Bpp == 4 ? IL_RGBA : IL_RGB, IL_UNSIGNED_BYTE, data);

    // bogus ATI drivers may require this call to work with mipmaps
    //glEnable(GL_TEXTURE_2D);

    TextureStruct * ret = new TextureStruct();

    ret->Bpp = Bpp;
    ret->w = w;
    ret->h = h;
    ret->size = w*h*Bpp;
    ret->data = data;
    
    ilDeleteImages(1, &img_id);

    return ret;
}

GLuint CreateNormalTexture(const char * filename)
{
    ILuint img_id;
    ilGenImages(1, &img_id); // generate one image ID (name)
    ilBindImage(img_id); // bind that generated id

    // set origin to LOWER LEFT corner (the orientation which OpenGL uses)
    ilEnable(IL_ORIGIN_SET);
    ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);

    // this will load image data to the currently bound image
    if(ilLoadImage(filename) == IL_FALSE)
    {
	ilDeleteImages(1, &img_id);
	printf("cannot load image \"%s\"\n", filename);
	return 0;
    }

    // if the image was correctly loaded, we can obtain some informatins about our image
    ILint w = ilGetInteger(IL_IMAGE_WIDTH);
    ILint h = ilGetInteger(IL_IMAGE_HEIGHT);
    ILenum f = ilGetInteger(IL_IMAGE_FORMAT);
    printf("loaded %i x %i image\n", w, h);
    // there are many possible image formats and data types
    // we will convert all image types to RGB or RGBA format, with one byte per channel
    unsigned Bpp = (f == IL_RGBA || f == IL_BGRA ? 4 : 3);
    unsigned char * data = new unsigned char[w * h * Bpp];

    // thiw will convert image to RGB or RGBA, one byte per channel and store data to our array
    ilCopyPixels(0, 0, 0, w, h, 1, Bpp == 4 ? IL_RGBA : IL_RGB, IL_UNSIGNED_BYTE, data);

    unsigned char * dataNormal = new unsigned char[w * h * Bpp];
    glm::vec3 vertex[9]; // for 8-neighborhood
    bool valid[9];
    const int xic[9] = {-1, 0, 1, 1, 1, 0, -1, -1, -1};
    const int yic[9] = {-1, -1, -1, 0, 1, 1, 1, 0, -1};
    float scaleheight = 1.0f;
    int cntvalid = 0;
    for(int yp = 0; yp < h; yp++)
    {
	for(int xp = 0; xp < w; xp++)
	{
	    glm::vec3 avgnormal(0, 0, 0);
	    int i = 0;
	    cntvalid = 0;
	    for(i = 0; i < 9; i++)
	    {
		valid[i] = false;
		int xi = xic[i];
		int yi = yic[i];
		vertex[i] = glm::vec3(0, 0, 0);
		int y = yp + yi;
		if((y < 0) || (y >= h))
		{
		    continue;
		}
		int x = xp + xi;
		if((x < 0) || (x >= w))
		{
		    continue;
		}
		// Compute the vertex for
		vertex[i] = glm::vec3((float) xi, (float) yi, scaleheight * (float) (data[Bpp * (w * y + x)]));
		valid[i] = true;
		cntvalid++;
	    } // for i
	    for(i = 0; i < 8; i++)
	    {
		int j = i % 8;
		int j2 = (i + 1) % 8;
		// vector product of two vectors
		if(valid[j] && valid[j2])
		{
		    glm::vec3 vv = vertex[j] * vertex[j2];
		    assert(fabs(glm::dot(vv, vertex[j])) < 1e-6);
		    assert(fabs(glm::dot(vv, vertex[j2])) < 1e-6);
		    avgnormal += glm::normalize(vv); // the same angle
		}
	    } // for j;
	    // compute normalized normal
	    avgnormal = glm::normalize(avgnormal);
	    // and store it to dataNormal
	    //std::cout << avgnormal.x << " " << avgnormal.y << " " << avgnormal.z << " | ";
	    i = Bpp * (w * yp + xp);
	    float kx = 128 + avgnormal.x * 128;
	    clampf(kx, 0, 255);
	    dataNormal[i] = (unsigned char) kx;
	    float ky = 128 + avgnormal.y * 128;
	    clampf(ky, 0, 255);
	    dataNormal[i + 1] = (unsigned char) ky;
	    float kz = 0.f;
	    if(avgnormal.z > 0) kz = avgnormal.z * 255;
	    clampf(kz, 0, 255);
	    dataNormal[i + 2] = (unsigned char) kz;
	    if(Bpp == 4)
		dataNormal[i + 3] = 255; // alpha channel
	} // for xp
	//std::cout << "============" << std::endl;
    } // for yp

    // bogus ATI drivers may require this call to work with mipmaps
    //glEnable(GL_TEXTURE_2D);

    GLuint texID;
    // generate and bind one texture
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    // set linear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // upload our image data to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, Bpp == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, dataNormal);
    // create mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // free our data (they were copied to OpenGL)
    delete [] data;
    delete [] dataNormal;
    ilDeleteImages(1, &img_id);

    return texID;
}

GLuint CreateHeightTexture(const char * filename, TextureStruct& texOut)
{
    ILuint img_id;
    ilGenImages(1, &img_id); // generate one image ID (name)
    ilBindImage(img_id); // bind that generated id

    // set origin to LOWER LEFT corner (the orientation which OpenGL uses)
    ilEnable(IL_ORIGIN_SET);
    ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);

    // this will load image data to the currently bound image
    if(ilLoadImage(filename) == IL_FALSE)
    {
	ilDeleteImages(1, &img_id);
	printf("cannot load image \"%s\"\n", filename);
	return 0;
    }

    // if the image was correctly loaded, we can obtain some informatins about our image
    ILint w = ilGetInteger(IL_IMAGE_WIDTH);
    ILint h = ilGetInteger(IL_IMAGE_HEIGHT);
    ILenum f = ilGetInteger(IL_IMAGE_FORMAT);
    printf("loaded %i x %i image\n", w, h);
    // there are many possible image formats and data types
    // we will convert all image types to RGB or RGBA format, with one byte per channel
    unsigned Bpp = (f == IL_RGBA || f == IL_BGRA ? 4 : 3);
    unsigned char * data = new unsigned char[w * h * Bpp];
    // thiw will convert image to RGB or RGBA, one byte per channel and store data to our array
    ilCopyPixels(0, 0, 0, w, h, 1, Bpp == 4 ? IL_RGBA : IL_RGB, IL_UNSIGNED_BYTE, data);

    // bogus ATI drivers may require this call to work with mipmaps
    //glEnable(GL_TEXTURE_2D);

    GLuint texID;
    // generate and bind one texture
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    // set linear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // upload our image data to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, Bpp == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    // create mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // free our data (they were copied to OpenGL)
    ilDeleteImages(1, &img_id);

    texOut.data = data;
    texOut.h = h;
    texOut.w = w;
    texOut.size = w * h * Bpp;
    texOut.Bpp = Bpp;

    return texID;
}