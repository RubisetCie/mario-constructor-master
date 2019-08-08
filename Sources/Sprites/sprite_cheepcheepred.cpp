/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "../../Headers/Sprites/sprite_cheepcheepred.hpp"
#include "../../Headers/globals.hpp"

#ifdef DEBUGMODE
#include <iostream>

using namespace std;
#endif

using namespace sf;

Sprite_CheepCheepRed::Sprite_CheepCheepRed(const Texture& texture, Collider* collision) : Sprite_CheepCheep(texture, collision)
{
}

void Sprite_CheepCheepRed::secureUpdate()
{
    if (!m_active)
        return;

    if (!m_killed)
    {
        if (m_movedistance.x == 0 && player != NULL)
        {
            if (zoneb)
            {
                if (waterb != NULL)
                {
                    if (!waterb->getType())
                    {
                        float height = waterb->getHeight();
                        if (m_aabb.top + 32 > height)
                            m_swimming = true;
                    }
                }
            }
            else
            {
                if (watera != NULL)
                {
                    if (!watera->getType())
                    {
                        float height = watera->getHeight();
                        if (m_aabb.top + 32 > height)
                            m_swimming = true;
                    }
                }
            }

            if (m_swimming)
            {
                float playerpos = player->getPosition().x;
                float pos = m_sprite.getPosition().x;

                if (pos >= playerpos+14)
                    m_movedistance.x = -1.4;
                else
                {
                    m_movedistance.x = 1.4;

                    m_sprite.setScale(-1, 1);
                }

                m_collider->type = C_UNSTOMPABLE;
            }
            else
            {
                float playerpos = player->getPosition().x;
                Vector2f pos(m_sprite.getPosition());

                m_aabb.top = cameraPos.y + 240;

                float speed(sqrt((m_aabb.top - pos.y) * 0.3615));

                if (speed > 12.0f)
                    speed = 12.0f;

                if (pos.x >= playerpos+14)
                {
                    m_aabb.left = cameraPos.x + 320;

                    m_movedistance = Vector2f((rand() / (RAND_MAX / 5.0f)) - 8.0f, -speed);
                }
                else
                {
                    m_aabb.left = cameraPos.x - 352;

                    m_movedistance = Vector2f((rand() / (RAND_MAX / 5.0f)) + 3.0f, -speed);

                    m_sprite.setScale(-1, 1);
                }
            }
        }
    }

    Sprite_CheepCheep::secureUpdate();
}

ID Sprite_CheepCheepRed::getID() const
{
    return ID_USELESS;
}

void Sprite_CheepCheepRed::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
