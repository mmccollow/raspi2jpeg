#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wand/MagickWand.h>
#include "bcm_host.h"

#ifndef ALIGN_TO_16
#define ALIGN_TO_16(x)  ((x + 15) & ~15)
#endif

int main(int argc, char *argv[]) {
  VC_IMAGE_TYPE_T imageType = VC_IMAGE_RGBA32;
  int8_t dmxBytesPerPixel = 4;
  int result = 0;
  char *program = argv[0];

  // initialize broadcom stuff
  bcm_host_init();

  // get a handle to the display
  DISPMANX_DISPLAY_HANDLE_T displayHandle
    = vc_dispmanx_display_open(0);

  // get display info struct for width and height
  DISPMANX_MODEINFO_T modeInfo;
  vc_dispmanx_display_get_info(displayHandle, &modeInfo);

  int32_t dmxWidth = modeInfo.width;
  int32_t dmxHeight = modeInfo.height;

  int32_t dmxPitch = dmxBytesPerPixel * ALIGN_TO_16(dmxWidth);

  // this will hold the pixels from the screenshot
  void *dmxImagePtr = malloc(dmxPitch * dmxHeight);

  uint32_t vcImagePtr = 0;
 	DISPMANX_RESOURCE_HANDLE_T resourceHandle;
  resourceHandle = vc_dispmanx_resource_create(imageType,
                                               dmxWidth,
                                               dmxHeight,
                                               &vcImagePtr);

  result = vc_dispmanx_snapshot(displayHandle,
                                resourceHandle,
                                DISPMANX_NO_ROTATE);
  if (result != 0) {
    vc_dispmanx_resource_delete(resourceHandle);
    vc_dispmanx_display_close(displayHandle);

    fprintf(stderr, "%s: vc_dispmanx_snapshot() failed\n", program);
    exit(EXIT_FAILURE);
  }

  VC_RECT_T rect;
  result = vc_dispmanx_rect_set(&rect, 0, 0, dmxWidth, dmxHeight);

  if (result != 0) {
    vc_dispmanx_resource_delete(resourceHandle);
    vc_dispmanx_display_close(displayHandle);

    fprintf(stderr, "%s: vc_dispmanx_rect_set() failed\n", program);
    exit(EXIT_FAILURE);
  }

  result = vc_dispmanx_resource_read_data(resourceHandle,
                                          &rect,
                                          dmxImagePtr,
                                          dmxPitch);


  if (result != 0) {
    vc_dispmanx_resource_delete(resourceHandle);
    vc_dispmanx_display_close(displayHandle);

    fprintf(stderr,
            "%s: vc_dispmanx_resource_read_data() failed\n",
            program);

    exit(EXIT_FAILURE);
  }

  vc_dispmanx_resource_delete(resourceHandle);
  vc_dispmanx_display_close(displayHandle);
  
  // initialize MagickWand
  MagickWand *wand = NULL;
  MagickWandGenesis();
  wand = NewMagickWand();
  
  // load our pixels into an Image
  MagickConstituteImage(wand, dmxWidth, dmxHeight, "RGBA", CharPixel, dmxImagePtr);
  
  // resize our image to 1/4 original
  MagickResizeImage(wand, dmxWidth/4, dmxHeight/4, LanczosFilter, 1);

  // set compression quality (1-100, higher = lower compression/better quality)
  MagickSetImageCompressionQuality(wand, 75);

  // write our image out
  MagickWriteImage(wand, "screenshot.jpg");

  // clean up 
  if (wand)
    wand = DestroyMagickWand(wand);
  free(dmxImagePtr);

  return(0);
}
