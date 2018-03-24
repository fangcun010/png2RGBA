#include <iostream>
#include <cstring>
#include <png.h>

using namespace std;

#define             VERSION             0.5

unsigned int Width,Height;
unsigned char *pData;

bool LoadFromFile(const char *strFile)
{
	png_image img;
	unsigned char *pBuffer=NULL;
	memset(&img,0,sizeof img);

	img.version=PNG_IMAGE_VERSION;

    if (png_image_begin_read_from_file(&img, strFile))
    {
        img.format = PNG_FORMAT_RGBA;

        pBuffer= new unsigned char[PNG_IMAGE_SIZE(img)];
        pData = new unsigned char[PNG_IMAGE_SIZE(img)];

        if (png_image_finish_read(&img , NULL, pBuffer,0, NULL))
        {
			Width=img.width;
			Height=img.height;

			for(int i=0;i<Height;i++)
			for(int j=0;j<Width;j++)
			{
                unsigned int pos=(Height-i-1)*Width*4+j*4;
				unsigned int tpos=i*Width*4+j*4;

                pData[tpos]=pBuffer[pos];
                pData[tpos+1]=pBuffer[pos+1];
                pData[tpos+2]=pBuffer[pos+2];
                pData[tpos+3]=pBuffer[pos+3];
			}

			delete []pBuffer;
			png_image_free(&img);
			return true;
        }
        else
		{
			if(pBuffer) delete []pBuffer;
			if(pData)
			{
				delete []pData;
				pData=NULL;
			}
			png_image_free(&img);
		}
    }

    return false;
}

int main(int argc,char *argv[])
{
        printf("png2RGBA Version %.1f\n",VERSION);
    if(argc!=3)
    {
        printf("Usage:png2RGBA pngFile RGBAFile\n");

        return 1;
    }

    if(LoadFromFile(argv[1]))
    {
        puts("Converting...");

        FILE *fp=fopen(argv[2],"wb");

        if(!fp)
        {
            printf("Write File:%s Error!\n",argv[2]);
            return 1;
        }
        else
        {
            fwrite(&Width,sizeof Width,1,fp);
            fwrite(&Height,sizeof Height,1,fp);
            fwrite(pData,4*Width*Height,1,fp);
        }

        fclose(fp);
    }
    else
    {
        printf("Read File:%s Error!\n",argv[1]);
        return 1;
    }

    delete []pData;

    puts("OK!");

    return 0;
}
