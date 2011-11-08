/* 
 * File:   InputManager.cpp
 * Author: j
 * 
 * Created on 7. listopad 2011, 14:36
 */

#include "InputManager.h"

void InputManager::Initialize()
{
    ::glutKeyboardFunc(InputManager::handleKeyPress);
    ::glutKeyboardUpFunc(InputManager::handleKeyRelease);
    ::glutSpecialFunc(InputManager::handleSpecialKeyPress);
    ::glutSpecialUpFunc(InputManager::handleSpecialKeyRelease);
}

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::handleKeyPress(unsigned char key, int x, int y)
{
    InputManager * m = InputManager::getInstance();
    m->press(key);
}

void InputManager::handleKeyRelease(unsigned char key, int x, int y)
{
    InputManager * m = InputManager::getInstance();
    m->release(key);
}

void InputManager::handleSpecialKeyPress(int key, int x, int y)
{
    InputManager * m = InputManager::getInstance();
    m->press(key);
}

void InputManager::handleSpecialKeyRelease(int key, int x, int y)
{
    InputManager * m = InputManager::getInstance();
    m->release(key);
}

/**
 * Handle normal standard key set
 * @param type
 * @param key
 */
void InputManager::press(unsigned char key)
{
    if(key >= INPUT_BITSET_SIZE)
    {
	std::cerr << "InputManager::press: Key " << key << " is out of range." << std::endl;
	return;
    }


    m_keys.set(key);
}

void InputManager::release(unsigned char key)
{
    m_keys.reset(key);
}

/**
 * Handle special key set
 * @param type
 * @param key
 */
void InputManager::press(int key)
{
    if(key >= INPUT_BITSET_SIZE)
    {
	std::cerr << "InputManager::press: Key " << key << " is out of range." << std::endl;
	return;
    }


    m_specialKeys.set(key);
}

void InputManager::release(int key)
{
    m_specialKeys.reset(key);
}

InputManager::Keyset* InputManager::getSet(const KeyType type)
{
    Keyset* set;
    switch(type)
    {
    case KEY_NORMAL:
	set = &m_keys;
    case KEY_SPECIAL:
	set = &m_specialKeys;
    };
    return set;
}

void InputManager::update()
{
    if(m_keys[27]) //ESC
    {
	exit(0);
    }

    CameraManager * cman = CameraManager::getInstance();

    if(m_keys['a'] == true) //left
    {
	cman->rotate(-15, glm::vec3(0, 1, 0));
    }
    if(m_keys['d']) //right
    {
	cman->rotate(15, glm::vec3(0, 1, 0));
    }
    if(m_keys['w']) //fwd
    {
	cman->translate(glm::vec3(0, 0, 1));
    }
    if(m_keys['s']) //bwd
    {
	cman->translate(glm::vec3(0, 0, -1));
    }
    if(m_keys['q']) //cam up
    {
	cman->translate(glm::vec3(0, 1, 0));
    }
    if(m_keys['e']) //cam down
    {
	cman->translate(glm::vec3(0, -1, 0));
    }
    if(m_keys['c'])
    {
	cman->nextCamera();
	m_keys.reset('c'); //Oneshot
    }

    SwoopManager * sman = SwoopManager::getInstance();
    if(m_specialKeys[GLUT_KEY_UP])
    {
	sman->forward();
    }
    if(m_specialKeys[GLUT_KEY_DOWN])
    {
	sman->backward();
    }
    if(m_specialKeys[GLUT_KEY_LEFT])
    {
	sman->left();
    }
    if(m_specialKeys[GLUT_KEY_RIGHT])
    {
	sman->right();
    }

}
