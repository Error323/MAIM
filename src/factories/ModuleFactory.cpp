#include "ModuleFactory.hpp"

ModuleFactory::~ModuleFactory() {
	std::list<Module*>::iterator i;
	for (i = free.begin(); i != free.end(); i++)
		delete *i;
}

Module* ModuleFactory::Instance() {
	Module *module;
	if (free.empty())
		module = new Module();
	else {
		module = free.front();
		free.pop_front();
	}
	return module;
}

void ModuleFactory::Release(Module *module) {
	free.push_back(module);
}
