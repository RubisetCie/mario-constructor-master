/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Graphics/checkbutton.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;
using namespace std;

CheckButton::CheckButton(void (*ptrFunction) (), const Texture& checkTexture, string helptext)
{
    m_sprite.setTexture(checkTexture);
    m_sprite.setTextureRect(IntRect(64, 0, 32, 32));
    m_sprite.setOrigin(16, 16);

    m_mark.setTexture(checkTexture);
    m_mark.setTextureRect(IntRect(32, 0, 32, 32));
    m_mark.setOrigin(16, 16);

    m_callfunction = ptrFunction;

    isCheck = false;

    m_helptext = helptext;
}

void CheckButton::setSound(FMOD_SOUND** clickSound)
{
    m_clicksound = clickSound;
}

void CheckButton::setPosition(float x, float y)
{
    m_sprite.setPosition(x, y);
    m_mark.setPosition(x, y);
}

void CheckButton::setChecked(bool checked)
{
    if (checked)
    {
        isCheck = true;

        m_mark.setTextureRect(IntRect(0, 0, 32, 32));
    }
    else
    {
        isCheck = false;

        m_mark.setTextureRect(IntRect(32, 0, 32, 32));
    }
}

bool CheckButton::getChecked()
{
    return isCheck;
}

Vector2f CheckButton::getPosition() const
{
    return m_sprite.getPosition();
}

void CheckButton::update()
{
    extern Text* helpText;

    FloatRect aabb(m_sprite.getGlobalBounds());

    if (aabb.contains(mpos_relative))
    {
        float scale = m_mark.getScale().x;

        if (helpText->getString() != m_helptext)
            helpText->setString(m_helptext);

        if (scale > 0.85)
            scale -= 0.1;
        else
            scale = 0.75;

        m_mark.setScale(scale, scale);

        if (mousePressed == 1)
        {
            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(0), *m_clicksound, 0, NULL);

            if (isCheck)
            {
                m_mark.setTextureRect(IntRect(32, 0, 32, 32));

                isCheck = false;

                m_callfunction();
            }
            else
            {
                m_mark.setTextureRect(IntRect(0, 0, 32, 32));

                isCheck = true;

                m_callfunction();
            }
        }
    }
    else
    {
        float scale = m_mark.getScale().x;

        if (helpText->getString() == m_helptext)
            helpText->setString("");

        if (scale < 0.9)
        {
            scale += 0.1;
        }
        else
        {
            scale = 1;
        }

        m_mark.setScale(scale, scale);
    }
}

void CheckButton::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
    target.draw(m_mark);
}
