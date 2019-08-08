/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Core/effect.hpp"

using namespace sf;

Effect::Effect(Vector2f position)
{
    m_destroyed = false;

    m_sprite.setPosition(position);
}

