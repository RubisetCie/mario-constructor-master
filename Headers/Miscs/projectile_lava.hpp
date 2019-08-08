/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PROJECTILELAVA_INCLUDED
#define PROJECTILELAVA_INCLUDED

#include <SFML/Graphics.hpp>

class Projectile_Lava : public sf::Drawable
{
    public :

        Projectile_Lava(sf::Texture* texture, sf::Vector2f position, sf::Vector2f speed);

        void update();

        sf::FloatRect m_aabb;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;
        sf::Vector2f m_movedistance;

        char m_frame;
};

#endif
