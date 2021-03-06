#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>

#include <string.h>
#include <bitset>

#include "MeshNode.h"

GLuint MeshNode::m_program = 0;
GLint MeshNode::m_ModelMatrixLoc = -1;
GLint MeshNode::m_ViewMatrixLoc = -1;
GLint MeshNode::m_MVPLoc = -1;
GLint MeshNode::m_posLoc = -1;
GLint MeshNode::m_colLoc = -1;
GLint MeshNode::m_norLoc = -1;
GLint MeshNode::m_texCoordLoc = -1;
GLint MeshNode::m_hasTexLoc = -1;
GLint MeshNode::m_normalSamplerLoc = -1;
GLint MeshNode::m_colorSamplerLoc = -1;


/// Helper debug function to traverse Assimp aiNodes
void traverse_node(aiNode * node)
{
    std::cout << "node: " << node->mName.data << " ch: " << node->mNumChildren << " meshes: " << node->mNumMeshes << std::endl;

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
	traverse_node(node->mChildren[i]);
    }

}

/**
 * Load shaders and get variable locations
 * @param file_name file for mesh
 * @param parent parent node
 * @param buildShaders whether the shaders should be built - could be used e.g. by subclasses
 */
MeshNode::MeshNode(const char* file_name, SceneNode* parent, const bool buildShaders) :
SceneNode(file_name, parent), m_vertexBufferObject(0), m_nVertices(0)
{
    m_hasTexture = false;
    if((m_program == 0) && buildShaders)
    {
	std::vector<GLuint> shaderList;

	// Push vertex shader and fragment shader
	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "MeshNode.vert"));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "MeshNode.frag"));

	// Create the program with two shaders
	m_program = CreateProgram(shaderList);
	m_ModelMatrixLoc = glGetUniformLocation(m_program, "M");
	m_ViewMatrixLoc = glGetUniformLocation(m_program, "V");
	m_MVPLoc = glGetUniformLocation(m_program, "MVP");
	m_hasTexLoc = glGetUniformLocation(m_program, "hasTexture");

	m_posLoc = glGetAttribLocation(m_program, "position");
	m_colLoc = glGetAttribLocation(m_program, "color");
	m_norLoc = glGetAttribLocation(m_program, "normal");
	m_texCoordLoc = glGetAttribLocation(m_program, "texCoord");
	m_normalSamplerLoc = glGetUniformLocation(m_program, "normalMap");
	m_colorSamplerLoc = glGetUniformLocation(m_program, "colorMap");
    }

    glGenBuffers(1, &m_vertexBufferObject);
}

/// Delete VBO
MeshNode::~MeshNode()
{
    glDeleteBuffers(1, &m_vertexBufferObject);
}

/**
 * Loads mesh using Assimp
 * @return true on success
 */
bool MeshNode::loadMesh()
{
    Assimp::Importer imp;
    int i;
    for(i = 0; i < 3; i++) minbox[i] = 1e38;
    for(i = 0; i < 3; i++) maxbox[i] = -1e38;

    const aiScene * scn = imp.ReadFile(nodeName().c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_GenUVCoords);
    //aiProcess_ImproveCacheLocality | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph

    if(!scn)
    {
	std::cerr << imp.GetErrorString() << std::endl;
	return false;
    }

    if(scn->mNumMeshes < 1)
    {
	std::cerr << "no meshes found in scene " << nodeName() << std::endl;
	return false;
    }

    std::cout << "loaded " << scn->mNumMeshes << " meshes" << std::endl;

    // merge all sub-meshes to one big mesh
    m_nVertices = 0;
    for(unsigned m = 0; m < scn->mNumMeshes; ++m)
	m_nVertices += scn->mMeshes[m]->mNumVertices;

    float * vertices = new float[12 * m_nVertices]; // 12 floats per vertex (xyz + RGBA + nor + texc)
    float * cur_vert = vertices;
    float * cur_col = vertices + 3 * m_nVertices;
    float * cur_nor = vertices + 7 * m_nVertices;
    float * cur_texc = vertices + 10 * m_nVertices;

    //float * texCoords = new float[2 * m_nVertices];

    for(unsigned m = 0; m < scn->mNumMeshes; ++m)
    {
	aiMesh * mesh = scn->mMeshes[m];
	memcpy(cur_vert, mesh->mVertices, mesh->mNumVertices * sizeof(float) * 3);
	memcpy(cur_nor, mesh->mNormals, mesh->mNumVertices * sizeof(float) * 3);


	//std::cout << "mesh: " << mesh->mName.data << std::endl;

	aiMaterial * material = scn->mMaterials[mesh->mMaterialIndex];
	bool hasTexCoords = mesh->HasTextureCoords(0);
	// copy mesh material color to all mesh vertices
	for(unsigned v = 0; v < mesh->mNumVertices; ++v)
	{
	    aiColor4D color;
	    material->Get<aiColor4D > (AI_MATKEY_COLOR_DIFFUSE, color);
	    *cur_col++ = color.r;
	    *cur_col++ = color.g;
	    *cur_col++ = color.b;
	    *cur_col++ = color.a;


	    // extend bounding box
	    for(int j = 0; j < 3; j++)
	    {
		if(cur_vert[3 * v + j] < minbox[j])
		    minbox[j] = cur_vert[3 * v + j];
		if(cur_vert[3 * v + j] > maxbox[j])
		    maxbox[j] = cur_vert[3 * v + j];
	    }

	    if(hasTexCoords)
	    {
		*cur_texc++ = mesh->mTextureCoords[0][v].x;
		*cur_texc++ = mesh->mTextureCoords[0][v].y;
		//		std::cout << mesh->mTextureCoords[0][v].x << " - " << mesh->mTextureCoords[0][v].y << std::endl;
	    }
	    else
	    {
		*cur_texc++ = 0.0;
		*cur_texc++ = 0.0;
	    }

	}

	cur_vert += mesh->mNumVertices * 3;
	cur_nor += mesh->mNumVertices * 3;
	//cur_texc += mesh->mNumVertices * 2;
    }

    //aiNode * root = scn->mRootNode;
    //    size_t length = root->mName.length;
    //traverse_node(root);
    //std::cout << "node: " << root->mName.data << std::endl;

    //    std::cout << "------------------" << std::endl;
    //    for(unsigned i = 10 * m_nVertices; i < 12 * m_nVertices; i+=2)
    //    {
    //	std::cout << vertices[i] << " - " << vertices[i + 1] << std::endl;
    //    }


    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, m_nVertices * sizeof(float) * 12, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    delete [] vertices;

    constructBox();

    return true;
}

/**
 * Draws a mesh
 * @param scene_params
 */
void MeshNode::draw(SceneParams * scene_params)
{
    // inherited draw - draws all children
    SceneNode::draw(scene_params);

    glm::mat4 MVP = scene_params->projection_mat * scene_params->view_mat * globalMatrix();

    glUseProgram(m_program);

    if(m_hasTexture)
    {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_normal_map);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_color_map);

	glUniform1i(m_normalSamplerLoc, 0);
	glUniform1i(m_colorSamplerLoc, 1);
    }

    glUniform1i(m_hasTexLoc, m_hasTexture);
    glUniformMatrix4fv(m_ModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(globalMatrix()));
    glUniformMatrix4fv(m_ViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(scene_params->view_mat));
    glUniformMatrix4fv(m_MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

    LightManager* lm = LightManager::getInstance();
    lm->uniformAmbient(m_program);

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

/// Loads color and normal texture
void MeshNode::loadTexture(const char * textureFile, const char * normFile)
{
    m_color_map = CreateTexture(textureFile);
    m_normal_map = CreateTexture(normFile);

    if(m_color_map != 0 && m_normal_map != 0)
    {
	m_hasTexture = true;
    }

}

/// prints the size of the geometry box without transformation
void
MeshNode::printBBoxSize()
{
    std::cout << "Min " << minbox[0] << " " << minbox[1] << " " << minbox[2] << " | ";
    std::cout << "Max " << maxbox[0] << " " << maxbox[1] << " " << maxbox[2] << std::endl;
}

/**
 * Constructs bounding box vertices
 *   v7     v6
 *   +------+
 *  /v4    /|v5
 * +------+ |
 * | |    | |
 * |v3----|-+v2
 * |/     |/
 * +------+
 * v0     v1
 */
void MeshNode::constructBox()
{
    //base                       x          y          z
    m_bbox[0] = glm::vec3(minbox[0], minbox[1], minbox[2]);
    m_bbox[1] = glm::vec3(maxbox[0], minbox[1], minbox[2]);
    m_bbox[2] = glm::vec3(maxbox[0], minbox[1], maxbox[2]);
    m_bbox[3] = glm::vec3(minbox[0], minbox[1], maxbox[2]);
    //top
    m_bbox[4] = glm::vec3(minbox[0], maxbox[1], minbox[2]);
    m_bbox[5] = glm::vec3(maxbox[0], maxbox[1], minbox[2]);
    m_bbox[6] = glm::vec3(maxbox[0], maxbox[1], maxbox[2]);
    m_bbox[7] = glm::vec3(minbox[0], maxbox[1], maxbox[2]);
}

/**
 * Get the local vertex of a given ID
 * @param vertex ID of a requested vertex
 * @return vertex
 */
glm::vec3 MeshNode::getBoxVertex(const int vertex) const
{
    return m_bbox[vertex];
}

/**
 * Get an array of all vertices
 */
glm::vec3 * MeshNode::getBoxVertices()
{
    return m_bbox;
}