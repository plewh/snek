#pragma once

#include "event.h"

void app_Init();
void app_Run();
void app_Cleanup();
void app_RequestQuit();
void app_Responder(event_t* ev);
