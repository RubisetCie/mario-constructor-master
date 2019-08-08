/*  Creator	: Matthieu (Rubisetcie) Carteron
	Language: C++
*/

#ifndef PAWN_INCLUDED
#define PAWN_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>
#include "collider.hpp"
#include "placeable.hpp"
#include "matrix.hpp"

class Pawn : public Placeable
{
    public :

        Pawn();

        bool makeMoveVertical(float mov, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more);
        bool makeMoveHorizontal(float mov, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more);

        void makeMoveVerticalFake(float mov);

        bool makeMoveVerticalHigh(float mov, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more);
        bool makeMoveHorizontalHigh(float mov, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more);

        bool makeMoveVerticalPlatform(float mov, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more);
        bool makeMoveHorizontalPlatform(float mov, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more);
        bool makeMoveVerticalHighPlatform(float mov, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more);
        bool makeMoveHorizontalHighPlatform(float mov, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more);

        bool makeMoveHorizontalNR(float mov, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more);
        bool makeMoveHorizontalHighNR(float mov, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more);
        bool makeMoveVerticalNR(float mov, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more);

        bool testVertical(float distance, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more);
        bool testHorizontal(float distance, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more);

        bool testVerticalHigh(float distance, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more);
        bool testHorizontalHigh(float distance, bool right, const sf::Vector2i& levelSize, Matrix* collision, std::list<Collider*>* more);

        sf::FloatRect m_aabb;

        Collider* m_platform;
};

#endif
