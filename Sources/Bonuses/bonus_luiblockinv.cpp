/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/globals.hpp"
#include "../../Headers/Bonuses/bonus_luiblockinv.hpp"
#include "../../Headers/Sprites/sprite_mushroom.hpp"
#include "../../Headers/Sprites/sprite_lui.hpp"

using namespace std;
using namespace sf;

Bonus_LuiBlockInv::Bonus_LuiBlockInv(Texture* blockTexture, Collider* collision, char once) : Bonus_BlockInv(blockTexture, collision, once)
{
    m_output.resize(4);
    m_output.setPrimitiveType(Quads);
}

void Bonus_LuiBlockInv::setPosition(Vector2f pos)
{
    Bonus_BlockInv::setPosition(pos);
}

void Bonus_LuiBlockInv::update()
{
    Bonus_BlockInv::update();
}

void Bonus_LuiBlockInv::secureUpdate()
{
    Bonus_BlockInv::secureUpdate();

    if (m_hit == 0 || m_hit == 3)
        return;

    Vector2f pos(m_sprite.getPosition());

    Placeable* item;

    switch (m_outputdir)
    {
        case 0 :
            if (m_mushroom)
            {
                if (m_output[0].position.y > pos.y - 32)
                {
                    m_output[0].position.y -= 0.6;
                    m_output[1].position.y -= 0.6;

                    m_output[2].texCoords.y += 0.6;
                    m_output[3].texCoords.y += 0.6;
                }
                else
                {
                    item = new Sprite_Mushroom(*itemsTxt[3]);
                    item->setPosition(Vector2f(pos.x, pos.y-32));
                    dynamic_cast<Sprite_Mushroom*>(item)->m_active = true;

                    if (zoneb)
                        listPlaceablesb.emplace(spriteIteratorb, item);
                    else
                        listPlaceables.emplace(spriteIterator, item);

                    m_hit = 3;
                }
            }
            else
            {
                if (m_output[0].position.y > pos.y - 31)
                {
                    m_output[0].position.y -= 0.6;
                    m_output[1].position.y -= 0.6;

                    m_output[2].texCoords.y += 0.6;
                    m_output[3].texCoords.y += 0.6;
                }
                else
                {
                    item = new Sprite_Lui(*itemsTxt[6]);
                    item->setPosition(Vector2f(pos.x+1, pos.y-31));

                    if (zoneb)
                        listPlaceablesb.emplace(spriteIteratorb, item);
                    else
                        listPlaceables.emplace(spriteIterator, item);

                    m_hit = 3;
                }
            }
            break;
        case 1 :
            if (m_mushroom)
            {
                if (m_output[0].position.x > pos.x - 32)
                {
                    m_output[0].position.x -= 0.6;
                    m_output[3].position.x -= 0.6;

                    m_output[1].texCoords.x += 0.6;
                    m_output[2].texCoords.x += 0.6;
                }
                else
                {
                    item = new Sprite_Mushroom(*itemsTxt[3]);
                    item->setPosition(Vector2f(pos.x-32, pos.y));

                    Sprite_Mushroom* mush = dynamic_cast<Sprite_Mushroom*>(item);
                    mush->m_movedistance.x = -1.5;
                    mush->m_active = true;

                    if (zoneb)
                        listPlaceablesb.emplace(spriteIteratorb, item);
                    else
                        listPlaceables.emplace(spriteIterator, item);

                    m_hit = 3;
                }
            }
            else
            {
                if (m_output[0].position.x > pos.x - 30)
                {
                    m_output[0].position.x -= 0.6;
                    m_output[3].position.x -= 0.6;

                    m_output[1].texCoords.x += 0.6;
                    m_output[2].texCoords.x += 0.6;
                }
                else
                {
                    item = new Sprite_Lui(*itemsTxt[6]);
                    item->setPosition(Vector2f(pos.x-30, pos.y+1));

                    if (zoneb)
                        listPlaceablesb.emplace(spriteIteratorb, item);
                    else
                        listPlaceables.emplace(spriteIterator, item);

                    m_hit = 3;
                }
            }
            break;
        case 2 :
            if (m_mushroom)
            {
                if (m_output[1].position.x < pos.x + 64)
                {
                    m_output[1].position.x += 0.6;
                    m_output[2].position.x += 0.6;

                    m_output[0].texCoords.x -= 0.6;
                    m_output[3].texCoords.x -= 0.6;
                }
                else
                {
                    item = new Sprite_Mushroom(*itemsTxt[3]);
                    item->setPosition(Vector2f(pos.x+32, pos.y));

                    Sprite_Mushroom* mush = dynamic_cast<Sprite_Mushroom*>(item);
                    mush->m_movedistance.x = 1.5;
                    mush->m_active = true;

                    if (zoneb)
                        listPlaceablesb.emplace(spriteIteratorb, item);
                    else
                        listPlaceables.emplace(spriteIterator, item);

                    m_hit = 3;
                }
            }
            else
            {
                if (m_output[1].position.x < pos.x + 62)
                {
                    m_output[1].position.x += 0.6;
                    m_output[2].position.x += 0.6;

                    m_output[0].texCoords.x -= 0.6;
                    m_output[3].texCoords.x -= 0.6;
                }
                else
                {
                    item = new Sprite_Lui(*itemsTxt[6]);
                    item->setPosition(Vector2f(pos.x+32, pos.y+1));

                    if (zoneb)
                        listPlaceablesb.emplace(spriteIteratorb, item);
                    else
                        listPlaceables.emplace(spriteIterator, item);

                    m_hit = 3;
                }
            }
            break;
        default :
            if (m_mushroom)
            {
                if (m_output[2].position.y < pos.y + 64)
                {
                    m_output[2].position.y += 0.6;
                    m_output[3].position.y += 0.6;

                    m_output[0].texCoords.y -= 0.6;
                    m_output[1].texCoords.y -= 0.6;
                }
                else
                {
                    item = new Sprite_Mushroom(*itemsTxt[3]);
                    item->setPosition(Vector2f(pos.x, pos.y+32));
                    dynamic_cast<Sprite_Mushroom*>(item)->m_active = true;

                    if (zoneb)
                        listPlaceablesb.emplace(spriteIteratorb, item);
                    else
                        listPlaceables.emplace(spriteIterator, item);

                    m_hit = 3;
                }
            }
            else
            {
                if (m_output[2].position.y < pos.y + 64)
                {
                    m_output[2].position.y += 0.6;
                    m_output[3].position.y += 0.6;

                    m_output[0].texCoords.y -= 0.6;
                    m_output[1].texCoords.y -= 0.6;
                }
                else
                {
                    item = new Sprite_Lui(*itemsTxt[6]);
                    item->setPosition(Vector2f(pos.x+1, pos.y+31));

                    if (zoneb)
                        listPlaceablesb.emplace(spriteIteratorb, item);
                    else
                        listPlaceables.emplace(spriteIterator, item);

                    m_hit = 3;
                }
            }
            break;
    }
}

void Bonus_LuiBlockInv::hit()
{
    Vector2f pos(m_sprite.getPosition());

    if (zoneb)
    {
        if (collisionMatrixb->getValue((pos.x/32), (pos.y/32) - 1) == 0)
            m_outputdir = 0;
        else if (collisionMatrixb->getValue((pos.x/32) - 1, (pos.y/32)) == 0)
            m_outputdir = 1;
        else if (collisionMatrixb->getValue((pos.x/32) + 1, (pos.y/32)) == 0)
            m_outputdir = 2;
        else
            m_outputdir = 3;
    }
    else
    {
        if (collisionMatrix->getValue((pos.x/32), (pos.y/32) - 1) == 0)
            m_outputdir = 0;
        else if (collisionMatrix->getValue((pos.x/32) - 1, (pos.y/32)) == 0)
            m_outputdir = 1;
        else if (collisionMatrix->getValue((pos.x/32) + 1, (pos.y/32)) == 0)
            m_outputdir = 2;
        else
            m_outputdir = 3;
    }

    if (game_powerup == 0)
        m_mushroom = true;
    else
        m_mushroom = false;

    switch (m_outputdir)
    {
        case 0 :
            if (m_mushroom)
            {
                m_output[0].position = pos;
                m_output[1].position = Vector2f(pos.x+32, pos.y);
                m_output[2].position = Vector2f(pos.x+32, pos.y);
                m_output[3].position = pos;

                m_output[0].texCoords = Vector2f(0, 0);
                m_output[1].texCoords = Vector2f(32, 0);
                m_output[2].texCoords = Vector2f(32, 0);
                m_output[3].texCoords = Vector2f(0, 0);
            }
            else
            {
                m_output[0].position = Vector2f(pos.x+1, pos.y+1);
                m_output[1].position = Vector2f(pos.x+31, pos.y+1);
                m_output[2].position = Vector2f(pos.x+31, pos.y);
                m_output[3].position = Vector2f(pos.x+1, pos.y);

                m_output[0].texCoords = Vector2f(0, 0);
                m_output[1].texCoords = Vector2f(30, 0);
                m_output[2].texCoords = Vector2f(30, -1);
                m_output[3].texCoords = Vector2f(0, -1);
            }

            break;

        case 1 :
            if (m_mushroom)
            {
                m_output[0].position = pos;
                m_output[1].position = pos;
                m_output[2].position = Vector2f(pos.x, pos.y+32);
                m_output[3].position = Vector2f(pos.x, pos.y+32);

                m_output[0].texCoords = Vector2f(0, 0);
                m_output[1].texCoords = Vector2f(0, 0);
                m_output[2].texCoords = Vector2f(0, 32);
                m_output[3].texCoords = Vector2f(0, 32);
            }
            else
            {
                m_output[0].position = Vector2f(pos.x+2, pos.y+1);
                m_output[1].position = Vector2f(pos.x, pos.y+1);
                m_output[2].position = Vector2f(pos.x, pos.y+32);
                m_output[3].position = Vector2f(pos.x+2, pos.y+32);

                m_output[0].texCoords = Vector2f(0, 0);
                m_output[1].texCoords = Vector2f(-2, 0);
                m_output[2].texCoords = Vector2f(-2, 31);
                m_output[3].texCoords = Vector2f(0, 31);
            }

            break;

        case 2 :
            if (m_mushroom)
            {
                m_output[0].position = Vector2f(pos.x+32, pos.y);
                m_output[1].position = Vector2f(pos.x+32, pos.y);
                m_output[2].position = Vector2f(pos.x+32, pos.y+32);
                m_output[3].position = Vector2f(pos.x+32, pos.y+32);

                m_output[0].texCoords = Vector2f(32, 0);
                m_output[1].texCoords = Vector2f(32, 0);
                m_output[2].texCoords = Vector2f(32, 32);
                m_output[3].texCoords = Vector2f(32, 32);
            }
            else
            {
                m_output[0].position = Vector2f(pos.x+32, pos.y+1);
                m_output[1].position = Vector2f(pos.x+30, pos.y+1);
                m_output[2].position = Vector2f(pos.x+30, pos.y+32);
                m_output[3].position = Vector2f(pos.x+32, pos.y+32);

                m_output[0].texCoords = Vector2f(32, 0);
                m_output[1].texCoords = Vector2f(30, 0);
                m_output[2].texCoords = Vector2f(30, 31);
                m_output[3].texCoords = Vector2f(32, 31);
            }

            break;

        default :
            if (m_mushroom)
            {
                m_output[0].position = Vector2f(pos.x, pos.y+32);
                m_output[1].position = Vector2f(pos.x+32, pos.y+32);
                m_output[2].position = Vector2f(pos.x+32, pos.y+32);
                m_output[3].position = Vector2f(pos.x, pos.y+32);

                m_output[0].texCoords = Vector2f(0, 32);
                m_output[1].texCoords = Vector2f(32, 32);
                m_output[2].texCoords = Vector2f(32, 32);
                m_output[3].texCoords = Vector2f(0, 32);
            }
            else
            {
                m_output[0].position = Vector2f(pos.x+1, pos.y+31);
                m_output[1].position = Vector2f(pos.x+31, pos.y+31);
                m_output[2].position = Vector2f(pos.x+31, pos.y+32);
                m_output[3].position = Vector2f(pos.x+1, pos.y+32);

                m_output[0].texCoords = Vector2f(0, 31);
                m_output[1].texCoords = Vector2f(30, 31);
                m_output[2].texCoords = Vector2f(30, 32);
                m_output[3].texCoords = Vector2f(0, 32);
            }

            break;
    }

    FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(5), sfxSamples[4], 0, NULL);
}

void Bonus_LuiBlockInv::draw(RenderTarget& target, RenderStates) const
{
    if (m_hit > 0)
    {
        if ( m_hit < 3)
        {
            if (m_mushroom)
                target.draw(m_output, itemsTxt[3]);
            else
                target.draw(m_output, itemsTxt[6]);
        }

        target.draw(m_sprite);
    }
}
