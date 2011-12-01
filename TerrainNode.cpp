#include "TerrainNode.h"

using namespace std;

GLuint TerrainNode::m_program = 0;
GLint TerrainNode::m_MmatrixLoc = -1;
GLint TerrainNode::m_VmatrixLoc = -1;
GLint TerrainNode::m_PmatrixLoc = -1;
GLint TerrainNode::m_texSamplerLoc = -1;
GLint TerrainNode::m_normalSamplerLoc = -1;
GLint TerrainNode::m_colorSamplerLoc = -1;
GLint TerrainNode::m_widthLoc = -1;
GLint TerrainNode::m_heightCoefLoc = -1;

TerrainNode::TerrainNode(const char *name, SceneNode *parent) :
SceneNode(name, parent),
m_height_map(0),
m_width(0),
m_nVertices(0)
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
	m_MmatrixLoc = glGetUniformLocation(m_program, "Mmatrix");
	m_VmatrixLoc = glGetUniformLocation(m_program, "Vmatrix");
	m_PmatrixLoc = glGetUniformLocation(m_program, "Pmatrix");
	m_texSamplerLoc = glGetUniformLocation(m_program, "heightMap");
	m_normalSamplerLoc = glGetUniformLocation(m_program, "normalMap");
	m_colorSamplerLoc = glGetUniformLocation(m_program, "colorMap");
	m_widthLoc = glGetUniformLocation(m_program, "imgWidth");
	m_heightCoefLoc = glGetUniformLocation(m_program, "heightCoef");
    }
}

/* Loads a terragen file as well as a texture for it,  */

/* interpretting it as a terrain. Resolution must be 513 x 513. */
int TerrainNode::load(const char * hmap, const char * cmap, const char * nmap)
{
    m_height_map = CreateHeightTexture(hmap, m_rawHeightMap);
    cout << "TerrainNode m_rawHeightMap: " << m_rawHeightMap.size << endl;
    if(nmap != NULL)
	m_normal_map = CreateTexture(nmap);
    else
	m_normal_map = CreateNormalTexture(hmap);
    m_color_map = CreateTexture(cmap);

    if(m_height_map == 0 || m_normal_map == 0 || m_color_map == 0)
	return false;

    glBindTexture(GL_TEXTURE_2D, m_height_map);
    // clamp to edge
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // query loaded texture size
    GLint w = 0, h = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &h);
    if(w != h || w < 2)
    {
	printf("please use just NxN image for heightmap (where N >= 2)");
	return false;
    }
    m_nVertices = (w - 1) * (h - 1) * 6;
    m_width = w;

    return true; // ok
}

void TerrainNode::draw(SceneParams * scene_params)
{
    // inherited draw - draws all children
    SceneNode::draw(scene_params);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_height_map);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normal_map);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_color_map);
    
    glUseProgram(m_program);

    glUniformMatrix4fv(m_MmatrixLoc, 1, GL_FALSE, glm::value_ptr(globalMatrix()));
    glUniformMatrix4fv(m_VmatrixLoc, 1, GL_FALSE, glm::value_ptr(scene_params->view_mat));
    glUniformMatrix4fv(m_PmatrixLoc, 1, GL_FALSE, glm::value_ptr(scene_params->projection_mat));
    glUniform1i(m_texSamplerLoc, 0);
    glUniform1i(m_normalSamplerLoc, 1);
    glUniform1i(m_colorSamplerLoc, 2);
    glUniform1i(m_widthLoc, m_width);
    glUniform1f(m_heightCoefLoc, 11.0f); //XXX Height coeficient
    // we dont use any attrib buffer, all information is encoded in textures
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, m_nVertices);
}
