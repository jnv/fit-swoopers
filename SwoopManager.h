/* 
 * File:   SwoopManager.h
 * Author: j
 *
 * Created on 8. listopad 2011, 1:35
 */

#ifndef SWOOPMANAGER_H
#define	SWOOPMANAGER_H

#include <glm/gtx/spline.hpp>
#include "MeshNode.h"
#include "TransformNode.h"
#include "Singleton.h"
#include "Config.h"
#include "CameraManager.h"
#include "CollidableNode.h"

class SwoopManager : public Singleton<SwoopManager>
{
public:
    friend class Singleton<SwoopManager>;
    SwoopManager();
    ~SwoopManager();

    static TransformNode* Initialize();

    void forward();
    void left();
    void right();
    void backward();
    void update(double);

    void reset();

    bool hasFinished() const
    {
        return m_finished;
    }


protected:
    void setup();
    void finished();
    void move();

    MeshNode * m_swoopNode;
    TransformNode * m_transformNode;
    bool m_inited;
    bool m_finished;

    float m_linePos;
    glm::vec3 m_lastPoint;

    glm::vec3 m_waypoints[4];

    float m_limit_left;
    float m_limit_right;
    float m_accel_fwd;
    float m_accel_bwd;
    float m_decel;
    float m_side_step;
    float m_side_deviation;
    float m_velocity;
    float m_velocity_max_fwd;
    float m_velocity_max_bwd;

private:

};

#endif	/* SWOOPMANAGER_H */

