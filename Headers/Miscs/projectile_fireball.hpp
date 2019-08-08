/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PROJECTILEFIREBALL_INCLUDED
#define PROJECTILEFIREBALL_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/projectile.hpp"

class Projectile_Fireball : public Projectile
{
    public :

        Projectile_Fireball(sf::Texture* texture);

        void cast(sf::Vector2f position, bool right);

        void update();

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;

        sf::Vector2f m_movedistance;
};

#endif
