#ifndef TILE_H
#define TILE_H

#include "Commoninc.h"

enum TileType
{
    TILE_EMPTY = 0,
    TILE_WALL  = 1
};

const int TILE_SIZE = 32;
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 15;

extern int tileMap[MAP_HEIGHT][MAP_WIDTH];

class Tile
{
public:
    static bool loadFromFile(const std::string& path);
    static bool loadTexture(SDL_Renderer* renderer, const std::string& filename);
    static void draw(SDL_Renderer* renderer);
    static bool isWall(int col, int row);
    static bool collidesWithWalls(float x, float y, float size);

    static SDL_Texture* wallTexture;
    static std::string  wallTextureFile;
};

#endif /* TILE_H */
