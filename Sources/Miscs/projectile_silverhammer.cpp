/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Miscs/projectile_silverhammer.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define HAMMER_MAXVSPEED 12
#define HAMMER_ROTATION 40

using namespace std;
using namespace sf;

Projectile_SilverHammer::Projectile_SilverHammer(Texture* texture, Vector2f position, Vector2f speed, unsigned char remindings)
{
    m_sprite.setTexture(*texture);
    m_sprite.setPosition(position);
    m_sprite.setOrigin(12, 16);

    m_aabb = FloatRect(position.x - 12, position.y - 12, 24, 24);

    m_movedistance = speed;

    m_remindings = remindings;

    m_destroyed = false;
}

void Projectile_SilverHammer::cast(Vector2f position, bool right)
{
}

void Projectile_SilverHammer::update()
{
    if (m_movedistance.y < HAMMER_MAXVSPEED - star_gravity)
        m_movedistance.y += star_gravity;

    if (m_movedistance.x > 0)
        m_sprite.rotate(HAMMER_ROTATION);
    else
        m_sprite.rotate(-HAMMER_ROTATION);

    if (m_remindings > 0)
    {
        Matrix* currentMatrix;
        Vector2u* currentScale;
        list<Collider*>* currentList;
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

        if (makeMoveHorizontal(m_movedistance.x, *currentScale, currentMatrix, currentList, false))
        {
            spritehitter.left = m_aabb.left - 8;
            spritehitter.top = m_aabb.top - 8;
            spritehitter.width = 40;
            spritehitter.height = 40;
            spritehitter_create = false;

            if (m_movedistance.x > 0)
                listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left+15, m_aabb.top-3)));
            else
                listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left-15, m_aabb.top-3)));

            if (m_aabb.left > cameraPos.x - 376 && m_aabb.left < cameraPos.x + 352 && m_aabb.top > cameraPos.y - 296 && m_aabb.top < cameraPos.y + 272)
                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);

            if (m_movedistance.x > 2.0)
                m_movedistance.x = -2.0;
            else if (m_movedistance.x < 2.0)
                m_movedistance.x = 2.0;
            else
                m_movedistance.x = -m_movedistance.x;

            m_movedistance.y = 0;

            m_remindings--;
        }

        if (makeMoveVertical(m_movedistance.y, *currentScale, currentMatrix, currentList, false))
        {
            spritehitter.left = m_aabb.left - 8;
            spritehitter.top = m_aabb.top - 8;
            spritehitter.width = 40;
            spritehitter.height = 40;
            spritehitter_create = false;

            if (m_movedistance.y > 0)
                listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left-3, m_aabb.top+15)));
            else
                listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left-3, m_aabb.top-15)));

            if (m_aabb.left > cameraPos.x - 376 && m_aabb.left < cameraPos.x + 352 && m_aabb.top > cameraPos.y - 296 && m_aabb.top < cameraPos.y + 272)
                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);

            if (m_movedistance.x > 2.0)
                m_movedistance.x = -2.0;
            else if (m_movedistance.x < 2.0)
                m_movedistance.x = 2.0;
            else
                m_movedistance.x = -m_movedistance.x;

            m_movedistance.y = (rand() / (RAND_MAX / 3.0f)) - 6.0f;

            if (m_remindings > 0)
                m_remindings--;
        }
    }
    else
    {
        m_aabb.left += m_movedistance.x;
        m_aabb.top += m_movedistance.y;
    }

    if (m_aabb.top > cameraPos.y + 272)
        m_destroyed = true;

    m_sprite.setPosition(m_aabb.left+12, m_aabb.top+12);

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
    }
}

void Projectile_SilverHammer::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
