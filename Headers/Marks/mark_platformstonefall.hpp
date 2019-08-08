/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKPLATFORMSTONEFALL_INCLUDED
#define MARKPLATFORMSTONEFALL_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/platformfall.hpp"

class Mark_PlatformStoneFall : public PlatformFall
{
    public :

        Mark_PlatformStoneFall(const sf::Texture& texture, Collider* const collision);

        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
