/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include "../../Headers/Marks/mark_spring.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Mark_Spring::Mark_Spring(const Texture& texture, unsigned int strengh) : Placeable()
{
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(IntRect(0, 0, 32, 64));

    m_strengh = strengh;
}

void Mark_Spring::setPosition(const Vector2f& pos)
{
    m_sprite.setPosition(pos);
}

void Mark_Spring::update()
{
    if (player == NULL)
        return;

    if (m_frame > 0)
    {
        switch (m_frame)
        {
            case 9 : m_sprite.setTextureRect(IntRect(32, 0, 32, 64)); break;
            case 6 : m_sprite.setTextureRect(IntRect(64, 0, 32, 64)); break;
            case 3 : m_sprite.setTextureRect(IntRect(32, 0, 32, 64)); break;
        }
    }
    else
        m_sprite.setTextureRect(IntRect(0, 0, 32, 64));

    FloatRect playerbbox(player->m_aabb);
    Vector2f position(m_sprite.getPosition());

    if (playerbbox.intersects(FloatRect(position, Vector2f(32, 64))))
    {
        if (playerbbox.top + playerbbox.height <= position.y + 12 && player->m_movedistance.y > 0)
        {
            player->bounce(m_strengh, true);

            m_frame = 10;

            FMOD_System_PlaySound(soundSystem, static_cast<FMOD_CHANNELINDEX>(14), sfxSamples[13], 0, NULL);
        }
    }
}

void Mark_Spring::secureUpdate()
{
    if (m_frame > 0)
        m_frame--;
}

void Mark_Spring::afterUpdate()
{
}

Vector2f Mark_Spring::getPosition() const
{
    return m_sprite.getPosition();
}

Vector2f Mark_Spring::getSize() const
{
    return Vector2f(32, 64);
}

ID Mark_Spring::getID() const
{
    return ID_USELESS;
}

void Mark_Spring::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
