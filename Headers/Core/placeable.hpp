/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PLACEABLE_INCLUDED
#define PLACEABLE_INCLUDED

#include <SFML/Graphics.hpp>

enum ID {ID_USELESS, ID_PLAYER, ID_CHECKPOINT, ID_INVISIBLE, ID_ENEMY, ID_WONTFALL, ID_SHELL, ID_STOMPABLE};

class Placeable : public sf::Drawable
{
    public :

        Placeable();

        virtual void setPosition(const sf::Vector2f& pos) = 0;

        virtual void update() = 0;
        virtual void secureUpdate() = 0;
        virtual void afterUpdate() = 0;

        virtual sf::Vector2f getPosition() const = 0;
        virtual sf::Vector2f getSize() const = 0;

        virtual ID getID() const = 0;

        bool m_destroyed;
};

#endif
