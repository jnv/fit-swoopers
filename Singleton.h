#ifndef _SINGLETON_H
#define	_SINGLETON_H
#include <cstdlib>
using namespace std;

/**
 * Non-thread-safe abstract singleton.
 * Derived from HTTPD project.
 * @source http://www.bombaydigital.com/arenared/2005/10/25/1
 * @author Jan Vlnas
 */
template <class T>
class Singleton
{
public:
    static T* getInstance(void);
    static void destroy(void);
protected:

    /// Nil.

    Singleton()
    {
    };

    /// Nil.

    ~Singleton()
    {
    };

private:
    /// Disabled
    Singleton(const Singleton&);
    /// Pointer to instance for the current class of singleton
    static T* m_instance;
};

/**
 * Main entry function.
 * Initializes derived class, if necessary.
 * Register singleton for destruction (destroy())
 *
 * @return T
 */
template <class T>
T* Singleton<T>::getInstance()
{
    if (!m_instance)
    {
        m_instance = new T;
        atexit(destroy);
    }
    return m_instance;
}

/**
 * Destroys instance. Called automatically at exit.
 */
template <class T>
void Singleton<T>::destroy(void)
{
    delete m_instance;
}

template <class T>
T* Singleton<T>::m_instance = 0;
#endif	/* _SINGLETON_H */


