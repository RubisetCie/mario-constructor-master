/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKPLATFORMREDFALL_INCLUDED
#define MARKPLATFORMREDFALL_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/platformfall.hpp"

class Mark_PlatformRedFall : public PlatformFall
{
    public :

        Mark_PlatformRedFall(const sf::Texture& texture, Collider* const collision);

        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
