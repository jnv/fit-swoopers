/* 
 * File:   CameraNode.h
 * Author: j
 *
 * Created on 6. listopad 2011, 11:47
 */

#ifndef CAMERANODE_H
#define	CAMERANODE_H

#include "SceneNode.h"

class CameraNode : public SceneNode
{
public:
    CameraNode(const char* name = "<unk>", SceneNode* parent = NULL);
    CameraNode(const CameraNode& orig);
    virtual ~CameraNode();
private:

};

#endif	/* CAMERANODE_H */

