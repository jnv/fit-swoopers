/* 
 * File:   LightManager.h
 * Author: j
 *
 * Created on 8. listopad 2011, 5:12
 */

#ifndef LIGHTMANAGER_H
#define	LIGHTMANAGER_H

#include <GL/glew.h>
#include "Singleton.h"

class LightManager : public Singleton<LightManager>
{
public:
    friend class Singleton<LightManager>;
    LightManager();
    ~LightManager();

    void uniformDirectional(GLuint);
    void uniformSpotlight(GLuint);
private:

};

#endif	/* LIGHTMANAGER_H */

