/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

#ifdef DEBUGMODE
#include <iostream>
#endif

#include "../../Headers/Graphics/podoboorentity.hpp"
#include "../../Headers/Graphics/entity.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

PodobooR_Entity::PodobooR_Entity(Texture* texture, Texture* platformEndTex, Texture* editorMoveable, float x, float y, float originx, float originy) : Entity(texture, x, y, originx, originy, 173)
{
    m_sprite.setRotation(90);

    m_sprites[0].setTexture(*editorMoveable);
    m_sprites[0].setColor(Color(20, 20, 20, 255));
    m_sprites[0].setPosition(x, y);

    m_sprites[1].setTexture(*platformEndTex);
    m_sprites[1].setTextureRect(IntRect(0, 0, 32, 32));
    m_sprites[1].setPosition(x, y);

    m_vertex[0].position = Vector2f(x+16, y+16);
    m_vertex[0].color = Color::Red;

    {
        Vector2f moveablePos(m_sprites[0].getPosition());

        m_vertex[1].position = Vector2f(moveablePos.x+16, moveablePos.y+16);
        m_vertex[1].color = Color(255, 128, 128, 255);
    }

    m_sprite.setTextureRect(IntRect(0, 0, 27, 32));

    m_grabend = false;
}

Vector2f PodobooR_Entity::getPosition() const
{
    return Vector2f(m_sprite.getPosition().x, m_sprite.getPosition().y);
}

Vector2u PodobooR_Entity::getSize() const
{
    float endPos = m_sprites[0].getPosition().x;
    float position = m_sprite.getPosition().x - m_sprite.getOrigin().x;

    return Vector2u(position - endPos, 32);
}

void PodobooR_Entity::roomResized(Vector2i newsize)
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

unsigned int PodobooR_Entity::getType() const
{
    return 175;
}

void PodobooR_Entity::save(vector<float>& entData)
{
    Vector2f position(m_sprite.getPosition());

    entData.emplace_back(position.x);
    entData.emplace_back(position.y);

    entData.emplace_back(m_sprites[0].getPosition().x);
}

void PodobooR_Entity::load(vector<float>& entData)
{
    Vector2f moveablePos(entData[2], m_sprite.getPosition().y);

    m_sprites[0].setPosition(moveablePos);
    m_sprites[1].setPosition(moveablePos.x, moveablePos.y);

    m_vertex[1].position = Vector2f(moveablePos.x+16, moveablePos.y+16);
}

void PodobooR_Entity::update()
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

        if (moveableX < originalPos)
            moveableX = originalPos;

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

        m_sprites[1].setPosition(moveableX, moveablePos.y);
    }
}

void PodobooR_Entity::drawb(RenderTarget* target) const
{
    target->draw(m_sprites[0]);
    target->draw(m_sprites[1]);
}

void PodobooR_Entity::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);

    target.draw(m_vertex, 2, Lines);
}
