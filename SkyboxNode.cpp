#include "SkyboxNode.h"

GLuint SkyboxNode::m_vertexBufferObject = 0;
GLuint SkyboxNode::m_program = 0;
GLint SkyboxNode::m_PVMmatrixLoc = -1;
GLint SkyboxNode::m_posLoc = -1;
GLint SkyboxNode::m_colLoc = -1;
GLuint SkyboxNode::m_color_map = -1;

/// Cube vertices. Based on http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/
static const float vertexData[] = {
    -1.0f, -1.0f, -1.0f, // triangle 1 : begin
    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f, -1.0f, // triangle 2 : begin
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f, // triangle 2 : end
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f
};

SkyboxNode::SkyboxNode(const char * name, SceneNode * parent) : SceneNode(name, parent)
{
    if(m_program == 0)
    {
	std::vector<GLuint> shaderList;

	// Push vertex shader and fragment shader
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "SkyboxNode.vert"));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "SkyboxNode.frag"));

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

SkyboxNode::~SkyboxNode()
{
    glDeleteTextures(1, &m_color_map);
}

void SkyboxNode::draw(SceneParams* scene_params)
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
    //glVertexAttribPointer(m_colLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*) (4 * sizeof(vertexData) / (8)));

    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexData) / (3 * sizeof(float))); // 3 floats per vertex

    glDisableVertexAttribArray(m_posLoc);
    //    glDisableVertexAttribArray(m_colLoc);
}

/**
 * Based on OpenGL SuperBible 5th edition
 * @param prefix
 */
void SkyboxNode::loadTexture(const char* prefix)
{
    std::string faces[6] = {"pos_x", "neg_x", "pos_y", "neg_y", "pos_z", "neg_z"};

    GLenum cube[6] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};


    glGenTextures(1, &m_color_map);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_color_map);

    // Set up texture maps
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(int i = 0; i < 6; i++)
    {
	std::string file = prefix + faces[i] + ".jpg";
	TextureStruct * texture = LoadTextureData(file.c_str());

	glTexImage2D(cube[i], 0, GL_RGBA, texture->w, texture->h, 0, texture->Bpp == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, texture->data);

	delete texture;
    }
    

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
}