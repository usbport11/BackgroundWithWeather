#ifndef backgroundH
#define backgroundH

#include "Object.h"

class MBackground: public MObject
{
private:
	stPoint Step;
public:
	MBackground();
	~MBackground();
	void Move(bool Left);
	void SetStep(float Vx, float Vy);
	void SetStep(stPoint Point);
};

#endif
