#ifndef DISPLAY_HANDLER_H
#define DISPLAY_HANDLER_H

#include <TinyScreen.h>

void initDisplayHandler(TinyScreen* display);
void displayReminder(const char* medicationName);
void displayAcknowledgment();

#endif