#ifndef objectH
#define objectH

class MObject
{
private:
	unsigned int TexId;
protected:
	stPlane TexCoord;
	stPlane Body;
public:
	MObject();
	~MObject();
	void Draw();
	void FlipTexX();
	void FlipTexY();
	void SetTexId(unsigned int inTexId);
	void SetTexCoord(stPlane inTexCoord);
	void SetTexCoord(float X, float Y);
	bool CreateByPoints(stPoint Point1, stPoint Point2);
	bool CreateByPoints(float x0, float y0, float x1, float y1);
	bool CreateBySize(stPoint Point, stPoint Size);
};

#endif
