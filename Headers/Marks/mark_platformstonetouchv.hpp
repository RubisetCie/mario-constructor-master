/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKPLATFORMSTONETOUCHV_INCLUDED
#define MARKPLATFORMSTONETOUCHV_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/platformmov.hpp"

class Mark_PlatformStoneTouchV : public PlatformMov
{
    public :

        Mark_PlatformStoneTouchV(const sf::Texture& texture, float target, float speed, Collider* const collision);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();

        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        bool m_start;
};

#endif
