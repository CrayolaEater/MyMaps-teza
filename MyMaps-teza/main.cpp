#include <SFML/Graphics.hpp>
#include<iostream>
using namespace sf;
using namespace std;
Event event;
Texture Harta;
Sprite HartaFundal;
int main()
{
    Harta.loadFromFile("poze/ImagineFundalHarta.png"); // incarcam textura hartii din fisier
    HartaFundal.setTexture(Harta);//setam textura sprite-ului
    RenderWindow fereastra(VideoMode(1000, 800), "MyMaps");
    while (fereastra.isOpen())
    {
        while (fereastra.pollEvent(event))
        {
            if (event.type == Event::Closed)//verific daca am apasat iconita de inchis fereastra
            {
                fereastra.close();
            }
        }
        fereastra.clear();
        fereastra.draw(HartaFundal);//desenam sprite-ul (ca sa il vedem)
        fereastra.display();
    }
    return 0;
}
