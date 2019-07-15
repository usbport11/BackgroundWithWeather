#include "stdafx.h"
#include "TextureLoader.h"

unsigned char TGA_UNCOMPRESSED[12] = {0,0,2,0,0,0,0,0,0,0,0,0};
unsigned char TGA_COMPRESSED[12] = {0,0,10,0,0,0,0,0,0,0,0,0};

MTextureLoader::MTextureLoader()
{
	pImageData = NULL;
	pFileName = NULL;
	pTexture = NULL;
	TextureCount = 0;
	CompressedTGA = false;
	BackwardRead = false;
}

MTextureLoader::~MTextureLoader()
{
	pImageData = NULL;
	pFileName = NULL;
	pTexture = NULL;
	TextureCount = 0;
	CompressedTGA = false;
	if(pSubImageData) delete [] pSubImageData;
	if(pImageData) delete [] pImageData;
}

stTexture* MTextureLoader::LoadTexture(const char* inFileName, unsigned int& inCount)
{
	if(!inFileName)
    {
        LogFile<<"Empty filename given"<<endl;
        return NULL;
    }
	pFileName = (char*)inFileName;
	if(!GetTGAHeader()) return NULL;
	char TestHeader = 0;
    if(!memcmp(TGA_UNCOMPRESSED, &TGAHeader, sizeof(stTGAHeader))) TestHeader = 1;
    if(!memcmp(TGA_COMPRESSED, &TGAHeader, sizeof(stTGAHeader))) TestHeader = 2;
    switch(TestHeader)
    {
        case 0:
            LogFile<<"Frong tga file header"<<endl; 
            return NULL;
        case 1:
            CompressedTGA = false;
            break;
        case 2:
            CompressedTGA = true;
            break;  
    }
	if(!CompressedTGA)
	{
		if(!LoadTGAUncompressed())
		{
			if(pImageData) delete [] pImageData;
			LogFile<<"Failed get uncompressed TGA"<<endl;
			return NULL;
		}
	}
	else
	{
		if(!LoadTGACompressed())
		{
			if(pImageData) delete [] pImageData;
			LogFile<<"Failed get compressed TGA"<<endl;
			return NULL;
		}
	}
	if(Width != Height) MultipleTexture = true;
	if(!MultipleTexture)
	{
		if(!CreateTexture())
		{
            LogFile<<"Failed create static texture"<<endl;
			if(pImageData) delete [] pImageData;
			return NULL;
		}
	}
	else
	{
		if(!CreateMultipleTexture())
		{
            LogFile<<"Failed create animated texture"<<endl;
			if(pImageData) delete [] pImageData;
			return NULL;
		}
	}
	LogFile<<"Free image memory"<<endl;
	if(pImageData) delete [] pImageData;
	
	inCount = TextureCount;
	LogFile<<"Texture count: "<<TextureCount<<". Texture address: "<<pTexture<<endl;
	
	return pTexture;
}

bool MTextureLoader::DeleteTexture(stTexture* Texture, unsigned int Count)
{
	if(!Count)
	{
		LogFile<<"Zero textures count given"<<endl;
		return false;
	}
	if(!Texture)
	{
		LogFile<<"Texture(s) is NULL"<<endl;
		return false;
	}
	unsigned int Address = (unsigned int)Texture; 
	unsigned int* Array = (unsigned int*)Texture;
	glDeleteTextures(Count, Array);
	GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        LogFile<<"Opengl error: "<<error<<" "<<(char*)gluErrorString(error)<<endl;
        delete [] Texture;
        return false;
    }
	
	delete [] Texture;
	
	LogFile<<"Texture(s) 0x"<<hex<<Address<<" successefully deleted"<<endl;
	
	return true;
}

stAnimation* MTextureLoader::LoadAnimation(vector<string> inFileNames, unsigned int& inCount)
{
	stAnimation* RetAnimations = NULL;
	unsigned int Size = inFileNames.size();
	if(!Size)
	{
		LogFile<<"No animation files recived"<<endl;
		inCount = 0;
		return NULL;
	}
	
	try
	{
		RetAnimations = new stAnimation [Size];
		inCount = Size;
		LogFile<<"Animation files count: "<<Size<<endl;
	}
	catch(bad_alloc& ba)
	{
		if(RetAnimations) delete [] RetAnimations;
		inCount = 0;
		LogFile<<"Cant allocate memory for animation"<<endl;
		return NULL;
	}
	
	for(unsigned int i=0; i<Size; i++)
	{
		RetAnimations[i].Texture = LoadTexture(inFileNames[i].c_str(), RetAnimations[i].Count);
		if(!RetAnimations[i].Texture)
		{
			if(RetAnimations) delete [] RetAnimations;
			LogFile<<"Something wron with "<<i<<" frame of animation"<<endl;
			inCount = 0;
			return NULL;
		}
		if(RetAnimations[i].Count <= 1)
		{
			//not animation - static texture
			if(RetAnimations) delete [] RetAnimations;
			inCount = 0;
			LogFile<<"Static texture recived. Not used with animation"<<endl;
			return NULL;
		}
		RetAnimations[i].Type = 1;
		RetAnimations[i].Next = i;
	}
	
	return RetAnimations;
}

bool MTextureLoader::DeleteAnimation(stAnimation* Animation, unsigned int Count)
{
	if(!Count)
	{
		LogFile<<"Zero animations count given"<<endl;
		return false;
	}
	if(!Animation)
	{
		LogFile<<"Aniamtion(s) is NULL"<<endl;
		return false;
	}
	LogFile<<"Count of animations that must deleted: "<<Count<<endl;
	for(unsigned int i=0; i<Count; i++)
	{
		DeleteTexture(Animation[i].Texture, Animation[i].Count);
	}
	delete [] Animation;
	LogFile<<"Animation(s) successefully deleted"<<endl;
	
	return true;
}

stAnimation* MTextureLoader::LoadAnimationUsingFile(const char* inFileName, unsigned int& inCount)
{
	stAnimation* RetAnimations = NULL;
	
	if(!inFileName)
	{
		LogFile<<"Empty configuration file name"<<endl;
		return NULL;
	}
	
	ifstream ConfigFile(inFileName);
	unsigned char AnimationLimit = 5;
	string FileLine;
	vector<string> FileNames;
	
	LogFile<<"Using config file for load animation"<<inFileName<<endl;
	//read animation file names from config file
    if(!ConfigFile.good())
    {
    	LogFile<<"Configuration file problem"<<endl;
		return NULL;
	}
    for(unsigned char i=0; i<AnimationLimit && ConfigFile.good(); i++)
    {
    	getline(ConfigFile, FileLine);
    	if(FileLine.size() < 5)
		{
			LogFile<<"Bad configuration file at line: "<<i<<endl;
			return NULL;
		}
    	FileNames.push_back(FileLine);
	}
	if(FileNames.size() < AnimationLimit)
	{
		LogFile<<"Animations count is too low"<<endl;
		return NULL;
	}
	RetAnimations = LoadAnimation(FileNames, inCount);
	//set animation type parameter
    RetAnimations[0].Type = RetAnimations[3].Type = RetAnimations[4].Type = 2;
    RetAnimations[1].Type = RetAnimations[2].Type = 1;
    //set animation next parameter
    RetAnimations[0].Next = 1;
    RetAnimations[1].Next = 1;
	RetAnimations[2].Next = 2;
	RetAnimations[3].Next = 3;
	RetAnimations[4].Next = 4;
	
	return RetAnimations;
}

void MTextureLoader::SetBackwardRead(bool inBackwardRead)
{
	BackwardRead = inBackwardRead;
}

bool MTextureLoader::GetTGAHeader()
{
    FILE* pTGA = fopen(pFileName, "rb");
    if(!pTGA)
    {     
        LogFile<<"Cannot open file: "<<pFileName<<endl;
        return false;
    }
    if(!fread(&TGAHeader, sizeof(stTGAHeader), 1, pTGA))
    {    
        LogFile<<"Cannot read header"<<endl;
        return false;
    }
	fclose(pTGA);
	
	return true;
}

bool MTextureLoader::CreateTexture()
{
    LogFile<<"Creating static texture"<<endl;
    TextureCount = 1;
    pTexture = new stTexture [TextureCount];
    glGenTextures(1, &pTexture[0].Id);
    glBindTexture(GL_TEXTURE_2D, pTexture[0].Id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImageData);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    LogFile<<"Texture id: "<<pTexture[0].Id<<endl;
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        LogFile<<error<<" "<<(char*)gluErrorString(error)<<endl;
        return false;
    }
    
    return true;
}

bool MTextureLoader::CreateMultipleTexture()
{
    LogFile<<"Creating animated texture"<<endl; 
	unsigned int SubImageSize;
	
	if(Width == Height)
    {
        LogFile<<"Wrong texture size. This is simple quad image"<<endl;
        return false;
    }
    TextureCount = Height / Width;
    if(TextureCount * Width != Height)
    {
        LogFile<<"Wrong texture size. TextureCount * Width != Height"<<endl;
        return false;
    }
	
	try
    {
		SubImageSize = TGAData.ImageSize / TextureCount;
		pSubImageData = new unsigned char [SubImageSize];
	}
	catch(bad_alloc& ba)
	{
		LogFile<<"Cant allocate memory for subimage"<<endl;
		if(!pSubImageData) delete [] pSubImageData;
        return false;
	}
	
	pTexture = new stTexture [TextureCount];
	for(unsigned short int i=0; i<TextureCount; i++)
	{
		memset(pSubImageData, 0, sizeof(pSubImageData));
		if(!BackwardRead)
			memcpy(pSubImageData, pImageData + i * SubImageSize, (SubImageSize));
		else
			memcpy(pSubImageData, pImageData + ((TextureCount - 1) - i) * SubImageSize, (SubImageSize));
		
		glGenTextures(1, &pTexture[i].Id);
		glBindTexture(GL_TEXTURE_2D, pTexture[i].Id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Width, 0, GL_RGBA, GL_UNSIGNED_BYTE, pSubImageData);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		LogFile<<"Texture id: "<<pTexture[i].Id<<endl;
		GLenum error = glGetError();
		if( error != GL_NO_ERROR )
		{
			LogFile<<error<<" "<<(char*)gluErrorString(error)<<endl;
			if(!pSubImageData) delete [] pSubImageData;
			return false;
		}
	}
	LogFile<<"Free subimage memory"<<endl;
	if(!pSubImageData) delete [] pSubImageData;
	
	return true;
}

bool MTextureLoader::LoadTGACompressed()
{ 
	FILE* pTGA = fopen(pFileName, "rb");
	fseek(pTGA, 12, 0);
	if(!pTGA)
    {     
        LogFile<<"Cannot open file"<<endl;
        return false;
    }
	
	LogFile<<"Start reading compressed TGA file"<<endl;
	if(!fread(TGAData.Header, sizeof(TGAData.Header), 1, pTGA))
    {
        LogFile<<"Failed read TGA header"<<endl;
		fclose(pTGA);
        return false;
    }
	
    Width = TGAData.Header[1] * 256 + TGAData.Header[0];
    Height = TGAData.Header[3] * 256 + TGAData.Header[2];
    BitsPerPixel = TGAData.Header[4];
    TGAData.Width = Width;
    TGAData.Height = Height;
    TGAData.BitsPerPixel = BitsPerPixel;
    if(Width <=0 || Height <=0)
	{
		fclose(pTGA);
		LogFile<<"Null width or height"<<endl;
		return false;
	}
    if(BitsPerPixel !=24 || BitsPerPixel != 32)
	{
		fclose(pTGA);
        LogFile<<"Bits per pixel not equal nor 24 nor 32"<<endl;
		return false;
	}
    if(BitsPerPixel == 24) Type = GL_RGB;
    if(BitsPerPixel == 32) Type = GL_RGBA;
    TGAData.BytesPerPixel = TGAData.BitsPerPixel / 8;
    TGAData.ImageSize = TGAData.BytesPerPixel * TGAData.Width * TGAData.Height;
    try
    {
        pImageData = new unsigned char [TGAData.ImageSize];
    }
    catch(bad_alloc& ba)
    {
		fclose(pTGA);
		LogFile<<"Cant allocate memory"<<endl;
        return false;
    }
    
    unsigned int PixelCount = TGAData.Width * TGAData.Height;
    unsigned int CurrentPixel = 0;
    unsigned int CurrentByte = 0;
    unsigned char* ColorBuffer = new unsigned char [TGAData.BytesPerPixel];
    unsigned char ChunkHeader;
    do
    {
        ChunkHeader = 0;
        if(fread(&ChunkHeader, sizeof(unsigned char), 1, pTGA))
		{
			fclose(pTGA);
			return false;
		}
        if(ChunkHeader < 128)
        {
            ChunkHeader ++;
            for(short int i=0; i<ChunkHeader; i++)
            {
                if(fread(ColorBuffer, 1, TGAData.BytesPerPixel, pTGA))
				{
					fclose(pTGA);
					return false;
				}
                pImageData[CurrentByte] = ColorBuffer[2];
                pImageData[CurrentByte + 1] = ColorBuffer[1];
                pImageData[CurrentByte + 2] = ColorBuffer[0];
                if(TGAData.BitsPerPixel == 4) pImageData[CurrentByte + 3] = ColorBuffer[3];
                CurrentByte += TGAData.BytesPerPixel;
                CurrentPixel ++;
            }
        }
        else
        {
            ChunkHeader -= 127;
            if(fread(ColorBuffer, 1, TGAData.BytesPerPixel, pTGA))
			{
				fclose(pTGA);
				return false;
			}
            for(short int i=0; i<ChunkHeader; i++)
            {
                pImageData[CurrentByte] = ColorBuffer[2];
                pImageData[CurrentByte + 1] = ColorBuffer[1];
                pImageData[CurrentByte + 2] = ColorBuffer[0];
                if(TGAData.BitsPerPixel == 4) pImageData[CurrentByte + 3] = ColorBuffer[3];
                CurrentByte += TGAData.BytesPerPixel;
                CurrentPixel ++;
            }
        }
    }
    while(CurrentPixel < PixelCount);
    fclose(pTGA);
    
	return true;
}

bool MTextureLoader::LoadTGAUncompressed()
{
	FILE* pTGA = fopen(pFileName, "rb");
	fseek(pTGA, 12, 0);
	if(!pTGA)
    {     
        LogFile<<"Cannot open file"<<endl;
        return false;
    }
	
	LogFile<<"Start reading uncompressed TGA file"<<endl;
    if(!fread(TGAData.Header, sizeof(TGAData.Header), 1, pTGA))
    {
        LogFile<<"Failed read TGA header"<<endl;
		fclose(pTGA);
        return false;
    }
	
    Width = TGAData.Header[1] * 256 + TGAData.Header[0];
    Height = TGAData.Header[3] * 256 + TGAData.Header[2];
    BitsPerPixel = TGAData.Header[4];
    TGAData.Width = Width;
    TGAData.Height = Height;
    TGAData.BitsPerPixel = BitsPerPixel;
    LogFile<<"TGAData.Width: "<<TGAData.Width<<endl;
    LogFile<<"TGAData.Height: "<<TGAData.Width<<endl;
    LogFile<<"TGAData.BitsPerPixel: "<<TGAData.BitsPerPixel<<endl;
    if(Width <=0 || Height <=0)
    {
        LogFile<<"Width or height are zero"<<endl;
		fclose(pTGA);
        return false;
    }
    if(BitsPerPixel != 24 && BitsPerPixel != 32) 
    {
        LogFile<<"Bits per pixel not 24 and not 32: "<<BitsPerPixel<<endl;
		fclose(pTGA);
        return false;
    }
    if(BitsPerPixel == 24) Type = GL_RGB;
    if(BitsPerPixel == 32) Type = GL_RGBA;
    TGAData.BytesPerPixel = TGAData.BitsPerPixel / 8;
    TGAData.ImageSize = TGAData.BytesPerPixel * TGAData.Width * TGAData.Height;
    LogFile<<"Type: "<<Type<<endl;
    LogFile<<"TGAData.BytesPerPixel: "<<TGAData.BytesPerPixel<<endl;
    try
    {
        pImageData = new unsigned char [TGAData.ImageSize];
        memset(pImageData, 0, TGAData.ImageSize);
    }
    catch(bad_alloc& ba)
    {
        LogFile<<"Can not allocate memmory"<<endl;
		fclose(pTGA);
        return false;
    }
    
    LogFile<<"Creating texture"<<endl;
    if(fread(pImageData, 1, TGAData.ImageSize, pTGA) != TGAData.ImageSize)
    {
        LogFile<<"Cant read image data"<<endl;
		fclose(pTGA);
        return false;
    }
    LogFile<<"Replacing color bytes in image"<<endl;
    for(unsigned int i = 0; i<(int)TGAData.ImageSize; i += TGAData.BytesPerPixel)
    {
        pImageData[i] ^= pImageData[i+2] ^= pImageData[i] ^= pImageData[i+2];
    }
    fclose(pTGA);
    
    return true;
}
