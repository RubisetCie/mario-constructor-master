/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef DRAGGER_ENTITY_INCLUDED
#define DRAGGER_ENTITY_INCLUDED

#include <SFML/Graphics.hpp>

class Dragger : public sf::Drawable
{
    public :

        Dragger(sf::Texture* mover, sf::Texture* fonttex, unsigned int number);

        void setPosition(const sf::Vector2f& position);

        void update();

        sf::Vector2f getPosition() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        sf::Sprite m_mover;
        sf::Sprite m_textid;

        sf::RectangleShape m_screenproc;

        bool m_grabnode;
};

#endif
