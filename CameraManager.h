#ifndef CAMERAMANAGER_H
#define	CAMERAMANAGER_H

#include <iostream>
#include <stdexcept>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Singleton.h"
#include "SceneNode.h"
#include "SceneParams.h"

class CameraManager : public Singleton<CameraManager>
{
public:
    typedef std::vector<SceneNode *> Cameras;
    friend class Singleton<CameraManager>;

    CameraManager();

    ~CameraManager();

    void nextCamera();
    bool isCurrent(SceneNode*) const;
    void addCamera(SceneNode*);
    void sceneUpdate(SceneNode*);
    void setSceneParams(SceneParams * scene_params)
    {
        m_scene_params = scene_params;
    }
    SceneParams * getSceneParams()
    {
        return m_scene_params;
    }
    void translate(const glm::vec3& tr);
    void rotate(const float, const glm::vec3&);

protected:
    Cameras m_cameras;
    SceneNode * m_current;
    unsigned int m_current_pos;
    SceneParams * m_scene_params;

private:

};

#endif	/* CAMERAMANAGER_H */

