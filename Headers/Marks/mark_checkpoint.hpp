/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKCHECKPOINT_INCLUDED
#define MARKCHECKPOINT_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Mark_CheckPoint : public Placeable
{
    public :

        Mark_CheckPoint(const sf::Texture& texture, bool zone);

        void setPosition(sf::Vector2f pos);

        void silentCheck();
        void unCheck();

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

        sf::FloatRect m_collision;

        bool m_checked;
        bool m_zone;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;
};

#endif
