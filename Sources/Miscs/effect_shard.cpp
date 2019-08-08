/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>
#include "../../Headers/Miscs/effect_shard.hpp"

#define SHARD_ROTATESPEED 25

using namespace sf;

Effect_Shard::Effect_Shard(Texture* shardTexture, Vector2f speed, Vector2f position, float startangle, float height) : Effect(position)
{
    m_sprite.setTexture(*shardTexture);
    m_sprite.setOrigin(8, 8);
    m_sprite.setRotation(startangle);

    m_movedistance = speed;
    m_startheight = height;
}

void Effect_Shard::update()
{
    if (m_movedistance.x > 0)
        m_sprite.rotate(SHARD_ROTATESPEED);
    else
        m_sprite.rotate(-SHARD_ROTATESPEED);

    m_movedistance.y += 0.25;

    m_sprite.move(m_movedistance);

    if (m_sprite.getPosition().y > m_startheight + 480)
        m_destroyed = true;
}

void Effect_Shard::draw(RenderTarget& target, RenderStates) const
{
    target.draw(m_sprite);
}
