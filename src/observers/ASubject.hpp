#ifndef ASUBJECT
#define ASUBJECT

#include <list>

class AObserver;

class ASubject {
public:
	virtual ~ASubject() {}

	virtual void Attach(AObserver*);
	virtual void Detach(AObserver*);
	virtual void Notify();

protected:
	ASubject() {}

	void RemoveObservers();

private:
	std::list<AObserver*> observers;
};

#endif
