#include <SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<fstream>
#include<math.h>
using namespace sf;
using namespace std;
fstream fisier;
#define infinit 9999999
#define NMAX 105
fstream distante;
Event event;
bool AlegeStart,AlegeUnOrasSpecific,AlegeSosire,AlesStart,AlesOrasSpecific,AlesSosire,viz[NMAX],Calculat;
int IndiceStart,IndiceOrasSpecific,IndiceSosire,IndiceStart2,IndiceStart3,IndiceSosire2,IndiceSosire3,MatriceCosturi[NMAX][NMAX],d[NMAX],tata[NMAX],DistantaTotala;
Texture Harta;
Sprite HartaFundal;
Text TextAjutator;
Font FontOrase;
RenderWindow fereastra;
class Oras;
class Buton;
vector<Buton>Butoane;
vector<Oras> Orase; //retin toate orasele
vector<Vector2f>PozitiiDeDesenatDrum;
vector<int>OraseParcurse;
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
vector<Text>TexteNormale;
void SeteazaCosturilePeInfinit()
{
    for(int i =0; i<NMAX; i++)
    {
        tata[i]=-2;
        for(int j=0; j<NMAX; j++)
        {
            if(i!=j)
            {
                MatriceCosturi[i][j]=infinit;
            }
        }
    }
}
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
    Vector2i InDreptulMarcajului;
public:
    bool ArataOras;
    Text NumeText;
    int indiceOras;
    Vector2f CentruMarcaj;
    void SeteazaPozaMarcaj(string cale)
    {
        TexturaMarcaj.loadFromFile(cale);
        MarcajOras.setTexture(&TexturaMarcaj);
    }
    void SeteazaCuloare(Color culoare)
    {
        MarcajOras.setFillColor(culoare);
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
class Buton
{
private:
    int pos_x, pos_y;
    void (*Functie)(void);
    RectangleShape forma;
    Texture textura;
    string caleImagine;
public:
    void Click()
    {
        Functie();
    }
    bool MousePeste()
    {
        if(forma.getGlobalBounds().contains(fereastra.mapPixelToCoords(Mouse::getPosition(fereastra))))
        {
            Color c(133,133,133);
            forma.setFillColor(c);
            return true;
        }
        forma.setFillColor(Color::White);
        return false;
    }
    void Deseneaza()
    {
        fereastra.draw(forma);
    }
    void SeteazaForma()
    {
        textura.loadFromFile(caleImagine);
        forma.setTexture(&textura);
    }
    Buton(int p_x,int p_y,int lg, int l,string cale,void(*functie)(void))
    {
        forma.setPosition(p_x,p_y);
        pos_x=p_x;
        pos_y=p_y;
        caleImagine=cale;
        Functie=functie;
        forma.setSize(Vector2f(lg,l));
    }
};
Oras CreeazaOras(int poz_x,int poz_y,string nume,string poza,int ffactor,int dim_text)
{
    Oras C(poz_x,poz_y,nume,poza,ffactor,dim_text);
    Orase.push_back(C);
    return C;
}
Buton CreeazaButon(int px,int py,int lg, int l,string cale,void(*functie)(void))
{
    Buton B(px,py,lg,l,cale,functie);
    Butoane.push_back(B);
}
void Dijkstra(int indiceOrasStart);
void DeseneazaDrumurile();
bool OrasulEsteParcurs(int indiceOras)
{
    for(int i=0; i<OraseParcurse.size(); i++)
    {
        if(OraseParcurse[i]==indiceOras)
        {
            return true;
        }
    }
    return false;
}
string IntToString(int nr);
void FaDrumulMinim()
{
    if(!Calculat)
    {
        if(!AlesOrasSpecific)
        {
            Dijkstra(IndiceStart);
            DeseneazaDrumurile();
            DistantaTotala=d[IndiceSosire2];
            string c="Distanta "+OraseCitite[IndiceStart].nume+"->"+OraseCitite[IndiceSosire2].nume+": "+IntToString(DistantaTotala)+" Km";
            TextAjutator.setString(c);
        }
        else
        {
            IndiceSosire=IndiceOrasSpecific;
            Dijkstra(IndiceStart);
            DistantaTotala=d[IndiceOrasSpecific];
            DeseneazaDrumurile();
            PozitiiDeDesenatDrum.erase(PozitiiDeDesenatDrum.begin(),PozitiiDeDesenatDrum.end());
            IndiceSosire=IndiceSosire2;
            IndiceStart=IndiceOrasSpecific;
            Dijkstra(IndiceStart);
            DistantaTotala+=d[IndiceSosire2];
            DeseneazaDrumurile();
            string c="Distanta "+OraseCitite[IndiceStart2].nume+"->"+OraseCitite[IndiceOrasSpecific].nume+"->"+OraseCitite[IndiceSosire2].nume+": "+IntToString(DistantaTotala)+" Km";
            TextAjutator.setString(c);
        }
        Calculat=true;
    }
}
void DeseneazaOrase()
{
    for(int i =0; i<Orase.size(); i++)
    {
        if(Orase[i].ArataOras)
        {
            fereastra.draw(Orase[i].NumeText);
        }
    }
    for(int i=0; i<Orase.size(); i++)
    {
        Orase[i].DeseneazaOras();
        if(Orase[i].MousePeste())
        {
            Orase[i].ArataOras=true;
            if(Mouse::isButtonPressed(Mouse::Left))
            {
                if(AlegeStart&&!AlesStart)
                {
                    IndiceStart=Orase[i].indiceOras;
                    IndiceStart3=IndiceStart;
                    Orase[i].SeteazaCuloare(Color::Green);
                    AlesStart=true;
                    if(AlesSosire)
                    {
                        FaDrumulMinim();
                    }
                }
                if(AlegeUnOrasSpecific&&!AlesOrasSpecific&&AlesStart)
                {
                    IndiceOrasSpecific=Orase[i].indiceOras;
                    Orase[i].SeteazaCuloare(Color::Yellow);
                    AlesOrasSpecific=true;
                }
                if(AlegeSosire&&!AlesSosire)
                {
                    IndiceSosire=Orase[i].indiceOras;
                    IndiceSosire2=IndiceSosire;
                    IndiceSosire3=IndiceSosire2;
                    Orase[i].SeteazaCuloare(Color::Blue);
                    IndiceStart2=IndiceStart;
                    AlesSosire=true;
                    if(AlesStart)
                    {
                        FaDrumulMinim();
                    }
                }
            }
        }
        else
        {
            if(!OrasulEsteParcurs(i))
            {
                Orase[i].ArataOras=false;
            }
        }
    }
}
void DeseneazaButoane()
{
    for(int i=0; i<Butoane.size(); i++)
    {
        Butoane[i].Deseneaza();
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
    int ind=0;
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
        Orase[Orase.size()-1].indiceOras=ind;
        ind++;
    }
    fisier.close();
    fisier.clear();
}
void VerificaButoanele()
{
    for(int i=0; i<Butoane.size(); i++)
    {
        if(Butoane[i].MousePeste())
        {
            if(Mouse::isButtonPressed(Mouse::Left))
            {
                Butoane[i].Click();
            }
        }
    }
}
void DeseneazaTexteleNormale()
{
    for(int i=0; i<TexteNormale.size(); i++)
    {
        fereastra.draw(TexteNormale[i]);
    }
}
void TextNormal(int pos_x, int pos_y, int dim, Color culoare,string text)
{
    Text t;
    t.setCharacterSize(dim);
    t.setString(text);
    t.setFillColor(culoare);
    t.setPosition(pos_x,pos_y);
    t.setFont(FontOrase);
    TexteNormale.push_back(t);
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
void SeteazaTexturile()
{
    for(int i=0; i<Orase.size(); i++)
    {
        Orase[i].SeteazaPozaMarcaj("poze/PunctOrasMarcat.png");
    }
    for(int i=0; i<Butoane.size(); i++)
    {
        Butoane[i].SeteazaForma();
    }
}
void Start()
{
    AlegeStart=true;
    TextAjutator.setString("Alege un Oras de pornire");
}
void AlegeOras()
{
    AlegeUnOrasSpecific=true;
    TextAjutator.setString("Alege un Oras de parcurs");
}
void Sosire()
{
    AlegeSosire=true;
    TextAjutator.setString("Alege un Oras de sosire");
}
int IndexOras(string oras)
{
    for(int i=0; i<OraseCitite.size(); i++)
    {
        if(OraseCitite[i].nume==oras)
        {
            return i;
        }
    }
}
void CitesteDistantele()
{
    Drum d;
    int n=0;
    distante.open("distante.in",ios::in);
    while(distante>>d.oras1>>d.oras2>>d.distanta)
    {
        //cout<<"Distanta dintre "<<OraseCitite[IndexOras(d.oras1)].nume<<" si "<<OraseCitite[IndexOras(d.oras2)].nume<<" este de:"<<d.distanta<<endl;
        MatriceCosturi[IndexOras(d.oras1)][IndexOras(d.oras2)]=MatriceCosturi[IndexOras(d.oras2)][IndexOras(d.oras1)]=d.distanta;
        n++;
        OraseSiDistante.push_back(d);
    }
    distante.close();
    distante.clear();
//    for(int i=0;i<11;i++)
//    {
//        for(int g=0; g<11;g++)
//        {
//            if(MatriceCosturi[i][g]==infinit)
//            {
//                cout<<"i"<<" ";
//            }
//            else
//            {
//                cout<<MatriceCosturi[i][g]<<" ";
//            }
//        }
//        cout<<endl;
//    }
}
void Dijkstra(int indiceOrasStart)
{
    for(int i=0; i<OraseCitite.size(); i++)
    {
        d[i]=MatriceCosturi[indiceOrasStart][i];
        tata[i]=indiceOrasStart;
        viz[i]=0;
    }
    viz[indiceOrasStart]=1;
    tata[indiceOrasStart]=-1;
    bool ok=1;
    while(ok)
    {
        int minim=infinit;
        int k =0;
        for(int i =0; i<OraseCitite.size(); i++)
        {
            if(!viz[i]&&d[i]<minim)
            {
                minim=d[i];
                k=i;
            }
        }
        if(minim!=infinit)
        {
            viz[k]=1;
            for(int i =0; i<OraseCitite.size(); i++)
            {
                if(!viz[i]&&d[k]+MatriceCosturi[k][i]<d[i])
                {
                    d[i]=d[k]+MatriceCosturi[k][i];
                    tata[i]=k;
                }
            }
        }
        else
        {
            ok=0;
        }
    }
}
void DeseneazaDrumurile()
{
    while(tata[IndiceSosire]!=-1)
    {
        PozitiiDeDesenatDrum.push_back(Orase[IndiceSosire].CentruMarcaj);
        OraseParcurse.push_back(IndiceSosire);
        //cout<<OraseCitite[IndiceSosire].nume<<"<-";
        IndiceSosire=tata[IndiceSosire];
    }
    //cout<<OraseCitite[IndiceStart].nume;
    //cout<<'\n';
    OraseParcurse.push_back(IndiceStart);
    PozitiiDeDesenatDrum.push_back(Orase[IndiceStart].CentruMarcaj);
//    if(AlesOrasSpecific)
//    {
//        PozitiiDeDesenatDrum.push_back(Orase[IndiceStart2].CentruMarcaj);
//    }
    for(int i =0; i<PozitiiDeDesenatDrum.size()-1; i++)
    {
        CreeazaLinie(PozitiiDeDesenatDrum[i],PozitiiDeDesenatDrum[i+1]);
    }
    for(int i=0; i<OraseParcurse.size(); i++)
    {
        Orase[OraseParcurse[i]].ArataOras=true;
    }
}
string IntToString(int nr)
{
    string c;
    while(nr)
    {
        c+=nr%10+'0';
        nr/=10;
    }
    reverse(c.begin(),c.end());
    return c;
}
int DistantaPanaLaDestinatie(string destinatie)
{
    return d[IndexOras(destinatie)];
}
void Reset()
{
    Linii.erase(Linii.begin(),Linii.end());
    PozitiiDeDesenatDrum.erase(PozitiiDeDesenatDrum.begin(),PozitiiDeDesenatDrum.end());
    for(int i =0 ; i<OraseParcurse.size(); i++)
    {
        Orase[OraseParcurse[i]].ArataOras=false;
    }
    OraseParcurse.erase(OraseParcurse.begin(),OraseParcurse.end());
    Orase[IndiceStart3].SeteazaCuloare(Color::White);
    Orase[IndiceOrasSpecific].SeteazaCuloare(Color::White);
    Orase[IndiceSosire2].SeteazaCuloare(Color::White);
    IndiceStart=IndiceSosire2=IndiceOrasSpecific=IndiceStart3=-1;
    Calculat=false;
    AlesOrasSpecific=AlesSosire=AlesStart=AlegeSosire=AlegeStart=AlegeUnOrasSpecific=false;
    TextAjutator.setString("");
    DistantaTotala=0;
}
int main()
{
    SeteazaCosturilePeInfinit();
    FontOrase.loadFromFile("good times rg.ttf");
    //Oras Iasi=CreeazaOras(738,160,"iasi",19,"poze/PunctOrasMarcat.png",-15,18);
    TextAjutator.setCharacterSize(26);
    TextAjutator.setFont(FontOrase);
    TextAjutator.setPosition(50,730);
    Harta.loadFromFile("poze/ImagineFundalHarta.png"); // incarcam textura hartii din fisier
    HartaFundal.setTexture(Harta);//setam textura sprite-ului
    fereastra.create(VideoMode(1100, 800), "MyMaps");
    Buton Startul=CreeazaButon(900,80,150,60,"poze/butoane/start.png",Start);
    Buton AlegeOrasul=CreeazaButon(900,160,150,60,"poze/butoane/Oras.png",AlegeOras);
    Buton Sosirea=CreeazaButon(900,240,150,60,"poze/butoane/sosire.png",Sosire);
    Buton Reseteaza=CreeazaButon(900,320,150,60,"poze/butoane/buton_reset.png",Reset);
    CitesteOraseleDinFisier();
    CitesteDistantele();
    SeteazaTexturile();
    while (fereastra.isOpen())
    {
        //cout<<"Din "<<OraseCitite[IndiceStart].nume<<" tre sa treaca prin "<<OraseCitite[IndiceOrasSpecific].nume<<" si sa ajunga in "<<OraseCitite[IndiceSosire].nume<<endl;
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
        fereastra.draw(TextAjutator);
        DeseneazaLinii();
        DeseneazaOrase();
        DeseneazaButoane();
        VerificaButoanele();
        fereastra.display();
    }
    AfiseazaOraseleInFisier();
    return 0;
}
