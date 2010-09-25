#ifndef OBJECT_FACTORY_HDR
#define OBJECT_FACTORY_HDR

#include <list>
#include <map>
#include <iostream>
#include <typeinfo>

#include "../main/AIHelper.hpp"
#include "./Logger.hpp"

template<class Object> 
class ObjectFactory
{
public:
	ObjectFactory() {}
	virtual ~ObjectFactory() {}

	static Object* Instance();
	static Object* Singleton(int inIndex = 0);
	static void Release(Object*);
	static void Free();
 
private:
	static std::map<int, Object*> sSingletons;
	static std::list<Object*> sFree;
	static std::list<Object*> sAll;
};

template<class Object>
void ObjectFactory<Object>::Free() 
{
	int bytes = 0;

	typename std::map<int, Object*>::iterator i;
	for (i = sSingletons.begin(); i != sSingletons.end(); i++)
	{
		bytes += sizeof(Object);
		delete i->second;
	}

	typename std::list<Object*>::iterator j;
	for (j = sAll.begin(); j != sAll.end(); j++)
	{
		bytes += sizeof(Object);
		delete *j;
	}

	LOG_DEBUG("ObjectFactory<"
		<< typeid(Object).name()
		<< ">::Free() destroyed " 
		<< sAll.size() + sSingletons.size()
		<< " objects, " 
		<< bytes 
		<< " bytes."
		<< std::endl);

	sFree.clear();
	sAll.clear();
	sSingletons.clear();
}

template<class Object>
Object* ObjectFactory<Object>::Singleton(int inIndex) 
{
	if (sSingletons.find(inIndex) == sSingletons.end())
		sSingletons[inIndex] = new Object();
	return sSingletons[inIndex];
}
 
template<class Object>
Object* ObjectFactory<Object>::Instance() 
{
	Object* object;

	if (sFree.empty())
	{
		object = new Object();
		sAll.push_back(object);
	}
	else
	{
		object = sFree.front();
		sFree.pop_front();
	}

	return object;
}
 
template<class Object>
void ObjectFactory<Object>::Release(Object* inObject) 
{
	sFree.push_back(inObject);
}

template<class Object>
std::list<Object*> ObjectFactory<Object>::sFree;

template<class Object>
std::list<Object*> ObjectFactory<Object>::sAll;

template<class Object>
std::map<int, Object*> ObjectFactory<Object>::sSingletons;

#endif
