#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <regex>
#include <cstdlib>

using namespace std;

const int wlk_tab_wynikow=100;
const string naz_plk_z_wyn("wyniki.txt");
const regex czy_numer("[0-9]{1,10}");
const int wlk_talii=52;
const int maks_wlk_reki_gracza=9;
const int wlk_jednego_koloru=13;
const int ilosc_kolorow=4;
const int ilosc_cykli_tasowania=100;
const int ilosc_znakow_na_kole=15;

class tabela_wynikow
{
private:
    string* imiona = new string[wlk_tab_wynikow];
    int* wyniki = new int[wlk_tab_wynikow];
    int najnizszy_wynik = 0;

    void wstaw_do_tabeli(int wygrana, string nazwa_gracza)
    {
        int pozycja_w_tabeli=0;
        for (int i=wlk_tab_wynikow-1;i>=0 ;i-- )
        {
                if(wygrana<(*(wyniki+i)))
                {
                    pozycja_w_tabeli=i;
                    break;
                }
        }
        if(wygrana>(*(wyniki)))
        {
            pozycja_w_tabeli=-1;
        }
        for (int i=wlk_tab_wynikow-1;i>pozycja_w_tabeli ;i-- )
        {
            if(i!=99)
            {
                (*(imiona+i+1))=(*(imiona+i));
                (*(wyniki+i+1))=(*(wyniki+i));
            }
        }
        (*(imiona+pozycja_w_tabeli+1))=nazwa_gracza;
        (*(wyniki+pozycja_w_tabeli+1))=wygrana;
        this->wpisz_wyniki(naz_plk_z_wyn);
    }

    void wpisz_wyniki(string nazwa_pliku)
    {
        ofstream cel_wynikow;
        cel_wynikow.open(nazwa_pliku);
        for (int i=0;i<wlk_tab_wynikow;i++)
        {
            cel_wynikow<<i+1<<". "<<*(imiona+i)<<"\t"<<*(wyniki+i)<<"\n";
        }
        najnizszy_wynik=*(wyniki+99);
    }
public:
    tabela_wynikow(string nazwa_pliku)
    {
        ifstream zrodlo_wynikow;
        zrodlo_wynikow.open(nazwa_pliku);
        for(int i=0;i<wlk_tab_wynikow;i++)
        {
            zrodlo_wynikow>>*(imiona+i);
            zrodlo_wynikow>>*(imiona+i);
            zrodlo_wynikow>>*(wyniki+i);
            if( (*(wyniki+i))<najnizszy_wynik || i==0 )
            {
                najnizszy_wynik=(*(wyniki+i));
            }
        }
    }

    ~tabela_wynikow()
    {
        delete[] imiona;
        delete[] wyniki;
    }

    void wypisz_wyniki_na_ekran()
    {
        for (int i=0;i<wlk_tab_wynikow ;i++ )
        {
            cout<<i+1<<". "<<*(imiona+i)<<"   "<<*(wyniki+i)<<endl;
        }
    }

    void sprawdz_czy_wstawic(int wygrana, string nazwa_gracza)
    {
        if(wygrana>najnizszy_wynik)
        {
            wstaw_do_tabeli(wygrana, nazwa_gracza);
        }
    }

};

class gracz
{
private:
    string nazwa_gracza;
    int ilosc_pieniendzy;
public:
    gracz(string nazwa_gracza)
    {
        gracz::nazwa_gracza=nazwa_gracza;
        gracz::ilosc_pieniendzy=(1 + ( std::rand() % ( 1000 - 1 + 1 ) ));
    }

    string daj_nazwe_gracza()
    {
        return gracz::nazwa_gracza;
    }

    int daj_ilosc_pieniendzy()
    {
        return gracz::ilosc_pieniendzy;
    }
    void zmien_ilosc_pieniendzy(int ilosc_do_zmiany)
    {
        ilosc_pieniendzy=ilosc_do_zmiany;
    }
    void zmien_nazwe_gracza(string nazwa_gracza)
    {
        gracz::nazwa_gracza=nazwa_gracza;
    }
};

class stanowisko_do_gry
{
public:
    stanowisko_do_gry(int zaklad)
    {
        stanowisko_do_gry::zaklad=zaklad;
    };
    int daj_wynik()
    {
        return zaklad;
    }
    ~stanowisko_do_gry()
    {

    };

protected:
    int zaklad;
    void oblicz_wynik(int mnoznik)
    {
            zaklad=zaklad*mnoznik;
    }
private:
};

class stanowisko_do_ruletki:public stanowisko_do_gry
{
public:
    stanowisko_do_ruletki(int zaklad):stanowisko_do_gry(zaklad)
    {
    };
    ~stanowisko_do_ruletki()
    {

    };
    void zakrec_ruletka()
    {
        wylosowana_liczba=(0 + ( std::rand() % ( 36 - 0 + 1 ) ));
        sprawdz_wynik(wylosowana_liczba,this->rodzaj_zakladu);
    }
    void podaj_rodzaj_zakladu(string rodzaj_zakladu)
    {
        stanowisko_do_ruletki::rodzaj_zakladu=rodzaj_zakladu;
    }
protected:
private:
    string rodzaj_zakladu;
    int wylosowana_liczba;
    void sprawdz_wynik(int wylosowana_liczba, string rodzaj_zakladu)
    {
        int mnoznik=0;
        if(regex_match(rodzaj_zakladu,czy_numer))
        {
            if(stoi(rodzaj_zakladu)==wylosowana_liczba)
            {
            mnoznik=4;
            }
        }
        else if(rodzaj_zakladu.compare("jedendoosiemnascie")==0)
        {
            if (wylosowana_liczba>0&&wylosowana_liczba<19)
            {
                mnoznik=1;
            }
        }
        else if(rodzaj_zakladu.compare("dziewietnascietrzydziesciszesc")==0)
        {
            if (wylosowana_liczba>18&&wylosowana_liczba<37)
            {
                mnoznik=1;
            }
        }
        else if(rodzaj_zakladu.compare("parzyste")==0)
        {
            if (wylosowana_liczba%2==0)
            {
                mnoznik=1;
            }
        }
        else if(rodzaj_zakladu.compare("nieparzyste")==0)
        {
            if (wylosowana_liczba%2!=0)
            {
                mnoznik=1;
            }
        }
        else if((rodzaj_zakladu.compare("czerwone")==0)&&((wylosowana_liczba>0&&wylosowana_liczba<10&&wylosowana_liczba%2!=0)||(wylosowana_liczba>11&&wylosowana_liczba<19&&wylosowana_liczba%2==0)||(wylosowana_liczba>18&&wylosowana_liczba<28&&wylosowana_liczba%2!=0)||(wylosowana_liczba>29&&wylosowana_liczba<37&&wylosowana_liczba%2==0)))
        {
                mnoznik=1;
        }
        else if(wylosowana_liczba!=0&&(rodzaj_zakladu.compare("czarne")==0)&&!((wylosowana_liczba>0&&wylosowana_liczba<10&&wylosowana_liczba%2!=0)||(wylosowana_liczba>11&&wylosowana_liczba<19&&wylosowana_liczba%2==0)||(wylosowana_liczba>18&&wylosowana_liczba<28&&wylosowana_liczba%2!=0)||(wylosowana_liczba>29&&wylosowana_liczba<37&&wylosowana_liczba%2==0)))
        {
                mnoznik=1;
        }
        else if(rodzaj_zakladu.compare("pierwszadwunastka")==0)
        {
            if (wylosowana_liczba>0&&wylosowana_liczba<13)
            {
                mnoznik=2;
            }
        }
        else if(rodzaj_zakladu.compare("drugadwunastka")==0)
        {
            if (wylosowana_liczba>12&&wylosowana_liczba<25)
            {
                mnoznik=2;
            }
        }
        else if(rodzaj_zakladu.compare("trzeciadwunastka")==0)
        {
            if (wylosowana_liczba>24&&wylosowana_liczba<37)
            {
                mnoznik=2;
            }
        }
        if(mnoznik==0)
        {
            mnoznik=-1;
        }
        oblicz_wynik(mnoznik);
        if(((wylosowana_liczba>0&&wylosowana_liczba<10&&wylosowana_liczba%2!=0)||(wylosowana_liczba>11&&wylosowana_liczba<19&&wylosowana_liczba%2==0)||(wylosowana_liczba>18&&wylosowana_liczba<28&&wylosowana_liczba%2!=0)||(wylosowana_liczba>29&&wylosowana_liczba<37&&wylosowana_liczba%2==0)))
        {
            cout<<"Wylosowano "<<to_string(wylosowana_liczba)<<" czerwone"<<endl;
        }
        else if(wylosowana_liczba!=0&&!((wylosowana_liczba>0&&wylosowana_liczba<10&&wylosowana_liczba%2!=0)||(wylosowana_liczba>11&&wylosowana_liczba<19&&wylosowana_liczba%2==0)||(wylosowana_liczba>18&&wylosowana_liczba<28&&wylosowana_liczba%2!=0)||(wylosowana_liczba>29&&wylosowana_liczba<37&&wylosowana_liczba%2==0)))
        {
            cout<<"Wylosowano "<<to_string(wylosowana_liczba)<<" czarne"<<endl;
        }
        else
        {
            cout<<"Wylosowano "<<to_string(wylosowana_liczba)<<" zielone"<<endl;
        }
    }

};
class stanowisko_do_gry_w_blackjacka:public stanowisko_do_gry
{
public:
    stanowisko_do_gry_w_blackjacka(int zaklad):stanowisko_do_gry(zaklad)
    {
        suma_krupier=0;
        suma_gracz=0;
        for (int i2=0;i2<ilosc_kolorow ;i2++ )
        {
           int rodzaj_karty=1;
            for (int i=0;i<wlk_jednego_koloru ;i++ )
            {
                rodzaj_karty++;
                if (i2==0)
                {
                    if (rodzaj_karty<11)
                    {
                       *(talia_kart+i+(i2*13))="k"+to_string(rodzaj_karty);
                    }
                    else if
                    (rodzaj_karty==11)
                    {
                       *(talia_kart+i+(i2*13))="kJ";
                    }
                    else if
                    (rodzaj_karty==12)
                    {
                       *(talia_kart+i+(i2*13))="kQ";
                    }
                    else if
                    (rodzaj_karty==13)
                    {
                       *(talia_kart+i+(i2*13))="kK";
                    }
                    else if
                    (rodzaj_karty==14)
                    {
                       *(talia_kart+i+(i2*13))="kA";
                    }
                }
                else if (i2==1)
                {
                    if (rodzaj_karty<11)
                    {
                       *(talia_kart+i+(i2*13))="c"+to_string(rodzaj_karty);
                    }
                    else if
                    (rodzaj_karty==11)
                    {
                       *(talia_kart+i+(i2*13))="cJ";
                    }
                    else if
                    (rodzaj_karty==12)
                    {
                       *(talia_kart+i+(i2*13))="cQ";
                    }
                    else if
                    (rodzaj_karty==13)
                    {
                       *(talia_kart+i+(i2*13))="cK";
                    }
                    else if
                    (rodzaj_karty==14)
                    {
                       *(talia_kart+i+(i2*13))="cA";
                    }
                }
                else if (i2==2)
                {
                    if (rodzaj_karty<11)
                    {
                       *(talia_kart+i+(i2*13))="p"+to_string(rodzaj_karty);
                    }
                    else if
                    (rodzaj_karty==11)
                    {
                       *(talia_kart+i+(i2*13))="pJ";
                    }
                    else if
                    (rodzaj_karty==12)
                    {
                       *(talia_kart+i+(i2*13))="pQ";
                    }
                    else if
                    (rodzaj_karty==13)
                    {
                       *(talia_kart+i+(i2*13))="pK";
                    }
                    else if
                    (rodzaj_karty==14)
                    {
                       *(talia_kart+i+(i2*13))="pA";
                    }
                }
                else if (i2==3)
                {
                    if (rodzaj_karty<11)
                    {
                       *(talia_kart+i+(i2*13))="t"+to_string(rodzaj_karty);
                    }
                    else if
                    (rodzaj_karty==11)
                    {
                       *(talia_kart+i+(i2*13))="tJ";
                    }
                    else if
                    (rodzaj_karty==12)
                    {
                       *(talia_kart+i+(i2*13))="tQ";
                    }
                    else if
                    (rodzaj_karty==13)
                    {
                       *(talia_kart+i+(i2*13))="tK";
                    }
                    else if
                    (rodzaj_karty==14)
                    {
                       *(talia_kart+i+(i2*13))="tA";
                    }
                }
        }
    }
    };
    ~stanowisko_do_gry_w_blackjacka()
    {
        delete[] talia_kart;
        delete[] renka_gracza;
        delete[] renka_krupiera;
    };
    void potasuj_talie()
    {
        for (int i=0;i<ilosc_cykli_tasowania ;i++ )
        {
            int indeks_tasowania1=(0 + ( std::rand() % ( 51 - 0 + 1 ) ));
            int indeks_tasowania2=0;
            while(true)
            {
                indeks_tasowania2=(0 + ( std::rand() % ( 51 - 0 + 1 ) ));
                if (indeks_tasowania1!=indeks_tasowania2)
                {
                    break;
                }
            }
            swap(*(talia_kart+indeks_tasowania1),*(talia_kart+indeks_tasowania2));
        }
    }
    void przekaz_karte_do_reki_gracza(int ktora_z_gory, int do_ktorego_miejsca_w_rence)
    {
        swap(*(talia_kart+ktora_z_gory),*(renka_gracza+do_ktorego_miejsca_w_rence));
    }
    void przekaz_karte_do_reki_krupiera(int ktora_z_gory, int do_ktorego_miejsca_w_rence)
    {
        swap(*(talia_kart+ktora_z_gory),*(renka_krupiera+do_ktorego_miejsca_w_rence));
    }
    void dodaj_do_sumy_gracza(int pozycja_w_rence)
    {
        if(regex_match((*(renka_gracza+pozycja_w_rence)).substr(1,1),czy_numer))
        {
            if(stoi((*(renka_gracza+pozycja_w_rence)).substr(1,1))!=1)
            {
                suma_gracz+=stoi((*(renka_gracza+pozycja_w_rence)).substr(1,1));
            }
            else
            {
                suma_gracz+=10;
            }
        }
        else
        {
            suma_gracz+=10;
        }
    }
    void dodaj_do_sumy_krupiera(int pozycja_w_rence)
    {
        if(regex_match((*(renka_krupiera+pozycja_w_rence)).substr(1,1),czy_numer))
        {
            if(stoi((*(renka_krupiera+pozycja_w_rence)).substr(1,1))!=1)
            {
                suma_krupier+=stoi((*(renka_krupiera+pozycja_w_rence)).substr(1,1));
            }
            else
            {
                suma_krupier+=10;
            }
        }
        else
        {
            suma_krupier+=10;
        }
    }
    void sprawdz_wynik()
    {
        int mnoznik=0;
        if((suma_krupier>suma_gracz&&suma_krupier<=21)||(suma_krupier<=21&&suma_gracz>21))
        {
            mnoznik=-1;
        }
        else if((suma_krupier<suma_gracz&&suma_gracz<=21)||(suma_krupier>21&&suma_gracz<=21))
        {
            mnoznik=2;
        }
        else
        {
            mnoznik=0;
        }
        oblicz_wynik(mnoznik);
    }
    string pokaz_karte_gracza(int ktora_w_rence)
    {
        return *(renka_gracza+ktora_w_rence);
    }
    string pokaz_karte_krupiera(int ktora_w_rence)
    {
        return *(renka_krupiera+ktora_w_rence);
    }
    int daj_sume_gracza()
    {
        return suma_gracz;
    }
    int daj_sume_krupiera()
    {
        return suma_krupier;
    }
protected:
    string* talia_kart=new string[wlk_talii];
    string* renka_gracza=new string[maks_wlk_reki_gracza];
    string* renka_krupiera=new string[maks_wlk_reki_gracza];
    int suma_krupier;
    int suma_gracz;
private:
};

class stanowisko_do_gry_w_jednorekiego_bandyte:public stanowisko_do_gry
{
public:
    stanowisko_do_gry_w_jednorekiego_bandyte(int zaklad):stanowisko_do_gry(zaklad)
    {

    };
    ~stanowisko_do_gry_w_jednorekiego_bandyte()
    {
        delete[] kolo1;
        delete[] kolo2;
        delete[] kolo3;
    };
    void zakrec_bebnami()
    {
            string zapamietana_wartosc="";
            int ilosc_przesuniec_bebna=(10 + ( std::rand() % ( 50 - 10 + 1 ) ));
            for (int i2=0;i2<ilosc_przesuniec_bebna;i2++)
            {
                for (int i3=(ilosc_znakow_na_kole-1);i3>=0;i3-- )
                {
                    if (i3==14)
                    {
                        zapamietana_wartosc=*(kolo1+i3);
                    }
                    else
                    {
                        *(kolo1+i3+1)=*(kolo1+i3);
                    }
                    if (i3==0)
                    {
                        *(kolo1+i3)=zapamietana_wartosc;
                    }
                }
            }
            ilosc_przesuniec_bebna=(10 + ( std::rand() % ( 50 - 10 + 1 ) ));
            for (int i2=0;i2<ilosc_przesuniec_bebna ;i2++)
            {
                for (int i3=(ilosc_znakow_na_kole-1);i3>=0;i3-- )
                {
                    if (i3==14)
                    {
                        zapamietana_wartosc=*(kolo2+i3);
                    }
                    else
                    {
                        *(kolo2+i3+1)=*(kolo2+i3);
                    }
                    if (i3==0)
                    {
                        *(kolo2+i3)=zapamietana_wartosc;
                    }
                }
            }
            ilosc_przesuniec_bebna=(10 + ( std::rand() % ( 50 - 10 + 1 ) ));
            for (int i2=0;i2<ilosc_przesuniec_bebna ;i2++)
            {
                for (int i3=(ilosc_znakow_na_kole-1);i3>=0;i3--)
                {
                    if (i3==14)
                    {
                        zapamietana_wartosc=*(kolo3+i3);
                    }
                    else
                    {
                        *(kolo3+i3+1)=*(kolo3+i3);
                    }
                    if (i3==0)
                    {
                        *(kolo3+i3)=zapamietana_wartosc;
                    }
                }
            }
    }
    void sprawdz_wynik()
    {
        int mnoznik=-1;
        string pierwsza_pozycja=*(kolo1+1);
        if ((*(kolo2+1)).compare(pierwsza_pozycja)==0)
        {
            if ((*(kolo3+1)).compare(pierwsza_pozycja)==0)
            {
                mnoznik=1;
                if (pierwsza_pozycja=="777")
                {
                    mnoznik+=1;
                }
                if (pierwsza_pozycja=="BAR")
                {
                    mnoznik+=2;
                }
            }
        }
        oblicz_wynik(mnoznik);
    }
    string daj_wartosc_kolo1(int ktora_pozycja)
    {
        return *(kolo1+ktora_pozycja);
    }
    string daj_wartosc_kolo2(int ktora_pozycja)
    {
        return *(kolo2+ktora_pozycja);
    }
    string daj_wartosc_kolo3(int ktora_pozycja)
    {
        return *(kolo3+ktora_pozycja);
    }
protected:
private:
    string* kolo1=new string[ilosc_znakow_na_kole]{"777","BAR","dUb","$$$","***","@@@","+++","^=^","8H8","XVX","###","HxH","TNT","<0>",">x<"};
    string* kolo2=new string[ilosc_znakow_na_kole]{">x<","BAR","@@@","###","<0>","***","HxH","+++","8H8","^=^","XVX","dUb","777","TNT","$$$"};
    string* kolo3=new string[ilosc_znakow_na_kole]{"777","BAR","8H8","^=^","***","dUb","HxH","###","$$$","<0>","@@@","+++","TNT",">x<","XVX"};
};


int ile_postawic(gracz gracz)
{
    string ilosc_postawiona="0";
    int ilosc_postawiona_int=0;
    bool czy_przyjety=false;
    while(true)
    {
        system("CLS");
        cout<<"Gracz: "<<gracz.daj_nazwe_gracza()<<" | Ilosc pieniendzy: "<<gracz.daj_ilosc_pieniendzy()<<endl<<endl;
        cout<<"Ile chcesz postawic: ";
        cin>>ilosc_postawiona;
        if(regex_match(ilosc_postawiona,czy_numer))
        {
            ilosc_postawiona_int=stoi(ilosc_postawiona);
            if (ilosc_postawiona_int>0&&ilosc_postawiona_int<=gracz.daj_ilosc_pieniendzy())
            {
                cout<<"Zaklad przyjety"<<endl;
                czy_przyjety=true;
            }
            else if (ilosc_postawiona_int>0)
            {
                cout<<"Nie masz tyle pieniendzy"<<endl;
            }
            else
            {
                cout<<"Musisz postawic wiecej niz 0"<<endl;
            }
        }
        else
        {
            cout<<"wprowdz liczbe dodatnia"<<endl;
        }
        system("PAUSE");
        if (czy_przyjety)
        {
           break;
        }
    }

    return ilosc_postawiona_int;
}
void zagraj_ruletka(gracz* gracz,tabela_wynikow* tabela,string rodzaj_zakladu)
{

                        int zaklad=ile_postawic(*gracz);

                        stanowisko_do_ruletki stanowisko_do_ruletki(zaklad);
                        stanowisko_do_ruletki.podaj_rodzaj_zakladu(rodzaj_zakladu);
                        stanowisko_do_ruletki.zakrec_ruletka();
                        if(stanowisko_do_ruletki.daj_wynik()>gracz->daj_ilosc_pieniendzy())
                        {
                            gracz->zmien_ilosc_pieniendzy(0);
                        }
                        else
                        {
                            gracz->zmien_ilosc_pieniendzy(gracz->daj_ilosc_pieniendzy()+stanowisko_do_ruletki.daj_wynik());
                        }
                        if (stanowisko_do_ruletki.daj_wynik()>0)
                        {
                            cout<<"Gratulacje, wygrales "<<stanowisko_do_ruletki.daj_wynik()<<endl;
                            tabela->sprawdz_czy_wstawic(stanowisko_do_ruletki.daj_wynik(),gracz->daj_nazwe_gracza());
                        }
                        else
                        {
                            cout<<"Niestety przegrales "<<stanowisko_do_ruletki.daj_wynik()*(-1)<<endl;
                        }

}
void zagraj_blackjack(gracz* gracz,tabela_wynikow* tabela)
{
    int i1=1;
    int i3=1;//pozycja ostatniej karty w rence gracza
    int i4=0;//pozycja ostatniej karty w rence krupiera
    bool czy_gracz_spasowal=false;
    bool czy_wyjsc=false;
    bool czy_wyjsc2=false;
    string opcja="";
    int zaklad=ile_postawic(*gracz);
    stanowisko_do_gry_w_blackjacka stanowisko_do_gry_w_blackjacka(zaklad);
    stanowisko_do_gry_w_blackjacka.potasuj_talie();
    stanowisko_do_gry_w_blackjacka.przekaz_karte_do_reki_gracza(0,0);
    stanowisko_do_gry_w_blackjacka.dodaj_do_sumy_gracza(0);
    stanowisko_do_gry_w_blackjacka.przekaz_karte_do_reki_gracza(1,1);
    stanowisko_do_gry_w_blackjacka.dodaj_do_sumy_gracza(1);
    //system("pause");
    system("cls");
    while (true)
    {
        i1++;
        if (!czy_gracz_spasowal)
        {
            while (true)
            {
            czy_wyjsc=false;
            system("cls");
            cout<<"Legenda: k - kier, c - karo, p - pik, t - trefl"<<endl<<endl;
            cout<<"Twoja reka:"<<endl<<endl;
            for (int i2=0;i2<i3+1 ;i2++)
            {
                cout<<stanowisko_do_gry_w_blackjacka.pokaz_karte_gracza(i2)<<" ";
            }
            cout<<endl<<endl;
            cout<<"Co chcesz zrobic: "<<endl;
            cout<<"1. Pociagnij karte"<<endl;
            cout<<"0. Spasuj"<<endl;
            cin>>opcja;
            if (regex_match(opcja,czy_numer))
            {
                switch(stoi(opcja))
                {
                case 1:
                    {
                     stanowisko_do_gry_w_blackjacka.przekaz_karte_do_reki_gracza(i1,i3+1);
                     stanowisko_do_gry_w_blackjacka.dodaj_do_sumy_gracza(i3+1);
                     i3++;
                     czy_wyjsc=true;
                    }
                    break;
                case 0:
                    {
                        czy_gracz_spasowal=true;
                        czy_wyjsc=true;
                    }
                    break;
                default:
                    cout<<"Niepoprawna opcja"<<endl;
                break;
                }
            }
            else
            {
                cout<<"Niepoprawna opcja"<<endl;
            }
            if(czy_wyjsc)
            {
                break;
            }

            system("pause");
            }
            if (i3==8)
            {
                czy_gracz_spasowal=true;
                system("cls");
                cout<<"Legenda: k - kier, c - karo, p - pik, t - trefl"<<endl<<endl;
                cout<<"Twoja reka:"<<endl<<endl;
                for (int i2=0;i2<i3+1 ;i2++)
                {
                    cout<<stanowisko_do_gry_w_blackjacka.pokaz_karte_gracza(i2)<<" ";
                }
                cout<<endl<<endl;
            }
        }
        else
        {
            czy_wyjsc2=false;
            cout<<"Kolej krupiera"<<endl;
            if (i4<2)
            {
                stanowisko_do_gry_w_blackjacka.przekaz_karte_do_reki_krupiera(i1,i4);
                stanowisko_do_gry_w_blackjacka.dodaj_do_sumy_krupiera(i4);
                i4++;
                cout<<"Krupier ciagnie karte"<<endl;
            }
            else
            {
                if (stanowisko_do_gry_w_blackjacka.daj_sume_krupiera()<16&&i3<8)
                {
                    stanowisko_do_gry_w_blackjacka.przekaz_karte_do_reki_krupiera(i1,i4);
                    stanowisko_do_gry_w_blackjacka.dodaj_do_sumy_krupiera(i4);
                    i4++;
                    cout<<"Krupier ciagnie karte"<<endl;
                }
                else
                {
                    czy_wyjsc2=true;
                    cout<<"Krupier spasowal"<<endl;
                }
            }

        }
        if (czy_wyjsc2)
        {
            break;
        }

    }
    system("pause");
        system("cls");
        stanowisko_do_gry_w_blackjacka.sprawdz_wynik();
        cout<<"Twoja reka:"<<endl;
        for (int i6=0;i6<=i3 ;i6++ )
        {
            cout<<stanowisko_do_gry_w_blackjacka.pokaz_karte_gracza(i6)<<" ";
        }
        cout<<endl;
        cout<<"Reka krupiera:"<<endl;
        for (int i6=0;i6<=i4 ;i6++ )
        {
            cout<<stanowisko_do_gry_w_blackjacka.pokaz_karte_krupiera(i6)<<" ";
        }
        cout<<endl;
        if (stanowisko_do_gry_w_blackjacka.daj_wynik()==0)
        {
            cout<<"Remis, zaklad zostaje zwrocony"<<endl;
        }
        if (stanowisko_do_gry_w_blackjacka.daj_wynik()>0)
        {
            cout<<"Gratulacje, wygrales "<<to_string(stanowisko_do_gry_w_blackjacka.daj_wynik())<<endl;
        }
        if (stanowisko_do_gry_w_blackjacka.daj_wynik()<0)
        {
            cout<<"Niestety, przegrales "<<to_string(stanowisko_do_gry_w_blackjacka.daj_wynik()*(-1))<<endl;;
        }
        gracz->zmien_ilosc_pieniendzy(gracz->daj_ilosc_pieniendzy()+stanowisko_do_gry_w_blackjacka.daj_wynik());
        tabela->sprawdz_czy_wstawic(stanowisko_do_gry_w_blackjacka.daj_wynik(),gracz->daj_nazwe_gracza());
}
void zagraj_jednoreki_bandyta(gracz* gracz,tabela_wynikow* tabela)
{
    int zaklad=ile_postawic(*gracz);
    system("cls");
    stanowisko_do_gry_w_jednorekiego_bandyte stanowisko_do_gry_w_jednorekiego_bandyte(zaklad);
    for (int i=0;i<3 ;i++ )
    {
        cout<<"|"<<stanowisko_do_gry_w_jednorekiego_bandyte.daj_wartosc_kolo1(i)<<"|"<<stanowisko_do_gry_w_jednorekiego_bandyte.daj_wartosc_kolo2(i)<<"|"<<stanowisko_do_gry_w_jednorekiego_bandyte.daj_wartosc_kolo3(i)<<"|"<<endl;
    }
    cout<<endl;
    cout<<"Wcisnij dowolny przycisk by pociagnac dzwignie"<<endl;
    system("pause");
    stanowisko_do_gry_w_jednorekiego_bandyte.zakrec_bebnami();
    stanowisko_do_gry_w_jednorekiego_bandyte.sprawdz_wynik();
    system("cls");
    for (int i=0;i<3 ;i++ )
    {
        cout<<"|"<<stanowisko_do_gry_w_jednorekiego_bandyte.daj_wartosc_kolo1(i)<<"|"<<stanowisko_do_gry_w_jednorekiego_bandyte.daj_wartosc_kolo2(i)<<"|"<<stanowisko_do_gry_w_jednorekiego_bandyte.daj_wartosc_kolo3(i)<<"|"<<endl;
    }
    if (stanowisko_do_gry_w_jednorekiego_bandyte.daj_wynik()>0)
    {
        cout<<"Gratulacje, wygrales "<<to_string(stanowisko_do_gry_w_jednorekiego_bandyte.daj_wynik())<<endl;
    }
    if (stanowisko_do_gry_w_jednorekiego_bandyte.daj_wynik()<0)
    {
        cout<<"Niestety, przegrales "<<to_string(stanowisko_do_gry_w_jednorekiego_bandyte.daj_wynik()*(-1))<<endl;;
    }
    gracz->zmien_ilosc_pieniendzy(gracz->daj_ilosc_pieniendzy()+stanowisko_do_gry_w_jednorekiego_bandyte.daj_wynik());
    tabela->sprawdz_czy_wstawic(stanowisko_do_gry_w_jednorekiego_bandyte.daj_wynik(),gracz->daj_nazwe_gracza());

}
void gra_w_ruletke(gracz* gracz,tabela_wynikow* tabela)
{
    string rodzaj_zakladu="";
    string opcja="";
    bool czy_wyjsc=false;
    while(true)
    {
        system("CLS");
        cout<<"Gracz: "<<gracz->daj_nazwe_gracza()<<" | Ilosc pieniendzy: "<<gracz->daj_ilosc_pieniendzy()<<endl<<endl;
        cout<<"Witaj przy stole do ruletki co chcesz zrobic: "<<endl;
        cout<<"1. Postaw na numery 1-18"<<endl;
        cout<<"2. Postaw na numery 19-36"<<endl;
        cout<<"3. Postaw na numery parzyste"<<endl;
        cout<<"4. Postaw na numery nieparzyste"<<endl;
        cout<<"5. Postaw na numery czerwone"<<endl;
        cout<<"6. Postaw na numery czarne"<<endl;
        cout<<"7. Postaw na numery 1-12"<<endl;
        cout<<"8. Postaw na numery 13-24"<<endl;
        cout<<"9. Postaw na numery 24-36"<<endl;
        cout<<"10. Postaw na pojedynczy numer"<<endl;
        cout<<"0. Powrot do menu glownego"<<endl;
        cin>>opcja;
        if(regex_match(opcja,czy_numer))
            {
            switch( stoi(opcja) )
                {
                    case 1:
                        {
                        rodzaj_zakladu="jedendoosiemnascie";
                        if(gracz->daj_ilosc_pieniendzy()>0)
                        {
                            zagraj_ruletka(gracz, tabela, rodzaj_zakladu);
                        }
                        else
                        {
                            cout<<"Nie masz pieniendzy, nie mozesz juz grac"<<endl;
                        }
                        }
                    break;

                    case 2:
                        {
                        rodzaj_zakladu="dziewietnascietrzydziesciszesc";
                        if(gracz->daj_ilosc_pieniendzy()>0)
                        {
                            zagraj_ruletka(gracz, tabela, rodzaj_zakladu);
                        }
                        else
                        {
                            cout<<"Nie masz pieniendzy, nie mozesz juz grac"<<endl;
                        }
                        }
                    break;

                    case 3:
                        {
                        rodzaj_zakladu="parzyste";
                        if(gracz->daj_ilosc_pieniendzy()>0)
                        {
                            zagraj_ruletka(gracz, tabela, rodzaj_zakladu);
                        }
                        else
                        {
                            cout<<"Nie masz pieniendzy, nie mozesz juz grac"<<endl;
                        }
                        }
                    break;

                    case 4:
                        {
                        rodzaj_zakladu="nieparzyste";
                        if(gracz->daj_ilosc_pieniendzy()>0)
                        {
                            zagraj_ruletka(gracz, tabela, rodzaj_zakladu);
                        }
                        else
                        {
                            cout<<"Nie masz pieniendzy, nie mozesz juz grac"<<endl;
                        }
                        }
                    break;

                    case 5:
                        {
                        rodzaj_zakladu="czerwone";
                        if(gracz->daj_ilosc_pieniendzy()>0)
                        {
                            zagraj_ruletka(gracz, tabela, rodzaj_zakladu);
                        }
                        else
                        {
                            cout<<"Nie masz pieniendzy, nie mozesz juz grac"<<endl;
                        }
                        }
                    break;

                    case 6:
                        {
                        rodzaj_zakladu="czarne";
                        if(gracz->daj_ilosc_pieniendzy()>0)
                        {
                            zagraj_ruletka(gracz, tabela, rodzaj_zakladu);
                        }
                        else
                        {
                            cout<<"Nie masz pieniendzy, nie mozesz juz grac"<<endl;
                        }
                        }
                    break;

                    case 7:
                        {
                        rodzaj_zakladu="pierwszadwunastka";
                        if(gracz->daj_ilosc_pieniendzy()>0)
                        {
                            zagraj_ruletka(gracz, tabela, rodzaj_zakladu);
                        }
                        else
                        {
                            cout<<"Nie masz pieniendzy, nie mozesz juz grac"<<endl;
                        }
                        }
                    break;

                    case 8:
                        {
                        rodzaj_zakladu="drugadwunastka";
                        if(gracz->daj_ilosc_pieniendzy()>0)
                        {
                            zagraj_ruletka(gracz, tabela, rodzaj_zakladu);
                        }
                        else
                        {
                            cout<<"Nie masz pieniendzy, nie mozesz juz grac"<<endl;
                        }
                        }
                    break;

                    case 9:
                        {
                        rodzaj_zakladu="trzeciadwunastka";
                        if(gracz->daj_ilosc_pieniendzy()>0)
                        {
                            zagraj_ruletka(gracz, tabela, rodzaj_zakladu);
                        }
                        else
                        {
                            cout<<"Nie masz pieniendzy, nie mozesz juz grac"<<endl;
                        }
                        }
                    break;

                    case 10:
                        {
                            bool czy_numer_przyjety=false;
                            string obstawiony_numer="";
                            if(gracz->daj_ilosc_pieniendzy()>0)
                            {
                            while(true)
                            {
                            system("CLS");
                            cout<<"Podaj numer, na ktory chcesz postawic: ";
                            obstawiony_numer="";
                            cin>>obstawiony_numer;
                            if (regex_match(obstawiony_numer,czy_numer))
                            {
                                int obstawiony_numer_int=stoi(obstawiony_numer);
                                if (obstawiony_numer_int>=0&&obstawiony_numer_int<37)
                                {
                                    obstawiony_numer=to_string(obstawiony_numer_int);
                                    czy_numer_przyjety=true;
                                }
                                else
                                {
                                    cout<<"Wprowadz numer od 0 do 36"<<endl;
                                }
                            }
                            else
                            {
                            cout<<"Wprowadz numer"<<endl;
                            }
                            if (czy_numer_przyjety)
                            {
                                cout<<"Obstawiam "<<obstawiony_numer<<endl;
                                system("Pause");
                                break;
                            }
                            system("Pause");
                            }
                            rodzaj_zakladu=obstawiony_numer;
                            zagraj_ruletka(gracz, tabela, rodzaj_zakladu);
                            }
                            else
                            {
                                cout<<"Nie masz pieniendzy, nie mozesz juz grac"<<endl;
                            }
                        }
                    break;

                    case 0:
                        czy_wyjsc=true;
                    break;

                    default:
                        cout<<"Niepoprawna opcja"<<endl;
                    break;
                }
            }
            else
            {
                cout<<"Niepoprawna opcja"<<endl;
            }
            if(czy_wyjsc)
            {
                break;
            }
            system("Pause");
    }

}
void gra_w_blackjacka(gracz* gracz, tabela_wynikow* tabela)
{
    string opcja="";
    bool czy_wyjsc=false;
    while (true)
    {
       system("CLS");
    cout<<"Gracz: "<<gracz->daj_nazwe_gracza()<<" | Ilosc pieniendzy: "<<gracz->daj_ilosc_pieniendzy()<<endl<<endl;
    cout<<"Witaj przy stole do blackjacka, co chcesz zrobic: "<<endl;
    cout<<"1. Postaw i zagraj"<<endl;
    cout<<"0. Powrot do menu glownego"<<endl;
    cin>>opcja;
    if (regex_match(opcja,czy_numer))
    {
        switch(stoi(opcja))
        {
            case 1:
            {
                if(gracz->daj_ilosc_pieniendzy()>0)
                    {
                        zagraj_blackjack(gracz, tabela);
                    }
                    else
                    {
                        cout<<"Nie masz pieniendzy, nie mozesz juz grac"<<endl;
                    }
                break;
            }
            case 0:
            {
                czy_wyjsc=true;
                break;
            }
            default:
                cout<<"Niepoprawna opcja"<<endl;
                break;
        }
    }
    else
    {
        cout<<"Niepoprawna opcja"<<endl;
    }
    if(czy_wyjsc)
    {
        break;
    }
    system("Pause");
    }
}
void gra_w_jednorekiego_bandyte(gracz* gracz, tabela_wynikow* tabela)
{
    string opcja="";
    bool czy_wyjsc=false;
    while (true)
    {
    system("CLS");
    cout<<"Gracz: "<<gracz->daj_nazwe_gracza()<<" | Ilosc pieniendzy: "<<gracz->daj_ilosc_pieniendzy()<<endl<<endl;
    cout<<"Witaj przy jednorekim bandycie, co chcesz zrobic: "<<endl;
    cout<<"1. Postaw i zagraj"<<endl;
    cout<<"0. Powrot do menu glownego"<<endl;
    cin>>opcja;
    if (regex_match(opcja,czy_numer))
    {
        switch(stoi(opcja))
        {
            case 1:
            {
                if(gracz->daj_ilosc_pieniendzy()>0)
                    {
                        zagraj_jednoreki_bandyta(gracz, tabela);
                    }
                    else
                    {
                        cout<<"Nie masz pieniendzy, nie mozesz juz grac"<<endl;
                    }
                break;
            }
            case 0:
            {
                czy_wyjsc=true;
                break;
            }
            default:
                cout<<"Niepoprawna opcja"<<endl;
                break;
        }
    }
    else
    {
        cout<<"Niepoprawna opcja"<<endl;
    }
    if(czy_wyjsc)
    {
        break;
    }
    system("Pause");
    }

}

int main()
{
    string opcja="";
    string nazwa_gracza="";
    ifstream test;
    fstream wyniki;
    regex nazwa_gracza_regex("[A-Za-z0-9]{3}");
    bool czy_wyjsc=false;

    srand(time(NULL));
    //stworzenie lub odczytanie pliku z wynikami i wrzucenie do obiektu tabela wynikow
    test.open(naz_plk_z_wyn);
    if(!test)
    {
        test.close();
        wyniki.open(naz_plk_z_wyn,fstream::out);
        for (int i=0;i<wlk_tab_wynikow;i++)
        {
            wyniki<<i+1<<". XXX\t0\n";

        }
        wyniki.close();
    }
    tabela_wynikow tabela(naz_plk_z_wyn);
    //koniec
    while(true)
    {
        cout<<"Podaj nazwe gracza (3 znaki alfanumeryczne dowolnej wielkosci): ";
        cin>>nazwa_gracza;
        if(regex_match(nazwa_gracza,nazwa_gracza_regex))
        {
            break;
        }
        cout<<"Niepoprawna nazwa"<<endl;
        system("PAUSE");
        system("CLS");
    }
    system("CLS");
    gracz gracz(nazwa_gracza);
    while(true)
        {
            cout<<"Gracz: "<<gracz.daj_nazwe_gracza()<<" | Ilosc pieniendzy: "<<gracz.daj_ilosc_pieniendzy()<<endl<<endl;
            cout<<"Wybierz co chcesz zrobic:"<<endl;
            cout<<"1. Zagraj w ruletke"<<endl;
            cout<<"2. Zagraj na jednorekim bandycie"<<endl;
            cout<<"3. Zagraj w blackjacka"<<endl;
            cout<<"4. Pokaz tabele wynikow"<<endl;
            cout<<"5. Rozpocznij gre od nowa"<<endl;
            cout<<"0. Wyjdz z gry"<<endl;
            cin>>opcja;
            if(regex_match(opcja,czy_numer))
            {
            switch( stoi(opcja) )
                {
                    case 1:
                        if (gracz.daj_ilosc_pieniendzy()<=0)
                        {
                            cout<<"Nie masz juz pieniendzy, nie mozesz dalej grac";
                        }
                        else
                        {
                            gra_w_ruletke(&gracz,&tabela);
                        }
                    break;

                    case 2:
                        if (gracz.daj_ilosc_pieniendzy()<=0)
                        {
                            cout<<"Nie masz juz pieniendzy, nie mozesz dalej grac";
                        }
                        else
                        {
                            gra_w_jednorekiego_bandyte(&gracz,&tabela);
                        }

                    break;

                    case 3:
                        if (gracz.daj_ilosc_pieniendzy()<=0)
                        {
                            cout<<"Nie masz juz pieniendzy, nie mozesz dalej grac";
                        }
                        else
                        {
                            gra_w_blackjacka(&gracz, &tabela);
                        }

                    break;

                    case 4:
                        system("CLS");
                        cout<<"Top 100 wynikow"<<endl<<endl;
                        tabela.wypisz_wyniki_na_ekran();
                    break;

                    case 5:
                        system("CLS");
                        while(true)
                        {
                        cout<<"Podaj nazwe gracza (3 znaki alfanumeryczne dowolnej wielkosci): ";
                        cin>>nazwa_gracza;
                        if(regex_match(nazwa_gracza,nazwa_gracza_regex))
                        {
                            break;
                        }
                        cout<<"Niepoprawna nazwa"<<endl;
                        system("PAUSE");
                        system("CLS");
                        }
                        gracz.zmien_nazwe_gracza(nazwa_gracza);
                        gracz.zmien_ilosc_pieniendzy((1 + ( std::rand() % ( 1000 - 1 + 1 ) )));
                        system("CLS");
                    break;

                    case 0:
                        czy_wyjsc=true;
                    break;

                    default:
                        cout<<"Niepoprawna opcja"<<endl;
                    break;
                }
            }
            else
            {
                cout<<"Niepoprawna opcja"<<endl;
            }
            if (czy_wyjsc)
            {
                system("CLS");
                cout<<"Do widzenia"<<endl;
                break;
            }
            else
            {
                system("PAUSE");
                system("CLS");
            }
        }

    return 0;
}
