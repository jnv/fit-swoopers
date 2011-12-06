#include <GL/glew.h>

#include "SkyNode.h"
static const float vertexData[] = {
    1, 1,
    -1, 1,
    1, -1,
    -1, -1,

    1, 1,
    0, 1,
    1, 0,
    0, 0
};


GLuint SkyNode::m_program = 0;
GLint SkyNode::m_PVMmatrixLoc = -1;
GLint SkyNode::m_posLoc = -1;
GLint SkyNode::m_moveLoc = -1;
GLint SkyNode::m_texCoordLoc = -1;
GLint SkyNode::m_colorSamplerLoc = -1;

SkyNode::SkyNode(const char * name, SceneNode * parent) : SceneNode(name, parent)
{

    m_move = 0;
    if(m_program == 0)
    {
	std::vector<GLuint> shaderList;

	// Push vertex shader and fragment shader
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "SkyNode.vert"));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "SkyNode.frag"));

	// Create the program with two shaders
	m_program = CreateProgram(shaderList);
	m_PVMmatrixLoc = glGetUniformLocation(m_program, "PVMmatrix");
	m_colorSamplerLoc = glGetUniformLocation(m_program, "colorMap");
	m_moveLoc = glGetUniformLocation(m_program, "texMove");
	m_posLoc = glGetAttribLocation(m_program, "position");
	m_texCoordLoc = glGetAttribLocation(m_program, "texCoord");
    }

    glGenBuffers(1, &m_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

SkyNode::~SkyNode()
{
}

bool SkyNode::loadTexture(const char* file)
{
    m_color_map = CreateTexture(file);

    if(m_color_map == 0)
    {
	return false;
    }


    glBindTexture(GL_TEXTURE_2D, m_color_map);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return true;
}

void SkyNode::draw(SceneParams *scene_params)
{
    SceneNode::draw(scene_params);

    m_move += 0.0001;

    glm::mat4 matrix = scene_params->projection_mat * scene_params->view_mat * globalMatrix();


    glUseProgram(m_program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_color_map);
    glUniformMatrix4fv(m_PVMmatrixLoc, 1, GL_FALSE, glm::value_ptr(matrix));
    glUniform1f(m_moveLoc, m_move);

    glUniform1i(m_colorSamplerLoc, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glEnableVertexAttribArray(m_posLoc);
    glEnableVertexAttribArray(m_texCoordLoc);
    // vertices of triangles
    glVertexAttribPointer(m_posLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(m_texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*) (4 * 2 * sizeof(float)));

    //glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(vertexData) / (3 * sizeof(float))); // 3 floats per vertex
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(m_posLoc);
    glDisableVertexAttribArray(m_texCoordLoc);

    //std::clog << m_move << std::endl;
}