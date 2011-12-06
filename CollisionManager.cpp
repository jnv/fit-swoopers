/* 
 * File:   CollisionManager.cpp
 * Author: j
 * 
 * Created on 4. prosinec 2011, 19:02
 */

#include "CollisionManager.h"

CollisionManager::CollisionManager()
{
    m_movable = NULL;
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::makeCollidable(MeshNode * mesh, const bool movable)
{
    std::string name = mesh->nodeName();
    if(movable)
    {
	name = "MOVABLE_" + name;
    }
    else
    {
	name = "COLLIDABLE_" + name;
    }

    CollidableNode * node = new CollidableNode(name.c_str(), mesh);

    if(movable)
    {
	m_movable = node;
    }
    else
    {
	m_collidables.push_back(node);
    }

}

void CollisionManager::reset()
{
    m_collidables.clear();
    m_movable = NULL;
}

void CollisionManager::draw()
{
    CollidableNode::SetDraw(true);
}

void CollisionManager::setDraw(const bool val)
{
    CollidableNode::SetDraw(val);
}

/**
 * Check if m_movable collides with any of m_collidable
 * Based on http://www.owenpellegrin.com/articles/vb-net/simple-collision-detection/
 * @return true if there's a collision
 */
bool CollisionManager::hasCollision()
{

    if(!m_movable)
    {
	return false;
    }

    glm::vec2 v0 = m_movable->getGlobalVertex2D(0);
    //    glm::vec2 v1 = m_movable->getGlobalVertex2D(1);
    glm::vec2 v2 = m_movable->getGlobalVertex2D(2);
    //    glm::vec2 v3 = m_movable->getGlobalVertex2D(3);

    float left = v0.x;
    float top = v0.y;
    float right = v2.x;
    float bottom = v2.y;


    for(Collidables::iterator it = m_collidables.begin(); it != m_collidables.end(); ++it)
    {
	bool isHoriz = false;
	bool isVert = false;
	bool isInner = false;

	v0 = (*it)->getGlobalVertex2D(0);
	v2 = (*it)->getGlobalVertex2D(2);

	//		std::cout << left << " < " << v0.x << " && ";
	//	std::cout << v0.x << " < " << right << std::endl;




	// left edge
	// left < left' < right
	//if(left < v0.x && v0.x < right)
	if(v0.x < left && left < v2.x)
	{
	    isHoriz = true;
	    //	    std::cout << "Horizontal collision on left edge" << std::endl;
	}

	// right edge
	// left < right' < right
	if(v0.x < right && right < v2.x)
	{
	    isHoriz = true;
	    //	    std::cout << "Horizontal collision on right edge" << std::endl;
	}

	//	std::cout << v2.y << " < " << top << " && ";
	//	std::cout << top << " < " << v0.y << std::endl;


	//top edge
	//top < top' < bottom
	if(v0.y < top && top < v2.y)
	    //if(top < v2.y && v2.y < bottom)
	{
	    isVert = true;
	    //	    std::cout << "Vertical collision on top edge" << std::endl;
	}

	//bottom edge
	//top < bottom' < bottom
	if(v0.y < bottom && bottom < v2.y)
	{
	    isVert = true;
	    //	    std::cout << "Vertical collision on bottom edge" << std::endl;
	}

	// inner collision
	if(!isHoriz && !isVert)
	{
	    //r2.Left < r1.Left AndAlso r1.Left < r2.Right Then
	    // left' < left < right'
	    if(v0.x < left && left < v2.x)
	    {
		//top' < top < bottom'
		if(v0.y < top && top < v2.y)
		{
		    isInner = true;
		    std::clog << "Inner collision" << std::endl;
		    return true;
		}
	    }

	}

	if(isHoriz && isVert)
	{
	    std::clog << "Collision" << std::endl;
	    return true;
	}


    }

    return false;
    //return (isHoriz && isVert) || isInner;
}