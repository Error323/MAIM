#ifndef MODULE_FACTORY
#define MODULE_FACTORY

#include <list>

#include "../observers/AObserver.hpp"

class ModuleReleasedObserver;
class ASubject;

// The idea here is to create a modulefactory with an ModuleReleased observer
// that lets the factory know when a module can be released again
template <class Module>
class ModuleFactory: AObserver {
	public:
		ModuleFactory(ModuleReleasedObserver* mro): moduleReleased(mro) {}
		~ModuleFactory(){}

		static Module* Instance();
	
	private:
		std::list<Module*> free;
		ModuleReleasedObserver *moduleReleased;

		void Update(ASubject *subject);

};
