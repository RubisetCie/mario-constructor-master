/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKPLATFORMREDTOUCHV_INCLUDED
#define MARKPLATFORMREDTOUCHV_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/platformmov.hpp"

class Mark_PlatformRedTouchV : public PlatformMov
{
    public :

        Mark_PlatformRedTouchV(const sf::Texture& texture, float target, float speed, Collider* const collision);

        void setPosition(sf::Vector2f pos);

        void update();
        void secureUpdate();

        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        bool m_start;
};

#endif
