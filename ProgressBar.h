#ifndef progressbarH
#define progressbarH

#include "Object.h"

class MProgressBar: public MObject
{
private:
	unsigned short int Count;
	unsigned short int Current;
	bool Horizontal;
	bool Inversion;
	bool Filling;
	float CoordStep;
	stPlane Border;
public:
	MProgressBar();
	~MProgressBar();
	bool Initialize(unsigned short int inCount, stPlane inBorder);
	bool Initialize(unsigned short int inCount, float x0, float y0, float x1, float y1);
	void Set(unsigned short int inCurrent);
	void Increase();
	void Decrease();
	void SetHorizontal(bool inHorizontal);
	void SetInversion(bool inInversion);
	void SetFilling(bool inFilling);
};

#endif
