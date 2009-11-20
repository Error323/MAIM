#include "SingletonObjectFactory.hpp"

template <class Object>
std::map<int,Object*> SingletonObjectFactory::objects;

SingletonObjectFactory::~SingletonObjectFactory() {
	std::map<int,Object*>::iterator i;
	for (i = objects.begin(); i != objects.end(); i++)
		delete i->second;
	objects.clear();
}

Object* SingletonObjectFactory::Instance(int i) {
	if (objects.find(i) == objects.end())
		objects[i] = new Object();

	return objects[i];
}
