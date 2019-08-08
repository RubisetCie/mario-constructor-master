/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PROJECTILEEBEET_INCLUDED
#define PROJECTILEEBEET_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/projectile.hpp"

class Projectile_Beet : public Projectile
{
    public :

        Projectile_Beet(sf::Texture* texture);

        void cast(sf::Vector2f position, bool right);

        void update();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;
        sf::Vector2f m_movedistance;

        unsigned char m_remindings;
};

#endif
