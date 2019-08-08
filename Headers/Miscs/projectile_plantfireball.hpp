/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PROJECTILEPLANTFIREBALL_INCLUDED
#define PROJECTILEPLANTFIREBALL_INCLUDED

#include <SFML/Graphics.hpp>

class Projectile_PlantFireball : public sf::Drawable
{
    public :

        Projectile_PlantFireball(sf::Texture* texture);

        void cast(sf::Vector2f position, sf::Vector2f speed);

        void update();

        sf::FloatRect m_aabb;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;

        sf::Vector2f m_movedistance;
};

#endif
