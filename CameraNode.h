#ifndef CAMERANODE_H
#define	CAMERANODE_H
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "SceneNode.h"
#include "TransformNode.h"

/**
 * Handles view matrix recalculation depending on node's globalMatrix
 */
class CameraNode : public SceneNode
{
public:
    CameraNode(const char* name = "camera", SceneNode* parent = NULL);
    ~CameraNode();

//    void update(double elapsed_time);
    void draw(SceneParams * scene_params);

    /// Activate current camera -> will recalculate scene_params on draw call
    void activate()
    {
        m_active = true;
    }
    /// Deactivate current camera -> won't have any effect on scene drawing
    void disable()
    {
        m_active = false;
    }

    void lookAt(const glm::vec3&, const glm::vec3&, const glm::vec3& = glm::vec3(0, 1.0, 0));


protected:
    /// Is current camera active?
    bool m_active;

};

#endif	/* CAMERANODE_H */

