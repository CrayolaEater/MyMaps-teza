#include <SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<fstream>
#include<math.h>
using namespace sf;
using namespace std;
fstream fisier;
fstream distante;
Event event;
Texture Harta;
Sprite HartaFundal;
Font FontOrase;
RenderWindow fereastra;
class Oras;
vector<Oras> Orase; //retin toate orasele
struct structuraOras
{
    int x,y,factor,dim_text;
    string nume;
};
struct Linie
{
    Vector2f pos1;
    Vector2f pos2;
};
vector<Linie> Linii;
struct Drum
{
    string oras1;
    string oras2;
    int distanta;
};
structuraOras oras;
vector<structuraOras> OraseCitite;
vector<Drum> OraseSiDistante;
class Oras
{
private: //ce i privat nu poate fi accesat din afara clasei
    void CreeazaMarcajul(string cale)//am facut functia care face marcajulll cu cercul
    {
        MarcajOras.setPosition(pos_x,pos_y); // setam pozitia marcajului orasului,am setat pozitia cercului
        MarcajOras.setSize(Vector2f(25,25));//setam lungimea si latimea marcajului orasului
        CentruMarcaj.x=pos_x+10;
        CentruMarcaj.y=pos_y+10;
        TexturaMarcaj.loadFromFile(cale);
        MarcajOras.setTexture(&TexturaMarcaj);
        InDreptulMarcajului.x=CentruMarcaj.x+factor;
        InDreptulMarcajului.y=CentruMarcaj.y-35;
    }
    void CreeazaTextul()
    {
        NumeText.setPosition(InDreptulMarcajului.x,InDreptulMarcajului.y);
        NumeText.setFont(FontOrase);
        NumeText.setFillColor(Color::White);
        NumeText.setString(Nume);
    }
    int pos_x, pos_y,factor;
    RectangleShape MarcajOras;
    Texture TexturaMarcaj;
    Vector2i CentruMarcaj,InDreptulMarcajului;
public:
    Text NumeText;
    void SeteazaPozaMarcaj(string cale)
    {
        TexturaMarcaj.loadFromFile(cale);
        MarcajOras.setTexture(&TexturaMarcaj);
    }
    void DeseneazaOras()
    {
        fereastra.draw(MarcajOras);
    }
    bool MousePeste()
    {
        if(MarcajOras.getGlobalBounds().contains(fereastra.mapPixelToCoords(Mouse::getPosition(fereastra)))) // daca mouse ul este in dreptul marcajului unu oras
        {
            return true;
        }
        return false;
    }
    string Nume;
    Oras(int poz_x,int poz_y,string nume,string poza,int ffactor,int dim_text)
    {
        pos_x=poz_x;
        pos_y=poz_y;
        Nume=nume;
        NumeText.setCharacterSize(dim_text);
        factor=ffactor;
        CreeazaMarcajul(poza);
        CreeazaTextul();
    }
};
Oras CreeazaOras(int poz_x,int poz_y,string nume,string poza,int ffactor,int dim_text)
{
    Oras C(poz_x,poz_y,nume,poza,ffactor,dim_text);
    Orase.push_back(C);
    return C;
}
void DeseneazaOrase()
{
    for(int i=0; i<Orase.size(); i++)
    {
        Orase[i].DeseneazaOras();
        if(Orase[i].MousePeste())
        {
            fereastra.draw(Orase[i].NumeText);
        }
    }
}
void CreeazaLinie(Vector2f pos1, Vector2f pos2)
{
    Linie l;
    l.pos1=pos1;
    l.pos2=pos2;
    Linii.push_back(l);
}
void DeseneazaLinii()
{
    for(int i =0; i<Linii.size(); i++)
    {
        Vertex line2[] =
        {
            Vertex(Linii[i].pos1, Color::Red),
            Vertex(Linii[i].pos2, Color::Red)
        };
        Vertex line3[]=
        {
            Vertex(Vector2f(Linii[i].pos1.x+1,Linii[i].pos1.y),Color::Red),
            Vertex(Vector2f(Linii[i].pos2.x+1,Linii[i].pos2.y),Color::Red)
        };
        Vertex line4[]=
        {
            Vertex(Vector2f(Linii[i].pos1.x+2,Linii[i].pos1.y),Color::Red),
            Vertex(Vector2f(Linii[i].pos2.x+2,Linii[i].pos2.y),Color::Red)
        };
        fereastra.draw(line2, 2, Lines);
        fereastra.draw(line3,2,Lines);
        fereastra.draw(line4,2,Lines);
    }
}
void CitesteOraseleDinFisier()
{
    int x,y,factor,dim_text;
    string nume;
    fisier.open("orase.in",ios::in);
    while(fisier>>x>>y>>nume>>factor>>dim_text)
    {
        structuraOras strorasel;
        strorasel.x=x;
        strorasel.y=y;
        strorasel.nume=nume;
        strorasel.factor=factor;
        strorasel.dim_text=dim_text;
        OraseCitite.push_back(strorasel);
        Oras orasel=CreeazaOras(x,y,nume,"poze/PunctOrasMarcat.png",factor,dim_text);
    }
    fisier.close();
    fisier.clear();
}
void CitesteDistantele()
{
    Drum d;
    distante.open("distante.in",ios::in);
    while(distante>>d.oras1>>d.oras2>>d.distanta)
    {
        OraseSiDistante.push_back(d);
    }
    distante.close();
    distante.clear();
    for(int i=0; i<OraseSiDistante.size(); i++)
    {
        cout<<d.oras1<<" "<<d.oras2<<" "<<d.distanta<<endl;
    }
}
void AfiseazaOraseleInFisier()
{
    fisier.open("orase.in",ios::out);
    for(int i=0; i<OraseCitite.size(); i++)
    {
        fisier<<OraseCitite[i].x<<" "<<OraseCitite[i].y<<" "<<OraseCitite[i].nume<<" "<<OraseCitite[i].factor<<" "<<OraseCitite[i].dim_text<<'\n';
    }
    fisier.close();
}
int main()
{
    FontOrase.loadFromFile("good times rg.ttf");
    //Oras Iasi=CreeazaOras(738,160,"iasi",19,"poze/PunctOrasMarcat.png",-15,18);
    Harta.loadFromFile("poze/ImagineFundalHarta.png"); // incarcam textura hartii din fisier
    HartaFundal.setTexture(Harta);//setam textura sprite-ului
    fereastra.create(VideoMode(1000, 800), "MyMaps");
    CitesteOraseleDinFisier();
    CitesteDistantele();
    for(int i=0; i<Orase.size(); i++)
    {
        Orase[i].SeteazaPozaMarcaj("poze/PunctOrasMarcat.png");
    }
    while (fereastra.isOpen())
    {
        //cout<<Mouse::getPosition(fereastra).x<<" "<<Mouse::getPosition(fereastra).y<<endl;
        while (fereastra.pollEvent(event))
        {
            if (event.type == Event::Closed)//verific daca am apasat iconita de inchis fereastra
            {
                fereastra.close();
            }
        }
        fereastra.clear();
        fereastra.draw(HartaFundal);//desenam sprite-ul (ca sa il vedem)
        DeseneazaOrase();
        DeseneazaLinii();
        fereastra.display();
    }
    AfiseazaOraseleInFisier();
    return 0;
}
