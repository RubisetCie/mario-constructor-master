/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

#ifdef DEBUGMODE
#include <iostream>
#endif

#include "../../Headers/Graphics/verticalenemyentity.hpp"
#include "../../Headers/Graphics/entity.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

VerticalEnemy_Entity::VerticalEnemy_Entity(Texture* texture, Texture* platformEndTex, Texture* editorMoveable, float x, float y, float originx, float originy, unsigned int type) : Entity(texture, x, y, originx, originy, type)
{
    m_sprites[0].setTexture(*editorMoveable);
    m_sprites[0].setColor(Color(20, 20, 20, 255));
    m_sprites[0].setPosition(x, y);

    m_sprites[1].setTexture(*platformEndTex);
    m_sprites[1].setTextureRect(IntRect(32, 0, 32, 32));
    m_sprites[1].setPosition(x+32, y);
    m_sprites[1].setRotation(90);

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

Vector2f VerticalEnemy_Entity::getPosition() const
{
    Vector2f currentPos(m_sprite.getPosition());

    return Vector2f(currentPos.x, min(currentPos.y - m_sprite.getOrigin().y, m_sprites[0].getPosition().y));
}

Vector2u VerticalEnemy_Entity::getSize() const
{
    float endPos = m_sprites[0].getPosition().y;
    float position = m_sprite.getPosition().y - m_sprite.getOrigin().y;

    if (endPos < position)
        return Vector2u(32, position - endPos);
    else
        return Vector2u(32, (endPos+32) - position);
}

void VerticalEnemy_Entity::roomResized(Vector2i newsize)
{
    Vector2f moveablePos(m_sprites[0].getPosition());

    if (moveablePos.y > (newsize.y * 480) - 32)
    {
        moveablePos.y = (newsize.y * 480) - 32;

        m_sprites[0].setPosition(moveablePos.x, moveablePos.y);
        m_sprites[1].setPosition(moveablePos.x+32, moveablePos.y);

        m_vertex[1].position = Vector2f(moveablePos.x+16, moveablePos.y+16);
    }
}

void VerticalEnemy_Entity::save(vector<float>& entData)
{
    Vector2f position(m_sprite.getPosition());

    entData.emplace_back(position.x);
    entData.emplace_back(position.y);

    entData.emplace_back(m_sprites[0].getPosition().y);
}

void VerticalEnemy_Entity::load(vector<float>& entData)
{
    Vector2f originalPos(m_sprite.getPosition());
    Vector2f moveablePos(originalPos.x, entData[2]);

    m_sprites[0].setPosition(moveablePos);

    if (moveablePos.y < originalPos.y)
    {
        m_sprites[1].setScale(-1, 1);
        m_sprites[1].setTextureRect(IntRect(0, 0, 32, 32));
        m_sprites[1].setPosition(moveablePos.x + 32, moveablePos.y + 32);
    }
    else if (moveablePos.y > originalPos.y)
    {
        m_sprites[1].setTextureRect(IntRect(0, 0, 32, 32));
        m_sprites[1].setPosition(moveablePos.x + 32, moveablePos.y);
    }
    else
        m_sprites[1].setPosition(moveablePos.x + 32, moveablePos.y);

    m_vertex[1].position = Vector2f(moveablePos.x+16, moveablePos.y+16);
}

void VerticalEnemy_Entity::update()
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

        float moveableY = mpos_relative.y - 16;
        float originalPos = getOriginalPosition().y;

        if (Keyboard::isKeyPressed(Keyboard::LShift))
            moveableY = roundf(moveableY / 16) * 16;

        if (moveableY < 0)
            moveableY = 0;

        if (sectionb)
        {
            if (moveableY > (roomScaleb.y * 480) - 32)
                moveableY = (roomScaleb.y * 480) - 32;
        }
        else
        {
            if (moveableY > (roomScale.y * 480) - 32)
                moveableY = (roomScale.y * 480) - 32;
        }

        m_sprites[0].setColor(Color::White);
        m_sprites[0].setPosition(moveablePos.x, moveableY);

        m_vertex[1].position = Vector2f(moveablePos.x+16, moveableY+16);

        if (moveableY < originalPos)
        {
            m_sprites[1].setScale(-1, 1);
            m_sprites[1].setTextureRect(IntRect(0, 0, 32, 32));
            m_sprites[1].setPosition(moveablePos.x+32, moveableY + 32);
        }
        else if (moveableY > originalPos)
        {
            m_sprites[1].setScale(1, 1);
            m_sprites[1].setTextureRect(IntRect(0, 0, 32, 32));
            m_sprites[1].setPosition(moveablePos.x+32, moveableY);
        }
        else
        {
            m_sprites[1].setScale(1, 1);
            m_sprites[1].setTextureRect(IntRect(32, 0, 32, 32));
            m_sprites[1].setPosition(moveablePos.x+32, moveableY);
        }
    }
}

void VerticalEnemy_Entity::drawb(RenderTarget* target) const
{
    target->draw(m_sprites[0]);
    target->draw(m_sprites[1]);
}

void VerticalEnemy_Entity::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);

    target.draw(m_vertex, 2, Lines);
}
