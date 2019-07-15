#include "stdafx.h"
#include "Weather.h"
	
MWeather::MWeather()
{
	SetPlane(ViewBox, 0, 0, 800, 600);
	SetPlane(VelocityLimit, 8, 8, 15, 15);
	NullPoint(GlobalDirection);
	Particles = NULL;
	PointSize = 1;
}
MWeather::~MWeather()
{
	if(Particles) delete [] Particles; 
}
bool MWeather::GenerateParticleArray(unsigned int ParticlesMin, unsigned int ParticlesMax)
{
	if(ParticlesMin >= ParticlesMax)
	{
		LogFile<<"Wrong min max values"<<endl;
		return false;
	}
	ParticlesLimit[0] = ParticlesMin;
	ParticlesLimit[1] = ParticlesMax;
	ParticlesCount = rand() % (ParticlesMax - ParticlesMin) + ParticlesMin + 1;
	try
	{
		Particles = new stParticle [ ParticlesCount ];
		LogFile<<"Generated particles count: "<<ParticlesCount<<endl;
	}
	catch(bad_alloc)
	{
		if(Particles) delete [] Particles;
		LogFile<<"Cannot allocate memory"<<endl;
		return false;
	}
	
	return true;
}
bool MWeather::GenerateParticle(unsigned int Number)
{
	if(Number >= ParticlesCount)
	{
		LogFile<<"Wrong particle number"<<endl;
		return false;
	}
	SetPoint(Particles[Number].Point, rand() % (int)(GenerateZone.Point[1].p[0] - GenerateZone.Point[0].p[0]) + GenerateZone.Point[0].p[0] + 1, 
		rand() % (int)(GenerateZone.Point[1].p[1] - GenerateZone.Point[0].p[1]) + GenerateZone.Point[0].p[1] + 1);
	switch(Type)
	{
		case 1:
			SetPoint(Particles[Number].Velocity, 1, rand() % (int)(VelocityLimit.Point[1].p[1] - VelocityLimit.Point[0].p[1]) + VelocityLimit.Point[0].p[1]);
			break;
		case 2:
			SetPoint(Particles[Number].Velocity, rand() % (int)(VelocityLimit.Point[1].p[1] - VelocityLimit.Point[0].p[1]) - VelocityLimit.Point[0].p[1]/2, 
				rand() % (int)(VelocityLimit.Point[1].p[1] - VelocityLimit.Point[0].p[1]) + VelocityLimit.Point[0].p[1]);
			break;
	}
	CopyPoint(GlobalDirection, Particles[Number].Direction);

	return true;
}
bool MWeather::GenerateParticles()
{
	for(unsigned int i=0; i<ParticlesCount; i++)
		GenerateParticle(i);
}
bool MWeather::CreateWeather(unsigned char inType)
{
	if(Particles)
	{
		LogFile<<"Weather was alredy created"<<endl;
		return false;
	}
	if(inType < 1 || inType > 3)
	{
		LogFile<<"Wrong wether type given"<<endl;
		return false;
	}
	if(!GenerateParticleArray(15, 70))
	{
		LogFile<<"Can not create particle array"<<endl;
		return false;
	}
	//based on type changes start zone, move type
	Type = inType; 
	switch(Type)
	{
		case 1: //rain
			SetPlane(GenerateZone, 0, 500, 800, 600);
			SetPoint(GlobalDirection, (rand() % 2) * 2 - 1, -1);//0,-1
			SetPlane(VelocityLimit, 8, 8, 15, 15);
			PointSize = 2;
			break;
		case 2: //snow
			SetPlane(GenerateZone, 0, 500, 800, 600);
			SetPoint(GlobalDirection, -1, -1);//-1,-1
			SetPlane(VelocityLimit, 0, 5, 10, 10);
			PointSize = 4;
			break;
	};
	GenerateParticles();
	
	return true;
}
void MWeather::MoveParticles()
{
	for(unsigned int i=0; i<ParticlesCount; i++)
	{
		Particles[i].Point = AddPoints(Particles[i].Point, MultiplyPoints(Particles[i].Direction, Particles[i].Velocity));
		if(Particles[i].Point.p[1] < ViewBox.Point[0].p[1]) GenerateParticle(i);
	}
}
void MWeather::Draw()
{
	glDisable(GL_TEXTURE_2D);
	glPointSize(PointSize);
	glBegin(GL_POINTS);
	for(unsigned int i=0; i<ParticlesCount; i++)
		glVertex2f(Particles[i].Point.p[0], Particles[i].Point.p[1]);
	glEnd();
	glPointSize(1);
	glEnable(GL_TEXTURE_2D);
}

