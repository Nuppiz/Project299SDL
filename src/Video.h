#ifndef VIDEO_H
#define VIDEO_H

// Video system core elements

class VideoSystem
{
public:
    int initVideoSystem();
    void deInitVideoSystem();
    SDL_Window* window;
    SDL_Renderer* renderer;
};

#endif /* VIDEO_H */
