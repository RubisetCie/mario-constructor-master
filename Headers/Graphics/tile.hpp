/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef TILE_INCLUDED
#define TILE_INCLUDED

#include <SFML/Graphics.hpp>

typedef struct TileData
{
    float x;
    float y;

    char tilex;
    char tiley;
} TileData;

class Tile : public sf::Drawable
{
    public :

        Tile(float x, float y, unsigned int tilex, unsigned int tiley);

        sf::Vector2f getPosition() const;

        void setColor(sf::Color color);

        TileData save();

        sf::Vertex m_quad[4];

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
