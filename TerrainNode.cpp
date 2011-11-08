#include "TerrainNode.h"

using namespace std;

GLuint TerrainNode::m_program = 0;
GLint TerrainNode::m_ModelMatrixLoc = -1;
GLint TerrainNode::m_MVPLoc = -1;
GLint TerrainNode::m_ViewMatrixLoc = -1;
GLint TerrainNode::m_posLoc = -1;
GLint TerrainNode::m_colLoc = -1;

TerrainNode::TerrainNode(const char *name, SceneNode *parent) :
SceneNode(name, parent),
m_vertexBufferObject(0),
vertexData(0),
m_nVertices(0),
m_nTriangles(0)
{
    if(m_program == 0)
    {
	// load the shaders for the first time
	std::vector<GLuint> shaderList;

	// Push vertex shader and fragment shader
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "TerrainNode.vert"));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "TerrainNode.frag"));

	// Create the program with two shaders
	m_program = CreateProgram(shaderList);
	m_ModelMatrixLoc = glGetUniformLocation(m_program, "M");
	m_ViewMatrixLoc = glGetUniformLocation(m_program, "V");
	m_MVPLoc = glGetUniformLocation(m_program, "MVP");
	m_posLoc = glGetAttribLocation(m_program, "position");
	m_colLoc = glGetAttribLocation(m_program, "color");
    }
}


/* Loads a terragen file as well as a texture for it,  */

/* interpretting it as a terrain. Resolution must be 513 x 513. */
int
TerrainNode::load(const char *baseFilename)
{
    long int size;
    FILE* rawFile;
    char file[256];

    // resolution of raw file -> 513x513
    int _resX = 513;
    int _resZ = 513;

    // distances between neighbour grid points along x, y, and z axis
    const float _deltaX = 1.0f / _resX;
    const float _deltaZ = 1.0f / _resZ;
    const float _deltaY = 1.0f / 256.0f; // height modulation

    // size of the grid
    const float _sizeX = 1.0f;
    const float _sizeZ = 1.0f;

    // origin of the grid - move the grid to the center of coordinate system
    const float _originX = -0.5f;
    const float _originZ = -0.5f;
    const float _originY = -0.5f;

    // the number of floats to read for heights
    size = 3L * _resX*_resZ;

    float *_vertices = 0;
    _vertices = new float[size];
    assert(_vertices);
    float *_colors = 0;
    _colors = new float[size];
    assert(_colors);

    // read the heights of terrain
    sprintf(file, "%s.raw", baseFilename);
    cout << "Loading terrain map file " << file << " [" << _resX << "x" << _resZ << "] ";
    cout.flush();

    rawFile = fopen(file, "rb");
    if(rawFile == NULL)
    {
	cerr << "CTerrain::Load(): Can't open input raw file" << file << endl;
	return -1;
    }

    typedef unsigned char BYTE;
    BYTE *buffer = new BYTE[3 * _resX]; // 2 bytes per one height, 3 bytes per one color
    assert(buffer);
    int counter;
    float height;
    BYTE low, high;

    // read a terragen file that contains grid heights
    for(int z = 0; z < _resZ; z++)
    {
	counter = 0;
	fread(buffer, 1, 2 * _resX, rawFile);

	for(int x = 0; x < _resX; x++)
	{
	    low = buffer[counter++]; // fread((char*)&low, 1, 1, rawFile);
	    high = buffer[counter++]; // fread((char*)&high, 1, 1, rawFile);
	    //height = high /0xFFL + low;
	    height = high;
	    // index for vertex in the array of floats
	    int i = (_resZ * z + x) * 3;
	    //cout << height * _deltaY << " ";

	    _vertices[i] = _originX + x*_deltaX;
	    _vertices[i + 1] = _originY + height * _deltaY;
	    _vertices[i + 2] = _originZ + z*_deltaZ;
	    _colors[i] = _colors[i + 1] = _colors[i + 2] = 0.5; // color is grey by default
	}
	cout << ".";
	cout.flush();
    }
    cout << endl;
    fclose(rawFile);

    // Read the texture with colors for vertices
    FILE* tgaFile;
    BYTE tmp[200];

    sprintf(file, "%s.tga", baseFilename);
    cout << "Loading terrain texture file " << file << " [" << _resX << "x" << _resZ << "] ";
    cout.flush();

    tgaFile = fopen(file, "rb");
    if(tgaFile == NULL)
    {
	cerr << "CTerrain::Load(): Can't open input tga file " << file << endl;
    }

    // read a targa (*.tga) file - contains a texture for terrain
    // skip header
    fread((char*) tmp, 18, 1, tgaFile);
    // read colors from file
    for(int z = 0; z < _resZ; z++)
    {
	// one line
	if(fread((void*) buffer, 1, 3 * _resX, tgaFile) != 3 * _resX)
	{
	    cout << "Problem reading TGA file" << endl;
	    exit(-1);
	}
	// index for vertex in the array of floats
	int i = (_resX * z) * 3;
	counter = 0;
	for(int x = 0; x < _resX; x++)
	{
	    _colors[i] = (float) (buffer[counter + 2]) / 255.0f; // red channel
	    _colors[i + 1] = (float) (buffer[counter + 1]) / 255.0f; // green channel
	    _colors[i + 2] = (float) (buffer[counter]) / 255.0f; // blue channel
	    counter += 3;
	    i += 3;
	}
	cout << ".";
	cout.flush();
    }
    cout << endl;
    fclose(tgaFile);
    delete [] buffer;

    // _resZ = 10;
    // _resX = 10;
    // Now create the vertices and colors for triangles by triangle strip
    // 2 triangles per cell
    // 3 vertices per triangle
    // 4 floats per vertex
    // (_resX-1) * (_resZ-1) is the number of cells
    int sizeV = 2 * 3 * 4 * (_resX - 1) * (_resZ - 1); // the size for vertices only
    vertexData = new float[2 * sizeV]; // for vertices and colors
    assert(vertexData);

    // set the global variables
    m_nVertices = sizeV / 4;
    m_nTriangles = sizeV / (3 * 4);

    // copy the data for heights and vertices for OpenGL bufferObject
    int iv = 0; // index for vertices
    int ic = sizeV; // index for vertices

    const int SV = 3;
    for(int iz = 0; iz < _resZ - 1; iz++)
    {
	for(int ix = 0; ix < _resX - 1; ix++)
	{
	    // each cell has 2 triangles - geometry + color
	    // geometry vertices for the first triangle, the first vertex
	    vertexData[iv++] = _vertices[(_resZ * iz + ix) * SV]; // x
	    vertexData[iv++] = _vertices[(_resZ * iz + ix) * SV + 1]; // y
	    vertexData[iv++] = _vertices[(_resZ * iz + ix) * SV + 2]; // z
	    vertexData[iv++] = 1.0f;
	    // the second vertex
	    vertexData[iv++] = _vertices[(_resZ * iz + ix + 1) * SV]; // x
	    vertexData[iv++] = _vertices[(_resZ * iz + ix + 1) * SV + 1]; // y
	    vertexData[iv++] = _vertices[(_resZ * iz + ix + 1) * SV + 2]; // z
	    vertexData[iv++] = 1.0f;
	    // the third vertex
	    vertexData[iv++] = _vertices[(_resZ * (iz + 1) + ix) * SV]; // x
	    vertexData[iv++] = _vertices[(_resZ * (iz + 1) + ix) * SV + 1]; // y
	    vertexData[iv++] = _vertices[(_resZ * (iz + 1) + ix) * SV + 2]; // z
	    vertexData[iv++] = 1.0f;

	    // geometry vertices for the second triangle, the first vertex
	    vertexData[iv++] = _vertices[(_resZ * (iz + 1) + ix) * SV]; // x
	    vertexData[iv++] = _vertices[(_resZ * (iz + 1) + ix) * SV + 1]; // y
	    vertexData[iv++] = _vertices[(_resZ * (iz + 1) + ix) * SV + 2]; // z
	    vertexData[iv++] = 1.0f;
	    // the second vertex
	    vertexData[iv++] = _vertices[(_resZ * iz + ix + 1) * SV]; // x
	    vertexData[iv++] = _vertices[(_resZ * iz + ix + 1) * SV + 1]; // y
	    vertexData[iv++] = _vertices[(_resZ * iz + ix + 1) * SV + 2]; // z
	    vertexData[iv++] = 1.0f;
	    // the third vertex
	    vertexData[iv++] = _vertices[(_resZ * (iz + 1) + ix + 1) * SV]; // x
	    vertexData[iv++] = _vertices[(_resZ * (iz + 1) + ix + 1) * SV + 1]; // y
	    vertexData[iv++] = _vertices[(_resZ * (iz + 1) + ix + 1) * SV + 2]; // z
	    vertexData[iv++] = 1.0f;

	    assert(ic < 2 * sizeV);
	    // colors for the first triangle, the first vertex
	    vertexData[ic++] = _colors[(_resZ * iz + ix) * SV]; // x
	    vertexData[ic++] = _colors[(_resZ * iz + ix) * SV + 1]; // y
	    vertexData[ic++] = _colors[(_resZ * iz + ix) * SV + 2]; // z
	    vertexData[ic++] = 0.0f;
	    // the second vertex
	    vertexData[ic++] = _colors[(_resZ * iz + ix + 1) * SV]; // x
	    vertexData[ic++] = _colors[(_resZ * iz + ix + 1) * SV + 1]; // y
	    vertexData[ic++] = _colors[(_resZ * iz + ix + 1) * SV + 2]; // z
	    vertexData[ic++] = 0.0f;
	    // the third vertex
	    vertexData[ic++] = _colors[(_resZ * (iz + 1) + ix) * SV]; // x
	    vertexData[ic++] = _colors[(_resZ * (iz + 1) + ix) * SV + 1]; // y
	    vertexData[ic++] = _colors[(_resZ * (iz + 1) + ix) * SV + 2]; // z
	    vertexData[ic++] = 0.0f;

	    // colors for the second triangle, the first vertex
	    vertexData[ic++] = _colors[(_resZ * (iz + 1) + ix) * SV]; // x
	    vertexData[ic++] = _colors[(_resZ * (iz + 1) + ix) * SV + 1]; // y
	    vertexData[ic++] = _colors[(_resZ * (iz + 1) + ix) * SV + 2]; // z
	    vertexData[ic++] = 0.0f;
	    // the second vertex
	    vertexData[ic++] = _colors[(_resZ * iz + ix + 1) * SV]; // x
	    vertexData[ic++] = _colors[(_resZ * iz + ix + 1) * SV + 1]; // y
	    vertexData[ic++] = _colors[(_resZ * iz + ix + 1) * SV + 2]; // z
	    vertexData[ic++] = 0.0f;
	    // the third vertex
	    vertexData[ic++] = _colors[(_resZ * (iz + 1) + ix + 1) * SV]; // x
	    vertexData[ic++] = _colors[(_resZ * (iz + 1) + ix + 1) * SV + 1]; // y
	    vertexData[ic++] = _colors[(_resZ * (iz + 1) + ix + 1) * SV + 2]; // z
	    vertexData[ic++] = 0.0f;
	} // for ix
    } // for iz

    delete [] _vertices;
    delete [] _colors;

    if(m_vertexBufferObject == 0)
    {
	glGenBuffers(1, &m_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 8 * m_nVertices * sizeof(float), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    return false; // ok
}

void TerrainNode::draw(SceneParams * scene_params)
{
    // inherited draw - draws all children
    SceneNode::draw(scene_params);

    glm::mat4 MVP = scene_params->projection_mat * scene_params->view_mat * globalMatrix();

    glUseProgram(m_program);
    glUniformMatrix4fv(m_ModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(globalMatrix()));
    glUniformMatrix4fv(m_ViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(scene_params->view_mat));
    glUniformMatrix4fv(m_MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

    LightManager* lm = LightManager::getInstance();
    lm->uniformAmbient(m_program);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glEnableVertexAttribArray(m_posLoc);
    glEnableVertexAttribArray(m_colLoc);
    // vertices of triangles
    glVertexAttribPointer(m_posLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    // 8 = 4 + 4 floats per vertex - color
    glVertexAttribPointer(m_colLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*) (4 * m_nVertices * sizeof(float)));
    glDrawArrays(GL_TRIANGLES, 0, m_nVertices); // 8 = 4+4 floats per vertex

    glDisableVertexAttribArray(m_posLoc);
    glDisableVertexAttribArray(m_colLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
