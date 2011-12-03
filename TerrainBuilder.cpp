/* 
 * File:   TerrainBuilder.cpp
 * Author: j
 * 
 * Created on 1. prosinec 2011, 23:07
 */

#include "TerrainBuilder.h"
#include "PyramidNode.h"
#include "TerragenNode.h"

TerrainBuilder::TerrainBuilder()
{
}

TerrainBuilder::~TerrainBuilder()
{
}

SceneNode * TerrainBuilder::prepareNode(SceneNode * parent, const char * hmapfile, const char * cmapfile, const char * nmapfile)
{
    m_terrainParent = new TransformNode("terrain-trans", parent);
    //m_terrainParent->scale(glm::vec3(2.0, 1.0, 2.0));

    //m_terrainNode = new TerrainNode("terrain", m_terrainParent);
    //m_terrainNode->load(hmapfile, cmapfile, nmapfile);
    m_terrainNode = new TerragenNode("terrain", m_terrainParent);

    float scale = Config::getInstance()->getFloat("terrain_scale");
    m_terrainNode->load("data/desert2", scale);

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

    int Bpp = (int) uBpp;

    for(int row = 0; row < h * Bpp; row += Bpp)
    {
	for(int col = 0; col < w * Bpp; col += Bpp)
	{
	    int offset = row * w + col;

	    if(pixels[offset] > 0 && pixels[offset] < 0xff)
	    {
		cout << "Found pixel " << hex << (int) pixels[offset] << dec << " at " << offset << endl;
		int x = col / Bpp;
		int y = row / Bpp;

		switch(pixels[offset])
		{
		case 0xcc:
		    cout << "Adding Swoop at" << x << "x" << y << endl;
		    addSwoop(x, y);
		    break;
		default:
		    cout << "Pyramid at " << x << "x" << y << endl;
		    addPyramid(x, y);
		}
	    }
	}
    }

    ilDeleteImages(1, &img_id);

    return true;
}

void TerrainBuilder::placeObjects()
{
    TransformNode * t = new TransformNode("strip-trans", m_terrainParent);
    int tWidth = m_terrainNode->getWidth();
    float scale = m_terrainNode->getScale();
    //t->translate(newX, 0, -newY);
    float objScale = 1.0 / tWidth;
    t->translate(0.0, -0.1, 0.0);
//    t->scale(glm::vec3(objScale, objScale, objScale));
    new LineStripNode("strip", t);
}

void TerrainBuilder::addPyramid(const int x, const int y)
{
    TransformNode * t = new TransformNode("pyra-trans", m_terrainParent);
    TransformNode * ts = new TransformNode("pyra-trans-scale", t);
    int tWidth = m_terrainNode->getWidth();
    float scale = m_terrainNode->getScale();
    float newX = ((float) x * scale) / (float) tWidth;
    float newY = ((float) y * scale) / (float) tWidth;
    t->translate(newX, 0, -newY);
    ts->scale(glm::vec3(0.1, 0.1, 0.1));

    CameraManager::getInstance()->createCamera("pyraCam", ts);

    PyramidNode * p = new PyramidNode("pyra", ts);
}

void TerrainBuilder::addSwoop(const int x, const int y)
{
    string model = Config::getInstance()->getString("swoop_model");
    float swoopY = Config::getInstance()->getFloat("swoop_elevation");

    int tWidth = m_terrainNode->getWidth();
    float scale = m_terrainNode->getScale();
    float newX = ((float) x * scale) / (float) tWidth;
    float newY = ((float) y * scale) / (float) tWidth;

    std::clog << "Placing swoop at height " << swoopY << std::endl;

    TransformNode * strans = SwoopManager::Initialize(model.c_str());
    strans->setParentNode(m_terrainParent);
    strans->translate(newX, swoopY, -newY);

    CameraManager::getInstance()->createCamera("swoop_cam", strans);
}