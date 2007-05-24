#ifndef __imageio_h__
#define __imageio_h__

/* image ctor, gets width, height of the image and returning array of pointers to rows */
typedef void* ImageCtor(void* pctorPrm, unsigned int width, unsigned int height, int bytesPerPixel, unsigned char** rowPtrs);

extern int DecodeImage(ImageCtor* pctor, void* pctorPrm, unsigned char* src, unsigned int srclength);

//int EncodeImage(StreamCtl* pctor, void* StreamCtlPrm, unsigned int width, unsigned int height, int bytesPerPixel, unsigned char** rowPtrs);

#endif
