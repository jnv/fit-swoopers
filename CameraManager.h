#ifndef CAMERAMANAGER_H
#define	CAMERAMANAGER_H

#include <iostream>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glut.h>
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
    void reset();

    void nextCamera();
    bool isCurrent(SceneNode*) const;
    void addCamera(SceneNode*);
    void sceneDraw(SceneNode*);
    void recalcView(SceneNode*);
    void setSceneParams(SceneParams * scene_params)
    {
        m_sceneParams = scene_params;
    }
    SceneParams * getSceneParams()
    {
        return m_sceneParams;
    }
    void translate(const glm::vec3& tr);
    void rotate(const float, const glm::vec3&);

    void arcballEnable(int, int);
    void arcballDisable();
    void arcballUpdate(int, int);

protected:
    Cameras m_cameras;
    SceneNode * m_current;
    unsigned int m_current_pos;
    SceneParams * m_sceneParams;
    bool m_abEnabled;
    int m_abLastX;
    int m_abCurX;
    int m_abLastY;
    int m_abCurY;

    void arcballCalculate();


private:

};

#endif	/* CAMERAMANAGER_H */

