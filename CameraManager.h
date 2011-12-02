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
    TransformNode * rotate;
    TransformNode * translate;
    CameraNode * camera;

    CameraStruct(const char* name, SceneNode * parent)
    {
        string sname = name;
        string tname = sname + "_translate";
        translate = new TransformNode(tname.c_str(), parent);
        string rname = sname + "_rotate";
        rotate = new TransformNode(rname.c_str(), translate);

        camera = new CameraNode(name, rotate);
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

    CameraStruct* createCamera(const char*, SceneNode *);

    void nextCamera();
    bool isCurrent(CameraNode*) const;
    void addCamera(CameraStruct*);
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
    CameraStruct * m_current;
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

