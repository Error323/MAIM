#ifndef MAI_AMODULE
#define MAI_AMODULE

#include <list>
#include <stack>

#include "../observers/AObserver.hpp"

class UnitDestroyed;
class ASubject;

class AModule: public AObserver {
	public:
		virtual ~AModule();

		virtual void Release() = 0;
		virtual void Filter(std::list<int> &units) = 0;
		virtual bool Update() = 0;
		virtual bool CanRun() = 0;

		void SetStack(std::stack<AModule*> &moduleStack);

	protected:
		AModule(UnitDestroyed *_ud): unitDestroyed(_ud);
		std::stack<AModule*> *moduleStack;
		std::list<int> units;

	private:
		UnitDestroyed *unitDestroyed;

		void Update(ASubject *subject);
};

#endif
