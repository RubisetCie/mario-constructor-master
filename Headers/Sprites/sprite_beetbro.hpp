/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEBEETBRO_INCLUDED
#define SPRITEBEETBRO_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>
#include "../Miscs/projectile_enemybeet.hpp"
#include "sprite_brother.hpp"

class Sprite_BeetBro : public Sprite_Brother
{
    public :

        Sprite_BeetBro(const sf::Texture& texture, Collider* collision);
        virtual ~Sprite_BeetBro();

        void update();
        void secureUpdate();
        void afterUpdate();

        void thruw();
        void dead();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        std::list<Projectile_EnemyBeet*> m_beet;
};

#endif
