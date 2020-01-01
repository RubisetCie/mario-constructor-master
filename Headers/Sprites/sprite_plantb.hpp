/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEPLANTB_INCLUDED
#define SPRITEPLANTB_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"
#include "../Core/collider.hpp"


class Sprite_PlantB : public Placeable
{
    public :

        Sprite_PlantB(sf::Texture* texture, Collider* collision);

        void setPosition(const sf::Vector2f& pos);

        void update();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    protected :

        sf::VertexArray m_array;
        sf::FloatRect m_aabb;
        sf::Texture* m_texture;

        Collider* m_collider;

        float m_startheight;

        unsigned short m_timer;
};

#endif
