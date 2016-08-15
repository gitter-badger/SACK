#include <sharemem.h>
#define DEFINE_DEFAULT_IMAGE_INTERFACE
#include <image.h>
#include <controls.h>
//#include <buttons.h>
//  #include <pssql.h>
//  #include <getopt.h>
#include <systray.h>
#include <msgclient.h>
#include "msgid.h"


typedef struct {
	struct{
		uint32_t bInited:1;
		uint32_t bDisplayed:1;
	}flags;

	struct{
		struct{
			uint32_t connected:1;
			uint32_t disconnected:1;
		}flags;
      PSERVICE_ROUTE MsgBase;
	}systrayserver;
	void (*DblClkCallback)(void);
   //
} GLOBAL;

#define g global_icon_data
static GLOBAL g;

//----------------------------------------------------------------------


//the handling of events generated by the server need to be handled here, like an event handler somewhereelse, sort of.
static int CPROC HandleEvents( PSERVICE_ROUTE SourceID, MSGIDTYPE MsgID
										 , uint32_t *params, size_t param_length
										 /*, uint32_t *result, uint32_t *result_length*/ )
{
   int retval;

	lprintf(WIDE("Welcome to EventHandler <HandleEvents>.  MsgID is %") _MsgID_f , MsgID);

	if ( !param_length && !MsgID )
      return 0;

	switch( MsgID )
	{
	case MSG_MateEnded:
		lprintf("EventHandler <HandleEvents>:MateEnded");
	// result_length passed here is NULL...
      //(*result_length) = INVALID_INDEX;
      return 0;
	case MSG_MateStarted:
		lprintf(WIDE("EventHandler <HandleEvents>.:MateStarted , client XX") WIDE(" is connecting to %s"), (char*)params );


      break;
//  	case MSG_POSClientReady:
//  		{
//           break;
//  		}
	default:
		{
			lprintf( WIDE("EventHandler <HandleEvents>.:Received message %") _MsgID_f WIDE(" from %") _32f WIDE("\n"), MsgID, 0 );
			retval = 0;
			break;
		}
	}
   return 0;
}



//----------------------------------------------------------------------

static int ConnectToServer( void )
{
	if( g.systrayserver.flags.disconnected )
	{
      xlprintf(LOG_ALWAYS)("systray server is unexpectedly disconnected?!. Bailing.");
		return FALSE;
	}
   if( !g.systrayserver.flags.connected )
   {
      xlprintf(LOG_ALWAYS)("systray server not connected, trying to connect.");
		//if( InitMessageService() )
      {
         g.systrayserver.MsgBase = LoadServiceEx( WIDE("systray"), HandleEvents );
         xlprintf(LOG_ALWAYS)( WIDE("systray message base is %p"), g.systrayserver.MsgBase );
         if( g.systrayserver.MsgBase )
            g.systrayserver.flags.connected = 1;
      }
   }
   if( !g.systrayserver.flags.connected )
      xlprintf(LOG_ALWAYS)( WIDE("Failed to connect") );
   return g.systrayserver.flags.connected;
}



//  // change the parameters to fit 
//  void CPROC clientf(Image image, int32_t x, int32_t y, CDATA c )
//  {
//  	if( !ConnectToServer() ) return;
//  	//lprintf( WIDE("Do plot color...") );
//     TransactServerMultiMessage( MSG_clientf, 4
//                               , NULL, NULL, 0
//                               , SafeNULL(MyImage), 2
//                               , &x, (ParamLen(x,c))
//                               , &xx, (ParamLen(x,c));
//  }

//----------------------------------------------------------------------


// change the parameters to fit 
int CPROC RegisterIconEx(CTEXTSTR icon DBG_PASS)
{
   xlprintf(LOG_NOISE)("systray_client's RegisterIconEx called for %s", icon);
	if( !ConnectToServer() ) return 0;

//  	TransactServerMultiMessage( MSG_RegisterIcon, 1
//                               , NULL, NULL, 0
//                               , icon, (strlen(icon) + 1 )
//  									  );

	if( !TransactServerMessage( g.systrayserver.MsgBase, MSG_RegisterIcon
									  , icon , (strlen(icon)+1)
									  , NULL, NULL, 0 ) )
	{
      xlprintf(LOG_ALWAYS)("ANNOOUNCEMENT! RegisterIconEx's TransactServerMessage returned FALSE");
	}
	else
	{
		xlprintf(LOG_ALWAYS)("ANNOOUNCEMENT! RegisterIconEx's TransactServerMessage returned TRUE");
	}

	xlprintf(LOG_NOISE)("I guess systray_client's RegisterIconEx successful for %s returning 2", icon);
   return 2;
}

//----------------------------------------------------------------------

void CPROC ChangeIconEx( CTEXTSTR icon DBG_PASS )
{
   xlprintf(LOG_ALWAYS)("Hey, thanks for trying to changeicon %s", icon);
}


//----------------------------------------------------------------------

void UnregisterIconEx( char * a DBG_PASS )
{

	if( !TransactServerMessage( g.systrayserver.MsgBase, MSG_UnregisterIcon
									  , a , (strlen(a)+1)
									  , NULL, NULL, 0 ) )
	{
	}
	else
	{
		xlprintf(LOG_NOISE)("Consider the icon unregistered." );
	}

}

//----------------------------------------------------------------------

void SetIconDoubleClick( void (*DoubleClick)(void ) )
{
	g.DblClkCallback = DoubleClick;
}

void TerminateIcon( void )
{
}

void AddSystrayMenuFunction(
									CTEXTSTR text, void (CPROC*function)(void) )
{
}

