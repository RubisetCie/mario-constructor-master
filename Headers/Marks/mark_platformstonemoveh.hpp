/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKPLATFORMSTONEMOVEH_INCLUDED
#define MARKPLATFORMSTONEMOVEH_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/platformmov.hpp"

class Mark_PlatformStoneMoveH : public PlatformMov
{
    public :

        Mark_PlatformStoneMoveH(const sf::Texture& texture, float target, float speed, Collider* const collision);

        void setPosition(sf::Vector2f pos);

        void update();
        void secureUpdate();

        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
