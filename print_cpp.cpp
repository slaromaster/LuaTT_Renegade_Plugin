#include "General.h"
#include "print_cpp.h"
#include "engine.h"

extern "C"
void Console_Output_C(const char* format, ...)
{
	char buffer[256];
	va_list arguments;
	va_start(arguments, format);
	vsprintf(buffer, format, arguments);
	va_end(arguments);

	Console_Output("%s", buffer);
}