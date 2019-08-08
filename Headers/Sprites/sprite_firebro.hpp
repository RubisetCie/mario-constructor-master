/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef SPRITEFIREBRO_INCLUDED
#define SPRITEFIREBRO_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>
#include "../Miscs/projectile_enemyfireball.hpp"
#include "sprite_brother.hpp"

class Sprite_FireBro : public Sprite_Brother
{
    public :

        Sprite_FireBro(const sf::Texture& texture, Collider* collision);
        virtual ~Sprite_FireBro();

        void update();
        void secureUpdate();
        void afterUpdate();

        void thruw();
        void dead();

        sf::Vector2f getPosition() const;
        sf::Vector2f getSize() const;

    private :

        void draw(sf::RenderTarget& target, sf::RenderStates) const;

        std::list<Projectile_EnemyFireball*> m_fire;
};

#endif
