#include "stdafx.h"
#include "Background.h"

MBackground::MBackground():MObject()
{
	NullPoint(Step);
}
MBackground::~MBackground()
{
}
void MBackground::Move(bool Left)
{
	if(Left)
	{
		TexCoord.Point[0].p[0] += Step.p[0];
		TexCoord.Point[1].p[0] += Step.p[0];
		if(TexCoord.Point[0].p[0] >= 1)
		{
			TexCoord.Point[0].p[0] = 0;
			TexCoord.Point[1].p[0] = 1; //may be not 1
		}
	}
	else
	{
		TexCoord.Point[0].p[0] -= Step.p[0];
		TexCoord.Point[1].p[0] -= Step.p[0];
		if(TexCoord.Point[1].p[0] <= 0)
		{
			TexCoord.Point[0].p[0] = 0;
			TexCoord.Point[1].p[0] = 1; //may be not 1
		}
	}
}
void MBackground::SetStep(float Vx, float Vy)
{
	SetPoint(Step, Vx, Vy);
}
void MBackground::SetStep(stPoint Point)
{
	CopyPoint(Point, Step);
}
