/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

#ifdef DEBUGMODE
#include <iostream>
#endif

#include "../../Headers/Graphics/horizontalplatformentity.hpp"
#include "../../Headers/Graphics/entity.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

HorizontalPlatform_Entity::HorizontalPlatform_Entity(Texture* texture, Texture* speedRegulatorTexture, Texture* sliderTexture, Texture* platformEndTex, Texture* editorMoveable, Texture* platformText, IntRect textRect, float x, float y, float originx, float originy, float regulatorPos, unsigned int type, bool iscloud) : Entity(texture, x, y, originx, originy, type)
{
    m_sprites[0].setTexture(*speedRegulatorTexture);
    m_sprites[0].setPosition(x-regulatorPos, y-48);

    {
        extern bool sectionb;

        extern Vector2i roomScale;
        extern Vector2i roomScaleb;

        Vector2f regulatorPos(m_sprites[0].getPosition());

        if (regulatorPos.x < 2)
            regulatorPos.x = 2;

        if (regulatorPos.y < 2)
            regulatorPos.y = y + 32;

        if (sectionb)
        {
            if (regulatorPos.x > (roomScaleb.x * 640) - 82)
                regulatorPos.x = (roomScaleb.x * 640) - 82;
        }
        else
        {
            if (regulatorPos.x > (roomScale.x * 640) - 82)
                regulatorPos.x = (roomScale.x * 640) - 82;
        }

        m_sprites[0].setPosition(regulatorPos);

        m_sprites[1].setTexture(*sliderTexture);
        m_sprites[1].setColor(Color::Red);
        m_sprites[1].setPosition(regulatorPos.x+10, regulatorPos.y+12);
    }

    m_sprites[2].setTexture(*editorMoveable);
    m_sprites[2].setColor(Color(20, 20, 20, 255));
    m_sprites[2].setPosition(x, y);

    m_sprites[3].setTexture(*platformEndTex);
    m_sprites[3].setTextureRect(IntRect(32, 0, 32, 32));
    m_sprites[3].setPosition(x, y);

    m_sprites[4].setTexture(*platformText);
    m_sprites[4].setTextureRect(textRect);
    m_sprites[4].setPosition(x-regulatorPos, y+16);

    m_sprites[5].setTexture(*texture);
    m_sprites[5].setOrigin(originx, originy);
    m_sprites[5].setPosition(x, y);
    m_sprites[5].setColor(Color(255, 255, 255, 128));

    if (iscloud)
    {
        m_sprite.setTextureRect(IntRect(0, 0, 126, 32));
        m_sprites[5].setTextureRect(IntRect(0, 0, 126, 32));
    }

    m_vertex[0].position = Vector2f(x+16, y+16);
    m_vertex[0].color = Color::Red;

    {
        Vector2f moveablePos(m_sprites[2].getPosition());

        m_vertex[1].position = Vector2f(moveablePos.x+16, moveablePos.y+16);
        m_vertex[1].color = Color(255, 128, 128, 255);
    }

    m_grabslider = false;
    m_grabend = false;
}

Vector2f HorizontalPlatform_Entity::getPosition() const
{
    Vector2f currentPos(m_sprite.getPosition());

    return Vector2f(min(currentPos.x - m_sprite.getOrigin().x, m_sprites[2].getPosition().x), currentPos.y);
}

Vector2u HorizontalPlatform_Entity::getSize() const
{
    float platSize = m_sprite.getGlobalBounds().width;
    float endPos = m_sprites[2].getPosition().x;
    float position = m_sprite.getPosition().x - m_sprite.getOrigin().x;

    if (endPos < position)
        return Vector2u(max((position - endPos) + platSize, platSize), 32);
    else
        return Vector2u(max((endPos+32) - position, platSize), 32);
}

void HorizontalPlatform_Entity::roomResized(Vector2i newsize)
{
    Vector2f moveablePos(m_sprites[2].getPosition());

    if (moveablePos.x > (newsize.x * 640) - 32)
    {
        moveablePos.x = (newsize.x * 640) - 32;

        m_sprites[2].setPosition(moveablePos.x, moveablePos.y);
        m_sprites[3].setPosition(moveablePos.x, moveablePos.y);

        m_sprites[5].setPosition(moveablePos.x, moveablePos.y);

        m_vertex[1].position = Vector2f(moveablePos.x+16, moveablePos.y+16);
    }
}

void HorizontalPlatform_Entity::save(vector<float>& entData)
{
    Vector2f position(m_sprite.getPosition());

    float moveableX = m_sprites[1].getPosition().x;
    float originalX = m_sprites[0].getPosition().x;

    entData.emplace_back(position.x);
    entData.emplace_back(position.y);

    entData.emplace_back(m_sprites[2].getPosition().x);

    entData.emplace_back(roundf((((((moveableX - originalX) - 10) / 44) * 5) + 1) * 10) / 10);
}

void HorizontalPlatform_Entity::load(vector<float>& entData)
{
    Vector2f originalPos(m_sprite.getPosition());
    Vector2f moveablePos(entData[2], originalPos.y);

    m_sprites[1].setPosition(roundf(((((entData[3] - 1) / 5) * 44) + 10) + m_sprites[0].getPosition().x), m_sprites[1].getPosition().y);

    m_sprites[2].setPosition(moveablePos);

    m_sprites[5].setPosition(moveablePos);

    if (moveablePos.x < originalPos.x)
    {
        m_sprites[3].setScale(-1, 1);
        m_sprites[3].setTextureRect(IntRect(0, 0, 32, 32));
        m_sprites[3].setPosition(moveablePos.x + 32, moveablePos.y);
    }
    else if (moveablePos.x > originalPos.x)
    {
        m_sprites[3].setTextureRect(IntRect(0, 0, 32, 32));
        m_sprites[3].setPosition(moveablePos.x, moveablePos.y);
    }
    else
        m_sprites[3].setPosition(moveablePos.x, moveablePos.y);

    m_vertex[1].position = Vector2f(moveablePos.x+16, moveablePos.y+16);
}

void HorizontalPlatform_Entity::update()
{
    extern int currentSelection;
    extern bool sectionb;

    Vector2f sliderPos(m_sprites[1].getPosition());
    Vector2f moveablePos(m_sprites[2].getPosition());

    m_sprites[1].setColor(Color::Red);
    m_sprites[2].setColor(Color(20, 20, 20, 255));

    if (currentSelection == 0)
    {
        extern bool toolbarVisible;

        if (toolbarVisible)
        {
            if (!(mpos_absolute.y < 150 || (mpos_absolute.x < 133 && mpos_absolute.y > 448) || (mpos_absolute.x > 459 && mpos_absolute.y > 448)))
            {
                if (FloatRect(sliderPos.x, sliderPos.y, 16, 28).contains(mpos_relative))
                {
                    m_sprites[1].setColor(Color::Yellow);

                    if (mousePressed == 1)
                        m_grabslider = true;
                }

                if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
                {
                    m_sprites[2].setColor(Color(255, 255, 128, 255));

                    if (mousePressed == 1)
                        m_grabend = true;
                }
            }
            else
            {
                m_grabslider = false;
                m_grabend = false;
            }
        }
        else
        {
            if (FloatRect(sliderPos.x, sliderPos.y, 16, 28).contains(mpos_relative))
            {
                m_sprites[1].setColor(Color::Yellow);

                if (mousePressed == 1)
                    m_grabslider = true;
            }

            if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
            {
                m_sprites[2].setColor(Color(255, 255, 128, 255));

                if (mousePressed == 1)
                    m_grabend = true;
            }
        }

        if (mousePressed == 0)
        {
            m_grabslider = false;
            m_grabend = false;
        }
    }
    else
    {
        extern bool toolbarVisible;

        if (toolbarVisible)
        {
            if (!(mpos_absolute.y < 150 || (mpos_absolute.x < 133 && mpos_absolute.y > 448) || (mpos_absolute.x > 459 && mpos_absolute.y > 448)))
            {
                if (FloatRect(sliderPos.x, sliderPos.y, 16, 28).contains(mpos_relative))
                {
                    m_sprites[1].setColor(Color::Yellow);

                    if (middlPressed == 1)
                        m_grabslider = true;
                }

                if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
                {
                    m_sprites[2].setColor(Color(255, 255, 128, 255));

                    if (middlPressed == 1)
                        m_grabend = true;
                }
            }
            else
            {
                m_grabslider = false;
                m_grabend = false;
            }
        }
        else
        {
            if (FloatRect(sliderPos.x, sliderPos.y, 16, 28).contains(mpos_relative))
            {
                m_sprites[1].setColor(Color::Yellow);

                if (middlPressed == 1)
                    m_grabslider = true;
            }

            if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
            {
                m_sprites[2].setColor(Color(255, 255, 128, 255));

                if (middlPressed == 1)
                    m_grabend = true;
            }
        }

        if (middlPressed == 0)
        {
            m_grabslider = false;
            m_grabend = false;
        }
    }

    if (m_grabslider)
    {
        float moveableX = mpos_relative.x - 8;
        float originalX = m_sprites[0].getPosition().x;

        if (moveableX < originalX + 10)
            moveableX = originalX + 10;

        if (moveableX > originalX + 54)
            moveableX = originalX + 54;

        m_sprites[1].setColor(Color::White);
        m_sprites[1].setPosition(moveableX, sliderPos.y);
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

        m_sprites[2].setColor(Color::White);
        m_sprites[2].setPosition(moveableX, moveablePos.y);

        m_sprites[5].setPosition(moveableX, moveablePos.y);

        m_vertex[1].position = Vector2f(moveableX+16, moveablePos.y+16);

        if (moveableX < originalPos)
        {
            m_sprites[3].setScale(-1, 1);
            m_sprites[3].setTextureRect(IntRect(0, 0, 32, 32));
            m_sprites[3].setPosition(moveableX + 32, moveablePos.y);
        }
        else if (moveableX > originalPos)
        {
            m_sprites[3].setScale(1, 1);
            m_sprites[3].setTextureRect(IntRect(0, 0, 32, 32));
            m_sprites[3].setPosition(moveableX, moveablePos.y);
        }
        else
        {
            m_sprites[3].setScale(1, 1);
            m_sprites[3].setTextureRect(IntRect(32, 0, 32, 32));
            m_sprites[3].setPosition(moveableX, moveablePos.y);
        }
    }
}

void HorizontalPlatform_Entity::drawb(RenderTarget* target) const
{
    target->draw(m_sprites[0]);
    target->draw(m_sprites[1]);

    if (m_sprites[2].getPosition().x != m_sprite.getPosition().x)
        target->draw(m_sprites[5]);

    target->draw(m_sprites[3]);
    target->draw(m_sprites[2]);
    target->draw(m_sprites[4]);
}

void HorizontalPlatform_Entity::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);

    target.draw(m_vertex, 2, Lines);
}
