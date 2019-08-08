/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/globals.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"
#include "../../Headers/Miscs/projectile_beet.hpp"

#define BEET_SPEED 2
#define BEET_MAXVSPEED 10
#define BEET_BOUNCE 7

using namespace std;
using namespace sf;

Projectile_Beet::Projectile_Beet(Texture* texture)
{
    m_sprite.setTexture(*texture);
    m_sprite.setPosition(-64, -64);

    m_aabb = FloatRect(-64, -64, 23, 32);

    m_movedistance = Vector2f(0, 0);
}

void Projectile_Beet::update()
{
    if (m_movedistance.y < BEET_MAXVSPEED - sprite_gravity)
        m_movedistance.y += sprite_gravity;

    // Collision Events :
    if (m_remindings > 0)
    {
        Matrix* currentMatrix;
        Vector2i* currentScale;
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
            if (m_movedistance.x > 0)
            {
                listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left+8, m_aabb.top+1)));

                spritehitter.left = m_aabb.left+19;
                spritehitter.top = m_aabb.top;
                spritehitter.width = 8;
                spritehitter.height = 32;
                spritehitter_create = false;

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

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(2), sfxSamples[9], 0, NULL);
            }

            m_remindings--;
            m_movedistance.x = -m_movedistance.x;
            m_movedistance.y = -BEET_BOUNCE;
        }

        if (makeMoveVertical(m_movedistance.y, *currentScale, currentMatrix, currentList, true))
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

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(2), sfxSamples[9], 0, NULL);
            }

            if (m_remindings > 0)
                m_remindings--;
        }

        if (m_aabb.left > cameraPos.x + 448 ||
            m_aabb.left < cameraPos.x - 471 ||
            m_aabb.top > cameraPos.y + 368 ||
            m_aabb.top < cameraPos.y - 400)
            m_aabb.left = -64;

        m_sprite.setPosition(m_aabb.left, m_aabb.top);
    }
    else
    {
        Vector2f pos(m_sprite.getPosition());

        if (pos.x > cameraPos.x + 332 ||
            pos.x < cameraPos.x - 332 ||
            pos.y > cameraPos.y + 240 ||
            pos.y < cameraPos.y - 272)
            m_aabb.left = -64;

        m_sprite.move(m_movedistance);
    }
}

void Projectile_Beet::cast(Vector2f position, bool right)
{
    m_sprite.setPosition(position);

    m_aabb.left = position.x;
    m_aabb.top = position.y;

    m_remindings = 3;

    if (right)
        m_movedistance = Vector2f(BEET_SPEED, -BEET_BOUNCE);
    else
        m_movedistance = Vector2f(-BEET_SPEED, -BEET_BOUNCE);
}

void Projectile_Beet::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
