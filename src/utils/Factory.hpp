#ifndef FACTORY
#define FACTORY

#include <list>
#include <map>
#include <iostream>
#include <typeinfo>

template<class Object> 
class Factory 
{
public:
	Factory() {}
	virtual ~Factory() {}

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
void Factory<Object>::Free() 
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

	std::cout 
		<< "Factory<"
		<< std::string(typeid(Object).name()).substr(1)
		<< ">::Free() destroyed " 
		<< sAll.size() + sSingletons.size()
		<< " objects, " 
		<< bytes 
		<< " bytes."
		<< std::endl;

	sFree.clear();
	sAll.clear();
	sSingletons.clear();
}

template<class Object>
Object* Factory<Object>::Singleton(int inIndex) 
{
	if (sSingletons.find(inIndex) == sSingletons.end())
		sSingletons[inIndex] = new Object();
	return sSingletons[inIndex];
}
 
template<class Object>
Object* Factory<Object>::Instance() 
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
void Factory<Object>::Release(Object* inObject) 
{
	sFree.push_back(inObject);
}

template<class Object>
std::list<Object*> Factory<Object>::sFree;

template<class Object>
std::list<Object*> Factory<Object>::sAll;

template<class Object>
std::map<int, Object*> Factory<Object>::sSingletons;

#endif // FACTORY
