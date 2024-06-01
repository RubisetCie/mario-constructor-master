/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include <cmath>

extern "C"
{
    #include <fmod.h>
}

#ifdef DEBUGMODE
#include <iostream>
#endif

#include "../../Headers/Graphics/lightentity.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

Light_Entity::Light_Entity(Texture* lamptexture, Texture* movetexture, Vector2f position, const Color& lightcolor, bool litTex, bool litEye)
{
    m_sprite.setTexture(*lamptexture);
    m_sprite.setPosition(position);
    m_sprite.setColor(lightcolor);

    m_moveable.setTexture(*movetexture);
    m_moveable.setPosition(position);

    m_circleradius.setOutlineColor(Color::White);
    m_circleradius.setFillColor(Color::Transparent);
    m_circleradius.setPointCount(32);
    m_circleradius.setPosition(position.x + 16, position.y + 16);
    m_circleradius.setOutlineThickness(1);
    m_circleradius.setRadius(0);

    m_linelink[0].position = position + Vector2f(16, 16);
    m_linelink[0].color = lightcolor;

    m_linelink[1].position = position + Vector2f(16, 16);
    m_linelink[1].color = Color::White;

    m_lightcolor = lightcolor;

    m_readytoselection = false;

    m_radius = 0;

    m_littextures = litTex;
    m_liteyecandies = litEye;

    m_grabmover = true;

    identifier = new char;
}

Light_Entity::~Light_Entity()
{
    delete identifier;
}

Vector2f Light_Entity::getPosition() const
{
    return m_sprite.getPosition();
}

void Light_Entity::setMoveablePos(float x, float y)
{
    m_moveable.setPosition(x, y);
}

void Light_Entity::setColor(Uint8 r, Uint8 g, Uint8 b)
{
    Color const tempColor = Color(r, g, b);

    m_sprite.setColor(tempColor);
    m_lightcolor = tempColor;

    m_linelink[0].color = tempColor;
}

void Light_Entity::setAlpha(Uint8 alpha)
{
    Color tempColor = m_sprite.getColor();

    tempColor.a = alpha;

    m_sprite.setColor(tempColor);
}

void Light_Entity::setTexIllumination(bool state)
{
    m_littextures = state;
}

void Light_Entity::setEyeIllumination(bool state)
{
    m_liteyecandies = state;
}

float Light_Entity::getRadius()
{
    return m_radius;
}

void Light_Entity::mapResized(Vector2i newsize)
{
    Vector2f moveablePos(m_moveable.getPosition());

    if (moveablePos.x > (newsize.x * 32) - 32)
        moveablePos.x = (newsize.x * 32) - 32;

    if (moveablePos.y > (newsize.y * 32) - 32)
        moveablePos.y = (newsize.y * 32) - 32;

    m_moveable.setPosition(moveablePos.x, moveablePos.y);
}

void Light_Entity::save(LightData& lightData)
{
    Vector2f position(m_sprite.getPosition());
    Vector2f positionb(m_moveable.getPosition());

    lightData.x = position.x;
    lightData.y = position.y;

    lightData.moveablex = positionb.x;
    lightData.moveabley = positionb.y;

    lightData.litTextures = m_littextures;
    lightData.litEyecandies = m_liteyecandies;

    lightData.lightColor = m_lightcolor;
}

void Light_Entity::load(LightData& lightData)
{
    Vector2f pos(lightData.x, lightData.y);
    Vector2f movpos(lightData.moveablex, lightData.moveabley);

    FloatRect bbox;

    m_sprite.setPosition(pos);
    m_sprite.setColor(lightData.lightColor);

    m_littextures = lightData.litTextures;
    m_liteyecandies = lightData.litEyecandies;

    m_lightcolor = lightData.lightColor;

    setMoveablePos(movpos.x, movpos.y);

    m_linelink[0].position = pos + Vector2f(16, 16);
    m_linelink[0].color = m_lightcolor;

    m_linelink[1].position = movpos + Vector2f(16, 16);
    m_linelink[1].color = Color::White;

    float deltax = pos.x - lightData.moveablex;
    float deltay = pos.y - lightData.moveabley;
    m_radius = sqrtf((deltax * deltax) + (deltay * deltay));

    m_circleradius.setPosition(pos.x + 16, pos.y + 16);
    m_circleradius.setRadius(m_radius);

    bbox = m_circleradius.getGlobalBounds();

    m_grabmover = false;

    m_circleradius.setOrigin(bbox.width / 2, bbox.height / 2);
}

void Light_Entity::update()
{
    extern int currentEditing;
    extern bool placementAllowed;
    extern void* selectedEntity;
    extern Sprite* selectionCandy;
    extern FMOD_SOUND* editemSelection;

    Vector2f moveablePos(m_moveable.getPosition());

    m_moveable.setColor(Color(20, 20, 20, 255));

    if (currentEditing == 3)
    {
        Vector2f lightPosition(m_sprite.getPosition());
        extern bool panelVisible;

        if (panelVisible)
        {
            if (!((mpos_absolute.x < 156 && mpos_absolute.y > 448) || (mpos_absolute.x > 534 && mpos_absolute.y > 448)))
            {
                if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
                {
                    m_moveable.setColor(Color(255, 255, 128, 255));

                    if (mousePressed == 1)
                    {
                        m_grabmover = true;

                        selectedEntity = this;
                        selectionCandy->setPosition(lightPosition);
                    }

                    placementAllowed = false;
                }

                if (FloatRect(lightPosition.x, lightPosition.y, 32, 32).contains(mpos_relative))
                {
                    if (mousePressed == 1 && m_readytoselection)
                    {
                        if (selectedEntity != this)
                        {
                            selectedEntity = this;
                            selectionCandy->setPosition(lightPosition);

                            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), editemSelection, 0, NULL);
                        }
                        else
                            selectedEntity = NULL;
                    }

                    placementAllowed = false;
                }
            }
            else
                m_grabmover = false;
        }
        else
        {
            if (FloatRect(moveablePos.x, moveablePos.y, 32, 32).contains(mpos_relative))
            {
                m_moveable.setColor(Color(255, 255, 128, 255));

                placementAllowed = false;

                if (mousePressed == 1)
                {
                    m_grabmover = true;

                    selectedEntity = this;
                    selectionCandy->setPosition(lightPosition);
                }
            }

            if (FloatRect(lightPosition.x, lightPosition.y, 32, 32).contains(mpos_relative))
            {
                if (mousePressed == 1 && m_readytoselection)
                {
                    if (selectedEntity != this)
                    {
                        selectedEntity = this;
                        selectionCandy->setPosition(lightPosition);

                        FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(3), editemSelection, 0, NULL);
                    }
                    else
                        selectedEntity = NULL;
                }

                placementAllowed = false;
            }
        }

        if (mousePressed == 0)
        {
            m_grabmover = false;
            m_readytoselection = true;
        }
    }

    if (m_grabmover)
    {
        FloatRect bbox;

        extern Vector2i mapScale;

        Vector2f cursorPos = mpos_relative - Vector2f(16, 16);
        Vector2f originalPos(m_sprite.getPosition());

        if (Keyboard::isKeyPressed(Keyboard::LShift))
        {
            cursorPos.x = roundf(cursorPos.x / 16) * 16;
            cursorPos.y = roundf(cursorPos.y / 16) * 16;
        }

        if (cursorPos.x < 0)
            cursorPos.x = 0;

        if (cursorPos.y < 0)
            cursorPos.y = 0;

        if (cursorPos.x > (mapScale.x * 32) - 32)
            cursorPos.x = (mapScale.x * 32) - 32;

        if (cursorPos.y > (mapScale.y * 32) - 32)
            cursorPos.y = (mapScale.y * 32) - 32;

        m_moveable.setColor(Color::White);
        m_moveable.setPosition(cursorPos);

        m_linelink[1].position = cursorPos + Vector2f(16, 16);

        float deltax = originalPos.x - cursorPos.x;
        float deltay = originalPos.y - cursorPos.y;
        m_radius = sqrtf((deltax * deltax) + (deltay * deltay));

        m_circleradius.setRadius(m_radius);

        bbox = m_circleradius.getGlobalBounds();

        m_circleradius.setOrigin(bbox.width / 2, bbox.height / 2);
    }
}

void Light_Entity::drawb(RenderTarget* target) const
{
    extern void* selectedEntity;

    target->draw(m_linelink, 2, Lines);
    target->draw(m_moveable);

    if (selectedEntity == this)
        target->draw(m_circleradius);
}

void Light_Entity::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
