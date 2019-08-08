/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKPLATFORMSHORTFALL_INCLUDED
#define MARKPLATFORMSHORTFALL_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/platformfall.hpp"

class Mark_PlatformShortFall : public PlatformFall
{
    public :

        Mark_PlatformShortFall(const sf::Texture& texture, Collider* const collision);

        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
