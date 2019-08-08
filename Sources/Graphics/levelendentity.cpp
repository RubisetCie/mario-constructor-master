/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

#include "../../Headers/Graphics/levelendentity.hpp"
#include "../../Headers/Graphics/entity.hpp"
#include "../../Headers/globals.hpp"

#define ARROW_MAXDISTANCE 320
#define ARROW_MINDISTANCE 32

using namespace sf;
using namespace std;

LevelEnd_Entity::LevelEnd_Entity(Texture* texture, Texture* textureb, Texture* moveableTexture, float x, float y) : Entity(texture, x, y, -2, 256, 2)
{
    m_spriteb.setTexture(*textureb);
    m_spriteb.setPosition(x-32, y);

    m_moveable.setTexture(*moveableTexture);
    m_moveable.setColor(Color(20, 20, 20, 255));
    m_moveable.setPosition(x-32, y);

    m_grab = false;
}

Vector2f LevelEnd_Entity::getPosition() const
{
    return Vector2f(m_moveable.getPosition().x, m_sprite.getPosition().y-256);
}

Vector2u LevelEnd_Entity::getSize() const
{
    float dist = m_sprite.getPosition().x - m_moveable.getPosition().x;

    return Vector2u(93 + dist, 288);
}

unsigned int LevelEnd_Entity::getType() const
{
    return 2;
}

void LevelEnd_Entity::save(vector<float>& entData)
{
    Vector2f position(m_sprite.getPosition());

    entData.emplace_back(position.x);
    entData.emplace_back(position.y);

    entData.emplace_back(m_moveable.getPosition().x);
}

void LevelEnd_Entity::load(vector<float>& entData)
{
    Vector2f moveablePos(entData[2], m_sprite.getPosition().y);

    m_moveable.setPosition(moveablePos);
    m_spriteb.setPosition(moveablePos.x + 1, moveablePos.y);
}

void LevelEnd_Entity::update()
{
    extern int currentSelection;

    Vector2f secondPos(m_moveable.getPosition());

    m_moveable.setColor(Color(20, 20, 20, 255));

    if (currentSelection == 0)
    {
        extern bool toolbarVisible;

        if (toolbarVisible)
        {
            if (!(mpos_absolute.y < 150 || (mpos_absolute.x < 133 && mpos_absolute.y > 448) || (mpos_absolute.x > 459 && mpos_absolute.y > 448)))
            {
                if (FloatRect(secondPos.x, secondPos.y, 32, 32).contains(mpos_relative))
                {
                    m_moveable.setColor(Color(255, 255, 128, 255));

                    if (mousePressed == 1)
                        m_grab = true;
                }
            }
            else
                m_grab = false;
        }
        else
        {
            if (FloatRect(secondPos.x, secondPos.y, 32, 32).contains(mpos_relative))
            {
                m_moveable.setColor(Color(255, 255, 128, 255));

                if (mousePressed == 1)
                    m_grab = true;
            }
        }

        if (mousePressed == 0)
            m_grab = false;
    }
    else
    {
        extern bool toolbarVisible;

        if (toolbarVisible)
        {
            if (!(mpos_absolute.y < 150 || (mpos_absolute.x < 133 && mpos_absolute.y > 448) || (mpos_absolute.x > 459 && mpos_absolute.y > 448)))
            {
                if (FloatRect(secondPos.x, secondPos.y, 32, 32).contains(mpos_relative))
                {
                    m_moveable.setColor(Color(255, 255, 128, 255));

                    if (middlPressed == 1)
                        m_grab = true;
                }
            }
            else
                m_grab = false;
        }
        else
        {
            if (FloatRect(secondPos.x, secondPos.y, 32, 32).contains(mpos_relative))
            {
                m_moveable.setColor(Color(255, 255, 128, 255));

                if (middlPressed == 1)
                    m_grab = true;
            }
        }

        if (middlPressed == 0)
            m_grab = false;
    }

    if (m_grab)
    {
        float moveableX = mpos_relative.x - 16;
        float originalX = getOriginalPosition().x;

        if (moveableX < originalX - ARROW_MAXDISTANCE)
            moveableX = originalX - ARROW_MAXDISTANCE;

        if (moveableX < 0)
            moveableX = 0;

        if (moveableX > originalX - ARROW_MINDISTANCE)
            moveableX = originalX - ARROW_MINDISTANCE;

        m_moveable.setColor(Color::White);

        if (Keyboard::isKeyPressed(Keyboard::LShift))
            moveableX = roundf(moveableX / 16) * 16;

        m_moveable.setPosition(moveableX, secondPos.y);
        m_spriteb.setPosition(moveableX + 1, secondPos.y);
    }
}

void LevelEnd_Entity::drawb(RenderTarget* target) const
{
    target->draw(m_moveable);
}

void LevelEnd_Entity::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);

    target.draw(m_spriteb);
}
