/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/globals.hpp"
#include "../../Headers/Sprites/sprite_koopagreen.hpp"

#ifdef DEBUGMODE
#include <iostream>
#endif

using namespace std;
using namespace sf;

Sprite_KoopaGreen::Sprite_KoopaGreen(const Texture& sprite_texture, Texture* const shell_texture, Collider* collision, bool shell) : Sprite_Koopa(sprite_texture, shell_texture, collision, shell)
{
}

void Sprite_KoopaGreen::secureUpdate()
{
    if (!m_active)
        return;

    if (m_state == 0)
    {
        if (m_movedistance.x == 0 && player != NULL)
        {
            float playerpos = player->getPosition().x;
            float pos = m_sprite.getPosition().x;

            m_movedistance.x = pos>=playerpos+14?-1.2:1.2;
        }
    }

    Sprite_Koopa::secureUpdate();
}

void Sprite_KoopaGreen::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
