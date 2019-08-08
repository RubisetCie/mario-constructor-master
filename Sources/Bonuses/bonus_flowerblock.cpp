/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/globals.hpp"
#include "../../Headers/Bonuses/bonus_flowerblock.hpp"
#include "../../Headers/Sprites/sprite_mushroom.hpp"
#include "../../Headers/Sprites/sprite_flower.hpp"

using namespace std;
using namespace sf;

Bonus_FlowerBlock::Bonus_FlowerBlock(Texture* blockTexture) : Bonus_Block(blockTexture)
{
    m_output.resize(4);
    m_output.setPrimitiveType(Quads);

    m_frame = 12;
}

void Bonus_FlowerBlock::setPosition(Vector2f pos)
{
    Bonus_Block::setPosition(pos);
}

void Bonus_FlowerBlock::update()
{
    Bonus_Block::update();
}

void Bonus_FlowerBlock::secureUpdate()
{
    Bonus_Block::secureUpdate();

    if (m_hit == 0 || m_hit == 3)
        return;

    Vector2f pos(m_sprite.getPosition());

    Placeable* item;

    switch (m_outputdir)
    {
        case 0 :
            if (m_output[0].position.y > pos.y - 32)
            {
                m_output[0].position.y -= 0.6;
                m_output[1].position.y -= 0.6;

                m_output[2].texCoords.y += 0.6;
                m_output[3].texCoords.y += 0.6;
            }
            else
            {
                if (m_mushroom)
                {
                    item = new Sprite_Mushroom(*itemsTxt[3]);
                    dynamic_cast<Sprite_Mushroom*>(item)->m_active = true;
                }
                else
                    item = new Sprite_Flower(*itemsTxt[4]);

                item->setPosition(Vector2f(pos.x, pos.y-32));

                if (zoneb)
                    listPlaceablesb.emplace(spriteIteratorb, item);
                else
                    listPlaceables.emplace(spriteIterator, item);

                m_hit = 3;
            }
            break;
        case 1 :
            if (m_output[0].position.x > pos.x - 32)
            {
                m_output[0].position.x -= 0.6;
                m_output[3].position.x -= 0.6;

                m_output[1].texCoords.x += 0.6;
                m_output[2].texCoords.x += 0.6;
            }
            else
            {
                if (m_mushroom)
                {
                    item = new Sprite_Mushroom(*itemsTxt[3]);

                    Sprite_Mushroom* mush = dynamic_cast<Sprite_Mushroom*>(item);
                    mush->m_active = true;
                    mush->m_movedistance.x = -1.5;
                }
                else
                    item = new Sprite_Flower(*itemsTxt[4]);

                item->setPosition(Vector2f(pos.x-32, pos.y));

                if (zoneb)
                    listPlaceablesb.emplace(spriteIteratorb, item);
                else
                    listPlaceables.emplace(spriteIterator, item);

                m_hit = 3;
            }
            break;
        case 2 :
            if (m_output[1].position.x < pos.x + 64)
            {
                m_output[1].position.x += 0.6;
                m_output[2].position.x += 0.6;

                m_output[0].texCoords.x -= 0.6;
                m_output[3].texCoords.x -= 0.6;
            }
            else
            {
                if (m_mushroom)
                {
                    item = new Sprite_Mushroom(*itemsTxt[3]);

                    Sprite_Mushroom* mush = dynamic_cast<Sprite_Mushroom*>(item);
                    mush->m_movedistance.x = 1.5;
                    mush->m_active = true;
                }
                else
                    item = new Sprite_Flower(*itemsTxt[4]);

                item->setPosition(Vector2f(pos.x+32, pos.y));

                if (zoneb)
                    listPlaceablesb.emplace(spriteIteratorb, item);
                else
                    listPlaceables.emplace(spriteIterator, item);

                m_hit = 3;
            }
            break;
        default :
            if (m_output[2].position.y < pos.y + 64)
            {
                m_output[2].position.y += 0.6;
                m_output[3].position.y += 0.6;

                m_output[0].texCoords.y -= 0.6;
                m_output[1].texCoords.y -= 0.6;
            }
            else
            {
                if (m_mushroom)
                {
                    item = new Sprite_Mushroom(*itemsTxt[3]);
                    dynamic_cast<Sprite_Mushroom*>(item)->m_active = true;
                }
                else
                    item = new Sprite_Flower(*itemsTxt[4]);

                item->setPosition(Vector2f(pos.x, pos.y+32));

                if (zoneb)
                    listPlaceablesb.emplace(spriteIteratorb, item);
                else
                    listPlaceables.emplace(spriteIterator, item);

                m_hit = 3;
            }
            break;
    }

    if (!m_mushroom)
    {
        if (m_frame < 15)
            m_frame++;
        else
            m_frame = 0;

        if (m_frame % 4 == 0)
        {
            if (m_frame == 0)
            {
                m_output[0].texCoords.x -= 96;
                m_output[1].texCoords.x -= 96;
                m_output[2].texCoords.x -= 96;
                m_output[3].texCoords.x -= 96;
            }
            else
            {
                m_output[0].texCoords.x += 32;
                m_output[1].texCoords.x += 32;
                m_output[2].texCoords.x += 32;
                m_output[3].texCoords.x += 32;
            }
        }
    }
}

void Bonus_FlowerBlock::hit()
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
            m_output[0].position = pos;
            m_output[1].position = Vector2f(pos.x+32, pos.y);
            m_output[2].position = Vector2f(pos.x+32, pos.y);
            m_output[3].position = pos;

            if (m_mushroom)
            {
                m_output[0].texCoords = Vector2f(0, 0);
                m_output[1].texCoords = Vector2f(32, 0);
                m_output[2].texCoords = Vector2f(32, 0);
                m_output[3].texCoords = Vector2f(0, 0);
            }
            else
            {
                m_output[0].texCoords = Vector2f(96, 0);
                m_output[1].texCoords = Vector2f(128, 0);
                m_output[2].texCoords = Vector2f(128, 0);
                m_output[3].texCoords = Vector2f(96, 0);
            }

            break;

        case 1 :
            m_output[0].position = pos;
            m_output[1].position = pos;
            m_output[2].position = Vector2f(pos.x, pos.y+32);
            m_output[3].position = Vector2f(pos.x, pos.y+32);

            if (m_mushroom)
            {
                m_output[0].texCoords = Vector2f(0, 0);
                m_output[1].texCoords = Vector2f(0, 0);
                m_output[2].texCoords = Vector2f(0, 32);
                m_output[3].texCoords = Vector2f(0, 32);
            }
            else
            {
                m_output[0].texCoords = Vector2f(96, 0);
                m_output[1].texCoords = Vector2f(96, 0);
                m_output[2].texCoords = Vector2f(96, 32);
                m_output[3].texCoords = Vector2f(96, 32);
            }

            break;

        case 2 :
            m_output[0].position = Vector2f(pos.x+32, pos.y);
            m_output[1].position = Vector2f(pos.x+32, pos.y);
            m_output[2].position = Vector2f(pos.x+32, pos.y+32);
            m_output[3].position = Vector2f(pos.x+32, pos.y+32);

            if (m_mushroom)
            {
                m_output[0].texCoords = Vector2f(32, 0);
                m_output[1].texCoords = Vector2f(32, 0);
                m_output[2].texCoords = Vector2f(32, 32);
                m_output[3].texCoords = Vector2f(32, 32);
            }
            else
            {
                m_output[0].texCoords = Vector2f(128, 0);
                m_output[1].texCoords = Vector2f(128, 0);
                m_output[2].texCoords = Vector2f(128, 32);
                m_output[3].texCoords = Vector2f(128, 32);
            }

            break;

        default :
            m_output[0].position = Vector2f(pos.x, pos.y+32);
            m_output[1].position = Vector2f(pos.x+32, pos.y+32);
            m_output[2].position = Vector2f(pos.x+32, pos.y+32);
            m_output[3].position = Vector2f(pos.x, pos.y+32);

            if (m_mushroom)
            {
                m_output[0].texCoords = Vector2f(0, 32);
                m_output[1].texCoords = Vector2f(32, 32);
                m_output[2].texCoords = Vector2f(32, 32);
                m_output[3].texCoords = Vector2f(0, 32);
            }
            else
            {
                m_output[0].texCoords = Vector2f(96, 32);
                m_output[1].texCoords = Vector2f(128, 32);
                m_output[2].texCoords = Vector2f(128, 32);
                m_output[3].texCoords = Vector2f(96, 32);
            }

            break;
    }

    if (pos.x > cameraPos.x - 384 && pos.x < cameraPos.x + 352 && pos.y > cameraPos.y - 304 && pos.y < cameraPos.y + 272)
        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(5), sfxSamples[4], 0, NULL);
}

void Bonus_FlowerBlock::draw(RenderTarget& target, RenderStates) const
{
    if (m_hit > 0 && m_hit < 3)
    {
        if (m_mushroom)
            target.draw(m_output, itemsTxt[3]);
        else
            target.draw(m_output, itemsTxt[4]);
    }

    target.draw(m_sprite);
}
