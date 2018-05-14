#pragma once

#include "render.h"
#include "frt.h"
#include "snk.h"

void   gs_Init();
void   gs_Cleanup();
void   gs_Tick();
void   gs_Responder();
snk_t const* gs_GetSnake();
frt_t const* gs_GetFruit();
