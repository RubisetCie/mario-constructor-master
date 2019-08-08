/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKPLATFORMCLOUDMOVEH_INCLUDED
#define MARKPLATFORMCLOUDMOVEH_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/platformmov.hpp"

class Mark_PlatformCloudMoveH : public PlatformMov
{
    public :

        Mark_PlatformCloudMoveH(const sf::Texture& texture, float target, float speed, Collider* const collision);

        void setPosition(sf::Vector2f pos);

        void update();
        void secureUpdate();

        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;
};

#endif
