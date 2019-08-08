/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Marks/mark_platformcloudfall.hpp"
#include "../../Headers/Marks/mark_mario.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Mark_PlatformCloudFall::Mark_PlatformCloudFall(const Texture& texture, Collider* const collision) : PlatformFall(texture, collision)
{
    m_sprite.setTextureRect(IntRect(0, 0, 126, 32));

    m_aabb.width = 126;
    m_aabb.height = 32;
}

void Mark_PlatformCloudFall::update()
{
    if (frame_Water % 6 == 0)
        m_sprite.setTextureRect(IntRect((frame_Water/6) * 126, 0, 126, 32));

    PlatformFall::update();
}

Vector2f Mark_PlatformCloudFall::getSize() const
{
    return Vector2f(126, 32);
}

void Mark_PlatformCloudFall::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
