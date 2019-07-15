#include "stdafx.h"

ofstream LogFile("log.txt");

void DrawTexturedBox(stPlane Box, unsigned int TexId, stPlane TexCoord)
{
	glBindTexture(GL_TEXTURE_2D, TexId);
    glBegin(GL_QUADS);
	    glTexCoord2f(TexCoord.Point[0].p[0], TexCoord.Point[0].p[1]); glVertex2f(Box.Point[0].p[0], Box.Point[0].p[1]);
	    glTexCoord2f(TexCoord.Point[0].p[0], TexCoord.Point[1].p[1]); glVertex2f(Box.Point[0].p[0], Box.Point[1].p[1]);
		glTexCoord2f(TexCoord.Point[1].p[0], TexCoord.Point[1].p[1]); glVertex2f(Box.Point[1].p[0], Box.Point[1].p[1]);
		glTexCoord2f(TexCoord.Point[1].p[0], TexCoord.Point[0].p[1]); glVertex2f(Box.Point[1].p[0], Box.Point[0].p[1]);
	glEnd();
}
