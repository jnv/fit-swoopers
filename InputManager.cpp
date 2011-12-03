/* 
 * File:   InputManager.cpp
 * Author: j
 * 
 * Created on 7. listopad 2011, 14:36
 */

#include "InputManager.h"
#include "GameManager.h"

void InputManager::Initialize()
{
    ::glutKeyboardFunc(InputManager::OnKeyPress);
    ::glutKeyboardUpFunc(InputManager::OnKeyRelease);
    ::glutSpecialFunc(InputManager::OnSpecialPress);
    ::glutSpecialUpFunc(InputManager::OnSpecialRelease);
    ::glutMouseFunc(InputManager::OnMouse);
    ::glutMotionFunc(InputManager::OnMotion);
}

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::OnKeyPress(unsigned char key, int x, int y)
{
    InputManager * m = InputManager::getInstance();
    m->press(key);
}

void InputManager::OnKeyRelease(unsigned char key, int x, int y)
{
    InputManager * m = InputManager::getInstance();
    m->release(key);
}

void InputManager::OnSpecialPress(int key, int x, int y)
{
    InputManager * m = InputManager::getInstance();
    m->press(key);
}

void InputManager::OnSpecialRelease(int key, int x, int y)
{
    InputManager * m = InputManager::getInstance();
    m->release(key);
}

void InputManager::OnMouse(int button, int state, int x, int y)
{
    CameraManager * cm = CameraManager::getInstance();
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
	cm->arcballEnable(x, y);
	//    arcball_on = true;
	//    last_mx = cur_mx = x;
	//    last_my = cur_my = y;
    }
    else
    {
	cm->arcballDisable();
	//    arcball_on = false;
    }
}

void InputManager::OnMotion(int x, int y)
{
    CameraManager * cm = CameraManager::getInstance();
    cm->arcballUpdate(x, y);
    //  if (arcball_on) {  // if left button is pressed
    //    cur_mx = x;
    //    cur_my = y;
    //  }
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

    if(key == 'c')
    {
	CameraManager::getInstance()->nextCamera();
	return;
    }

    if(key == 'r')
    {
	GameManager::Reset();
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

    if(m_keys['a'] == true) //cam left
    {
	cman->translate(glm::vec3(-0.1, 0, 0));

    }
    if(m_keys['d']) //cam right
    {
	cman->translate(glm::vec3(0.1, 0, 0));
    }
    if(m_keys['w']) //fwd
    {
	//cman->translate(glm::vec3(0, 0, 0.2));
	cman->translate(glm::vec3(0, 0, -0.1)); //XXX ain't this be another way around?
    }
    if(m_keys['s']) //bwd
    {
	//cman->translate(glm::vec3(0, 0, -0.2));
	cman->translate(glm::vec3(0, 0, 0.1)); //XXX ain't this be another way around?

    }
    if(m_keys['q']) //cam rotate left
    {
	//cman->rotate(-5, glm::vec3(0, 1, 0));
	cman->rotate(1.f, glm::vec3(0, 1, 0));
    }
    if(m_keys['e']) //cam rotate right
    {
	//cman->rotate(5, glm::vec3(0, 1, 0));
	cman->rotate(-1.f, glm::vec3(0, 1, 0));
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


    if(m_specialKeys[GLUT_KEY_PAGE_UP])
    {
	cman->translate(glm::vec3(0, 0.01, 0.0));
    }
    if(m_specialKeys[GLUT_KEY_PAGE_DOWN])
    {
	cman->translate(glm::vec3(0, -0.01, 0.0));
    }
}
