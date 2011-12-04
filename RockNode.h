/* 
 * File:   RockNode.h
 * Author: j
 *
 * Created on 4. prosinec 2011, 23:50
 */

#ifndef ROCKNODE_H
#define	ROCKNODE_H
#include "MeshNode.h"

class RockNode : public MeshNode
{
public:
    RockNode(const char* file_name = "", SceneNode* parent = NULL);
    virtual ~RockNode();

private:

};

#endif	/* ROCKNODE_H */

