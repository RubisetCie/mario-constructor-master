/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Graphics/infobutton.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

InfoButton::InfoButton(const Texture& texture, unsigned char menuNumb)
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(96 * menuNumb, 0, 96, 69));
    m_sprite.setOrigin(48, 23);

    m_menunumb = menuNumb;
}

void InfoButton::setSound(FMOD_SOUND** clickSound)
{
    m_clicksound = clickSound;
}

void InfoButton::setPosition(float x, float y)
{
    m_sprite.setPosition(x, y);
}

void InfoButton::update()
{
    extern bool selectionWillFadeOut;
    extern unsigned char currentInfo;

    extern void RecalculateInfos(unsigned char info);

    FloatRect aabb(m_sprite.getPosition().x-48, m_sprite.getPosition().y-23, 96, 46);

    if (aabb.contains(mpos_relative))
    {
        float color = m_sprite.getColor().r;

        if (selectionWillFadeOut)
        {
            if (color > 24)
                color -= 24;
            else
            {
                color = 0;
                selectionWillFadeOut = false;
            }
        }
        else
        {
            if (color < 231)
                color += 24;
            else
            {
                color = 255;
                selectionWillFadeOut = true;
            }
        }

        m_sprite.setColor(Color(color, color, 0, 255));

        if (mousePressed == 1 && currentInfo != m_menunumb)
        {
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), *m_clicksound, 0, NULL);

            RecalculateInfos(m_menunumb);

            currentInfo = m_menunumb;
        }
    }
    else
        m_sprite.setColor(Color(255, 255, 255, 255));
}

void InfoButton::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
