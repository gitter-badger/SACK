#include <msgclient.h>

sttic struct {
	struct {
		uint32_t connected : 1; // have we already connected?
	} flags;

	// once we connect to the server, this base
	// is used to bias all messages which are enumerated
	// from 0, and are often direct indexes into a table
   // of server message handling functions.
	uint32_t MsgBase;
} l;

static void CPROC EventProcessor( uint32_t EventMsg, uint32_t *data, uint32_t length )
{
	// messages may come back as events....
	// they are not the result of a previously sent
	// message.  This would be a callback invocation in
   // a tightly coupled environment.
	switch( EventMsg )
	{
	case MSG_MateEnded:
		// server closed - all client resources defunct.
		break;
	case MSG_DispatchPending:
		// the server has no further events, events
		// which may have been gathered should now be
		// dispatched, there will be no further notice
      // until another real event happens.
		break;
	case MSG_ServiceClose:
      // attempt to gracefully handle the server going away.
		break;
	case MSG_EventUser:
		// this message, and all further messages enumerated
      // from this value are events which are user defined.
      break;
	}
}

static int CPROC ConnectToServer( void )
{
   AddIdleProc( (IdleProc)ProcessDisplayMessages, 0 );
	if( !l.flags.connected )
	{
		if( InitMessageService() )
		{
			l.MsgBase = LoadService( WIDE("some_service_name"), DisplayEventProcessor );
			if( l.MsgBase )
			{
				l.flags.connected = 1;
				LocalInit();  // allow this local library to do init upon connect
            // perhaps coordinate with the service certain startup information.
			}
		}
	}
	if( !l.flags.connected )
		Log( WIDE("Failed to connect") );
   return l.flags.connected;
}

static void DisconnectFromServer( void )
{
	if( l.flags.connected )
	{
      Log( WIDE("Disconnecting from server (display)") );
		UnloadService( l.MsgBase );
		l.flags.connected = 0;
      l.MsgBase = 0;
	}
}

//-------------------------------------------------------------------------
//  BEGIN User Implmentation.
//-------------------------------------------------------------------------
.// all of these are example functions only, 1 simple send with
// no required responce.

PUBLIC( void, SimpleVoidProc )( uint32_t width, uint32_t height )
{
   if( !ConnectToServer() ) return;
	SendServerMessage( MSG_SimpleVoidProc + g.MsgBase, &width, ParamLength( width, height ) );
	// this may also be implemented with transact message
   // with a NULL result buffer...
	//TransactServerMessage( MSG_MoveDisplay, &display, ParamLength( display, y )
	//							, NULL, NULL, NULL );
}

PUBLIC( PRENDERER, OpenDisplayAboveSizedAt )   ( uint32_t attributes, uint32_t width, uint32_t height, int32_t x, int32_t y, PRENDERER parent )
{
	if( ConnectToServer() )
	{
      // this is the message_id of the responce.
		uint32_t Responce;
      // this is the message data result
		uint32_t data[5];
		// this is the result length of the buffer, it
		// is initially the max size of the responce buffer,
		// it is updated upon successful transmission/responce
      // to be the actual length of data returned.
		uint32_t len = 20;
		if( TransactServerMessage( MSG_OpenDisplayAboveSizedAt // mesasge ID
										 , &attributes, 6 * sizeof( uint32_t ) // out message data, and length
										 , &Responce, data, &len )  // receive information
			&& // both conditions MUST be true for success.

			// The responce is generated by the server library itself, and
			// will be the initial message ID or'ed with a status...
			Responce == (MSG_OpenDisplayAboveSizedAt|SERVER_SUCCESS)
		  )
		{
         PRENDERER renderer = NULL; // just so this compiles...
         // handle the server's data returned.
			return renderer;
		}
	}
   return (PRENDERER)NULL;
}

//-------------------------------------------------------------------------
// and now, all that is required for an application to link to this
// and use these functions is done.

// The following step is to make this a swappable interface, which
// the application might be able to load, which will make it
// compatible with a library which it may use tightly coupled-like.

// alternative ends of this might be a main()  which uses all of these
// things itself, however, the functions would probably nto be decalred as PUBLIC() then.

//-------------------------------------------------------------------------
// TBI (to be implemented)
//-------------------------------------------------------------------------


