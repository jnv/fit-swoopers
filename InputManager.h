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

    InputManager();
    ~InputManager();

    static void Initialize();
    static void OnKeyPress(unsigned char key, int x, int y);
    static void OnKeyRelease(unsigned char key, int x, int y);
    static void OnSpecialPress(int key, int x, int y);
    static void OnSpecialRelease(int key, int x, int y);
    static void OnMouse(int, int, int, int);
    static void OnMotion(int, int);

    void update();

protected:
    void press(unsigned char);
    void release(unsigned char);
    void press(int);
    void release(int);
    Keyset* getSet(const KeyType);
    Keyset m_keys;
    Keyset m_specialKeys;

};

#endif	/* INPUTMANAGER_H */

