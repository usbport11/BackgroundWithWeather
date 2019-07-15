#include "stdafx.h"
#include "ProgressBar.h"

MProgressBar::MProgressBar():MObject()
{
	Count = 0;
	Current = 0;
	Horizontal = true;
	Inversion = false;
	Filling = true;
	NullPlane(Border);
}

MProgressBar::~MProgressBar()
{
}

bool MProgressBar::Initialize(unsigned short int inCount, stPlane inBorder)
{
	if(IsPlaneNull(inBorder))
	{
		LogFile<<"Empty plane given"<<endl;
		return false;
	}
	if(inCount <= 0)
	{
		LogFile<<"Wrong count given"<<endl;
		return false;
	}
	CopyPlane(inBorder, Border);
	CopyPlane(Border, Body);
	Count = inCount;
	Current = 0;
	if(Horizontal) CoordStep = (Body.Point[1].p[0] - Body.Point[0].p[0]) / Count;
	else CoordStep = (Body.Point[1].p[1] - Body.Point[0].p[1]) / Count;
	
	return true;
}

bool MProgressBar::Initialize(unsigned short int inCount, float x0, float y0, float x1, float y1)
{
	stPlane inBorder;
	SetPlane(inBorder, x0, y0, x1, y1);
	if(!Initialize(inCount, inBorder)) return false;
	
	return true;
}

void MProgressBar::Set(unsigned short int inCurrent)
{
	Current = inCurrent;
	if(Current >= Count) Current = Count - 1;
	if(!Inversion)
	{	
		if(Filling)
		{
			if(Horizontal) Body.Point[1].p[0] = Border.Point[0].p[0] + CoordStep * (Current + 1);
			else CoordStep = Body.Point[1].p[1] = Border.Point[0].p[1] + CoordStep * (Current + 1);
		}
		else
		{
			if(Horizontal) Body.Point[0].p[0] = Border.Point[0].p[0] + CoordStep * (Current + 1);
			else CoordStep = Body.Point[0].p[1] = Border.Point[0].p[1] + CoordStep * (Current + 1);
		}
	}
	else
	{
		if(Filling)
		{
			if(Horizontal) Body.Point[0].p[0] = Border.Point[1].p[0] - CoordStep * (Current + 1);
			else CoordStep = Body.Point[0].p[1] = Border.Point[0].p[1] - CoordStep * (Current + 1);
		}
		else
		{
			if(Horizontal) Body.Point[1].p[0] = Border.Point[1].p[0] - CoordStep * (Current + 1);
			else CoordStep = Body.Point[1].p[1] = Border.Point[0].p[1] - CoordStep * (Current + 1);
		}
	}
	LogFile<<"Progress bar value: "<<Current<<endl;
}

void MProgressBar::Increase()
{
	if(Current < Count) Current ++;
	Set(Current);
}

void MProgressBar::Decrease()
{
	if(Current > 0) Current --;
	Set(Current);
}

void MProgressBar::SetHorizontal(bool inHorizontal)
{
	Horizontal = inHorizontal;
}

void MProgressBar::SetInversion(bool inInversion)
{
	Inversion = inInversion;
}

void MProgressBar::SetFilling(bool inFilling)
{
	Filling = inFilling;
}
