#include "ASubject.hpp"

#include "AObserver.hpp"

void Subject::Attach(AObserver *observer) {
	observers.push_back(observer);
}

void Subject::Detach(AObserver *observer) {
	observers.remove(observer);
}

void Subject::Notify() {
	std::list<AObserver*>::iterator i;
	for (i = observers.begin(); i != observers.end(); i++)
		(*i)->Update(this);
}
