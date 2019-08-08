/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PROJECTILEHAMMER_INCLUDED
#define PROJECTILEHAMMER_INCLUDED

#include <SFML/Graphics.hpp>

class Projectile_Hammer : public sf::Drawable
{
    public :

        Projectile_Hammer(sf::Texture* texture, sf::Vector2f position, sf::Vector2f speed);

        void update();

        sf::FloatRect m_aabb;

         bool m_destroyed;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;

        sf::Vector2f m_movedistance;
};

#endif
