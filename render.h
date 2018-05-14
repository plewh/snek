#pragma once

#include "event.h"
#include "defs.h"

void r_Init();
void r_Cleanup();
void r_Responder(event_t* ev);
void r_DoFrame();
void r_Tick();
