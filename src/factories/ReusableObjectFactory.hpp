#ifndef REUSABLE_OBJECT_FACTORY
#define REUSABLE_OBJECT_FACTORY

#include <list>

template <class Object>
class ReusableObjectFactory {
	public:
		ReusableObjectFactory(){}
		~ReusableObjectFactory();

		static Object* Instance();
		static void Release(Object*);
	
	private:
		static std::list<Object*> free;
};

#endif
