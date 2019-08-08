/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PROJECTILEPODOBOO_INCLUDED
#define PROJECTILEPODOBOO_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/projectile.hpp"

class Projectile_Podoboo : public sf::Drawable
{
    public :

        Projectile_Podoboo(sf::Texture* texture, sf::Vector2f position, sf::Vector2f speed, sf::Vector2f size, float angle);

        void update();

        bool m_destroyed;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;
        sf::FloatRect m_aabb;
        sf::Vector2f m_movedistance;
};

#endif
