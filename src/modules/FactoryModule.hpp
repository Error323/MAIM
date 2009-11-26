#ifndef FACTORY_MODULE
#define FACTORY_MODULE

#include <string>
#include <map>

#include "./AModule.hpp"

#define MIN_QUEUE 2
#define MAX_QUEUE 5

class AIHelper;

class FactoryModule: public AModule {
	public:
		FactoryModule() {
			name = "FactoryModule";
		}
		~FactoryModule(){}

		void Init(AIHelper*);
		void Release();
		void Filter(std::map<int, AIUnit*>&);
		bool Update();
		bool CanRun();
};

#endif
