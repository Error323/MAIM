#include "ASubject.hpp"
#include "AObserver.hpp"

void ASubject::Attach(AObserver* observer) {
	observers.push_back(observer);
}

void ASubject::Detach(AObserver* observer) {
	observers.remove(observer);
}

void ASubject::NotifyObservers() {
	std::list<AObserver*>::iterator i;
	for (i = observers.begin(); i != observers.end(); i++)
		(*i)->Notify(this);
}

void ASubject::RemoveObservers() {
	observers.clear();
}
