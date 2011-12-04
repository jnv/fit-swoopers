#ifndef CAMERAMANAGER_H
#define	CAMERAMANAGER_H
#include <iostream>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <GL/glut.h>
#include "Singleton.h"
#include "SceneNode.h"
#include "SceneParams.h"
#include "CameraNode.h"
#include "TransformNode.h"

struct CameraStruct
{
    TransformNode * local;
    TransformNode * global;
    CameraNode * camera;

    CameraStruct(const char* name, SceneNode * parent)
    {
        string sname = name;
        string tname = sname + "_global";
        global = new TransformNode(tname.c_str(), parent);
        string rname = sname + "_local";
        local = new TransformNode(rname.c_str(), global);

        camera = new CameraNode(name, local);
    }
};

class CameraManager : public Singleton<CameraManager>
{
public:
    typedef std::vector<CameraStruct *> Cameras;
    friend class Singleton<CameraManager>;

    CameraManager();

    ~CameraManager();
    void reset();

    CameraStruct* createCamera(const char*, SceneNode *, const bool = false);

    void nextCamera();
    bool isCurrent(CameraNode*) const;
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
    void addCamera(CameraStruct*, const bool = false);

    Cameras m_cameras;
    CameraStruct * m_current;
    unsigned int m_current_pos;
    SceneParams * m_sceneParams;
    bool m_abEnabled;
    int m_abLastX;
    int m_abCurX;
    int m_abLastY;
    int m_abCurY;
    glm::vec3 m_translate;

    void arcballCalculate();


private:

};

#endif	/* CAMERAMANAGER_H */

