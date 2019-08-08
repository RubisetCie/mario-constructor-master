/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PROJECTILE_INCLUDED
#define PROJECTILE_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>
#include "../Core/matrix.hpp"
#include "../Core/collider.hpp"

class Projectile : public sf::Drawable
{
    public :

        virtual void cast(sf::Vector2f position, bool right) = 0;

        bool makeMoveVertical(float mov, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more, bool hitenemies);
        bool makeMoveHorizontal(float mov, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more, bool hitenemies);

        sf::FloatRect m_aabb;

    protected :

        void draw(sf::RenderTarget& target, sf::RenderStates) const = 0;
};

#endif
