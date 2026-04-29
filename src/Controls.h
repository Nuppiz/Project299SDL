#ifndef CONTROLS_H
#define CONTROLS_H

#include "Commonstruct.h"

extern Vec2 mousePos;
extern bool mouseLeftClicked;

void updateControls();
void handleControlEvent(const SDL_Event& e);

#endif /* CONTROLS_H */
