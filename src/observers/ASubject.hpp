#ifndef ASUBJECT_HDR
#define ASUBJECT_HDR

#include <list>

class AObserver;

class ASubject {
public:
	virtual ~ASubject() {}

	virtual void Attach(AObserver*);
	virtual void Detach(AObserver*);
	virtual void NotifyObservers();

protected:
	ASubject() {}

	void RemoveObservers();

private:
	std::list<AObserver*> observers;
	std::list<AObserver*> detached;
};

#endif
