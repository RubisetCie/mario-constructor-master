/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

#ifdef DEBUGMODE
#include <iostream>
#endif

#include "../../Headers/Graphics/rotodiscentity.hpp"
#include "../../Headers/Graphics/entity.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

Rotodisc_Entity::Rotodisc_Entity(Texture* texture, Texture* discText, Texture* editorMoveable, Texture* speedRegulatorTexture, Texture* sliderTexture, Texture* counterTex, float x, float y, float originx, float originy, unsigned int type, bool showCircle) : Entity(texture, x, y, originx, originy, type)
{
    m_sprite.setTextureRect(IntRect(896, 96, 32, 32));

    m_sprites[0].setTexture(*editorMoveable);
    m_sprites[0].setColor(Color(20, 20, 20, 255));
    m_sprites[0].setPosition(x, y);

    m_sprites[1].setTexture(*discText);
    m_sprites[1].setTextureRect(IntRect(0, 0, 32, 30));
    m_sprites[1].setPosition(x, y);

    m_sprites[2].setTexture(*speedRegulatorTexture);
    m_sprites[2].setPosition(x-24, y-48);

    {
        extern bool sectionb;

        extern Vector2u roomScale;
        extern Vector2u roomScaleb;

        Vector2f regulatorPos(m_sprites[2].getPosition());

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

        m_sprites[2].setPosition(regulatorPos);

        m_sprites[3].setTexture(*sliderTexture);
        m_sprites[3].setColor(Color::Red);
        m_sprites[3].setPosition(regulatorPos.x+10, regulatorPos.y);
    }

    if (counterTex != NULL)
    {
        m_sprites[4].setTexture(*counterTex);
        m_sprites[4].setTextureRect(IntRect(0, 32, 15, 16));
        m_sprites[4].setPosition(x + 16, y + 16);
    }

    m_showcircle = showCircle;

    if (showCircle)
    {
        m_shape.setOutlineColor(Color::White);
        m_shape.setFillColor(Color::Transparent);
        m_shape.setPointCount(32);
        m_shape.setPosition(x + 16, y + 16);
        m_shape.setOutlineThickness(1);
        m_shape.setRadius(0);
    }

    m_grabend = false;
    m_grabslider = false;

    m_distance = 0;
}

Vector2f Rotodisc_Entity::getPosition() const
{
    Vector2f position(m_sprite.getPosition());

    position.x -= m_distance;
    position.y -= m_distance;

    return position;
}

Vector2u Rotodisc_Entity::getSize() const
{
    return Vector2u((m_distance * 2) + 32, (m_distance * 2) + 32);
}

void Rotodisc_Entity::save(vector<float>& entData)
{
    Vector2f position(m_sprite.getPosition());
    Vector2f positionb(m_sprites[0].getPosition());

    float moveableX = m_sprites[3].getPosition().x;
    float originalX = m_sprites[2].getPosition().x;

    entData.emplace_back(position.x);
    entData.emplace_back(position.y);

    entData.emplace_back(positionb.x);
    entData.emplace_back(positionb.y);

    entData.emplace_back(roundf((((((moveableX - originalX) - 10) / 44) * 5) + 1) * 10) / 10);
}

void Rotodisc_Entity::load(vector<float>& entData)
{
    Vector2f moveablePos(entData[2], entData[3]);
    Vector2f originalPos(m_sprite.getPosition());

    FloatRect bbox;

    m_sprites[0].setPosition(moveablePos);
    m_sprites[1].setPosition(moveablePos);

    m_sprites[3].setPosition(roundf(((((entData[4] - 1) / 5) * 44) + 10) + m_sprites[2].getPosition().x), m_sprites[3].getPosition().y);

    if (m_showcircle)
    {
        m_distance = roundf(sqrtf(((originalPos.x - entData[2]) * (originalPos.x - entData[2])) + ((originalPos.y - entData[3]) * (originalPos.y - entData[3]))));

        m_shape.setRadius(m_distance);

        bbox = m_shape.getGlobalBounds();

        m_shape.setOrigin(roundf(bbox.width / 2), roundf(bbox.height / 2));
    }
}

void Rotodisc_Entity::roomResized(Vector2i newsize)
{
    Vector2f moveablePos(m_sprites[0].getPosition());

    if (moveablePos.x > (newsize.x * 640) - 32)
        moveablePos.x = (newsize.x * 640) - 32;

    if (moveablePos.y > (newsize.y * 480) - 32)
        moveablePos.y = (newsize.y * 480) - 32;

    m_sprites[0].setPosition(moveablePos.x, moveablePos.y);
    m_sprites[1].setPosition(moveablePos.x, moveablePos.y);
}

void Rotodisc_Entity::update()
{
    extern int currentSelection;
    extern bool sectionb;

    Vector2f sliderPos(m_sprites[3].getPosition());
    Vector2f moveablePos(m_sprites[0].getPosition());

    m_sprites[0].setColor(Color(20, 20, 20, 255));
    m_sprites[3].setColor(Color::Red);

    if (currentSelection == 0)
    {
        extern bool toolbarVisible;

        if (toolbarVisible)
        {
            if (!(mpos_absolute.y < 150 || (mpos_absolute.x < 133 && mpos_absolute.y > 448) || (mpos_absolute.x > 459 && mpos_absolute.y > 448)))
            {
                if (FloatRect(sliderPos.x, sliderPos.y, 16, 28).contains(mpos_relative))
                {
                    m_sprites[3].setColor(Color::Yellow);

                    if (mousePressed == 1)
                        m_grabslider = true;
                }

                if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
                {
                    m_sprites[0].setColor(Color(255, 255, 128, 255));

                    if (mousePressed == 1)
                        m_grabend = true;
                }
            }
            else
            {
                m_grabend = false;
                m_grabslider = false;
            }
        }
        else
        {
            if (FloatRect(sliderPos.x, sliderPos.y, 16, 28).contains(mpos_relative))
            {
                m_sprites[3].setColor(Color::Yellow);

                if (mousePressed == 1)
                    m_grabslider = true;
            }

            if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
            {
                m_sprites[0].setColor(Color(255, 255, 128, 255));

                if (mousePressed == 1)
                    m_grabend = true;
            }
        }

        if (mousePressed == 0)
        {
            m_grabend = false;
            m_grabslider = false;
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
                    m_sprites[3].setColor(Color::Yellow);

                    if (middlPressed == 1)
                        m_grabslider = true;
                }

                if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
                {
                    m_sprites[0].setColor(Color(255, 255, 128, 255));

                    if (middlPressed == 1)
                        m_grabend = true;
                }
            }
            else
            {
                m_grabend = false;
                m_grabslider = false;
            }
        }
        else
        {
            if (FloatRect(sliderPos.x, sliderPos.y, 16, 28).contains(mpos_relative))
            {
                m_sprites[3].setColor(Color::Yellow);

                if (middlPressed == 1)
                    m_grabslider = true;
            }

            if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
            {
                m_sprites[0].setColor(Color(255, 255, 128, 255));

                if (middlPressed == 1)
                    m_grabend = true;
            }
        }

        if (middlPressed == 0)
        {
            m_grabend = false;
            m_grabslider = false;
        }
    }

    if (m_grabslider)
    {
        float moveableX = mpos_relative.x - 8;
        float originalX = m_sprites[2].getPosition().x;

        if (moveableX < originalX + 10)
            moveableX = originalX + 10;

        if (moveableX > originalX + 54)
            moveableX = originalX + 54;

        m_sprites[3].setColor(Color::White);
        m_sprites[3].setPosition(moveableX, sliderPos.y);
    }

    if (m_grabend)
    {
        extern Vector2u roomScaleb;
        extern Vector2u roomScale;

        Vector2f cursorPos = mpos_relative - Vector2f(16, 16);
        Vector2f originalPos = getOriginalPosition();

        if (Keyboard::isKeyPressed(Keyboard::LShift))
        {
            cursorPos.x = roundf(cursorPos.x / 16) * 16;
            cursorPos.y = roundf(cursorPos.y / 16) * 16;
        }

        if (cursorPos.x < 0)
            cursorPos.x = 0;

        if (cursorPos.y < 0)
            cursorPos.y = 0;

        if (sectionb)
        {
            if (cursorPos.x > (roomScaleb.x * 640) - 32)
                cursorPos.x = (roomScaleb.x * 640) - 32;

            if (cursorPos.y > (roomScaleb.y * 480) - 32)
                cursorPos.y = (roomScaleb.y * 480) - 32;
        }
        else
        {
            if (cursorPos.x > (roomScale.x * 640) - 32)
                cursorPos.x = (roomScale.x * 640) - 32;

            if (cursorPos.y > (roomScale.y * 480) - 32)
                cursorPos.y = (roomScale.y * 480) - 32;
        }

        m_sprites[0].setColor(Color::White);
        m_sprites[0].setPosition(cursorPos);

        m_sprites[1].setPosition(cursorPos);

        if (m_showcircle)
        {
            FloatRect bbox;

            m_distance = roundf(sqrtf(((originalPos.x - cursorPos.x) * (originalPos.x - cursorPos.x)) + ((originalPos.y - cursorPos.y) * (originalPos.y - cursorPos.y))));

            m_shape.setRadius(m_distance);

            bbox = m_shape.getGlobalBounds();

            m_shape.setOrigin(roundf(bbox.width / 2), roundf(bbox.height / 2));
        }
    }
}

void Rotodisc_Entity::drawb(RenderTarget* target) const
{
    target->draw(m_sprites[2]);
    target->draw(m_sprites[3]);
    target->draw(m_sprites[1]);
    target->draw(m_sprites[0]);

    if (m_enttype == 203 || m_enttype == 205)
        target->draw(m_sprites[4]);

    if (m_showcircle)
        target->draw(m_shape);
}

void Rotodisc_Entity::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
