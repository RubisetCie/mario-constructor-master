/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

#ifdef DEBUGMODE
#include <iostream>
#endif

#include "../../Headers/Graphics/horizontalenemyentity.hpp"
#include "../../Headers/Graphics/entity.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

HorizontalEnemy_Entity::HorizontalEnemy_Entity(Texture* texture, Texture* platformEndTex, Texture* editorMoveable, float x, float y, float originx, float originy, unsigned int type) : Entity(texture, x, y, originx, originy, type)
{
    m_sprites[0].setTexture(*editorMoveable);
    m_sprites[0].setColor(Color(20, 20, 20, 255));
    m_sprites[0].setPosition(x, y);

    m_sprites[1].setTexture(*platformEndTex);
    m_sprites[1].setTextureRect(IntRect(32, 0, 32, 32));
    m_sprites[1].setPosition(x, y);

    m_vertex[0].position = Vector2f(x+16, y+16);
    m_vertex[0].color = Color::Red;

    {
        Vector2f moveablePos(m_sprites[0].getPosition());

        m_vertex[1].position = Vector2f(moveablePos.x+16, moveablePos.y+16);
        m_vertex[1].color = Color(255, 128, 128, 255);
    }

    m_sprite.setTextureRect(IntRect(0, 0, 32, 47));

    m_grabend = false;
}

Vector2f HorizontalEnemy_Entity::getPosition() const
{
    Vector2f currentPos(m_sprite.getPosition());

    return Vector2f(min(currentPos.x - m_sprite.getOrigin().x, m_sprites[0].getPosition().x), currentPos.y);
}

Vector2u HorizontalEnemy_Entity::getSize() const
{
    float endPos = m_sprites[0].getPosition().x;
    float position = m_sprite.getPosition().x - m_sprite.getOrigin().x;

    if (endPos < position)
        return Vector2u(position - endPos, 48);
    else
        return Vector2u((endPos+32) - position, 48);
}

void HorizontalEnemy_Entity::roomResized(Vector2i newsize)
{
    Vector2f moveablePos(m_sprites[0].getPosition());

    if (moveablePos.x > (newsize.x * 640) - 32)
    {
        moveablePos.x = (newsize.x * 640) - 32;

        m_sprites[0].setPosition(moveablePos.x, moveablePos.y);
        m_sprites[1].setPosition(moveablePos.x, moveablePos.y);

        m_vertex[1].position = Vector2f(moveablePos.x+16, moveablePos.y+16);
    }
}

void HorizontalEnemy_Entity::save(vector<float>& entData)
{
    Vector2f position(m_sprite.getPosition());

    entData.emplace_back(position.x);
    entData.emplace_back(position.y);

    entData.emplace_back(m_sprites[0].getPosition().x);
}

void HorizontalEnemy_Entity::load(vector<float>& entData)
{
    Vector2f originalPos(m_sprite.getPosition());
    Vector2f moveablePos(entData[2], originalPos.y);

    m_sprites[0].setPosition(moveablePos);

    if (moveablePos.x < originalPos.x)
    {
        m_sprites[1].setScale(-1, 1);
        m_sprites[1].setTextureRect(IntRect(0, 0, 32, 32));
        m_sprites[1].setPosition(moveablePos.x + 32, moveablePos.y);
    }
    else if (moveablePos.x > originalPos.x)
    {
        m_sprites[1].setTextureRect(IntRect(0, 0, 32, 32));
        m_sprites[1].setPosition(moveablePos.x, moveablePos.y);
    }
    else
        m_sprites[1].setPosition(moveablePos.x, moveablePos.y);

    m_vertex[1].position = Vector2f(moveablePos.x+16, moveablePos.y+16);
}

void HorizontalEnemy_Entity::update()
{
    extern int currentSelection;
    extern bool sectionb;

    Vector2f moveablePos(m_sprites[0].getPosition());

    m_sprites[0].setColor(Color(20, 20, 20, 255));

    if (currentSelection == 0)
    {
        extern bool toolbarVisible;

        if (toolbarVisible)
        {
            if (!(mpos_absolute.y < 150 || (mpos_absolute.x < 133 && mpos_absolute.y > 448) || (mpos_absolute.x > 459 && mpos_absolute.y > 448)))
            {
                if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
                {
                    m_sprites[0].setColor(Color(255, 255, 128, 255));

                    if (mousePressed == 1)
                        m_grabend = true;
                }
            }
            else
                m_grabend = false;
        }
        else
        {
            if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
            {
                m_sprites[0].setColor(Color(255, 255, 128, 255));

                if (mousePressed == 1)
                    m_grabend = true;
            }
        }

        if (mousePressed == 0)
            m_grabend = false;
    }
    else
    {
        extern bool toolbarVisible;

        if (toolbarVisible)
        {
            if (!(mpos_absolute.y < 150 || (mpos_absolute.x < 133 && mpos_absolute.y > 448) || (mpos_absolute.x > 459 && mpos_absolute.y > 448)))
            {
                if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
                {
                    m_sprites[0].setColor(Color(255, 255, 128, 255));

                    if (middlPressed == 1)
                        m_grabend = true;
                }
            }
            else
                m_grabend = false;
        }
        else
        {
            if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
            {
                m_sprites[0].setColor(Color(255, 255, 128, 255));

                if (middlPressed == 1)
                    m_grabend = true;
            }
        }

        if (middlPressed == 0)
            m_grabend = false;
    }

    if (m_grabend)
    {
        extern Vector2u roomScaleb;
        extern Vector2u roomScale;

        float moveableX = mpos_relative.x - 16;
        float originalPos = getOriginalPosition().x;

        if (Keyboard::isKeyPressed(Keyboard::LShift))
            moveableX = roundf(moveableX / 16) * 16;

        if (moveableX < 0)
            moveableX = 0;

        if (sectionb)
        {
            if (moveableX > (roomScaleb.x * 640) - 32)
                moveableX = (roomScaleb.x * 640) - 32;
        }
        else
        {
            if (moveableX > (roomScale.x * 640) - 32)
                moveableX = (roomScale.x * 640) - 32;
        }

        m_sprites[0].setColor(Color::White);
        m_sprites[0].setPosition(moveableX, moveablePos.y);

        m_vertex[1].position = Vector2f(moveableX+16, moveablePos.y+16);

        if (moveableX < originalPos)
        {
            m_sprites[1].setScale(-1, 1);
            m_sprites[1].setTextureRect(IntRect(0, 0, 32, 32));
            m_sprites[1].setPosition(moveableX+ 32, moveablePos.y);
        }
        else if (moveableX > originalPos)
        {
            m_sprites[1].setScale(1, 1);
            m_sprites[1].setTextureRect(IntRect(0, 0, 32, 32));
            m_sprites[1].setPosition(moveableX, moveablePos.y);
        }
        else
        {
            m_sprites[1].setScale(1, 1);
            m_sprites[1].setTextureRect(IntRect(32, 0, 32, 32));
            m_sprites[1].setPosition(moveableX, moveablePos.y);
        }
    }
}

void HorizontalEnemy_Entity::drawb(RenderTarget* target) const
{
    target->draw(m_sprites[0]);
    target->draw(m_sprites[1]);
}

void HorizontalEnemy_Entity::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);

    target.draw(m_vertex, 2, Lines);
}
