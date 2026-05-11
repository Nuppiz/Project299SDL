#include "Commoninc.h"
#include "Actor.h"
#include "Tile.h"
#include "Vectors.h"

static constexpr float VISION_COS = 0.9063f; // cos(25°)

static constexpr double PI  = 3.14159265358979323846;

bool Actor::loadSpriteSheet(SDL_Renderer* renderer, const std::string& filename)
{
    spriteSheet = IMG_LoadTexture(renderer, filename.c_str());
    if (!spriteSheet)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load sprite sheet: %s", filename.c_str());
        return false;
    }
    return true;
}

AnimState Actor::selectAnimation() const
{
    bool facingRight = (direction.x >= 0.0f);
    bool isMoving    = (control & (CTRL_LEFT | CTRL_RIGHT)) != 0;

    if (shootTimer > 0.0f)
    {
        // atan2 range: (-π, π]. 0=right, π/2=down, ±π=left, -π/2=up
        bool aimUp    = (angle >= -3.0*PI/4.0 && angle < -PI/4.0);
        bool aimDown  = (angle >   PI/4.0     && angle <  3.0*PI/4.0);
        bool aimRight = (angle >= -PI/4.0     && angle <= PI/4.0);

        if (aimUp)    return onGround ? ANIM_SHOOT_UP_STAND    : ANIM_SHOOT_UP_JUMP;
        if (aimDown)  return ANIM_SHOOT_DOWN;
        if (aimRight) return onGround ? ANIM_SHOOT_RIGHT_STAND : ANIM_SHOOT_RIGHT_JUMP;
        return onGround ? ANIM_SHOOT_LEFT_STAND : ANIM_SHOOT_LEFT_JUMP;
    }

    if (!onGround) return facingRight ? ANIM_JUMP_RIGHT : ANIM_JUMP_LEFT;
    if (isMoving)  return facingRight ? ANIM_WALK_RIGHT : ANIM_WALK_LEFT;
    return facingRight ? ANIM_STAND_RIGHT : ANIM_STAND_LEFT;
}

void Actor::updateAnimation(float deltaTime)
{
    if (shootTimer > 0.0f)
        shootTimer -= deltaTime;

    AnimState newState = selectAnimation();
    if (newState != animState)
    {
        animState = newState;
        animFrame = 0;
        animTimer = 0.0f;
    }

    AnimDef& anim = animDefs[(int)animState];
    if (anim.frameCount > 1)
    {
        animTimer += deltaTime;
        if (animTimer >= anim.frameDuration)
        {
            animTimer -= anim.frameDuration;
            animFrame = (animFrame + 1) % anim.frameCount;
        }
    }
}

void Actor::resolveCollision(Actor& other)
{
    float overlapX = SDL_min(position.x + radius_w, other.position.x + other.radius_w)
                   - SDL_max(position.x, other.position.x);
    float overlapY = SDL_min(position.y + radius_w, other.position.y + other.radius_w)
                   - SDL_max(position.y, other.position.y);

    if (overlapX <= 0 || overlapY <= 0) return;

    if (overlapX < overlapY)
    {
        float push     = overlapX / 2.0f;
        float thisDir  = (position.x < other.position.x) ? -1.0f : 1.0f;
        float otherDir = -thisDir;

        bool thisCanMove  = !Tile::collidesWithWalls(position.x       + thisDir  * push, position.y,       (float)radius_w);
        bool otherCanMove = !Tile::collidesWithWalls(other.position.x + otherDir * push, other.position.y, (float)other.radius_w);

        if      (thisCanMove && otherCanMove) { position.x += thisDir * push;       other.position.x += otherDir * push; }
        else if (thisCanMove)                 { position.x += thisDir * overlapX; }
        else if (otherCanMove)                { other.position.x += otherDir * overlapX; }
    }
    else
    {
        float push     = overlapY / 2.0f;
        float thisDir  = (position.y < other.position.y) ? -1.0f : 1.0f;
        float otherDir = -thisDir;

        bool thisCanMove  = !Tile::collidesWithWalls(position.x,       position.y       + thisDir  * push, (float)radius_w);
        bool otherCanMove = !Tile::collidesWithWalls(other.position.x, other.position.y + otherDir * push, (float)other.radius_w);

        if      (thisCanMove && otherCanMove) { position.y += thisDir * push;       other.position.y += otherDir * push; }
        else if (thisCanMove)                 { position.y += thisDir * overlapY; }
        else if (otherCanMove)                { other.position.y += otherDir * overlapY; }
    }
}

void EnemyActor::updateAI(float deltaTime, const Vec2& playerPos)
{
    control = 0;

    // Sight check: transition patrol modes to chase when player is within vision cone and line of sight
    if (ai_mode == AI_PATROL || ai_mode == AI_JUMP_AND_MOVE)
    {
        if (sight_range > 0.0f)
        {
            Vec2  toPlayer = getVec2(position, playerPos);
            float distSq   = getVec2LengthSquared(toPlayer);
            if (distSq <= sight_range * sight_range)
            {
                Vec2 toPlayerNorm = normalizeVec2(toPlayer);
                float dot = dotVec2(direction, toPlayerNorm);
                if (dot >= VISION_COS)
                {
                    Vec2 center = { position.x + radius_w / 2.0f, position.y + radius_h / 2.0f };
                    if (Tile::hasLineOfSight(center, playerPos))
                        ai_mode = AI_CHASE;
                }
            }
        }
    }

    switch (ai_mode)
    {
        case AI_PATROL:
        {
            bool hitBound = (aiMoveDir > 0 && position.x >= patrolRight)
                         || (aiMoveDir < 0 && position.x <= patrolLeft);
            bool hitWall  = Tile::collidesWithWalls(position.x + aiMoveDir, position.y, (float)radius_w);
            if (hitBound || hitWall) aiMoveDir = -aiMoveDir;
            control |= (aiMoveDir > 0) ? CTRL_RIGHT : CTRL_LEFT;
            break;
        }
        case AI_JUMP_IN_PLACE:
            jumpTimer -= deltaTime;
            if (jumpTimer <= 0.0f && onGround)
            {
                control   |= CTRL_JUMP;
                jumpTimer  = jumpInterval;
            }
            break;

        case AI_JUMP_AND_MOVE:
        {
            bool hitBound = (aiMoveDir > 0 && position.x >= patrolRight)
                         || (aiMoveDir < 0 && position.x <= patrolLeft);
            bool hitWall  = Tile::collidesWithWalls(position.x + aiMoveDir, position.y, (float)radius_w);
            if (hitBound || hitWall) aiMoveDir = -aiMoveDir;
            control |= (aiMoveDir > 0) ? CTRL_RIGHT : CTRL_LEFT;
            jumpTimer -= deltaTime;
            if (jumpTimer <= 0.0f && onGround)
            {
                control   |= CTRL_JUMP;
                jumpTimer  = jumpInterval;
            }
            break;
        }
        case AI_CHASE:
        {
            Vec2  toPlayer = getVec2(position, playerPos);
            float distSq   = getVec2LengthSquared(toPlayer);

            if (distSq > attack_range * attack_range)
            {
                if      (toPlayer.x > 0.0f) control |= CTRL_RIGHT;
                else if (toPlayer.x < 0.0f) control |= CTRL_LEFT;
            }
            break;
        }
    }
}

void Actor::update(float deltaTime, int winW, int winH)
{
    const float GRAVITY    = 800.0f;
    const float JUMP_FORCE = 400.0f;

    if      (control & CTRL_RIGHT) direction.x =  1.0f;
    else if (control & CTRL_LEFT)  direction.x = -1.0f;

    float newX = position.x;
    if (control & CTRL_LEFT)  newX -= move_speed * deltaTime;
    if (control & CTRL_RIGHT) newX += move_speed * deltaTime;
    if (!Tile::collidesWithWalls(newX, position.y, (float)radius_w))
        position.x = newX;

    if ((control & CTRL_JUMP) && onGround)
    {
        velocity.y = -JUMP_FORCE;
        onGround   = false;
    }

    if (onGround)
        velocity.y = 0.0f;
    else
        velocity.y += GRAVITY * deltaTime;

    float newY = position.y + velocity.y * deltaTime;
    if (!Tile::collidesWithWalls(position.x, newY, (float)radius_w))
    {
        position.y = newY;
    }
    else
    {
        if (velocity.y > 0)
        {
            // Snap to the exact top of the blocking tile so the actor doesn't float
            int tileRow = (int)((newY + radius_w - 1) / TILE_SIZE);
            position.y  = (float)(tileRow * TILE_SIZE - radius_w);
        }
        velocity.y = 0.0f;
    }

    position.x = SDL_clamp(position.x, 0.0f, (float)(winW - radius_w));
    position.y = SDL_clamp(position.y, 0.0f, (float)(winH - radius_h));

    // Probe 1px down — stable ground detection independent of gravity increment size
    onGround = Tile::collidesWithWalls(position.x, position.y + 1.0f, (float)radius_w);

    grid_loc.x = (int)(position.x / TILE_SIZE);
    grid_loc.y = (int)(position.y / TILE_SIZE);
}

void Actor::draw(SDL_Renderer* renderer)
{
    SDL_FRect dst { position.x, position.y, (float)radius_w, (float)radius_h };

    if (!spriteSheet)
    {
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderFillRect(renderer, &dst);
        return;
    }

    AnimDef& anim = animDefs[(int)animState];
    SDL_FRect src {
        (float)(animFrame       * ANIM_FRAME_W),
        (float)(anim.sheetRow   * ANIM_FRAME_H),
        (float)ANIM_FRAME_W,
        (float)ANIM_FRAME_H
    };
    SDL_RenderTexture(renderer, spriteSheet, &src, &dst);
}

EnemyActor spawnEnemy(float x, float y, float speed, uint8_t aiMode,
                      float patrolLeft, float patrolRight, float jumpInterval,
                      float sightRange, float attackRange)
{
    EnemyActor enemy   = {};
    enemy.position     = { x, y };
    enemy.direction    = { 1.0f, 0.0f };
    enemy.velocity     = { 0.0f, 0.0f };
    enemy.move_speed   = speed;
    enemy.radius_w     = TILE_SIZE;
    enemy.radius_h     = TILE_SIZE;
    enemy.health       = 100;
    enemy.onGround     = true;
    enemy.r            = 255;
    enemy.g            = 0;
    enemy.b            = 0;
    enemy.control      = 0;
    enemy.spriteSheet  = nullptr;
    enemy.animState    = ANIM_STAND_RIGHT;
    enemy.animFrame    = 0;
    enemy.animTimer    = 0.0f;
    enemy.shootTimer   = 0.0f;
    enemy.ai_mode      = aiMode;
    enemy.patrolLeft   = patrolLeft;
    enemy.patrolRight  = patrolRight;
    enemy.aiMoveDir    = 1.0f;
    enemy.jumpTimer    = 0.0f;
    enemy.jumpInterval = jumpInterval;
    enemy.sight_range  = sightRange;
    enemy.attack_range = attackRange;
    return enemy;
}

PlayerActor spawnPlayer(float x, float y)
{
    PlayerActor player  = {};
    player.position     = { x, y };
    player.direction    = { 1.0f, 0.0f };
    player.velocity     = { 0.0f, 0.0f };
    player.move_speed   = 200.0f;
    player.radius_w     = TILE_SIZE;
    player.radius_h     = TILE_SIZE;
    player.health       = 100;
    player.onGround     = true;
    player.r            = 0;
    player.g            = 255;
    player.b            = 0;
    player.control      = 0;
    player.spriteSheet  = nullptr;
    player.animState    = ANIM_STAND_RIGHT;
    player.animFrame    = 0;
    player.animTimer    = 0.0f;
    player.shootTimer   = 0.0f;
    return player;
}
