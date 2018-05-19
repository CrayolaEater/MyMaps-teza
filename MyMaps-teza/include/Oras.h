#ifndef ORAS_H
#define ORAS_H
using namespace std;
using namespace sf;

class Oras
{
private:
    void CreeazaMarcajul(string cale)
    {
        MarcajOras.setPosition(pos_x,pos_y);
        MarcajOras.setSize(Vector2f(10,10));
        MarcajOras.setFillColor(Color::Yellow);
        CentruMarcaj.x=pos_x+5;
        CentruMarcaj.y=pos_y+5;
    }
    int pos_x, pos_y;
    RectangleShape MarcajOras;
    Texture TexturaMarcaj;
    Vector2i CentruMarcaj;
    Text NumeText;
public:
    void DeseneazaOras(&RenderWindow fereastra)
    {
        fereastra.draw(MarcajOras);
    }
    int Distanta;
    string Nume;
    Oras(int poz_x,int poz_y,string nume, int Distanta,string poza)
    {
        pos_x=poz_x;
        pos_y=poz_y;
        nume=Nume;
        CreeazaMarcajul("42342432");
    }
};

#endif // ORAS_H
