#include "CollidableNode.h"

GLuint CollidableNode::m_program = 0;
GLint CollidableNode::m_PVMmatrixLoc = -1;
GLint CollidableNode::m_posLoc = -1;
bool CollidableNode::m_draw = false;

/// Prepares shader for debug drawing
CollidableNode::CollidableNode(const char * name, MeshNode * parent) : SceneNode(name, parent)
{
    m_vertices = parent->getBoxVertices();

    if(m_program == 0)
    {
	std::vector<GLuint> shaderList;

	// Push vertex shader and fragment shader
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "CollidableNode.vert"));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "CollidableNode.frag"));

	// Create the program with two shaders
	m_program = CreateProgram(shaderList);
	m_PVMmatrixLoc = glGetUniformLocation(m_program, "PVM");
	m_posLoc = glGetAttribLocation(m_program, "inPosition");

    }

    glGenBuffers(1, &m_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 8, m_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

CollidableNode::~CollidableNode()
{
}

/// Draws bounding "box" (rather say... vertices) if m_draw is enabled
void CollidableNode::draw(SceneParams * scene_params)
{
    // inherited draw - draws all children
    SceneNode::draw(scene_params);

    if(!CollidableNode::m_draw)
    {
	return;
    }

    glm::mat4 matrix = scene_params->projection_mat * scene_params->view_mat * globalMatrix();

    glUseProgram(m_program);
    glUniformMatrix4fv(m_PVMmatrixLoc, 1, GL_FALSE, glm::value_ptr(matrix));

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glEnableVertexAttribArray(m_posLoc);
    // vertices of triangles
    glVertexAttribPointer(m_posLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINE_LOOP, 0, 8); // 8 = 4+4 floats per vertex

    glDisableVertexAttribArray(m_posLoc);
}

/// Inverts state of m_draw
void CollidableNode::SwitchDraw()
{
    CollidableNode::m_draw = !CollidableNode::m_draw;
}

/// Sets m_draw
void CollidableNode::SetDraw(const bool value)
{
    CollidableNode::m_draw = value;
}

/**
 * Returns the vertex of given index in world space coords
 * @param index
 * @return vertex position
 */
glm::vec3 CollidableNode::getGlobalVertex(const int index) const
{
    glm::vec4 result = globalMatrix() * glm::vec4(m_vertices[index], 1.0);

    return glm::vec3(result);
}

/**
 * Get just X and Z of vertex of given index (we don't check for Y in collision testing
 * @param index
 * @return vertex position in world space coords
 */
glm::vec2 CollidableNode::getGlobalVertex2D(const int index) const
{
    glm::vec4 result = globalMatrix() * glm::vec4(m_vertices[index], 1.0);

    return glm::vec2(result.x, result.z);
}

