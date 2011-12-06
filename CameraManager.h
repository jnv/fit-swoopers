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

/**
 * Stores camera and its transform nodes
 */
struct CameraStruct
{
    /// Local transformation
    TransformNode * local;
    /// Global transformation (mostly leftover)
    TransformNode * global;
    /// Camera
    CameraNode * camera;

    /// Initializes all contained nodes

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

/**
 * Singleton to manage cameras
 */
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

    /// Vector of all associated cameras
    Cameras m_cameras;
    /// Currently used camera (struct)
    CameraStruct * m_current;
    /// Index of the current camera
    unsigned int m_current_pos;
    /// Direct access to scene_params
    SceneParams * m_sceneParams;

    /// Whether to calculate arcball (activated with mouse)
    bool m_abEnabled;
    /// Last arcball X position
    int m_abLastX;
    /// Current arcball X position
    int m_abCurX;
    /// Last arcball Y position
    int m_abLastY;
    /// Current arcball Y position
    int m_abCurY;

    //glm::vec3 m_translate;

    void arcballCalculate();

private:

};

#endif	/* CAMERAMANAGER_H */

