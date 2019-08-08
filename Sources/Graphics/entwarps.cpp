/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

#include "../../Headers/Graphics/entwarps.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

EntWarps::EntWarps(Texture* texture, Texture* moveableTex, Texture* noteTex, float x, float y, unsigned char warpId, bool section, bool entrance)
{
    m_sprite[0].setTexture(*texture);
    m_sprite[0].setPosition(x, y);
    m_sprite[0].setColor(Color(255, 255, 255, 146));

    if (entrance)
        m_sprite[0].setTextureRect(IntRect(0, 0, 64, 64));
    else
        m_sprite[0].setTextureRect(IntRect(64, 0, 64, 64));

    m_sprite[1].setTexture(*texture);
    m_sprite[1].setTextureRect(IntRect(128, 0, 29, 28));
    m_sprite[1].setPosition(x+18, y+66);

    m_sprite[2].setTexture(*moveableTex);
    m_sprite[2].setPosition(x+16, y+64);
    m_sprite[2].setColor(Color(20, 20, 20, 255));

    m_sprite[3].setTexture(*noteTex);
    m_sprite[3].setTextureRect(IntRect(warpId * 15, 0, 15, 16));
    m_sprite[3].setColor(Color(255, 255, 255, 220));
    m_sprite[3].setPosition(x+1, y+1);

    m_entrance = entrance;
    m_section = section;

    m_wid = warpId;

    m_direction = 0;

    m_grabdir = false;

    m_companionWarps = NULL;
}

Vector2f EntWarps::getPosition() const
{
    return m_sprite[0].getPosition();
}

unsigned char EntWarps::getDirection() const
{
    return m_direction;
}

bool EntWarps::getEntrance()
{
    return m_entrance;
}

bool EntWarps::getSection()
{
    return m_section;
}

void EntWarps::setId(unsigned char id)
{
    m_wid = id;

	m_sprite[3].setTextureRect(IntRect(id * 15, 0, 15, 16));
}

void EntWarps::setDirection(unsigned char dir)
{
    Vector2f centeredPoint(m_sprite[0].getPosition() + Vector2f(32, 32));

    m_direction = dir;

    switch (dir)
    {
        case 0 :

            m_sprite[2].setPosition(centeredPoint.x - 16, centeredPoint.y + 32);

            m_sprite[1].setRotation(0);
            m_sprite[1].setPosition(centeredPoint.x - 14, centeredPoint.y + 34);

            break;

        case 1 :

            m_sprite[2].setPosition(centeredPoint.x + 32, centeredPoint.y - 16);

            m_sprite[1].setRotation(-90);
            m_sprite[1].setPosition(centeredPoint.x + 34, centeredPoint.y + 14);

            break;

        case 2 :

            m_sprite[2].setPosition(centeredPoint.x - 16, centeredPoint.y - 64);

            m_sprite[1].setRotation(180);
            m_sprite[1].setPosition(centeredPoint.x + 14, centeredPoint.y - 34);

            break;

        case 3 :

            m_sprite[2].setPosition(centeredPoint.x - 64, centeredPoint.y - 16);

            m_sprite[1].setRotation(90);
            m_sprite[1].setPosition(centeredPoint.x - 34, centeredPoint.y - 14);

            break;
    }
}

WarpsData EntWarps::save()
{
    WarpsData data;
    Vector2f position(m_sprite[0].getPosition());

    data.entrancex = position.x;
    data.entrancey = position.y;

    data.entrancesection = m_section;

    data.entrancedir = m_direction;

    if (m_companionWarps == NULL)
    {
        data.exitx = -1;
        data.exity = -1;

        data.exitsection = 0;

        data.exitdir = 0;
    }
    else
    {
        Vector2f companionPosition(m_companionWarps->getPosition());

        data.exitx = companionPosition.x;
        data.exity = companionPosition.y;

        data.exitsection = m_companionWarps->getSection();

        data.exitdir = m_companionWarps->getDirection();
    }

    return data;
}

void EntWarps::update()
{
    extern int currentSelection;

    Vector2f moveablePos(m_sprite[2].getPosition());

    m_sprite[2].setColor(Color(20, 20, 20, 255));

    if (currentSelection == 0)
    {
        extern bool toolbarVisible;

        if (toolbarVisible)
        {
            if (!(mpos_absolute.y < 150 || (mpos_absolute.x < 133 && mpos_absolute.y > 448) || (mpos_absolute.x > 459 && mpos_absolute.y > 448)))
            {
                if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
                {
                    m_sprite[2].setColor(Color(255, 255, 128, 255));

                    if (mousePressed == 1)
                        m_grabdir = true;
                }
            }
            else
                m_grabdir = false;
        }
        else
        {
            if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
            {
                m_sprite[2].setColor(Color(255, 255, 128, 255));

                if (mousePressed == 1)
                    m_grabdir = true;
            }
        }

        if (mousePressed == 0)
            m_grabdir = false;
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
                    m_sprite[2].setColor(Color(255, 255, 128, 255));

                    if (middlPressed == 1)
                        m_grabdir = true;
                }
            }
            else
                m_grabdir = false;
        }
        else
        {
            if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
            {
                m_sprite[2].setColor(Color(255, 255, 128, 255));

                if (middlPressed == 1)
                    m_grabdir = true;
            }
        }

        if (middlPressed == 0)
            m_grabdir = false;
    }

    if (m_grabdir)
    {
        Vector2f centeredPoint(m_sprite[0].getPosition() + Vector2f(32, 32));

        float deltaX = mpos_relative.x - centeredPoint.x;
        float deltaY = mpos_relative.y - centeredPoint.y;

        float angle = roundf((atan2f(deltaY, deltaX) * 180 / 3.141592) / 90) * 90;

        m_sprite[2].setColor(Color::White);

        if (angle == 0)
        {
            m_sprite[2].setPosition(centeredPoint.x + 32, centeredPoint.y - 16);

            m_sprite[1].setRotation(-90);
            m_sprite[1].setPosition(centeredPoint.x + 34, centeredPoint.y + 14);

            m_direction = 1;
        }
        else if (angle == 90)
        {
            m_sprite[2].setPosition(centeredPoint.x - 16, centeredPoint.y + 32);

            m_sprite[1].setRotation(0);
            m_sprite[1].setPosition(centeredPoint.x - 14, centeredPoint.y + 34);

            m_direction = 0;
        }
        else if (angle == 180 || angle == -180)
        {
            m_sprite[2].setPosition(centeredPoint.x - 64, centeredPoint.y - 16);

            m_sprite[1].setRotation(90);
            m_sprite[1].setPosition(centeredPoint.x - 34, centeredPoint.y - 14);

            m_direction = 3;
        }
        else if (angle == -90)
        {
            m_sprite[2].setPosition(centeredPoint.x - 16, centeredPoint.y - 64);

            m_sprite[1].setRotation(180);
            m_sprite[1].setPosition(centeredPoint.x + 14, centeredPoint.y - 34);

            m_direction = 2;
        }
    }
}

void EntWarps::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite[0]);
    target.draw(m_sprite[1]);
    target.draw(m_sprite[2]);
    target.draw(m_sprite[3]);
}
