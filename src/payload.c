#include <woody.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

char **singleton_payload(void)
{
	static char *payload;

	return (&payload);
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

char *get_payload(void)
{
	return *singleton_payload();
}

void set_payload(void)
{
	*singleton_payload() =
		"\x57\x56\x52\xe8\x0c\x00\x00\x00\x2e\x2e"
		"\x2e\x57\x4f\x4f\x44\x59\x2e\x2e\x2e\x0a"
		"\xbf\x01\x00\x00\x00\x5e\xba\x0c\x00\x00"
		"\x00\xb8\x01\x00\x00\x00\x0f\x05\x5a\x5e"
		"\x5f\xe9\xfc\xff\xff\xff";
}
