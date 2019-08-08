/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Marks/mark_platformstonefall.hpp"
#include "../../Headers/Marks/mark_mario.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Mark_PlatformStoneFall::Mark_PlatformStoneFall(const Texture& texture, Collider* const collision) : PlatformFall(texture, collision)
{
    m_aabb.width = 120;
    m_aabb.height = 32;
}

Vector2f Mark_PlatformStoneFall::getSize() const
{
    return Vector2f(120, 32);
}

void Mark_PlatformStoneFall::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
