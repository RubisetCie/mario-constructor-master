/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_koopared.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_KoopaRed::Sprite_KoopaRed(const Texture& sprite_texture, Texture* const shell_texture, Collider* collision, bool shell) : Sprite_Koopa(sprite_texture, shell_texture, collision, shell)
{
}

void Sprite_KoopaRed::secureUpdate()
{
    if (!m_active)
        return;

    if (m_state == 0)
    {
        if (m_movedistance.x == 0 && player != NULL)
        {
            float playerpos = player->getPosition().x;
            float pos = m_sprite.getPosition().x;

            m_movedistance.x = pos>=playerpos+14?-1.8:1.8;
        }
    }

    Sprite_Koopa::secureUpdate();
}

ID Sprite_KoopaRed::getID() const
{
    if (m_state < 2)
    {
        if (m_collider->type == C_STOMPABLE)
            return ID_WONTFALL;
        else
            return ID_USELESS;
    }
    else
        return ID_SHELL;
}

void Sprite_KoopaRed::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
