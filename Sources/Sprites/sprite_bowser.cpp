/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_bowser.hpp"
#include "../../Headers/Miscs/effect_score.hpp"
#include "../../Headers/Miscs/effect_fire.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

#define BOWSER_MAXVSPEED 8
#define BOWSER_FADESPEED 8
#define BOWSER_INVINCIBLETIME 140

using namespace std;
using namespace sf;

Sprite_Bowser::Sprite_Bowser(const Texture& texture, Collider* collision) : Pawn()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 64, 72));
    m_sprite.setOrigin(32, 0);

    if (koopaAttacks[5])
        m_cannon.setTexture(*hazardsTxt[2]);

    m_movedistance = Vector2f(0, 0);

    m_cannonspeed = -3;

    m_aabb.width = 52;
    m_aabb.height = 55;

    m_collider = collision;

    m_onfloor = false;

    m_frame = 0;
    m_timer = 0;
    m_firehealth = 5;
    m_currentattack = 0;
    m_invincible = 0;

    m_deathtimer = 0;

    m_alarm[0] = koopaFrequences[0] + (rand() % 20) - 10;
    m_alarm[1] = koopaFrequences[1] + (rand() % 20) - 10;
    m_alarm[2] = koopaFrequences[2] + (rand() % 20) - 10;
    m_alarm[3] = koopaFrequences[3] + (rand() % 20) - 10;
    m_alarm[4] = koopaFrequences[4] + (rand() % 20) - 10;
    m_alarm[5] = koopaFrequences[5] + (rand() % 20) - 10;
}

Sprite_Bowser::~Sprite_Bowser()
{
    for (list<Projectile_Flame*>::iterator it = m_flames.begin(); it != m_flames.end(); it++)
        delete *it;

    for (list<Projectile_Bulletbill*>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
        delete *it;
}

void Sprite_Bowser::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);

    m_limit[0] = pos.x-192;
    m_limit[1] = pos.x+192;

    m_aabb.left = pos.x-26;
    m_aabb.top = pos.y+17;
}

void Sprite_Bowser::update()
{
    if (koopaEngaged == 0)
    {
        if (player->m_aabb.left < m_aabb.left + 12)
            koopaEngaged = 1;
        else
            koopaEngaged = 2;

        if (koopaMusic == 0)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), musicSamples[27], 0, &musicChannel);
        else
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(20), musicSamples[24], 0, &musicChannel);

        // Bowser HUD initialization :
        hudBowser->append(Vertex(Vector2f(-64, -64), Color::White, Vector2f(9, 0)));
        hudBowser->append(Vertex(Vector2f(-64, -64), Color::White, Vector2f(73, 0)));
        hudBowser->append(Vertex(Vector2f(-64, -64), Color::White, Vector2f(73, 42)));
        hudBowser->append(Vertex(Vector2f(-64, -64), Color::White, Vector2f(9, 42)));

        for (register unsigned char i = 0; i < koopaHealth; i++)
        {
            hudBowser->append(Vertex(Vector2f(-64, -64), Color::White, Vector2f(0, 7)));
            hudBowser->append(Vertex(Vector2f(-64, -64), Color::White, Vector2f(9, 7)));
            hudBowser->append(Vertex(Vector2f(-64, -64), Color::White, Vector2f(9, 39)));
            hudBowser->append(Vertex(Vector2f(-64, -64), Color::White, Vector2f(0, 39)));
        }

        #ifdef DEBUGMODE
        cout << "Speed 1 : " << koopaSpeeds[0] << "     Frenquence : " << static_cast<unsigned int>(koopaFrequences[0]) << endl;
        cout << "Speed 2 : " << koopaSpeeds[1] << "     Frenquence : " << static_cast<unsigned int>(koopaFrequences[1]) << endl;
        cout << "Speed 3 : " << koopaSpeeds[2] << "     Frenquence : " << static_cast<unsigned int>(koopaFrequences[2]) << endl;
        cout << "Speed 4 : " << koopaSpeeds[3] << "     Frenquence : " << static_cast<unsigned int>(koopaFrequences[3]) << endl;
        cout << "Speed 5 : " << koopaSpeeds[4] << "     Frenquence : " << static_cast<unsigned int>(koopaFrequences[4]) << endl;
        cout << "Speed 6 : " << koopaSpeeds[5] << "     Frenquence : " << static_cast<unsigned int>(koopaFrequences[5]) << endl;
        #endif // DEBUGMODE
    }

    if (koopaHealth == 0)
        return;

    if (m_currentattack > 0)
    {
        if (m_fireready > 0)
            m_sprite.setTextureRect(IntRect(192, 0, 64, 72));
        else
            m_sprite.setTextureRect(IntRect(256, 0, 64, 72));
    }
    else
    {
        if (m_onfloor)
        {
            if (m_frame < 15)
                m_frame++;
            else
                m_frame = 0;

            unsigned char temp = ceil(m_frame / 4);
            switch (temp)
            {
                case 0 : m_sprite.setTextureRect(IntRect(0, 0, 64, 72)); break;
                case 1 : m_sprite.setTextureRect(IntRect(64, 0, 64, 72)); break;
                case 2 : m_sprite.setTextureRect(IntRect(0, 0, 64, 72)); break;
                case 3 : m_sprite.setTextureRect(IntRect(128, 0, 64, 72)); break;
            }
        }
        else
            m_sprite.setTextureRect(IntRect(320, 0, 64, 72));
    }

    if (player == NULL)
        return;

    // Animation :
    if (m_aabb.left >= player->m_aabb.left - 12)
    {
        m_sprite.setScale(1, 1);

        if (koopaAttacks[5])
            m_cannon.setPosition(m_aabb.left - 6, m_aabb.top + 16);
    }
    else
    {
        m_sprite.setScale(-1, 1);

        if (koopaAttacks[5])
            m_cannon.setPosition(m_aabb.left + 26, m_aabb.top + 16);
    }

    if (m_invincible > 0)
    {
        float alpha = m_sprite.getColor().a;

        if (m_fadestate)
        {
            alpha += BOWSER_FADESPEED;

            m_sprite.setColor(Color(255, 255, 255, alpha));

            if (alpha > 255 - BOWSER_FADESPEED)
                m_fadestate = false;
        }
        else
        {
            alpha -= BOWSER_FADESPEED;

            m_sprite.setColor(Color(255, 255, 255, alpha));

            if (alpha < BOWSER_FADESPEED)
                m_fadestate = true;
        }

        m_invincible--;
    }
    else
        m_sprite.setColor(Color(255, 255, 255, 255));

    if (!player->m_active)
        return;

    if (enemystomper.intersects(m_aabb) || enemystomper_pt == m_collider)
    {
        koopaHealth--;

        m_invincible = BOWSER_INVINCIBLETIME;
        m_fadestate = false;
        m_firehealth = 5;

        m_collider->rect.left = -64;

        player->disorient();

        hudBowser->resize(hudBowser->getVertexCount() - 4);

        if (koopaHealth > 0)
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(16), sfxSamples[25], 0, NULL);
        else
        {
            m_movedistance = Vector2f(0, 0);

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(16), sfxSamples[26], 0, NULL);
        }
    }

    if (player->m_animpowerup > 0)
        return;

    if (game_powerup <= 1 && player->m_animtrace > 0)
        return;

    if (m_invincible > BOWSER_INVINCIBLETIME - 8)
        return;

    if (player->m_aabb.intersects(m_aabb))
    {
        if (player->m_invincibility > 0)
        {
            koopaHealth--;

            m_invincible = BOWSER_INVINCIBLETIME;
            m_fadestate = false;
            m_firehealth = 5;

            m_collider->rect.left = -64;

            hudBowser->resize(hudBowser->getVertexCount() - 4);

            listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left+11, m_aabb.top+12)));

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);

            if (koopaHealth > 0)
                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(16), sfxSamples[25], 0, NULL);
            else
            {
                m_movedistance = Vector2f(0, 0);

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(16), sfxSamples[26], 0, NULL);
            }
        }
        else
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
}

void Sprite_Bowser::secureUpdate()
{
    if (koopaEngaged == 0)
        return;

    if (koopaHealth > 0)
    {
        // Jump move :
        if (koopaAttacks[0])
        {
            if (m_alarm[0] > 0)
                m_alarm[0]--;
        }

        // Horizontal moves :
        if (koopaAttacks[1])
        {
            if (m_alarm[1] > 0)
                m_alarm[1]--;
            else
            {
                if (m_movedistance.x == 0)
                    m_movedistance.x = rand()%2==1?koopaSpeeds[1]:-koopaSpeeds[1];
                else
                    m_movedistance.x = 0;

                m_alarm[1] = koopaFrequences[1] + (rand() % 20) - 10;
            }
        }

        // Simple fire :
        if (koopaAttacks[2])
        {
            if (m_alarm[2] > 0)
                m_alarm[2]--;
            else
            {
                if (m_pendings.size() < 3)
                    m_pendings.emplace(0);

                m_alarm[2] = koopaFrequences[2] + (rand() % 20) - 10;
            }
        }

        // Triple fire :
        if (koopaAttacks[3])
        {
            if (m_alarm[3] > 0)
                m_alarm[3]--;
            else
            {
                if (m_pendings.size() < 3)
                    m_pendings.emplace(1);

                m_alarm[3] = koopaFrequences[3] + (rand() % 20) - 10;
            }
        }

        // Homing fire :
        if (koopaAttacks[4])
        {
            if (m_alarm[4] > 0)
                m_alarm[4]--;
            else
            {
                if (m_pendings.size() < 3)
                    m_pendings.emplace(2);

                m_alarm[4] = koopaFrequences[4] + (rand() % 20) - 10;
            }
        }

        // Cannon :
        if (koopaAttacks[5])
        {
            if (m_alarm[5] > 0)
                m_alarm[5]--;
            else
            {
                float dir = m_sprite.getScale().x;

                m_bullets.emplace_back(new Projectile_Bulletbill(hazardsTxt[3], Vector2f(m_aabb.left + 26 - (16 * dir), m_aabb.top + 32), Vector2f(koopaSpeeds[5] * (-dir), 0), false));

                listEffect.emplace_back(new Effect_Fire(effectTxt[9], Vector2f(m_aabb.left + 11 - (32 * dir), m_aabb.top + 17)));

                m_alarm[5] = koopaFrequences[5] + (rand() % 20) - 10;

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(12), sfxSamples[20], 0, NULL);
            }
        }

        if (m_currentattack == 0)
        {
            if (m_pendings.size() > 0)
            {
                m_currentattack = m_pendings.front() + 1;
                m_pendings.pop();

                m_fireready = koopaFrequences[m_currentattack + 1] / 4;
            }
        }
        else
        {
            m_fireready--;

            if (m_fireready == 0)
            {
                float height;
                float dir = m_sprite.getScale().x;

                if (player == NULL)
                    height = m_aabb.top + 15;
                else
                    height = player->m_aabb.top;

                switch (m_currentattack)
                {
                    case 1 :
                        m_flames.emplace_back(new Projectile_Flame(enemiesTxt[40], Vector2f(m_aabb.left + 26 - (16 * dir), m_aabb.top + 15), koopaSpeeds[2] * (-dir), height));
                        break;
                    case 2 :
                        m_flames.emplace_back(new Projectile_Flame(enemiesTxt[40], Vector2f(m_aabb.left + 26 - (16 * dir), m_aabb.top + 15), koopaSpeeds[3] * (-dir), height));
                        m_flames.emplace_back(new Projectile_Flame(enemiesTxt[40], Vector2f(m_aabb.left + 26 - (16 * dir), m_aabb.top + 15), koopaSpeeds[3] * (-dir), height - 32));
                        m_flames.emplace_back(new Projectile_Flame(enemiesTxt[40], Vector2f(m_aabb.left + 26 - (16 * dir), m_aabb.top + 15), koopaSpeeds[3] * (-dir), height - 64));
                        break;
                    case 3 :
                        m_flames.emplace_back(new Projectile_Flame(enemiesTxt[40], Vector2f(m_aabb.left + 26 - (16 * dir), m_aabb.top + 15), koopaSpeeds[4] * (-dir), height, true));
                        break;
                }

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(15), sfxSamples[24], 0, NULL);
            }
            else if (m_fireready <= -koopaAttacks[m_currentattack + 1] / 6)
                m_currentattack = 0;
        }

        if (enemykiller.intersects(m_aabb) && enemykiller_fire != 2)
        {
            if (enemykiller_fire == 0)
            {
                m_firehealth--;

                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(9), sfxSamples[12], 0, NULL);

                if (m_firehealth <= 0)
                {
                    koopaHealth--;

                    m_invincible = BOWSER_INVINCIBLETIME;
                    m_fadestate = false;
                    m_firehealth = 5;

                    m_collider->rect.left = -64;

                    hudBowser->resize(hudBowser->getVertexCount() - 4);

                    if (koopaHealth > 0)
                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(16), sfxSamples[25], 0, NULL);
                    else
                    {
                        m_movedistance = Vector2f(0, 0);

                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(16), sfxSamples[26], 0, NULL);
                    }
                }
            }
            else
            {
                koopaHealth--;

                m_invincible = BOWSER_INVINCIBLETIME;
                m_fadestate = false;
                m_firehealth = 5;

                m_collider->rect.left = -64;

                hudBowser->resize(hudBowser->getVertexCount() - 4);

                if (koopaHealth > 0)
                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(16), sfxSamples[25], 0, NULL);
                else
                {
                    m_movedistance = Vector2f(0, 0);

                    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(16), sfxSamples[26], 0, NULL);
                }
            }
        }
    }

    if (koopaHealth == 0)
    {
        m_deathtimer++;

        if (m_deathtimer % 2 == 0)
        {
            m_fadestate = !m_fadestate;

            m_sprite.setTextureRect(IntRect(384 + (m_fadestate * 64), 0, 64, 72));
        }

        if (m_deathtimer == 1)
        {
            if (koopaAttacks[5])
            {
                m_cannon.setScale(1, -1);
                m_cannon.move(0, 31);
            }
        }
        else if (m_deathtimer >= 140)
        {
            m_sprite.move(0, m_movedistance.y);
            m_movedistance.y += 0.14;

            float dist = m_sprite.getPosition().y;

            if (dist > m_aabb.top + 512)
            {
                if (player->m_active)
                    player->clearCourse();

                while (m_pendings.size() > 0)
                    m_pendings.pop();

                m_flames.clear();
                m_bullets.clear();

                m_tracing.clear();
                m_tracingb.clear();

                m_destroyed = true;
            }

            if (m_deathtimer == 140)
                FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(16), sfxSamples[27], 0, NULL);
        }

        m_cannonspeed += 0.06;

        m_cannon.move(0, m_cannonspeed);
    }
    else
    {
        Matrix* currentMatrix;
        Vector2i* currentScale;
        list<Collider*>* currentList;

        if (m_onfloor)
        {
            if (m_alarm[0] == 0)
            {
                m_movedistance.y = -koopaSpeeds[0];

                m_alarm[0] = koopaFrequences[0] + (rand() % 20) - 10;
            }
        }
        else
        {
            if (m_movedistance.y < BOWSER_MAXVSPEED - star_gravity)
                m_movedistance.y += star_gravity;
        }

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

        if (m_movedistance.x != 0)
        {
            if (makeMoveHorizontalHigh(m_movedistance.x, *currentScale, currentMatrix, currentList))
                m_movedistance.x = 0;
        }

        if (m_movedistance.y != 0)
        {
            if (makeMoveVerticalHigh(m_movedistance.y, *currentScale, currentMatrix, currentList))
            {
                if (m_movedistance.y > 0)
                    m_onfloor = true;

                m_movedistance.y = 0;
            }
        }

        if (m_platform != NULL)
        {
            float hspeed(m_platform->speed.x);

            makeMoveHorizontalHighPlatform(hspeed, *currentScale, currentMatrix, currentList);
            makeMoveVerticalHighPlatform(m_platform->speed.y, *currentScale, currentMatrix, currentList);

            m_limit[0] += hspeed;
            m_limit[1] += hspeed;
        }

        if (m_onfloor)
        {
            if (!testVerticalHigh(1, *currentScale, currentMatrix, currentList))
            {
                m_onfloor = false;
                m_platform = NULL;
            }
        }

        if (m_aabb.left < m_limit[0])
        {
            m_aabb.left = m_limit[0];
            m_movedistance.x = 0;
        }
        else if (m_aabb.left > m_limit[1])
        {
            m_aabb.left = m_limit[1];
            m_movedistance.x = 0;
        }

        m_sprite.setPosition(roundf(m_aabb.left+26), roundf(m_aabb.top-17));

        if (m_invincible == 0)
        {
            m_collider->rect.left = m_aabb.left;
            m_collider->rect.top = m_aabb.top;
        }
    }

    if (!m_tracing.empty() || !m_tracingb.empty())
        updateTrace();

    if (koopaTrail < 22)
    {
        if (m_invincible == 0)
        {
            if (m_tracetimer < koopaTrail)
                m_tracetimer++;
            else
            {
                if (m_sprite.getScale().x > 0)
                    addTrace(Vector2f(m_aabb.left - 6, m_aabb.top - 17), m_sprite.getTextureRect(), true);
                else
                    addTrace(Vector2f(m_aabb.left - 6, m_aabb.top - 17), m_sprite.getTextureRect(), false);

                m_tracetimer = 0;
            }
        }

        if (!m_tracing.empty())
            mainTexture.draw(&m_tracing.front(), m_tracing.size(), Quads, enemiesTxt[39]);

        if (!m_tracingb.empty())
            mainTexture.draw(&m_tracingb.front(), m_tracingb.size(), Quads, enemiesTxt[39]);
    }

    if (!m_flames.empty())
    {
        vector<list<Projectile_Flame*>::iterator> toDestroy;

        for (list<Projectile_Flame*>::iterator it = m_flames.begin(); it != m_flames.end(); it++)
        {
            (*it)->update();

            if ((*it)->m_destroyed)
                toDestroy.emplace_back(it);
        }

        for (vector<list<Projectile_Flame*>::iterator>::iterator itb = toDestroy.begin(); itb != toDestroy.end(); itb++)
        {
            delete **itb;
            m_flames.erase(*itb);
        }
    }

    if (!m_bullets.empty())
    {
        vector<list<Projectile_Bulletbill*>::iterator> toDestroy;

        for (list<Projectile_Bulletbill*>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
        {
            (*it)->update();

            if ((*it)->m_destroyed)
                toDestroy.emplace_back(it);
        }

        for (vector<list<Projectile_Bulletbill*>::iterator>::iterator itb = toDestroy.begin(); itb != toDestroy.end(); itb++)
        {
            delete **itb;
            m_bullets.erase(*itb);
        }
    }
}

void Sprite_Bowser::afterUpdate()
{
    for (list<Projectile_Flame*>::iterator it = m_flames.begin(); it != m_flames.end(); it++)
        mainTexture.draw(**it);

    for (list<Projectile_Bulletbill*>::iterator it = m_bullets.begin(); it != m_bullets.end(); it++)
        mainTexture.draw(**it);
}

Vector2f Sprite_Bowser::getPosition() const
{
    return Vector2f(m_aabb.left - 6, m_aabb.top - 17);
}

Vector2f Sprite_Bowser::getSize() const
{
    return Vector2f(64, 72);
}

ID Sprite_Bowser::getID() const
{
    if (m_invincible == 0 && koopaHealth > 0)
        return ID_STOMPABLE;
    else
        return ID_USELESS;
}

void Sprite_Bowser::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);

    if (koopaAttacks[5])
        target.draw(m_cannon);
}
