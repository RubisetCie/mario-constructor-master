/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

#ifdef DEBUGMODE
#include <iostream>
#endif

#include "../../Headers/Graphics/podobooentity.hpp"
#include "../../Headers/Graphics/entity.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

Podoboo_Entity::Podoboo_Entity(Texture* texture, Texture* platformEndTex, Texture* editorMoveable, float x, float y, float originx, float originy) : Entity(texture, x, y, originx, originy, 172)
{
    m_sprites[0].setTexture(*editorMoveable);
    m_sprites[0].setColor(Color(20, 20, 20, 255));
    m_sprites[0].setPosition(x, y);

    m_sprites[1].setTexture(*platformEndTex);
    m_sprites[1].setTextureRect(IntRect(0, 0, 32, 32));
    m_sprites[1].setPosition(x+32, y+32);
    m_sprites[1].setRotation(90);
    m_sprites[1].setScale(-1, 1);

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

Vector2f Podoboo_Entity::getPosition() const
{
    return Vector2f(m_sprite.getPosition().x, m_sprites[0].getPosition().y);
}

Vector2u Podoboo_Entity::getSize() const
{
    float endPos = m_sprites[0].getPosition().y;
    float position = m_sprite.getPosition().y - m_sprite.getOrigin().y;

    return Vector2u(32, position - endPos);
}

unsigned int Podoboo_Entity::getType() const
{
    return 172;
}

void Podoboo_Entity::save(vector<float>& entData)
{
    Vector2f position(m_sprite.getPosition());

    entData.emplace_back(position.x);
    entData.emplace_back(position.y);

    entData.emplace_back(m_sprites[0].getPosition().y);
}

void Podoboo_Entity::load(vector<float>& entData)
{
    Vector2f moveablePos(m_sprite.getPosition().x, entData[2]);

    m_sprites[0].setPosition(moveablePos);
    m_sprites[1].setPosition(moveablePos.x+32, moveablePos.y+32);

    m_vertex[1].position = Vector2f(moveablePos.x+16, moveablePos.y+16);
}

void Podoboo_Entity::update()
{
    extern int currentSelection;

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
        float moveableY = mpos_relative.y - 16;
        float originalPos = getOriginalPosition().y;

        if (Keyboard::isKeyPressed(Keyboard::LShift))
            moveableY = roundf(moveableY / 16) * 16;

        if (moveableY < 0)
            moveableY = 0;

        if (moveableY > originalPos)
            moveableY = originalPos;

        m_sprites[0].setColor(Color::White);
        m_sprites[0].setPosition(moveablePos.x, moveableY);

        m_vertex[1].position = Vector2f(moveablePos.x+16, moveableY+16);

        m_sprites[1].setPosition(moveablePos.x + 32, moveableY + 32);
    }
}

void Podoboo_Entity::drawb(RenderTarget* target) const
{
    target->draw(m_sprites[0]);
    target->draw(m_sprites[1]);
}

void Podoboo_Entity::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);

    target.draw(m_vertex, 2, Lines);
}
