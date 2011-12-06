
#include "LineStripNode.h"

GLuint LineStripNode::m_vertexBufferObject = 0;
GLuint LineStripNode::m_program = 0;
GLint LineStripNode::m_PVMmatrixLoc = -1;
GLint LineStripNode::m_posLoc = -1;
GLint LineStripNode::m_colLoc = -1;

static const float vertexData[] = {
    -0.5, 0, 0.5,
    2.75, 0, -0.25,
    0.25, 0, -2.75,
    3.5, 0, -3.5,
};

/// Init shaders
LineStripNode::LineStripNode(const char * name, SceneNode * parent) :
SceneNode(name, parent)
{
    if(m_program == 0)
    {
	std::vector<GLuint> shaderList;

	// Push vertex shader and fragment shader
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "LineStripNode.vert"));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "LineStripNode.frag"));

	// Create the program with two shaders
	m_program = CreateProgram(shaderList);
	m_PVMmatrixLoc = glGetUniformLocation(m_program, "PVMmatrix");
	m_posLoc = glGetAttribLocation(m_program, "position");
    }
    if(m_vertexBufferObject == 0)
    {
	glGenBuffers(1, &m_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

/// Draw LINE_STRIP
void LineStripNode::draw(SceneParams * scene_params)
{
    // inherited draw - draws all children
    SceneNode::draw(scene_params);

    glm::mat4 matrix = scene_params->projection_mat * scene_params->view_mat * globalMatrix();

    glUseProgram(m_program);
    glUniformMatrix4fv(m_PVMmatrixLoc, 1, GL_FALSE, glm::value_ptr(matrix));

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glEnableVertexAttribArray(m_posLoc);
    //    glEnableVertexAttribArray(m_colLoc);
    // vertices of triangles
    glVertexAttribPointer(m_posLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // 8 = 4 + 4 floats per vertex - color
    //    glVertexAttribPointer(m_colLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*) (4 * sizeof(vertexData) / (8)));

    glDrawArrays(GL_LINE_STRIP, 0, sizeof(vertexData) / (3 * sizeof(float))); // 8 = 4+4 floats per vertex

    glDisableVertexAttribArray(m_posLoc);
    //    glDisableVertexAttribArray(m_colLoc);
}
