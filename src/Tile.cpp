#include <fstream>
#include <string>
#include "Tile.h"

// MAP_HEIGHT rows, MAP_WIDTH columns
int tileMap[MAP_HEIGHT][MAP_WIDTH] = {};

SDL_Texture* Tile::wallTexture    = nullptr;
std::string  Tile::wallTextureFile = "";

bool Tile::loadTexture(SDL_Renderer* renderer, const std::string& filename)
{
    wallTextureFile = filename;
    wallTexture = IMG_LoadTexture(renderer, filename.c_str());
    if (!wallTexture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load tile texture: %s", filename.c_str());
        return false;
    }
    return true;
}

bool Tile::loadFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to open level file: %s", path.c_str());
        return false;
    }

    std::string line;
    int row = 0;

    while (std::getline(file, line) && row < MAP_HEIGHT)
    {
        for (int col = 0; col < MAP_WIDTH && col < (int)line.size(); col++)
        {
            if      (line[col] == '1') tileMap[row][col] = TILE_WALL;
            else if (line[col] == '0') tileMap[row][col] = TILE_EMPTY;
        }
        row++;
    }

    if (row < MAP_HEIGHT)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Level file has too few rows: %s", path.c_str());
        return false;
    }

    return true;
}

void Tile::draw(SDL_Renderer* renderer)
{
    for (int row = 0; row < MAP_HEIGHT; row++)
    {
        for (int col = 0; col < MAP_WIDTH; col++)
        {
            if (tileMap[row][col] == TILE_WALL)
            {
                SDL_FRect rect {
                    (float)(col * TILE_SIZE),
                    (float)(row * TILE_SIZE),
                    (float)TILE_SIZE,
                    (float)TILE_SIZE
                };
                if (wallTexture)
                    SDL_RenderTexture(renderer, wallTexture, nullptr, &rect);
                else
                {
                    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
    }
}

bool Tile::isWall(int col, int row)
{
    if (col < 0 || col >= MAP_WIDTH || row < 0 || row >= MAP_HEIGHT)
        return true; // Treat out-of-bounds as a wall
    return tileMap[row][col] == TILE_WALL;
}

bool Tile::collidesWithWalls(float x, float y, float size)
{
    // Check all four corners of the square against the tile grid
    int left   = (int)(x / TILE_SIZE);
    int right  = (int)((x + size - 1) / TILE_SIZE);
    int top    = (int)(y / TILE_SIZE);
    int bottom = (int)((y + size - 1) / TILE_SIZE);

    return isWall(left, top)  || isWall(right, top) ||
           isWall(left, bottom) || isWall(right, bottom);
}