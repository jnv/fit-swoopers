#ifndef CAMERAMANAGER_H
#define	CAMERAMANAGER_H
#include <iostream>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <GL/glut.h>
#include "Singleton.h"
#include "SceneNode.h"
#include "SceneParams.h"
#include "CameraNode.h"
#include "TransformNode.h"

class CameraManager : public Singleton<CameraManager>
{
public:
    typedef std::vector<CameraNode *> Cameras;
    friend class Singleton<CameraManager>;

    CameraManager();

    ~CameraManager();
    void reset();

    void createCamera(const char*, SceneNode *, TransformNode *&, CameraNode *&);

    void nextCamera();
    bool isCurrent(CameraNode*) const;
    void addCamera(CameraNode*);
    void sceneDraw(CameraNode*);
    void recalcView(CameraNode*);

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
    CameraNode * m_current;
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

