/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/globals.hpp"
#include "../../Headers/Bonuses/bonus_lifeblock.hpp"
#include "../../Headers/Sprites/sprite_life.hpp"

using namespace std;
using namespace sf;

Bonus_LifeBlock::Bonus_LifeBlock(Texture* blockTexture) : Bonus_Block(blockTexture)
{
    m_output.resize(4);
    m_output.setPrimitiveType(Quads);
}

void Bonus_LifeBlock::setPosition(const Vector2f& pos)
{
    Bonus_Block::setPosition(pos);

    m_output[0].position = pos;
    m_output[1].position = Vector2f(pos.x+32, pos.y);
    m_output[2].position = Vector2f(pos.x+32, pos.y+32);
    m_output[3].position = Vector2f(pos.x, pos.y+32);
}

void Bonus_LifeBlock::update()
{
    Bonus_Block::update();
}

void Bonus_LifeBlock::secureUpdate()
{
    Bonus_Block::secureUpdate();

    if (m_hit == 0 || m_hit == 3)
        return;

    Vector2f pos(m_sprite.getPosition());

    switch (m_outputdir)
    {
        case 0 :
            if (m_output[0].position.y > pos.y - 32)
            {
                m_output[0].position.y -= 1;
                m_output[1].position.y -= 1;

                m_output[2].texCoords.y += 1;
                m_output[3].texCoords.y += 1;
            }
            else
            {
                Sprite_Life* item = new Sprite_Life(*itemsTxt[8]);

                item->setPosition(Vector2f(pos.x, pos.y-32));
                item->m_active = true;

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
                m_output[0].position.x -= 1;
                m_output[3].position.x -= 1;

                m_output[1].texCoords.x += 1;
                m_output[2].texCoords.x += 1;
            }
            else
            {
                Sprite_Life* item = new Sprite_Life(*itemsTxt[8]);

                item->setPosition(Vector2f(pos.x-32, pos.y));
                item->m_movedistance.x = -2.2;
                item->m_active = true;

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
                m_output[1].position.x += 1;
                m_output[2].position.x += 1;

                m_output[0].texCoords.x -= 1;
                m_output[3].texCoords.x -= 1;
            }
            else
            {
                Sprite_Life* item = new Sprite_Life(*itemsTxt[8]);

                item->setPosition(Vector2f(pos.x+32, pos.y));
                item->m_movedistance.x = 2.2;
                item->m_active = true;

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
                m_output[2].position.y += 1;
                m_output[3].position.y += 1;

                m_output[0].texCoords.y -= 1;
                m_output[1].texCoords.y -= 1;
            }
            else
            {
                Sprite_Life* item = new Sprite_Life(*itemsTxt[8]);

                item->setPosition(Vector2f(pos.x, pos.y+32));
                item->m_active = true;

                if (zoneb)
                    listPlaceablesb.emplace(spriteIteratorb, item);
                else
                    listPlaceables.emplace(spriteIterator, item);

                m_hit = 3;
            }
            break;
    }
}

void Bonus_LifeBlock::hit()
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

    switch (m_outputdir)
    {
        case 0 :
            m_output[0].position = pos;
            m_output[1].position = Vector2f(pos.x+32, pos.y);
            m_output[2].position = Vector2f(pos.x+32, pos.y);
            m_output[3].position = pos;

            m_output[0].texCoords = Vector2f(0, 0);
            m_output[1].texCoords = Vector2f(32, 0);
            m_output[2].texCoords = Vector2f(32, 0);
            m_output[3].texCoords = Vector2f(0, 0);

            break;

        case 1 :
            m_output[0].position = pos;
            m_output[1].position = pos;
            m_output[2].position = Vector2f(pos.x, pos.y+32);
            m_output[3].position = Vector2f(pos.x, pos.y+32);

            m_output[0].texCoords = Vector2f(0, 0);
            m_output[1].texCoords = Vector2f(0, 0);
            m_output[2].texCoords = Vector2f(0, 32);
            m_output[3].texCoords = Vector2f(0, 32);

            break;

        case 2 :
            m_output[0].position = Vector2f(pos.x+32, pos.y);
            m_output[1].position = Vector2f(pos.x+32, pos.y);
            m_output[2].position = Vector2f(pos.x+32, pos.y+32);
            m_output[3].position = Vector2f(pos.x+32, pos.y+32);

            m_output[0].texCoords = Vector2f(32, 0);
            m_output[1].texCoords = Vector2f(32, 0);
            m_output[2].texCoords = Vector2f(32, 32);
            m_output[3].texCoords = Vector2f(32, 32);

            break;

        default :
            m_output[0].position = Vector2f(pos.x, pos.y+32);
            m_output[1].position = Vector2f(pos.x+32, pos.y+32);
            m_output[2].position = Vector2f(pos.x+32, pos.y+32);
            m_output[3].position = Vector2f(pos.x, pos.y+32);

            m_output[0].texCoords = Vector2f(0, 32);
            m_output[1].texCoords = Vector2f(32, 32);
            m_output[2].texCoords = Vector2f(32, 32);
            m_output[3].texCoords = Vector2f(0, 32);

            break;
    }

    if (pos.x > cameraPos.x - 384 && pos.x < cameraPos.x + 352 && pos.y > cameraPos.y - 304 && pos.y < cameraPos.y + 272)
        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(5), sfxSamples[4], 0, NULL);
}

void Bonus_LifeBlock::draw(RenderTarget& target, RenderStates) const
{
    if (m_hit > 0 && m_hit < 3)
        target.draw(m_output, itemsTxt[8]);

    target.draw(m_sprite);
}
