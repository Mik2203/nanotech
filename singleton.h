#ifndef SINGLETON_H
#define SINGLETON_H


template <class T>
class Singleton {

    static T* _instance;
protected:
    Singleton(){}
    virtual ~Singleton(){_instance=NULL;}
public:
    static T* instance();
    static void free();
};

template <class T>
T*  Singleton<T>::_instance = nullptr;

template <class T>
T*  Singleton<T>::instance() { if(!_instance)_instance=new T; return _instance; }

template <class T>
void  Singleton<T>::free() { delete _instance; }

#endif // SINGLETON_H
