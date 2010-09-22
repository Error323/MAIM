#ifndef AOBSERVER_HDR
#define AOBSERVER_HDR

class ASubject;

class AObserver {
public:
	virtual ~AObserver() {}
	virtual void Notify(ASubject* subject) = 0;

protected:
	AObserver() {}
};

#endif
