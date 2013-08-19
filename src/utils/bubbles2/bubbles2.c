#define DEFINE_DEFAULT_RENDER_INTERFACE
#define DEFINE_DEFAULT_IMAGE_INTERFACE

#include <render.h>
#include <image.h>

typedef struct bubble_thing
{
	CDATA base;
	CDATA dest;
	CDATA current;
	int length;
	int step;
	int x, y;

	int over; // boolean, starts as over others... so no colision

   int delx, dely;
} BUBBLE, *PBUBBLE;

struct {
	Image shadow;
	Image shaded;
	Image cover;
	_32 w, h;
	PRENDERER render;

	CDATA base;
	CDATA dest;
	CDATA current;

	PLIST bubbles;
} l;


void ChooseColorDest( PBUBBLE bubble )
{
	if( bubble->step >= bubble->length )
	{
		bubble->base = bubble->dest;
		bubble->step = 0;
		bubble->length = ( rand() * 12L / RAND_MAX ) + 1;
		bubble->dest = AColor( rand() * 256 / RAND_MAX
									, rand() * 256 / RAND_MAX
									, rand() * 256 / RAND_MAX
									, 255
									 );
	}
	bubble->current = ColorAverage( bubble->base, bubble->dest
											, bubble->step++, bubble->length );
}

void MoveBubbles( void )
{
	INDEX idx;
	PBUBBLE bubble;
	LIST_FORALL( l.bubbles, idx, PBUBBLE, bubble )
	{
      bubble->x += bubble->delx;
		bubble->y += bubble->dely;
		if( !bubble->delx || bubble->x > l.w )
         bubble->delx = -((rand() * 12 / RAND_MAX)+1);
		if( !bubble->dely || bubble->y > l.h )
         bubble->dely = -((rand() * 12 / RAND_MAX)+1);
		if( bubble->x < 0 )
         bubble->delx = ((rand() * 12 / RAND_MAX)+1);
		if( bubble->y < 0 )
			bubble->dely = ((rand() * 12 / RAND_MAX)+1);

	}
}

void DrawBubbles( Image image, int x, int y, CDATA c )
{
	//BlotImageAlpha( image, l.shadow, x+20, y+20, ALPHA_TRANSPARENT_INVERT+128 );

   BlotScaledImageSizedToShadedAlpha( image, l.shaded, x, y, 150, 150, 1, c );
   BlotScaledImageSizedToAlpha( image, l.cover, x, y, 150, 150, ALPHA_TRANSPARENT );
   //BlotImageAlpha( image, l.cover, x+0, y+0, ALPHA_TRANSPARENT );
	//BlotImageAlphaShaded( image, l.shaded, x+0, y+0, ALPHA_TRANSPARENT, c );
}

void DrawAllBubbles( Image image )
{
	INDEX idx;
	PBUBBLE bubble;
	LIST_FORALL( l.bubbles, idx, PBUBBLE, bubble )
	{
      ChooseColorDest( bubble );
      DrawBubbles( image, bubble->x, bubble->y, bubble->current );
	}
}

void CPROC UpdateImage( PTRSZVAL psv, PRENDERER renderer )
{
	Image image = GetDisplayImage( renderer );
	ClearImage( image );
	DrawAllBubbles( image );
//	DrawBubbles( image, 300, 100, 0 );
  // DrawBubbles( image, 0, 0, 1 );
	//DrawBubbles( image, 100, 300, 2 );
	UpdateDisplay( renderer );
}

void CPROC Ticker( PTRSZVAL psv )
{
	MoveBubbles();
	Redraw( l.render );
}


int main( int argc, char **argv )
{
	int x, y, w, h;
	x = argc > 1?atoi(argv[1]):0;
	y = argc > 2?atoi(argv[2]):0;
	w = argc > 3?atoi(argv[3]):0;
	h = argc > 4?atoi(argv[4]):0;
	l.shadow = LoadImageFile( "117.png" );
	l.cover = LoadImageFile( "123.png" );
	l.shaded = LoadImageFile( "121.png" );
	{
		GetDisplaySize( &l.w, &l.h );
		if( w )
			l.w = w;
		if( h )
         l.h = h;
		l.render = OpenDisplaySizedAt( DISPLAY_ATTRIBUTE_NO_AUTO_FOCUS|DISPLAY_ATTRIBUTE_NO_MOUSE|DISPLAY_ATTRIBUTE_CHILD|DISPLAY_ATTRIBUTE_LAYERED
											  , l.w, l.h, x, y );
		SetRedrawHandler( l.render, UpdateImage, 0 );
		MakeTopmost( l.render );
		UpdateDisplay( l.render );
		{
			int n;
			int x;
			for( n = 0; n < 40; n++ )

			{
				PBUBBLE bubble = New( BUBBLE );
				MemSet( bubble, 0, sizeof( BUBBLE ) );
				bubble->x = n * 160;
				while( bubble->x > 1650 )
				{
					bubble->x -= 1650;
               bubble->y += 160;
				}
				AddLink( &l.bubbles, bubble );
			}
		}


		AddTimer( 1000/24, Ticker, 0 );
		while( DisplayIsValid( l.render ) )
			WakeableSleep( 1000 );
	}
	return 0;
}

