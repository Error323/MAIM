#include "SingletonObjectFactory.hpp"

template <class Object>
std::map<int,Object*> SingletonObjectFactory<Object>::objects;

/*
template <class Object>
SingletonObjectFactory<Object>::~SingletonObjectFactory<Object>() {
	std::map<int,Object*>::iterator i;
	for (i = objects.begin(); i != objects.end(); i++)
		delete i->second;
	objects.clear();
}
*/

template <class Object>
Object* SingletonObjectFactory<Object>::Instance(int i) {
	if (objects.find(i) == objects.end())
		objects[i] = new Object();

	return objects[i];
}
