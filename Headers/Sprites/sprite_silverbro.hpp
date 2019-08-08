/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITESILVERBRO_INCLUDED
#define SPRITESILVERBRO_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>
#include "../Miscs/projectile_silverhammer.hpp"
#include "sprite_brother.hpp"

class Sprite_SilverBro : public Sprite_Brother
{
    public :

        Sprite_SilverBro(const sf::Texture& texture, Collider* collision);
        virtual ~Sprite_SilverBro();

        void update();
        void secureUpdate();
        void afterUpdate();

        void thruw();
        void dead();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        std::list<Projectile_SilverHammer*> m_hammer;
};

#endif
