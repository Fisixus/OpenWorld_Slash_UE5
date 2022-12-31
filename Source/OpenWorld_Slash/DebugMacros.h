#pragma once

#include "DrawDebugHelpers.h"

#define DEBUG_PRINT_SCREEN(Key, Message) if(GEngine)  GEngine->AddOnScreenDebugMessage(Key, 30.f, FColor::Magenta, Message);

#define DEBUG_DRAW_SPHERE(Location, Color) if(GetWorld())  DrawDebugSphere(GetWorld(), Location, 25.f, 12, Color, true);
#define DEBUG_DRAW_SPHERE_SingleFrame(Location, Color) if(GetWorld())  DrawDebugSphere(GetWorld(), Location, 25.f, 12, Color, false, -1);

#define DEBUG_DRAW_LINE(StartLoc, EndLoc, Color) if(GetWorld())  DrawDebugLine(GetWorld(), StartLoc, EndLoc, Color, true, -1, 0, 1);
#define DEBUG_DRAW_LINE_SingleFrame(StartLoc, EndLoc, Color) if(GetWorld())  DrawDebugLine(GetWorld(), StartLoc, EndLoc, Color, false, -1, 0, 1);

#define DEBUG_DRAW_POINT(Location, Color) if(GetWorld())  DrawDebugPoint(GetWorld(), Location, 15.f, Color, true);
#define DEBUG_DRAW_POINT_SingleFrame(Location, Color) if(GetWorld())  DrawDebugPoint(GetWorld(), Location, 15.f, Color, false, -1);

#define DEBUG_DRAW_VECTOR(StartLoc, EndLoc, Color)  if(GetWorld()) \
{ \
DEBUG_DRAW_LINE(StartLoc, EndLoc, Color) \
DEBUG_DRAW_POINT(EndLoc, Color)\
}

#define DEBUG_DRAW_VECTOR_SingleFrame(StartLoc, EndLoc, Color)  if(GetWorld()) \
{ \
DEBUG_DRAW_LINE_SingleFrame(StartLoc, EndLoc, Color) \
DEBUG_DRAW_POINT_SingleFrame(EndLoc, Color)\
}


