#include "ReusableObjectFactory.hpp"

template<class Object>
std::list<Object*> ReusableObjectFactory<Object>::free;

template<class Object>
ReusableObjectFactory<Object>::~ReusableObjectFactory() {
	typename std::list<Object*>::iterator i;
	for (i = free.begin(); i != free.end(); i++)
		delete *i;
	free.clear();
}
 
template<class Object>
Object* ReusableObjectFactory<Object>::Instance() {
	Object* object;

	if (free.empty())
		object = new Object();
	else {
		object = free.front();
		free.pop_front();
	}

	return object;
}
 
template<class Object>
void ReusableObjectFactory<Object>::Release(Object* object) {
	free.push_back(object);
}
