/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef MARKLEVELEND_INCLUDED
#define MARKLEVELEND_INCLUDED

#include <SFML/Graphics.hpp>
#include "../Core/placeable.hpp"

class Mark_LevelEnd : public Placeable
{
    public :

        Mark_LevelEnd(const sf::Texture& texture, const sf::Texture& limitTexture, const sf::Texture& barTexture, float limit);

        void setPosition(const sf::Vector2f& pos);

        void update();
        void secureUpdate();
        void afterUpdate();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

        ID getID() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_sprite;
        sf::Sprite m_end;
        sf::Sprite m_limit;

        sf::Vector2f m_endspeed;

        unsigned char m_cleared;

        float m_limitdist;
        float m_endpos;

        bool m_enddir;
};

#endif
