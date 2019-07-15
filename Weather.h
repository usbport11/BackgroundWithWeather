#ifndef weatherH
#define weatherH

struct stParticle
{
	stPoint Point;
	stPoint Velocity;
	stPoint Direction;
};

class MWeather
{
private:
	unsigned int ParticlesLimit[2];
	stPlane VelocityLimit;
	unsigned int ParticlesCount;
	stParticle* Particles;
	unsigned char Type;
	stPlane GenerateZone;
	stPlane ViewBox;
	stPoint GlobalDirection;
	unsigned char PointSize;
	
	bool GenerateParticleArray(unsigned int ParticlesMin, unsigned int ParticlesMax);
	bool GenerateParticles();
	bool GenerateParticle(unsigned int Number);
public:
	MWeather();
	~MWeather();
	bool CreateWeather(unsigned char inType);
	void MoveParticles();
	void Draw();
};

#endif
