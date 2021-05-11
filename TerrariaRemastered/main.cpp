#include "Engine.h"


int main()
{
	Engine en;
	en.start_game();

	return 0;
}
//#include <SFML/Graphics.hpp>
//
//using namespace sf;
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        RectangleShape rectangles[16];
//
//        for (int i = 0; i < 16; i++)
//        {
//            rectangles[i].setSize(Vector2f(50., 50.));
//            rectangles[i].setFillColor(Color(150, 50, i * 15));
//            rectangles[i].setOutlineColor(Color(0, 0, 0));
//            rectangles[i].setOutlineThickness(1);
//        }
//        
//        for (int i = 0; i < 4; i++)
//            for (int j = 0; j < 4; j++)
//                rectangles[i * 4 + j].setPosition(i * 50, j * 50);
//
//        window.clear();
//        for (int i = 0; i < 16; i++)
//            window.draw(rectangles[i]);
//        window.display();
//    }
//
//    return 0;
//}
