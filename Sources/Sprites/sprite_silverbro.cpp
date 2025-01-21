/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_silverbro.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_SilverBro::Sprite_SilverBro(const Texture& texture, Collider* collision) : Sprite_Brother(texture, collision)
{
    m_sprite.setTextureRect(IntRect(0, 0, 38, 48));
    m_sprite.setOrigin(25, 0);
}

Sprite_SilverBro::~Sprite_SilverBro()
{
    for (list<Projectile_SilverHammer*>::iterator it = m_hammer.begin(); it != m_hammer.end(); it++)
        delete *it;
}

void Sprite_SilverBro::update()
{
    if (m_killed)
        return;

    if (m_animspeed > 0)
    {
        if (m_timer < m_animspeed)
            m_timer++;
        else
        {
            m_timer = 0;

            if (m_frame == 0)
                m_frame = 1;
            else
                m_frame = 0;
        }
    }

    if (m_alarm[4] > 0)
        m_alarm[4]--;
    else
    {
        if (m_animspeed > 0)
            m_animspeed = 0;
        else
            m_animspeed = rand() % 6 + 10;

        m_alarm[4] = rand() % 90 + 40;
    }

    m_sprite.setTextureRect(IntRect(m_frame * 38 + (m_alarm[2] <= 0) * 76, 0, 38, 48));

    Sprite_Brother::update();
}

void Sprite_SilverBro::secureUpdate()
{
    Sprite_Brother::secureUpdate();

    if (!m_hammer.empty())
    {
        vector<list<Projectile_SilverHammer*>::iterator> toDestroy;

        for (list<Projectile_SilverHammer*>::iterator it = m_hammer.begin(); it != m_hammer.end(); it++)
        {
            (*it)->update();

            if ((*it)->m_destroyed)
                toDestroy.emplace_back(it);
        }

        for (vector<list<Projectile_SilverHammer*>::iterator>::iterator itb = toDestroy.begin(); itb != toDestroy.end(); itb++)
        {
            delete **itb;
            m_hammer.erase(*itb);
        }
    }
}

void Sprite_SilverBro::afterUpdate()
{
    if (!m_hammer.empty())
    {
        for (list<Projectile_SilverHammer*>::iterator it = m_hammer.begin(); it != m_hammer.end(); it++)
            mainTexture.draw(**it);
    }
}

void Sprite_SilverBro::thruw()
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

    if (m_sprite.getScale().x > 0)
    {
        if (!testHorizontal(-1, *currentScale, currentMatrix, currentList))
        {
            m_hammer.emplace_back(new Projectile_SilverHammer(enemiesTxt[31], Vector2f(m_aabb.left - 6, m_aabb.top + 3), Vector2f((rand() / (RAND_MAX / 2.0f)) - 8.0f, (rand() / (RAND_MAX / 3.0f)) - 4.5f), rand() % 3 + 1));

            m_alarm[2] = rand() % 70 + 30;
            m_alarm[3] = rand() % 40 - 60;

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(10), sfxSamples[10], 0, NULL);
        }
    }
    else
    {
        if (!testHorizontal(1, *currentScale, currentMatrix, currentList))
        {
            m_hammer.emplace_back(new Projectile_SilverHammer(enemiesTxt[31], Vector2f(m_aabb.left + 26, m_aabb.top + 3), Vector2f((rand() / (RAND_MAX / 2.0f)) + 6.0f, (rand() / (RAND_MAX / 3.0f)) - 4.5f), rand() % 3 + 1));

            m_alarm[2] = rand() % 70 + 30;
            m_alarm[3] = rand() % 40 - 60;

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(10), sfxSamples[10], 0, NULL);
        }
    }
}

void Sprite_SilverBro::dead()
{
    if (m_hammer.empty())
        m_destroyed = true;
}

Vector2f Sprite_SilverBro::getPosition() const
{
    return Vector2f(m_aabb.left - 9, m_aabb.top - 16);
}

Vector2f Sprite_SilverBro::getSize() const
{
    return Vector2f(38, 48);
}

void Sprite_SilverBro::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}

