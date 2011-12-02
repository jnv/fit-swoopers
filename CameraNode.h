#ifndef CAMERANODE_H
#define	CAMERANODE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "SceneNode.h"
#include "TransformNode.h"

class CameraNode : public SceneNode
{
public:
    CameraNode(const char* name = "camera", SceneNode* parent = NULL);
    ~CameraNode();

//    void update(double elapsed_time);
    void draw(SceneParams * scene_params);

    void setTransform(TransformNode * t)
    {
        m_transform = t;
    }

    TransformNode * getTransform()
    {
        return m_transform;
    }

    void activate()
    {
        m_active = true;
    }

    void disable()
    {
        m_active = false;
    }

protected:
    TransformNode * m_transform;
    bool m_active;

};

#endif	/* CAMERANODE_H */

