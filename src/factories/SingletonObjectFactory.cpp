#include "SingletonObjectFactory.hpp"

SingletonObjectFactory::SingletonObjectFactory() {
	object = NULL;
}

SingletonObjectFactory::~SingletonObjectFactory() {
	if (object != NULL)
		delete object;
}

Object* SingletonObjectFactory::Instance() {
	if (object == NULL)
		object = new Object();
	return object;
}
