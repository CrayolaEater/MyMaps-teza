#include <SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<fstream>
using namespace sf;
using namespace std;
fstream fisier;
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
vector<structuraOras> OraseCitite;
class Oras
{
private: //ce i privat nu poate fi accesat din afara clasei
    void CreeazaMarcajul(string cale)//am facut functia care face marcajulll cu cercul
    {
        MarcajOras.setPosition(pos_x,pos_y); // setam pozitia marcajului orasului,am setat pozitia cercului
        MarcajOras.setSize(Vector2f(25,25));//setam lungimea si latimea marcajului orasului
        CentruMarcaj.x=pos_x+5;
        CentruMarcaj.y=pos_y+5;
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
    Text NumeText;
public:
    void SeteazaPozaMarcaj(string cale)
    {
        TexturaMarcaj.loadFromFile(cale);
        MarcajOras.setTexture(&TexturaMarcaj);
    }
    void DeseneazaOras()
    {
        fereastra.draw(MarcajOras);
        fereastra.draw(NumeText);
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
    for(int i=0;i<Orase.size();i++)
    {
        Orase[i].DeseneazaOras();
    }
}
void CitesteOraseleDinFisier()
{
    cout<<"da";
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
void AfiseazaOraseleInFisier()
{
    fisier.open("orase.in",ios::out);
    for(int i=0;i<OraseCitite.size();i++)
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
    for(int i=0; i<Orase.size();i++)
    {
        Orase[i].SeteazaPozaMarcaj("poze/PunctOrasMarcat.png");
    }
    while (fereastra.isOpen())
    {
        cout<<Mouse::getPosition(fereastra).x<<" "<<Mouse::getPosition(fereastra).y<<endl;
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
        fereastra.display();
    }
    AfiseazaOraseleInFisier();
    return 0;
}
