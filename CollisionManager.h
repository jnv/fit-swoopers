#ifndef COLLISIONMANAGER_H
#define	COLLISIONMANAGER_H
#include "Singleton.h"
#include "CollidableNode.h"
#include <vector>

/**
 * Singleton for collision checking
 */
class CollisionManager : public Singleton<CollisionManager>
{
public:
    typedef std::vector<CollidableNode *> Collidables;

    friend class Singleton<CollisionManager>;

    void makeCollidable(MeshNode *, const bool = false);
    void reset();
    void draw();
    void setDraw(const bool);
    bool hasCollision();

protected:
    Collidables m_collidables;
    CollidableNode * m_movable;

private:
    CollisionManager();
    virtual ~CollisionManager();

};

#endif	/* COLLISIONMANAGER_H */

