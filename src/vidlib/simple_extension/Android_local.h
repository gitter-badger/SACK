
#include <network.h>
#include "protocol.h"

typedef struct vidlib_proxy_image
{
	int x, y, w, h;
	int string_mode;
	int blot_method;
	struct vidlib_proxy_image *parent;
	struct vidlib_proxy_image *child;
	struct vidlib_proxy_image *next;
	struct vidlib_proxy_image *prior;

	INDEX filegroup;
	TEXTSTR filename;
	Image image;
	INDEX render_id;
	INDEX id;

	struct vidlib_proxy_image_flags {
		BIT_FIELD bUsed : 1;
	} flags;

	uint8_t* buffer;
	size_t sendlen;
	size_t buf_avail;
	uint8_t* websock_buffer;
	size_t websock_sendlen;
	size_t websock_buf_avail;
} *PVPImage;

struct server_socket_state
{
	struct server_socket_flags {
		BIT_FIELD get_length : 1;
	} flags;
	POINTER buffer;
	int read_length;
};

typedef struct vidlib_proxy_renderer
{
	uint32_t w, h;
	int32_t x, y;
	uint32_t attributes;
	struct vidlib_proxy_renderer *above, *under;
	PLIST remote_render_id;  // this is synced with same index as l.clients
	struct vidlib_proxy_image *image;  // representation of the output surface
	struct vidlib_proxy_renderer_flags
	{
		BIT_FIELD hidden : 1;
	} flags;
	INDEX id;
	MouseCallback mouse_callback;
	uintptr_t psv_mouse_callback;
	KeyProc key_callback;
	uintptr_t psv_key_callback;
	RedrawCallback redraw;
	uintptr_t psv_redraw;
} *PVPRENDER;

#define l vidlib_proxy_server_local
struct vidlib_proxy_local
{
	TEXTSTR application_title;
	PLIST renderers;
	PLIST images;
	PIMAGE_INTERFACE real_interface;
	uint8_t key_states[256];
	CRITICALSECTION message_formatter;
} l;

CTEXTSTR SACK_Vidlib_GetKeyText( int pressed, int key_index, int *used );
void SACK_Vidlib_ProcessKeyState( int pressed, int key_index, int *used );


