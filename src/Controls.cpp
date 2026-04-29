#include "Commoninc.h"
#include "Controls.h"

Vec2 mousePos       = { 0.0f, 0.0f };
bool mouseLeftClicked = false;

void updateControls()
{
    float mx, my;
    SDL_GetMouseState(&mx, &my);
    mousePos.x    = mx;
    mousePos.y    = my;
    mouseLeftClicked = false;
}

void handleControlEvent(const SDL_Event& e)
{
    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT)
        mouseLeftClicked = true;
}
