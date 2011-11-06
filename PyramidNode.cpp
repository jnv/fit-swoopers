
#include "PyramidNode.h"
#include "util.h"

GLuint PyramidNode::m_vertexBufferObject = 0;
GLuint PyramidNode::m_program = 0;
GLint PyramidNode::m_PVMmatrixLoc = -1;
GLint PyramidNode::m_posLoc = -1;
GLint PyramidNode::m_colLoc = -1;

static const float vertexData[] = {
   // vertices for the base of the pyramid
   0.0f,  0.0f, 0.0f, 1.0,
   1.0f,	0.0f, 0.0f, 1.0,
   0.5f,	0.866f, 0.0f, 1.0,

   0.0f,  0.0f, 0.0f, 1.0,
   0.5f,	0.866f, 0.0f, 1.0,
   -0.5f,	0.866f, 0.0f, 1.0,

   0.0f,  0.0f, 0.0f, 1.0,
   -0.5f,	0.866f, 0.0f, 1.0,
   -1.0f,	0.0f, 0.0f, 1.0,

   0.0f,  0.0f, 0.0f, 1.0,
   -1.0f,	0.0f, 0.0f, 1.0,
   -0.5f,	-0.866f, 0.0f, 1.0,

   0.0f,  0.0f, 0.0f, 1.0,
   -0.5f,	-0.866f, 0.0f, 1.0,
   0.5f,	-0.866f, 0.0f, 1.0,

   0.0f,  0.0f, 0.0f, 1.0,
   0.5f,	-0.866f, 0.0f, 1.0,
   1.0f,	0.0f, 0.0f, 1.0,

   // vertices for the sides of the pyramid
   0.0f,  0.0f, 1.0f, 1.0,
   0.5f,	0.866f, 0.0f, 1.0,
   1.0f,	0.0f, 0.0f, 1.0,

   0.0f,  0.0f, 1.0f, 1.0,
   -0.5f,	0.866f, 0.0f, 1.0,
   0.5f,	0.866f, 0.0f, 1.0,

   0.0f,  0.0f, 1.0f, 1.0,
   -1.0f,	0.0f, 0.0f, 1.0,
   -0.5f,	0.866f, 0.0f, 1.0,

   0.0f,  0.0f, 1.0f, 1.0,
   -0.5f,	-0.866f, 0.0f, 1.0,
   -1.0f,	0.0f, 0.0f, 1.0,

   0.0f,  0.0f, 1.0f, 1.0,
   0.5f,	-0.866f, 0.0f, 1.0,
  -0.5f,	-0.866f, 0.0f, 1.0,


   0.0f,  0.0f, 1.0f, 1.0,
   1.0f,	0.0f, 0.0f, 1.0,
   0.5f,	-0.866f, 0.0f, 1.0,

   // color for the bottom of the pyramid
   1.0f,  1.0f, 1.0f, 1.0,
   1.0f,	0.0f, 0.0f, 1.0,
   1.0f,  1.0f, 0.0f, 1.0f,

   1.0f, 1.0f, 1.0f, 1.0f,
   1.0f, 1.0f, 0.0f, 1.0f,
   0.0f, 1.0f, 0.0f, 1.0f,

   1.0f, 1.0f, 1.0f, 1.0f,
   0.0f, 1.0f, 0.0f, 1.0f,
   0.0f, 1.0f, 1.0f, 1.0f,

   1.0f, 1.0f, 1.0f, 1.0f,
   0.0f, 1.0f, 1.0f, 1.0f,
   0.0f, 0.0f, 1.0f, 1.0f,

   1.0f, 1.0f, 1.0f, 1.0f,
   0.0f, 0.0f, 1.0f, 1.0f,
   1.0f, 0.0f, 1.0f, 1.0f,

   1.0f, 1.0f, 1.0f, 1.0f,
   1.0f, 0.0f, 1.0f, 1.0f,
   1.0f, 0.0f, 0.0f, 1.0f,

   // color for the sides of the pyramid
   1.0f,  1.0f, 1.0f, 1.0,
   1.0f,  1.0f, 0.0f, 1.0f,
   1.0f,	0.0f, 0.0f, 1.0,

   1.0f, 1.0f, 1.0f, 1.0f,
   0.0f, 1.0f, 0.0f, 1.0f,
   1.0f, 1.0f, 0.0f, 1.0f,

   1.0f, 1.0f, 1.0f, 1.0f,
   0.0f, 1.0f, 1.0f, 1.0f,
   0.0f, 1.0f, 0.0f, 1.0f,

   1.0f, 1.0f, 1.0f, 1.0f,
   0.0f, 0.0f, 1.0f, 1.0f,
   0.0f, 1.0f, 1.0f, 1.0f,

   1.0f, 1.0f, 1.0f, 1.0f,
   1.0f, 0.0f, 1.0f, 1.0f,
   0.0f, 0.0f, 1.0f, 1.0f,

   1.0f, 1.0f, 1.0f, 1.0f,
   1.0f, 0.0f, 0.0f, 1.0f,
   1.0f, 0.0f, 1.0f, 1.0f,
};


PyramidNode::PyramidNode(const char * name, SceneNode * parent):
  SceneNode(name, parent)
{
  if(m_program == 0)
  {
    std::vector<GLuint> shaderList;

    // Push vertex shader and fragment shader
    shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "PyramidNode.vert"));
    shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "PyramidNode.frag"));

    // Create the program with two shaders
    m_program = CreateProgram(shaderList);
    m_PVMmatrixLoc = glGetUniformLocation(m_program, "PVMmatrix");
    m_posLoc = glGetAttribLocation(m_program, "position");
    m_colLoc = glGetAttribLocation(m_program, "color");
  }

  if(m_vertexBufferObject == 0)
  {
    glGenBuffers(1, &m_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}

void PyramidNode::draw(SceneParams * scene_params)
{
  // inherited draw - draws all children
  SceneNode::draw(scene_params);

  Matrix4f matrix = scene_params->projection_mat * scene_params->view_mat * globalMatrix();

  glUseProgram(m_program);
  glUniformMatrix4fv(m_PVMmatrixLoc, 1, GL_FALSE, matrix);

  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
  glEnableVertexAttribArray(m_posLoc);
  glEnableVertexAttribArray(m_colLoc);
  // vertices of triangles
  glVertexAttribPointer(m_posLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
  // 8 = 4 + 4 floats per vertex - color
  glVertexAttribPointer(m_colLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*)(4*sizeof(vertexData)/(8)));

  glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexData)/(8*sizeof(float))); // 8 = 4+4 floats per vertex

  glDisableVertexAttribArray(m_posLoc);
  glDisableVertexAttribArray(m_colLoc);
}
