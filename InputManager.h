#ifndef INPUTMANAGER_H
#define	INPUTMANAGER_H

#include <bitset>

#include <GL/glew.h> //Make glew happy...
#include <GL/glut.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Singleton.h"
#include "CameraManager.h"
#include "SwoopManager.h"
#include "LightManager.h"

#define INPUT_BITSET_SIZE 255

class InputManager : public Singleton<InputManager>
{
public:
    friend class Singleton<InputManager>;

    enum KeyType
    {
        KEY_NORMAL,
        KEY_SPECIAL
    };

    typedef std::bitset<INPUT_BITSET_SIZE> Keyset;

    static void Initialize();
    static void OnKeyPress(unsigned char, int, int);
    static void OnKeyRelease(unsigned char, int, int);
    static void OnSpecialPress(int, int, int);
    static void OnSpecialRelease(int, int, int);
    static void OnMouse(int, int, int, int);
    static void OnMotion(int, int);

    void update();

protected:
    InputManager();
    ~InputManager();

    void press(unsigned char);
    void release(unsigned char);
    void press(int);
    void release(int);
    Keyset* getSet(const KeyType);
    Keyset m_keys;
    Keyset m_specialKeys;
};

#endif	/* INPUTMANAGER_H */

