#ifndef SINGLETON_OBJECT_FACTORY
#define SINGLETON_OBJECT_FACTORY

#include <map>

template<class Object>
class SingletonObjectFactory {
	public:
		SingletonObjectFactory(){}
		~SingletonObjectFactory();

		static Object* Instance(int i = 0);
	
	private:
		static std::map<int,Object> objects;
};

#endif
