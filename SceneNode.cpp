#include "SceneNode.h"

/**
 * Generic constructor to set name, parent and local matrix
 * @param name
 * @param parent
 */
SceneNode::SceneNode(const char *name, SceneNode *parent):
  m_name(name), m_parent(0)
{
  setParentNode(parent);
  m_local_mat = glm::mat4(1.0f);
}

/**
 * Recursively destroy all children
 */
SceneNode::~SceneNode()
{
  setParentNode(NULL);

  while(!m_children.empty())
  {
    //std::clog << "Deleting " <<  m_children.back()->nodeName() << std::endl;
    SceneNode * n = m_children.back();
    m_children.pop_back();
    delete n;
  }
}

/**
 * Recursively update children based on the elapsed_time
 * @param elapsed_time
 */
void SceneNode::update(double elapsed_time)
{
  // if we have parent, multiply parent's matrix with ours
  if(m_parent)
    m_global_mat = m_parent->globalMatrix() * m_local_mat;
  else
    m_global_mat = m_local_mat;

  for(Children::iterator it = m_children.begin(); it != m_children.end(); ++it)
  {
    SceneNode * child = *it;
    if(child)
      child->update(elapsed_time);
    else
      std::cerr << "node " << m_name << " has NULL child, that should not happen" << std::endl;
  }
}

/**
 * Draw children
 * @param scene_params
 */
void SceneNode::draw(SceneParams * scene_params)
{
  for(Children::iterator it = m_children.begin(); it != m_children.end(); ++it)
  {
    SceneNode * child = *it;
    if(child)
      child->draw(scene_params);
    else
      std::cerr << "node " << m_name << " has NULL child, that should not happen" << std::endl;
  }
}

void SceneNode::setParentNode(SceneNode * new_parent)
{
  if(m_parent == new_parent)
    return;

  if(m_parent != NULL)
    m_parent->removeChildNode(this);

  m_parent = new_parent;

  if(new_parent != NULL)
    new_parent->addChildNode(this);
}

void SceneNode::addChildNode(SceneNode* node)
{
  if(node == NULL)
    return;

  node->setParentNode(this);
  m_children.push_back(node);
}

void SceneNode::removeChildNode(SceneNode* node)
{
  for(Children::iterator it = m_children.begin(); it != m_children.end(); ++it)
  {
    SceneNode * child = *it;
    if(child == node)
    {
      m_children.erase(it);
      break;
    }
  }
}

void SceneNode::dump(unsigned indent)
{
  // prepare indentation string (2 spaces for indentation level)
  std::string ind(indent * 2, ' ');

  // print name
  std::cout << ind << "- " << nodeName() << std::endl;

  // dump all children, raise indentation
  for(Children::iterator it = m_children.begin(); it != m_children.end(); ++it)
    (*it)->dump(indent + 1);
}