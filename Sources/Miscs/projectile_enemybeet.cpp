/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/globals.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"
#include "../../Headers/Miscs/projectile_enemybeet.hpp"

#define BEET_MAXVSPEED 10
#define BEET_BOUNCE 8

using namespace std;
using namespace sf;

Projectile_EnemyBeet::Projectile_EnemyBeet(Texture* texture, Vector2f position, Vector2f speed)
{
    m_sprite.setTexture(*texture);
    m_sprite.setPosition(position);

    m_aabb = FloatRect(position.x, position.y, 23, 32);

    m_movedistance = speed;

    m_remindings = 3;

    m_destroyed = false;
}

void Projectile_EnemyBeet::update()
{
    if (m_movedistance.y < BEET_MAXVSPEED - sprite_gravity)
        m_movedistance.y += sprite_gravity;

    // Collision Events :
    if (m_remindings > 0)
    {
        Matrix* currentMatrix;
        Vector2u* currentScale;
        list<Collider*>* currentList;

        bool playSound;

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

        if (m_aabb.left > cameraPos.x - 375 && m_aabb.left < cameraPos.x + 352 && m_aabb.top > cameraPos.y - 304 && m_aabb.top < cameraPos.y + 272)
            playSound = true;
        else
            playSound = false;

        if (makeMoveHorizontal(m_movedistance.x, *currentScale, currentMatrix, currentList, false))
        {
            if (m_movedistance.x > 0)
            {
                listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left+8, m_aabb.top+1)));

                spritehitter.left = m_aabb.left+19;
                spritehitter.top = m_aabb.top;
                spritehitter.width = 8;
                spritehitter.height = 32;
                spritehitter_create = false;

                if (playSound)
                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(2), sfxSamples[9], 0, NULL);
            }
            else
            {
                listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left-15, m_aabb.top+1)));

                spritehitter.left = m_aabb.left-4;
                spritehitter.top = m_aabb.top;
                spritehitter.width = 8;
                spritehitter.height = 32;
                spritehitter_create = false;

                if (playSound)
                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(2), sfxSamples[9], 0, NULL);
            }

            m_remindings--;
            m_movedistance.x = -m_movedistance.x;
            m_movedistance.y = -BEET_BOUNCE;
        }

        if (makeMoveVertical(m_movedistance.y, *currentScale, currentMatrix, currentList, false))
        {
            if (m_movedistance.y > 0)
            {
                listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left-4, m_aabb.top+17)));

                spritehitter.left = m_aabb.left;
                spritehitter.top = m_aabb.top+36;
                spritehitter.width = 23;
                spritehitter.height = 8;
                spritehitter_create = false;

                m_movedistance.x = -m_movedistance.x;
                m_movedistance.y = -BEET_BOUNCE;

                if (playSound)
                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(2), sfxSamples[9], 0, NULL);
            }
            else
            {
                listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left-4, m_aabb.top-15)));

                spritehitter.left = m_aabb.left;
                spritehitter.top = m_aabb.top-4;
                spritehitter.width = 32;
                spritehitter.height = 8;
                spritehitter_create = false;

                m_movedistance.y = 0;

                if (playSound)
                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(2), sfxSamples[9], 0, NULL);
            }

            if (m_remindings > 0)
                m_remindings--;
        }

        if (m_aabb.left > cameraPos.x + 480 ||
            m_aabb.left < cameraPos.x - 503 ||
            m_aabb.top > cameraPos.y + 400)
            m_destroyed = true;

        m_sprite.setPosition(m_aabb.left, m_aabb.top);

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

            m_movedistance.x = -m_movedistance.x;
            m_movedistance.y = -BEET_BOUNCE;

            if (m_remindings > 0)
                m_remindings--;

            listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left-4, m_aabb.top+17)));
        }
    }
    else
    {
        float pos = m_sprite.getPosition().y;

        if (pos > cameraPos.y + 272)
            m_destroyed = true;

        m_sprite.move(m_movedistance);
    }
}

void Projectile_EnemyBeet::cast(Vector2f position, bool right)
{
}

void Projectile_EnemyBeet::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
