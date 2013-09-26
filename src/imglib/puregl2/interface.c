#ifndef IMAGE_LIBRARY_SOURCE
#define IMAGE_LIBRARY_SOURCE
#endif
#define STRUCT_ONLY
#include <stdhdrs.h>
#include <imglib/imagestruct.h>
#include <procreg.h>
#include <sqlgetoption.h>
#undef IMAGE_SOURCE
#include <image.h>
#include <image3d.h>
#include "../fntglobal.h"
#include "local.h"
#include "shaders.h"

IMAGE_NAMESPACE

IMAGE_INTERFACE RealImageInterface = {
  SetStringBehavior
, SetBlotMethod

, BuildImageFileEx               
, MakeImageFileEx                
, MakeSubImageEx                 
, RemakeImageEx                  
, LoadImageFileEx
, UnmakeImageFileEx              

, SetImageBound
, FixImagePosition

, ResizeImageEx                  
, MoveImage                      

   , BlatColor                      
   , BlatColorAlpha                 
   , BlotImageEx                    
   , BlotImageSizedEx               
   , BlotScaledImageSizedEx         

   //, &plotraw                        
#ifdef STUPID_NO_DATA_EXPORTS
   , &_plot                           
   , &_plotalpha                      
   , &_getpixel

   , &_do_line                        
   , &_do_lineAlpha                   

   , &_do_hline                       
   , &_do_vline                       
   , &_do_hlineAlpha                  
   , &_do_vlineAlpha
#else
   , &plot                           
   , &plotalpha                      
   , &getpixel

   , &do_line                        
   , &do_lineAlpha                   

   , &do_hline                       
   , &do_vline                       
   , &do_hlineAlpha                  
   , &do_vlineAlpha
#endif
   , GetDefaultFont
   , GetFontHeight
   , GetStringSizeFontEx

   , PutCharacterFont               
   , PutCharacterVerticalFont       
   , PutCharacterInvertFont         
   , PutCharacterVerticalInvertFont 

   , PutStringFontEx
   , PutStringVerticalFontEx
   , PutStringInvertFontEx
   , PutStringInvertVerticalFontEx

   //, PutMenuStringFontEx
   //, PutCStringFontEx
   , GetMaxStringLengthFont
, GetImageSize
, LoadFont
                                 , UnloadFont
                                 , NULL // begin transfer
                                 , NULL // contin transfer
                                 , NULL // decode image
                                 , NULL // accept font
#ifdef STUPID_NO_DATA_EXPORTS
												 , &_ColorAverage
#else
												 , &ColorAverage
#endif
                                 , SyncImage
                                , GetImageSurface
                                , IntersectRectangle
                                , MergeRectangle
                                , GetImageAuxRect
                                , SetImageAuxRect
                                , OrphanSubImage
                                , AdoptSubImage

,MakeSpriteImageFileEx
,MakeSpriteImageEx
,rotate_scaled_sprite
,rotate_sprite
													 ,NULL //BlotSprite
                                        ,DecodeMemoryToImage

													 ,InternalRenderFontFile
													 ,InternalRenderFont

													 ,RenderScaledFontData
													 ,RenderFontFileScaledEx
												 , DestroyFont
                                     , NULL // *global_font_data
												 , GetFontRenderData
                                     , SetFontRendererData

												 , SetSpriteHotspot
												 , SetSpritePosition
                                     , UnmakeSprite
												 , GetGlobalFonts
												 , GetStringRenderSizeFontEx
												 , LoadImageFileFromGroupEx
                                     , RenderScaledFont
                                     , RenderScaledFontEx

									 , GetRedValue     
									 , GetGreenValue   
									 , GetBlueValue    
									 , GetAlphaValue   
									 , SetRedValue     
									 , SetGreenValue   
									 , SetBlueValue    
									 , SetAlphaValue   
									 , MakeColor       
												 , MakeAlphaColor
												 , GetImageTransformation
												 , SetImageRotation
												 , RotateImageAbout
												 , MarkImageDirty

                                     , NULL  // DumpFontCache
                                     , NULL  // RerenderFont

												 , ReloadOpenGlTexture
												 , ReloadOpenGlShadedTexture
												 , ReloadOpenGlMultiShadedTexture
												 , SetImageTransformRelation
                                     , Render3dImage
                                     , DumpFontFile

};


IMAGE_3D_INTERFACE Image3dInterface = {
	GetShader,
      CompileShader,
      CompileShaderEx,
		EnableShader,
		SetShaderEnable,
		SetShaderModelView,
};

#undef GetImageInterface
#undef DropImageInterface
#undef GetImage3dInterface

static PIMAGE_INTERFACE CPROC GetImageInterface ( void )
{
	return (PIMAGE_INTERFACE)&RealImageInterface;
}

static POINTER CPROC GetImage3dInterface( void )
{
	return &Image3dInterface;
}

static void  CPROC DropImage3dInterface ( POINTER p )
{
	;
}


static void  CPROC DropImageInterface ( PIMAGE_INTERFACE p )
{
	;
}

#ifdef __WATCOM_CPLUSPLUS__
#pragma initialize 45
#endif


PRIORITY_PRELOAD( ImageRegisterInterface, IMAGE_PRELOAD_PRIORITY )
{
	RegisterInterface( WIDE("puregl2.image"), (void*(CPROC*)(void))GetImageInterface, (void(CPROC*)(void*))DropImageInterface );
	RegisterInterface( WIDE("puregl2.image.3d"), GetImage3dInterface, DropImage3dInterface );
	l.scale = (RCOORD)SACK_GetProfileInt( GetProgramName(), "SACK/Image Library/Scale", 10 );
	if( l.scale == 0.0 )
	{
		l.scale = (RCOORD)SACK_GetProfileInt( GetProgramName(), "SACK/Image Library/Inverse Scale", 2 );
		if( l.scale == 0.0 )
			l.scale = 1;
	}
	else
		l.scale = 1.0f / l.scale;

   // this initializes some of the interface methods
   SetBlotMethod( BLOT_C );
}

int link_interface_please;
IMAGE_NAMESPACE_END


// $Log: interface.c,v $
// Revision 1.27  2005/04/13 18:29:14  jim
// Export DecodeMemoryToImage in interface.
//
// Revision 1.26  2005/04/05 11:56:04  panther
// Adding sprite support - might have added an extra draw callback...
//
// Revision 1.25  2005/01/26 06:51:58  panther
// Make image interface declaration static (private)
//
// Revision 1.24  2005/01/18 10:48:19  panther
// Define image interface export so there's no conflict between image and display_image
//
// Revision 1.23  2004/10/25 10:39:58  d3x0r
// Linux compilation cleaning requirements...
//
// Revision 1.22  2004/10/04 20:08:38  d3x0r
// Minor adjustments for static linking
//
// Revision 1.21  2004/06/21 07:47:13  d3x0r
// Account for newly moved structure files.
//
// Revision 1.20  2004/03/04 01:09:50  d3x0r
// Modifications to force slashes to wlib.  Updates to Interfaces to be gotten from common interface manager.
//
// Revision 1.19  2003/09/19 16:40:35  panther
// Implement Adopt and Orphan sub image - for up coming Sheet Control
//
// Revision 1.18  2003/09/18 12:14:49  panther
// MergeRectangle Added.  Seems Control edit near done (fixing move/size errors)
//
// Revision 1.17  2003/09/15 17:06:37  panther
// Fixed to image, display, controls, support user defined clipping , nearly clearing correct portions of frame when clearing hotspots...
//
// Revision 1.16  2003/08/30 10:05:01  panther
// Fix clipping blotted images beyond dest boundries
//
// Revision 1.15  2003/07/24 15:21:34  panther
// Changes to make watcom happy
//
// Revision 1.14  2003/04/24 00:03:49  panther
// Added ColorAverage to image... Fixed a couple macros
//
// Revision 1.13  2003/03/29 15:52:53  panther
// Add DropImageInterface
//
// Revision 1.12  2003/03/25 08:45:51  panther
// Added CVS logging tag
//
