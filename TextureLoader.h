#ifndef textureloaderH
#define textureloaderH

struct stTGAHeader
{
    unsigned char Header[12];
};

struct stTGAData
{
    unsigned char Header[6];
    unsigned int BytesPerPixel;
    unsigned int ImageSize;
    unsigned int Type;
    unsigned int Width;
    unsigned int Height;
    unsigned int BitsPerPixel;
};

struct stTexture
{
    unsigned int Id;
};

struct stAnimation
{
	stTexture* Texture;
	unsigned int Count;
	unsigned char Type;//1 - repeat animation (default), 2 - finite animation
	unsigned int Next;//if next == current - skip, if next != current and type == 2 - change animation
};

class MTextureLoader
{
private:  
	//source data
	FILE* pFile;
	char* pFileName;
	stTexture* pTexture;
	unsigned int TextureCount;
	bool MultipleTexture;
	bool BackwardRead;
	//image data
	unsigned char* pSubImageData;
	unsigned char* pImageData;
    unsigned int Width;
    unsigned int Height;
	unsigned int BitsPerPixel;
    unsigned int Type;
	//tga data
	stTGAHeader TGAHeader;
    stTGAData TGAData;
	bool CompressedTGA;
	
	bool GetTGAHeader();
	bool CreateTexture();
	bool CreateMultipleTexture();
	bool LoadTGACompressed();
	bool LoadTGAUncompressed();
	
public:
	MTextureLoader();
	~MTextureLoader();
	stTexture* LoadTexture(const char* inFileName, unsigned int& inCount);
	bool DeleteTexture(stTexture* Texture, unsigned int Count);
	stAnimation* LoadAnimation(vector<string> inFileNames, unsigned int& inCount);
	bool DeleteAnimation(stAnimation* Animation, unsigned int Count);
	stAnimation* LoadAnimationUsingFile(const char* inFileName, unsigned int& inCount);
	void SetBackwardRead(bool inBackwardRead);
};

#endif
