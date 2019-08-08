/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKPLATFORMCLOUDFALL_INCLUDED
#define MARKPLATFORMCLOUDFALL_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/platformfall.hpp"

class Mark_PlatformCloudFall : public PlatformFall
{
    public :

        Mark_PlatformCloudFall(const sf::Texture& texture, Collider* const collision);

        void update();

        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
