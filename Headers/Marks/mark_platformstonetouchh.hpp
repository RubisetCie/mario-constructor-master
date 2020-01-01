/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKPLATFORMSTONETOUCHH_INCLUDED
#define MARKPLATFORMSTONETOUCHH_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/platformmov.hpp"

class Mark_PlatformStoneTouchH : public PlatformMov
{
    public :

        Mark_PlatformStoneTouchH(const sf::Texture& texture, float target, float speed, Collider* const collision);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();

        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        bool m_start;
};

#endif
