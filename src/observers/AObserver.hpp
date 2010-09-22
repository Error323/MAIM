#ifndef AOBSERVER
#define AOBSERVER

class ASubject;

class AObserver {
public:
	virtual ~AObserver() {}
	virtual void Update(ASubject *subject) = 0;

protected:
	AObserver() {}
};

#endif
