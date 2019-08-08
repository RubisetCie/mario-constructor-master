/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_firebro.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_FireBro::Sprite_FireBro(const Texture& texture, Collider* collision) : Sprite_Brother(texture, collision)
{
    m_sprite.setTextureRect(IntRect(0, 0, 33, 48));
    m_sprite.setOrigin(20, 0);
}

Sprite_FireBro::~Sprite_FireBro()
{
    for (list<Projectile_EnemyFireball*>::iterator it = m_fire.begin(); it != m_fire.end(); it++)
        delete *it;
}

void Sprite_FireBro::update()
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

    m_sprite.setTextureRect(IntRect(m_frame * 33 + (m_alarm[2] <= 0) * 66, 0, 33, 48));

    Sprite_Brother::update();
}

void Sprite_FireBro::secureUpdate()
{
    Sprite_Brother::secureUpdate();

    if (!m_fire.empty())
    {
        vector<list<Projectile_EnemyFireball*>::iterator> toDestroy;

        for (list<Projectile_EnemyFireball*>::iterator it = m_fire.begin(); it != m_fire.end(); it++)
        {
            (*it)->update();

            if ((*it)->m_destroyed)
                toDestroy.emplace_back(it);
        }

        for (vector<list<Projectile_EnemyFireball*>::iterator>::iterator itb = toDestroy.begin(); itb != toDestroy.end(); itb++)
        {
            delete **itb;
            m_fire.erase(*itb);
        }
    }
}

void Sprite_FireBro::afterUpdate()
{
    if (!m_fire.empty())
    {
        for (list<Projectile_EnemyFireball*>::iterator it = m_fire.begin(); it != m_fire.end(); it++)
            mainTexture.draw(**it);
    }
}

void Sprite_FireBro::thruw()
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
            m_fire.emplace_back(new Projectile_EnemyFireball(itemsTxt[9], Vector2f(m_aabb.left - 9, m_aabb.top + 10), Vector2f(-7, 0)));

            m_alarm[2] = rand() % 70 + 30;
            m_alarm[3] = rand() % 40 - 60;

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[8], 0, NULL);
        }
    }
    else
    {
        if (!testHorizontal(1, *currentScale, currentMatrix, currentList))
        {
            m_fire.emplace_back(new Projectile_EnemyFireball(itemsTxt[9], Vector2f(m_aabb.left + 7, m_aabb.top + 10), Vector2f(7, 0)));

            m_alarm[2] = rand() % 70 + 30;
            m_alarm[3] = rand() % 40 - 60;

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(1), sfxSamples[8], 0, NULL);
        }
    }
}

void Sprite_FireBro::dead()
{
    if (m_fire.empty())
        m_destroyed = true;
}

Vector2f Sprite_FireBro::getPosition() const
{
    return Vector2f(m_aabb.left - 4, m_aabb.top - 16);
}

Vector2f Sprite_FireBro::getSize() const
{
    return Vector2f(33, 48);
}

void Sprite_FireBro::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}

