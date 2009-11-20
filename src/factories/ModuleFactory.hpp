#ifndef MODULE_FACTORY
#define MODULE_FACTORY

#include <list>

// The idea here is to create a modulefactory with an ModuleReleased observer
// that lets the factory know when a module can be released again
template <class Module>
class ModuleFactory: AObserver {
	public:
		ModuleFactory(ModuleReleasedObserver* mro): moduleReleased(mro) {}

		static Module* Instance();
		static void Release(Module*);
	
	private:
		std::list<Module*> free;


};
