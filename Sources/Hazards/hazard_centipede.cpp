/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>

#ifdef DEBUGMODE
#include <iostream>
#endif // DEBUGMODE

#include "../../Headers/Hazards/hazard_centipede.hpp"

#define CENTIPEDE_ROTATION 0.157079
#define CENTIPEDE_SPEED 2.5
#define CENTIPEDE_WAITTIME 14

using namespace sf;
using namespace std;

Hazard_Centipede::Hazard_Centipede(Vector2f* nodes, short size, list<Collider*>::iterator collision)
{
    m_array.setPrimitiveType(Quads);

    for (register unsigned char i = 5; i >= 0; i--)
    {
        collision--;
        m_collisions[i] = *collision;
    }

    for (register unsigned short i = 0; i < size; i++)
        m_nodes.emplace_back(nodes[i]);

    m_maxnodes = size;
    m_start = 0;

    m_resolver[0] = Vector2f(0, 0);
    m_resolver[1] = Vector2f(0, 0);

    m_currentnode[0] = 0;
    m_currentnode[1] = 0;
    m_currentnode[2] = 0;
    m_currentnode[3] = 0;
    m_currentnode[4] = 0;
    m_currentnode[5] = 0;

    m_currenttimer[0] = 0;
    m_currenttimer[1] = 0;
    m_currenttimer[2] = 0;
    m_currenttimer[3] = 0;
    m_currenttimer[4] = 0;
    m_currenttimer[5] = 0;
}

Hazard_Centipede::~Hazard_Centipede()
{
    FMOD_BOOL isPlayin;
    FMOD_Channel_IsPlaying(m_channel, &isPlayin);

    if (isPlayin)
        FMOD_Channel_Stop(m_channel);
}

void Hazard_Centipede::setPosition(Vector2f pos)
{
    for (register unsigned char i = 0; i < 6; i++)
    {
        m_array.append(Vertex(Vector2f((i * 32) + pos.x + 6, pos.y + 22), Color::White, Vector2f(37, 6)));
        m_array.append(Vertex(Vector2f((i * 32) + pos.x + 27, pos.y + 22), Color::White, Vector2f(58, 6)));
        m_array.append(Vertex(Vector2f((i * 32) + pos.x + 27, pos.y + 43), Color::White, Vector2f(58, 27)));
        m_array.append(Vertex(Vector2f((i * 32) + pos.x + 6, pos.y + 43), Color::White, Vector2f(37, 27)));
    }

    for (register unsigned char i = 0; i < 6; i++)
    {
        m_array.append(Vertex(Vector2f((i * 32) + pos.x, pos.y), Color::White, Vector2f(0, 0)));
        m_array.append(Vertex(Vector2f(((i+1) * 32) + pos.x, pos.y), Color::White, Vector2f(32, 0)));
        m_array.append(Vertex(Vector2f(((i+1) * 32) + pos.x, pos.y + 32), Color::White, Vector2f(32, 32)));
        m_array.append(Vertex(Vector2f((i * 32) + pos.x, pos.y + 32), Color::White, Vector2f(0, 32)));
    }
}

void Hazard_Centipede::update()
{
    if (player == NULL)
        return;

    if (!player->m_active)
        return;

    if (m_start == 0)
    {
        Vector2f position(m_array[24].position);

        if (player->m_aabb.left > position.x - 28 && player->m_aabb.left < position.x + 192 && player->m_aabb.top + player->m_aabb.height == position.y)
        {
            m_start = 1;

            m_collisions[0]->speed = Vector2f(CENTIPEDE_SPEED, 0);
            m_collisions[1]->speed = Vector2f(CENTIPEDE_SPEED, 0);
            m_collisions[2]->speed = Vector2f(CENTIPEDE_SPEED, 0);
            m_collisions[3]->speed = Vector2f(CENTIPEDE_SPEED, 0);
            m_collisions[4]->speed = Vector2f(CENTIPEDE_SPEED, 0);
            m_collisions[5]->speed = Vector2f(CENTIPEDE_SPEED, 0);

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(18), sfxSamples[19], 0, &m_channel);
        }
    }
    else if (m_start == 1)
    {
        if (m_resolver[0].y < 0)
        {
            if (player->m_aabb.intersects(FloatRect(m_collisions[0]->rect.left, m_collisions[0]->rect.top + m_resolver[0].y - 1, 32, -m_resolver[0].y + 1)))
                player->m_aabb.top -= m_resolver[0].y;
        }

        if (m_resolver[1].x != 0)
        {
            if (player->m_aabb.intersects(FloatRect(m_collisions[5]->rect.left, m_collisions[5]->rect.top, 32, 32)))
                player->pushH(m_resolver[1].x > 0?m_resolver[1].x:-m_resolver[1].x);
        }
        else if (m_resolver[1].y != 0)
        {
            if (player->m_aabb.intersects(FloatRect(m_collisions[5]->rect.left, m_collisions[5]->rect.top, 32, 32)))
                player->pushV(m_resolver[1].y > 0?-m_resolver[1].y:m_resolver[1].y);
        }
    }
}

void Hazard_Centipede::secureUpdate()
{
    if (m_start == 1)
    {
        for (register unsigned char i = 0; i < 6; i++)
        {
            Vector2f mainPos(m_array[24 + (i*4)].position);
            Vector2f center(mainPos + Vector2f(16, 32));

            // Rotation gears :
            for (register unsigned char j = 0; j < 4; j++)
            {
                Vector2f pos(m_array[(i*4) + j].position);

                m_array[(i*4) + j].position = Vector2f(fma((pos.x - center.x), cos(CENTIPEDE_ROTATION), center.x) - (pos.y - center.y) * sin(CENTIPEDE_ROTATION),
                                                       fma((pos.x - center.x), sin(CENTIPEDE_ROTATION), center.y) + (pos.y - center.y) * cos(CENTIPEDE_ROTATION));
            }

            // Horizontal movement :
            if (m_currentnode[i] % 2 == 0)
            {
                float position(m_collisions[i]->rect.left);
                float target(m_nodes[m_currentnode[i]].x);
                float absspeed = abs(m_collisions[i]->speed.x);

                if (position > target - absspeed && position < target + absspeed)
                {
                    m_collisions[i]->rect.left = target;
                    m_collisions[i]->speed = Vector2f(0, 0);

                    m_currentnode[i]++;

                    if (m_currentnode[i] < m_maxnodes)
                        m_currenttimer[i] = CENTIPEDE_WAITTIME;
                }
            }
            // Vertical movement :
            else
            {
                float position(m_collisions[i]->rect.top);
                float target(m_nodes[m_currentnode[i]].y);
                float absspeed = abs(m_collisions[i]->speed.y);

                if (position > target - absspeed && position < target + absspeed)
                {
                    m_collisions[i]->rect.top = target;
                    m_collisions[i]->speed = Vector2f(0, 0);

                    m_currentnode[i]++;

                    if (m_currentnode[i] < m_maxnodes)
                        m_currenttimer[i] = CENTIPEDE_WAITTIME;
                }
            }

            // Wait timer handle :
            if (m_currenttimer[i] > 0)
            {
                m_currenttimer[i]--;

                if (m_currenttimer[i] == 1)
                {
                    if (m_currentnode[i] % 2 == 0)
                    {
                        if (m_nodes[m_currentnode[i]].x > m_collisions[i]->rect.left)
                        {
                            m_collisions[i]->speed = Vector2f(CENTIPEDE_SPEED, 0);

                            if (i < 5)
                                m_collisions[i]->rect.left = m_collisions[i+1]->rect.left - 32;
                        }
                        else
                        {
                            m_collisions[i]->speed = Vector2f(-CENTIPEDE_SPEED, 0);

                            if (i < 5)
                                m_collisions[i]->rect.left = m_collisions[i+1]->rect.left + 32;
                        }
                    }
                    else
                    {
                        if (m_nodes[m_currentnode[i]].y > m_collisions[i]->rect.top)
                        {
                            m_collisions[i]->speed = Vector2f(0, CENTIPEDE_SPEED);

                            if (i < 5)
                                m_collisions[i]->rect.top = m_collisions[i+1]->rect.top - 32;
                        }
                        else
                        {
                            m_collisions[i]->speed = Vector2f(0, -CENTIPEDE_SPEED);

                            if (i < 5)
                                m_collisions[i]->rect.top = m_collisions[i+1]->rect.top + 32;
                        }
                    }
                }
            }

            // Moving :
            m_collisions[i]->rect.left += m_collisions[i]->speed.x;
            m_collisions[i]->rect.top += m_collisions[i]->speed.y;

            // Centipede alignment :
            Vector2f collisionPos(m_collisions[i]->rect.left, m_collisions[i]->rect.top);
            Vector2f delta(mainPos - collisionPos);

            if (i == 0)
                m_resolver[0] = delta;
            else if (i == 5)
                m_resolver[1] = delta;

            m_array[(i*4) + 24].position = collisionPos;
            m_array[(i*4) + 25].position = Vector2f(collisionPos.x + 32, collisionPos.y);
            m_array[(i*4) + 26].position = Vector2f(collisionPos.x + 32, collisionPos.y + 32);
            m_array[(i*4) + 27].position = Vector2f(collisionPos.x, collisionPos.y + 32);

            for (register unsigned char j = 0; j < 4; j++)
                m_array[(i*4) + j].position -= delta;
        }

        if (m_currentnode[0] == m_maxnodes)
        {
            m_start = 2;

            m_collisions[1]->destroyed = true;
            m_collisions[2]->destroyed = true;
            m_collisions[3]->destroyed = true;
            m_collisions[4]->destroyed = true;
            m_collisions[5]->destroyed = true;

            FMOD_Channel_Stop(m_channel);
        }

        Vector2f pos(m_array[24].position);

        if (pos.x > cameraPos.x - 544 && pos.x < cameraPos.x + 512 && pos.y > cameraPos.y - 464 && pos.y - 160 < cameraPos.y + 432)
            FMOD_Channel_SetVolume(m_channel, 1.0f);
        else
            FMOD_Channel_SetVolume(m_channel, 0.0f);
    }
}

void Hazard_Centipede::afterUpdate()
{
}

Vector2f Hazard_Centipede::getPosition() const
{
    Vector2f position = m_array[24].position;
    return Vector2f(position.x - 160, position.y - 160);
}

Vector2f Hazard_Centipede::getSize() const
{
    return Vector2f(352, 352);
}

ID Hazard_Centipede::getID() const
{
    return ID_USELESS;
}

void Hazard_Centipede::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_array, hazardsTxt[10]);
}
