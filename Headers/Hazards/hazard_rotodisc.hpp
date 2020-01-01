/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDROTODISC_INCLUDED
#define HAZARDROTODISC_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Hazard_Rotodisc : public Placeable
{
    public :

        Hazard_Rotodisc(const sf::Texture& texture, const sf::Vector2f& disk_pos, float speed);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    protected :

        sf::Sprite m_sprite;
        sf::Sprite m_disk;

        sf::FloatRect m_aabb;

        sf::Vector2f m_position;

        float m_speed;
        float m_radius;

        float t;
};

#endif
