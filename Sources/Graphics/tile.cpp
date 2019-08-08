/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#include <SFML/Graphics.hpp>

#include "../../Headers/Graphics/tile.hpp"
#include "../../Headers/globals.hpp"

using namespace sf;

Tile::Tile(float x, float y, unsigned int tilex, unsigned int tiley)
{
    m_quad[0].position.x = x;
    m_quad[0].position.y = y;
    m_quad[0].texCoords.x = tilex * 32;
    m_quad[0].texCoords.y = tiley * 32;

    m_quad[1].position.x = x + 32;
    m_quad[1].position.y = y;
    m_quad[1].texCoords.x = (tilex+1) * 32;
    m_quad[1].texCoords.y = tiley * 32;

    m_quad[2].position.x = x + 32;
    m_quad[2].position.y = y + 32;
    m_quad[2].texCoords.x = (tilex+1) * 32;
    m_quad[2].texCoords.y = (tiley+1) * 32;

    m_quad[3].position.x = x;
    m_quad[3].position.y = y + 32;
    m_quad[3].texCoords.x = tilex * 32;
    m_quad[3].texCoords.y = (tiley+1) * 32;
}

void Tile::setColor(sf::Color color)
{
    m_quad[0].color = color;
    m_quad[1].color = color;
    m_quad[2].color = color;
    m_quad[3].color = color;
}

TileData Tile::save()
{
    TileData data;

    data.x = m_quad[0].position.x;
    data.y = m_quad[0].position.y;

    data.tilex = static_cast<char>(m_quad[0].texCoords.x / 32);
    data.tiley = static_cast<char>(m_quad[0].texCoords.y / 32);

    return data;
}

Vector2f Tile::getPosition() const
{
    return m_quad[0].position;
}

void Tile::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(m_quad, 4, Quads, states);
}
