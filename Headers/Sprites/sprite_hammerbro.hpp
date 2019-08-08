/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEHAMMERBRO_INCLUDED
#define SPRITEHAMMERBRO_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>
#include "../Miscs/projectile_hammer.hpp"
#include "sprite_brother.hpp"

class Sprite_HammerBro : public Sprite_Brother
{
    public :

        Sprite_HammerBro(const sf::Texture& texture, Collider* collision);
        virtual ~Sprite_HammerBro();

        void update();
        void secureUpdate();
        void afterUpdate();

        void thruw();
        void dead();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        std::list<Projectile_Hammer*> m_hammer;
};

#endif
