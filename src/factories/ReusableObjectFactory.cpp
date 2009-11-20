#include "ReusableObjectFactory.hpp"

std::list<Object*> ReusableObjectFactory::free;

ReusableObjectFactory::~ReusableObjectFactory() {
	std::list<Object*>::iterator i;
	for (i = free.begin(); i != free.end(); i++)
		delete *i;
	free.clear();
}

Object* ReusableObjectFactory::Instance() {
	Object *object;
	if (free.empty())
		object = new Object();
	else {
		object = free.front();
		free.pop_front();
	}
	return object;
}

void ReusableObjectFactory::Release(Object *object) {
	free.push_back(object);
}
