/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Marks/mark_platformredfall.hpp"
#include "../../Headers/Marks/mark_mario.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Mark_PlatformRedFall::Mark_PlatformRedFall(const Texture& texture, Collider* const collision) : PlatformFall(texture, collision)
{
    m_aabb.width = 96;
    m_aabb.height = 32;
}

Vector2f Mark_PlatformRedFall::getSize() const
{
    return Vector2f(96, 16);
}

void Mark_PlatformRedFall::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
