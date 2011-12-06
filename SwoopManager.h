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
#include "CollisionManager.h"
#include "LightManager.h"
#include <sstream>


/// Singleton to handle swoop bike's initialization, movement, collision
class SwoopManager : public Singleton<SwoopManager>
{
public:
    friend class Singleton<SwoopManager>;
    static TransformNode* Initialize();

    void forward();
    void left();
    void right();
    void backward();
    void bump();

    void update(double);

    void reset();

    /// Whether the swoop has reached the final waypoint
    bool hasFinished() const
    {
        return m_finished;
    }


protected:
    void setup();
    void finished();
    void move();
    glm::vec3 getSplinePoint(const float s) const;


    MeshNode * m_swoopNode;
    TransformNode * m_transformNode;
    bool m_inited;
    bool m_finished;

    float m_linePos;
    glm::vec3 m_lastPoint;

    glm::vec3 * m_points;

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

    int m_nPoints;

    bool m_collides;

private:
    SwoopManager();
    ~SwoopManager();

};

#endif	/* SWOOPMANAGER_H */

