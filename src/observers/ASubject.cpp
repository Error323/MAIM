#include "ASubject.hpp"
#include "AObserver.hpp"

void ASubject::AttachObserver(AObserver* observer) {
	observers.push_back(observer);
}

void ASubject::DetachObserver(AObserver* observer) {
	detached.push_back(observer);
}

void ASubject::NotifyObservers() {
	std::list<AObserver*>::iterator i;
	for (i = observers.begin(); i != observers.end(); i++)
		(*i)->Notify(this);

	for (i = detached.begin(); i != detached.end(); i++)
		observers.remove(*i);
	
	detached.clear();
}

void ASubject::RemoveObservers() {
	observers.clear();
}
