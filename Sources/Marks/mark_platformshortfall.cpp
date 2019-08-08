/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Marks/mark_platformshortfall.hpp"
#include "../../Headers/Marks/mark_mario.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Mark_PlatformShortFall::Mark_PlatformShortFall(const Texture& texture, Collider* const collision) : PlatformFall(texture, collision)
{
    m_aabb.width = 32;
    m_aabb.height = 32;
}

Vector2f Mark_PlatformShortFall::getSize() const
{
    return Vector2f(32, 16);
}

void Mark_PlatformShortFall::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
