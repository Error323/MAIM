#include "ModuleFactory.hpp"

#include "../observers/ASubject.hpp"
#include "../observers/ModuleReleasedObserver.hpp"

Module* ModuleFactory::Instance() {
	Module *m;
	if (free.empty())
		m = new Module();
	else {
		m = free.front();
		m.pop_front();
	}
	return m;
}

void ModuleFactory::Update(ASubject *subject) {
	if (subject == moduleReleased) {
		Module *m = dynamic_cast<Module*>(subject);
		free.push_back(m);
		return;
	}
}
