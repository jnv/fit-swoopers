/* 
 * File:   CameraNode.h
 * Author: j
 *
 * Created on 6. listopad 2011, 11:47
 */

#ifndef CAMERANODE_H
#define	CAMERANODE_H

#include "SceneNode.h"
#include "CameraManager.h"

class CameraNode : public SceneNode
{
public:
    CameraNode(const char* name = "camera", SceneNode* parent = NULL);
    ~CameraNode();

    void update(double elapsed_time);
private:

};

#endif	/* CAMERANODE_H */

