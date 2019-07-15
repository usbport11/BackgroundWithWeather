#include "stdafx.h"
#include "Object.h"

MObject::MObject()
{
	TexId = 0;
	NullPlane(Body);
	SetPlane(TexCoord, 0, 0, 1, 1);
}

MObject::~MObject()
{
	TexId = 0;
	NullPlane(Body);
	SetPlane(TexCoord, 0, 0, 0, 0);
}

void MObject::Draw()
{
	glBindTexture(GL_TEXTURE_2D, TexId);
    glBegin(GL_QUADS);
	    glTexCoord2f(TexCoord.Point[0].p[0], TexCoord.Point[0].p[1]); glVertex2f(Body.Point[0].p[0], Body.Point[0].p[1]);
	    glTexCoord2f(TexCoord.Point[0].p[0], TexCoord.Point[1].p[1]); glVertex2f(Body.Point[0].p[0], Body.Point[1].p[1]);
		glTexCoord2f(TexCoord.Point[1].p[0], TexCoord.Point[1].p[1]); glVertex2f(Body.Point[1].p[0], Body.Point[1].p[1]);
		glTexCoord2f(TexCoord.Point[1].p[0], TexCoord.Point[0].p[1]); glVertex2f(Body.Point[1].p[0], Body.Point[0].p[1]);
	glEnd();
}

void MObject::FlipTexX()
{
	swap(TexCoord.Point[0].p[0], TexCoord.Point[1].p[0]);
}

void MObject::FlipTexY()
{
	swap(TexCoord.Point[1].p[0], TexCoord.Point[1].p[1]);
}

void MObject::SetTexId(unsigned int inTexId)
{
	TexId = inTexId;
}

void MObject::SetTexCoord(stPlane inTexCoord)
{
	if(IsPlaneNull(inTexCoord)) return;
	else CopyPlane(inTexCoord, TexCoord);
}

void MObject::SetTexCoord(float X, float Y)
{
	if(X < 1 || Y < 1 ) return;
	SetPoint(TexCoord.Point[1], X, Y);
}

bool MObject::CreateByPoints(stPoint Point1, stPoint Point2)
{
	SetPlane(Body, Point1, Point2);
	
	return true;
}

bool MObject::CreateByPoints(float x0, float y0, float x1, float y1)
{
    SetPlane(Body, x0, y0, x1, y1);
	
    return true;
}

bool MObject::CreateBySize(stPoint Point, stPoint Size)
{
	SetPlaneByOffset(Body, Point, Size);
	
	return true;
}
