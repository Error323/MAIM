#ifndef SINGLETON_OBJECT_FACTORY
#define SINGLETON_OBJECT_FACTORY

template<class Object>
class SingletonObjectFactory {
	public:
		SingletonObjectFactory(){}
		~SingletonObjectFactory();

		static Object* Instance();
	
	private:
		static Object *object;
};

#endif
