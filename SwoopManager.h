/* 
 * File:   SwoopManager.h
 * Author: j
 *
 * Created on 8. listopad 2011, 1:35
 */

#ifndef SWOOPMANAGER_H
#define	SWOOPMANAGER_H

#include "MeshNode.h"
#include "TransformNode.h"
#include "Singleton.h"

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
protected:
    MeshNode * m_swoopNode;
    TransformNode * m_transformNode;

private:

};

#endif	/* SWOOPMANAGER_H */

