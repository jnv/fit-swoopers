#include "RockNode.h"


GLuint RockNode::m_program = 0;
GLint RockNode::m_MmatrixLoc = -1;
GLint RockNode::m_VmatrixLoc = -1;
GLint RockNode::m_PmatrixLoc = -1;
GLint RockNode::m_normalSamplerLoc = -1;
GLint RockNode::m_colorSamplerLoc = -1;
GLint RockNode::m_widthLoc = -1;

RockNode::RockNode(SceneNode* parent)
: MeshNode("data/rock.obj", parent, false)
{


    if(m_program == 0)
    {
	// load the shaders for the first time
	std::vector<GLuint> shaderList;

	// Push vertex shader and fragment shader
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "RockNode.vert"));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "RockNode.frag"));

	// Create the program with two shaders
	m_program = CreateProgram(shaderList);
	m_MmatrixLoc = glGetUniformLocation(m_program, "Mmatrix");
	m_VmatrixLoc = glGetUniformLocation(m_program, "Vmatrix");
	m_PmatrixLoc = glGetUniformLocation(m_program, "Pmatrix");
	m_normalSamplerLoc = glGetUniformLocation(m_program, "normalMap");
	m_colorSamplerLoc = glGetUniformLocation(m_program, "colorMap");
	m_posLoc = glGetAttribLocation(m_program, "position");
	m_colLoc = glGetAttribLocation(m_program, "color");
	m_norLoc = glGetAttribLocation(m_program, "normal");
	m_texCoordLoc = glGetAttribLocation(m_program, "texCoord");

	//	m_posLoc = glGetAttribLocation(m_program, "position");
	//	m_colLoc = glGetAttribLocation(m_program, "color");
	//	m_norLoc = glGetAttribLocation(m_program, "normal");
    }

    //glGenBuffers(1, &m_vertexBufferObject);

}

RockNode::~RockNode()
{
}

void RockNode::loadTexture()
{
    const char * textureFile = "data/rock-text.png";
    const char * normFile = "data/rock-norm.png";


    m_normal_map = CreateTexture(normFile);
    m_color_map = CreateTexture(textureFile);


    //glBindTexture(GL_TEXTURE_2D, m_color_map);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

void RockNode::draw(SceneParams* scene_params)
{

    SceneNode::draw(scene_params);

    glUseProgram(m_program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_normal_map);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_color_map);


    glUniformMatrix4fv(m_MmatrixLoc, 1, GL_FALSE, glm::value_ptr(globalMatrix()));
    glUniformMatrix4fv(m_VmatrixLoc, 1, GL_FALSE, glm::value_ptr(scene_params->view_mat));
    glUniformMatrix4fv(m_PmatrixLoc, 1, GL_FALSE, glm::value_ptr(scene_params->projection_mat));
    glUniform1i(m_normalSamplerLoc, 0);
    glUniform1i(m_colorSamplerLoc, 1);

    //    glUniform1i(m_widthLoc, m_width);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glEnableVertexAttribArray(m_posLoc);
    glEnableVertexAttribArray(m_colLoc);
    glEnableVertexAttribArray(m_norLoc);
    glEnableVertexAttribArray(m_texCoordLoc);
    glVertexAttribPointer(m_posLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(m_colLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*) (3 * sizeof(float) * m_nVertices));
    glVertexAttribPointer(m_norLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*) (7 * sizeof(float) * m_nVertices));
    glVertexAttribPointer(m_texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*) (10 * sizeof(float) * m_nVertices));

    glDrawArrays(GL_TRIANGLES, 0, m_nVertices);

    glDisableVertexAttribArray(m_posLoc);
    glDisableVertexAttribArray(m_colLoc);
    glDisableVertexAttribArray(m_norLoc);
    glDisableVertexAttribArray(m_texCoordLoc);
}
