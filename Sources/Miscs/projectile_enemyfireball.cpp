/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Miscs/effect_fire.hpp"
#include "../../Headers/Miscs/projectile_enemyfireball.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define FIREBALL_MAXVSPEED 10
#define FIREBALL_ROTATION 25
#define FIREBALL_BOUNCE 4.5

using namespace std;
using namespace sf;

Projectile_EnemyFireball::Projectile_EnemyFireball(Texture* texture, Vector2f position, Vector2f speed)
{
    m_sprite.setTexture(*texture);
    m_sprite.setPosition(position);
    m_sprite.setOrigin(6, 8);

    m_aabb = FloatRect(position.x - 6, position.y - 8, 15, 16);

    m_movedistance = speed;

    m_destroyed = false;
}

void Projectile_EnemyFireball::update()
{
    if (m_movedistance.y < FIREBALL_MAXVSPEED - sprite_gravity)
        m_movedistance.y += sprite_gravity;

    Matrix* currentMatrix;
    Vector2i* currentScale;
    list<Collider*>* currentList;

    // Collision Events :
    if (zoneb)
    {
        currentMatrix = collisionMatrixb;
        currentScale = &levelScaleb;
        currentList = &listColliderb;
    }
    else
    {
        currentMatrix = collisionMatrix;
        currentScale = &levelScale;
        currentList = &listCollider;
    }

    if (m_movedistance.x > 0)
        m_sprite.rotate(FIREBALL_ROTATION);
    else
        m_sprite.rotate(-FIREBALL_ROTATION);

    if (makeMoveHorizontal(m_movedistance.x, *currentScale, currentMatrix, currentList, false))
    {
        listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left-9, m_aabb.top-7)));
        m_destroyed = true;
    }

    if (makeMoveVertical(m_movedistance.y, *currentScale, currentMatrix, currentList, false))
    {
        if (m_movedistance.y > 0)
            m_movedistance.y = -FIREBALL_BOUNCE;
        else
        {
            listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left-9, m_aabb.top-7)));
            m_destroyed = true;
        }
    }

    if (m_aabb.left > cameraPos.x + 368 ||
        m_aabb.left < cameraPos.x - 384 ||
        m_aabb.top > cameraPos.y + 288 ||
        m_aabb.top < cameraPos.y - 304)
        m_destroyed = true;

    m_sprite.setPosition(m_aabb.left+6, m_aabb.top+8);

    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (player->m_animpowerup > 0)
        return;

    if (game_powerup <= 1 && player->m_animtrace > 0)
        return;

    if (player->m_invincibility > 0)
        return;

    if (player->m_aabb.intersects(m_aabb))
    {
        if (game_powerup == 0)
            player->death();
        else if (game_powerup == 1)
        {
            player->setPower(0, true);

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[6], 0, NULL);
        }
        else
        {
            player->setPower(1, true);

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), sfxSamples[6], 0, NULL);
        }

        listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left-9, m_aabb.top-7)));
        m_destroyed = true;
    }
}

void Projectile_EnemyFireball::cast(Vector2f position, bool right)
{
}

void Projectile_EnemyFireball::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
