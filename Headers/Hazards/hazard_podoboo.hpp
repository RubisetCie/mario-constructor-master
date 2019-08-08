/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef HAZARDPODOBOO_INCLUDED
#define HAZARDPODOBOO_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Hazard_Podoboo : public Placeable
{
    public :

        Hazard_Podoboo(const sf::Texture& texture, float maxheight);

        void update();
        void afterUpdate();

        sf::Vector2f getPosition() const;

        ID getID() const;

    protected :

        sf::Sprite m_sprite;
        sf::FloatRect m_aabb;

        float m_speed;
        float m_movedistance;
        float m_startheight;
        float m_maxheight;

        unsigned char m_timer;
};

#endif
