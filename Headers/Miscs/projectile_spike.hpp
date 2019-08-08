/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PROJECTILESPIKE_INCLUDED
#define PROJECTILESPIKE_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Graphics/trace.hpp"

class Projectile_Spike : public sf::Drawable, public Trace
{
    public :

        Projectile_Spike(sf::Texture* texture, sf::Vector2f position, sf::Vector2f speed);

        void update();

        bool m_destroyed;

        sf::FloatRect m_aabb;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;
        sf::Vector2f m_movedistance;

        char m_tracetimer;
};

#endif
