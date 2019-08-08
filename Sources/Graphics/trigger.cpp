/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/globals.hpp"
#include "../../Headers/Graphics/trigger.hpp"

using namespace sf;
using namespace std;

Trigger::Trigger(Vector2f position, Vector2f size, unsigned char number, Texture* textTexture, Texture* moveableTex, Texture* iconTex, float height)
{
    m_rectangle.setFillColor(Color(255, 0, 255, 128));
    m_rectangle.setOutlineColor(Color(200, 200, 255));
    m_rectangle.setOutlineThickness(2);

    m_rectangle.setPosition(position);
    m_rectangle.setSize(size);

    m_text[0].setTexture(*textTexture);
    m_text[0].setTextureRect(IntRect(0, 0, 108, 16));
    m_text[0].setPosition(position + Vector2f(2, 2));

    m_text[1].setTexture(*textTexture);
    m_text[1].setTextureRect(IntRect(number * 15, 16, 15, 16));
    m_text[1].setPosition(position + Vector2f(120, 2));

    m_text[2].setTexture(*textTexture);
    m_text[2].setTextureRect(IntRect(number * 15, 16, 15, 16));

    {
        Vector2f moveablePos((position.x + size.x / 2) - 16, (position.y + size.y / 2) - 16);

        m_height[0].setTexture(*moveableTex);
        m_height[1].setTexture(*iconTex);

        if (height >= 0)
        {
            m_height[0].setPosition(moveablePos.x, height - 16);
            m_height[1].setPosition(moveablePos.x, height - 16);

            m_text[2].setPosition(moveablePos.x + 17, height - 16);
        }
        else
        {
            m_height[0].setPosition(moveablePos);
            m_height[1].setPosition(moveablePos);

            m_text[2].setPosition(moveablePos.x + 17, moveablePos.y);
        }
    }

    m_grabheight = false;
}

Vector2f Trigger::getPosition() const
{
    Vector2f currentPos(m_rectangle.getPosition());

    return Vector2f(currentPos.x, min(currentPos.y, m_height[0].getPosition().y));
}

float Trigger::getMoveablePosition()
{
    return m_height[0].getPosition().y;
}

Vector2f Trigger::getFullPosition()
{
    return m_rectangle.getPosition();
}

Vector2u Trigger::getSize()
{
    float moveablePos = m_height[0].getPosition().y;
    float currentPos = m_rectangle.getPosition().y;

    Vector2f currentSize(m_rectangle.getSize());

    if (moveablePos > currentPos)
        return Vector2u(currentSize.x, max(moveablePos - currentPos, currentSize.y));
    else
        return Vector2u(currentSize.x, currentPos - moveablePos);
}

Vector2f Trigger::getFullSize()
{
    return m_rectangle.getSize();
}

void Trigger::decreaseNumb()
{
    int currentNumb = m_text[1].getTextureRect().left;

	m_text[1].setTextureRect(IntRect(currentNumb - 15, 16, 15, 16));
	m_text[2].setTextureRect(IntRect(currentNumb - 15, 16, 15, 16));
}

void Trigger::update()
{
    extern bool toolbarVisible;

    Vector2f moveablePos(m_height[0].getPosition());

    if (toolbarVisible)
    {
        if (!((mpos_absolute.x > 572 && mpos_absolute.y > 448) || (mpos_absolute.x < 75 && mpos_absolute.y < 478 && mpos_absolute.y > 414)))
        {
            if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
            {
                m_height[0].setColor(Color(255, 255, 128, 255));

                if (mousePressed == 1 || middlPressed == 1)
                    m_grabheight = true;
            }
        }
        else
            m_grabheight = false;
    }
    else
    {
        if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
        {
            m_height[0].setColor(Color(255, 255, 128, 255));

            if (mousePressed == 1 || middlPressed == 1)
                m_grabheight = true;
        }
    }

    if (mousePressed == 0 && middlPressed == 0)
        m_grabheight = false;

    if (m_grabheight)
    {
        extern bool sectionb;

        extern Vector2u roomScaleb;
        extern Vector2u roomScale;

        float moveableY = mpos_relative.y - 16;

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

        mousePressed = 2;

        m_height[0].setColor(Color::White);
        m_height[0].setPosition(moveablePos.x, moveableY);

        m_height[1].setPosition(moveablePos.x, moveableY);

        m_text[2].setPosition(moveablePos.x + 17, moveableY);
    }
}

void Trigger::safeUpdate()
{
    extern bool sectionb;

    m_height[0].setColor(Color(20, 20, 20, 255));

    if (sectionb)
    {
        extern unsigned char levelLiquidTypeb;

        if (levelLiquidTypeb == 1)
            m_height[1].setTextureRect(IntRect(0, 0, 32, 32));
        else
            m_height[1].setTextureRect(IntRect(32, 0, 32, 32));
    }
    else
    {
        extern unsigned char levelLiquidType;

        if (levelLiquidType == 1)
            m_height[1].setTextureRect(IntRect(0, 0, 32, 32));
        else
            m_height[1].setTextureRect(IntRect(32, 0, 32, 32));
    }
}

void Trigger::setMoveablePosition(float pos)
{
    float moveableX = m_height[0].getPosition().x;

    m_height[0].setPosition(moveableX, pos);
    m_height[1].setPosition(moveableX, pos);

    m_text[2].setPosition(moveableX + 17, pos);
}

TriggerData Trigger::save()
{
    TriggerData data;

    Vector2f position(m_rectangle.getPosition());
    Vector2f size(m_rectangle.getSize());

    data.x = position.x;
    data.y = position.y;

    data.width = size.x;
    data.height = size.y;

    data.targetHeight = m_height[0].getPosition().y + 16;

    return data;
}

void Trigger::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(m_rectangle);

    target.draw(m_height[1]);
    target.draw(m_height[0]);

    target.draw(m_text[0]);
    target.draw(m_text[1]);
    target.draw(m_text[2]);
}
