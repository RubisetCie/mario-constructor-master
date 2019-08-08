/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_beetbro.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_BeetBro::Sprite_BeetBro(const Texture& texture, Collider* collision) : Sprite_Brother(texture, collision)
{
    m_sprite.setTextureRect(IntRect(0, 0, 38, 48));
    m_sprite.setOrigin(25, 0);
}

Sprite_BeetBro::~Sprite_BeetBro()
{
    for (list<Projectile_EnemyBeet*>::iterator it = m_beet.begin(); it != m_beet.end(); it++)
        delete *it;
}

void Sprite_BeetBro::update()
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

void Sprite_BeetBro::secureUpdate()
{
    Sprite_Brother::secureUpdate();

    if (!m_beet.empty())
    {
        vector<list<Projectile_EnemyBeet*>::iterator> toDestroy;

        for (list<Projectile_EnemyBeet*>::iterator it = m_beet.begin(); it != m_beet.end(); it++)
        {
            (*it)->update();

            if ((*it)->m_destroyed)
                toDestroy.emplace_back(it);
        }

        for (vector<list<Projectile_EnemyBeet*>::iterator>::iterator itb = toDestroy.begin(); itb != toDestroy.end(); itb++)
        {
            delete **itb;
            m_beet.erase(*itb);
        }
    }
}

void Sprite_BeetBro::afterUpdate()
{
    if (!m_beet.empty())
    {
        for (list<Projectile_EnemyBeet*>::iterator it = m_beet.begin(); it != m_beet.end(); it++)
            mainTexture.draw(**it);
    }
}

void Sprite_BeetBro::thruw()
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

    if (m_sprite.getScale().x > 0)
    {
        if (!testHorizontal(-1, *currentScale, currentMatrix, currentList))
        {
            m_beet.emplace_back(new Projectile_EnemyBeet(itemsTxt[10], Vector2f(m_aabb.left - 6, m_aabb.top - 5), Vector2f((rand() / (RAND_MAX / 3.5f)) - 4.5f, (rand() / (RAND_MAX / 3.0f)) - 8.0f)));

            m_alarm[2] = rand() % 70 + 30;
            m_alarm[3] = rand() % 40 - 60;

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[8], 0, NULL);
        }
    }
    else
    {
        if (!testHorizontal(1, *currentScale, currentMatrix, currentList))
        {
            m_beet.emplace_back(new Projectile_EnemyBeet(itemsTxt[10], Vector2f(m_aabb.left + 26, m_aabb.top - 5), Vector2f((rand() / (RAND_MAX / 3.5f)) + 1.0f, (rand() / (RAND_MAX / 3.0f)) - 8.0f)));

            m_alarm[2] = rand() % 70 + 30;
            m_alarm[3] = rand() % 40 - 60;

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[8], 0, NULL);
        }
    }
}

void Sprite_BeetBro::dead()
{
    if (m_beet.empty())
        m_destroyed = true;
}

Vector2f Sprite_BeetBro::getPosition() const
{
    return Vector2f(m_aabb.left - 9, m_aabb.top - 16);
}

Vector2f Sprite_BeetBro::getSize() const
{
    return Vector2f(38, 48);
}

void Sprite_BeetBro::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}

