#ifndef AMODULE
#define AMODULE

#include <list>
#include <stack>

#include "../observers/AUnitDestroyedObserver.hpp"

class AModule: public AUnitDestroyedObserver {
	public:
		virtual ~AModule();

		virtual void Release() = 0;
		virtual void Filter(std::list<int> &units) = 0;
		virtual bool Update() = 0;
		virtual bool CanRun() = 0;

		void SetStack(std::stack<AModule*> &moduleStack);

	protected:
		AModule();
		std::stack<AModule*> *moduleStack;
		std::list<int> units;

	private:

		// Implementation
		void UnitDestroyed(int unit);
};

#endif
