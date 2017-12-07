
#include <stdio.h>
#include <stdarg.h>
#include "Log.hpp"

//DLT_DECLARE_CONTEXT(TelematicsContext);

#define TELEMATICS_LOG_TAG "[Telematics]"

void initDltViewer(void)
{
    //DLT_REGISTER_APP("Telematics", "TelematicsService");
    //DLT_REGISTER_CONTEXT(TelematicsContext, "Telematics", "TelematicsService");
}

void printMessages(int type, const char *file, int line, const char *msgType, const char *format, ... )
{
    do {
      char buffer[2056]; //Max 4096 bytes can be routed more than this would cause a crash.
      char tmpbuf[1024];
      {
        va_list args;
        va_start (args, format);
        vsnprintf (tmpbuf, sizeof (tmpbuf), format, args);
        va_end (args);
      }
      (void)msgType;
      snprintf (buffer, sizeof (buffer), "%s [%s:%d] : %s", TELEMATICS_LOG_TAG, FILENAME(file), line, tmpbuf);
      //DLT_LOG (TelematicsContext, type, DLT_STRING(buffer));
      std::cout << buffer << std::endl;
    }while(0);
}
