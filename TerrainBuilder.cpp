/* 
 * File:   TerrainBuilder.cpp
 * Author: j
 * 
 * Created on 1. prosinec 2011, 23:07
 */

#include "TerrainBuilder.h"
#include "PyramidNode.h"

TerrainBuilder::TerrainBuilder()
{
}

TerrainBuilder::~TerrainBuilder()
{
}

SceneNode * TerrainBuilder::prepareNode(SceneNode * parent, const char * hmapfile, const char * cmapfile, const char * nmapfile)
{
    m_terrainParent = new TransformNode("terrain-trans", parent);
    m_terrainParent->translate(glm::vec3(-0.5, -0.5, -2.0));

    m_terrainNode = new TerrainNode("terrain", m_terrainParent);
    m_terrainNode->load(hmapfile, cmapfile, nmapfile);

    return m_terrainParent;
}

bool TerrainBuilder::loadObjects(const char* mapfile)
{
    ILuint img_id;
    ilGenImages(1, &img_id); // generate one image ID (name)
    ilBindImage(img_id); // bind that generated id

    // set origin to LOWER LEFT corner (the orientation which OpenGL uses)
    ilEnable(IL_ORIGIN_SET);
    ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);

    // this will load image data to the currently bound image
    if(ilLoadImage(mapfile) == IL_FALSE)
    {
	ilDeleteImages(1, &img_id);
	cout << "cannot load image \"" << mapfile << "\" as an objects map" << endl;
	return 0;
    }

    // if the image was correctly loaded, we can obtain some informatins about our image
    ILint w = ilGetInteger(IL_IMAGE_WIDTH);
    ILint h = ilGetInteger(IL_IMAGE_HEIGHT);
    ILenum f = ilGetInteger(IL_IMAGE_FORMAT);
    printf("loaded %i x %i image\n", w, h);
    // there are many possible image formats and data types
    // we will convert all image types to RGB or RGBA format, with one byte per channel
    unsigned uBpp = (f == IL_RGBA || f == IL_BGRA ? 4 : 3);
    unsigned char * data = new unsigned char[w * h * uBpp];
    // thiw will convert image to RGB or RGBA, one byte per channel and store data to our array
    ilCopyPixels(0, 0, 0, w, h, 1, uBpp == 4 ? IL_RGBA : IL_RGB, IL_UNSIGNED_BYTE, data);

    ILubyte * pixels = ilGetData();
    int size = w * h * uBpp;

    int Bpp = (int)uBpp;

    for(int row = 0; row < h * Bpp; row+=Bpp)
    {
	for(int col = 0; col < w * Bpp; col+=Bpp)
	{
	    int offset = row * w + col;

	    if(pixels[offset] > 0 && pixels[offset] < 0xff)
	    {
		cout << "Found pixel " << hex << (int) pixels[offset] << dec << " at " << offset;
		int x = col / Bpp;
		int y = row / Bpp;
		cout << " " << x << "x" << y << endl;

		addPyramid(x, y);
	    }
	}
    }

    ilDeleteImages(1, &img_id);
}

void TerrainBuilder::placeObjects()
{
    
}

void TerrainBuilder::addPyramid(const int x, const int y)
{
    TransformNode * t = new TransformNode("pyra-trans", m_terrainParent);
    TransformNode * ts = new TransformNode("pyra-trans-scale", t);
    int tWidth = m_terrainNode->getWidth();
    float newX = (float)x / (float)tWidth;
    float newY = (float)y / (float)tWidth;
    t->translate(newX, 0, -newY);
    ts->scale(glm::vec3(0.1, 0.1, 0.1));

    PyramidNode * p = new PyramidNode("pyra", ts);
}