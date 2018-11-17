#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>

#define NIESKONCZONOSC 1000

using namespace std;

struct MapaPomocnicza
{
    int status; //1 - punkt jest na liscie otwartej, 2 - na zamknietej, 5 - przeszkoda, 0 - na zadnej
    int xRodzica;
    int yRodzica;
    double f;
    int kosztDojscia;
};

void Algorytm();

int** Wczytaj(string nazwa, int rozmiar);

double ObliczF(int PozycjaX, int PozycjaY, int CelX, int CelY, int koszt);

int main(){

    Algorytm();

    cout << endl << endl << "Zakonczono dzialanie programu. Aby zakonczyc, wcisnij dowolny klawisz" << endl;

    system("pause");

    return 0;
}

void Algorytm(){

    string nazwaPliku = "grid.txt";

    int wymiar = 20;


    //TWORZENIE MAPY
    int **mapa;
    mapa = new int*[wymiar];

    for(int i = 0 ; i < wymiar ; i++)
        mapa[i] = new int[wymiar];

    mapa = Wczytaj(nazwaPliku, wymiar);

    //WYPISANIE MAPY NA EKRAN
    for(int i = 0 ; i < wymiar ; i++){
        for(int j = 0 ; j < wymiar ; j++)
            cout << mapa[i][j] << " ";
        cout << endl;
    }
    cout << endl;


    //#######################################################
    //WYKONANIE ALGORYTMU


    //Liczymy wspolrzedne od lewego gornego rogu
    //Wpolrzedne punktu startowego i koncowego
    int xStart = 0;
    int yStart = 19;

    int xCel = 19;
    int yCel = 0;

    //Wspolrzedne aktualnie rozpatrywanej pozycji, poczatkowo ustawione na start
    int xPoz = xStart;
    int yPoz = yStart;

    //Ustalamy koszt ruchu
    int kosztRuchu = 1;

    //TWORZENIE MAPY POMOCNICZEJ PRZECHOWUJACEJ INFORMACJE O DANYM POLU
    MapaPomocnicza info[wymiar][wymiar];

    //Ustawienie przeszkod na mapie pomocniczej
    for(int i = 0 ; i < wymiar ; i++){
        for(int j = 0 ; j < wymiar ; j++)
            info[i][j].status = mapa[i][j];
    }


    //Dodanie punktu startowego do listy zamknietej
    info[yStart][xStart].status = 2;

    //Ustawenie rodzica i kosztu dojscia dla kratki poczatkowej
    info[yStart][xStart].xRodzica = xStart;
    info[yStart][xStart].yRodzica = yStart;
    info[yStart][xStart].kosztDojscia = 0;


    //Wykorzystujemy heurystyke Euklidesowa, liczymy wartosc f dla punktu startowego
    info[yStart][xStart].f = ObliczF(xPoz, yPoz, xCel, yCel, info[yStart][xStart].kosztDojscia);

    bool flaga = false;

    while(!((xPoz == xCel) && (yPoz == yCel))){

        //DODAWANIE POL NA LISTE OTWARTA
        if(yPoz != 0 && info[yPoz-1][xPoz].status != 5 && info[yPoz-1][xPoz].status != 2){
            if(info[yPoz-1][xPoz].status == 1){
                if(info[yPoz-1][xPoz].f > (ObliczF(xPoz, yPoz-1, xCel, yCel, info[yPoz][xPoz].kosztDojscia)) + kosztRuchu){
                    info[yPoz-1][xPoz].xRodzica = xPoz;
                    info[yPoz-1][xPoz].yRodzica = yPoz;
                    info[yPoz-1][xPoz].kosztDojscia = info[yPoz][xPoz].kosztDojscia + kosztRuchu;
                    info[yPoz-1][xPoz].f = ObliczF(xPoz, yPoz-1, xCel, yCel, info[yPoz][xPoz].kosztDojscia) + kosztRuchu;
                    }
            }
            else{
                info[yPoz-1][xPoz].status = 1;
                info[yPoz-1][xPoz].xRodzica = xPoz;
                info[yPoz-1][xPoz].yRodzica = yPoz;
                info[yPoz-1][xPoz].kosztDojscia = info[yPoz][xPoz].kosztDojscia + kosztRuchu;
                info[yPoz-1][xPoz].f = ObliczF(xPoz, yPoz-1, xCel, yCel, info[yPoz][xPoz].kosztDojscia) + kosztRuchu;
             //   cout << "Dodaje kratke y:" << yPoz-1 << " x:" << xPoz << " do listy otwartej. Rodzic y:" << info[yPoz-1][xPoz].yRodzica << " x:" << info[yPoz-1][xPoz].xRodzica << endl;
            }
        }


        if(yPoz != 19 && info[yPoz+1][xPoz].status != 5 && info[yPoz+1][xPoz].status != 2){
            if(info[yPoz+1][xPoz].status == 1){
                if(info[yPoz+1][xPoz].f > (ObliczF(xPoz, yPoz+1, xCel, yCel, info[yPoz][xPoz].kosztDojscia)) + kosztRuchu){
                    info[yPoz+1][xPoz].xRodzica = xPoz;
                    info[yPoz+1][xPoz].yRodzica = yPoz;
                    info[yPoz+1][xPoz].kosztDojscia = info[yPoz][xPoz].kosztDojscia + kosztRuchu;
                    info[yPoz+1][xPoz].f = ObliczF(xPoz, yPoz+1, xCel, yCel, info[yPoz][xPoz].kosztDojscia) + kosztRuchu;
                }
            }
            else{
                info[yPoz+1][xPoz].status = 1;
                info[yPoz+1][xPoz].xRodzica = xPoz;
                info[yPoz+1][xPoz].yRodzica = yPoz;
                info[yPoz+1][xPoz].kosztDojscia = info[yPoz][xPoz].kosztDojscia + kosztRuchu;
                info[yPoz+1][xPoz].f = ObliczF(xPoz, yPoz+1, xCel, yCel, info[yPoz][xPoz].kosztDojscia) + kosztRuchu;
           //     cout << "Dodaje kratke y:" << yPoz+1 << " x:" << xPoz << " do listy otwartej. Rodzic y:" << info[yPoz+1][xPoz].yRodzica << " x:" << info[yPoz+1][xPoz].xRodzica << endl;
            }
        }


        if(xPoz != 0 && info[yPoz][xPoz-1].status != 5 && info[yPoz][xPoz-1].status != 2){
            if(info[yPoz][xPoz-1].status == 1){
                if(info[yPoz][xPoz-1].f > (ObliczF(xPoz-1, yPoz, xCel, yCel, info[yPoz][xPoz].kosztDojscia)) + kosztRuchu){
                    info[yPoz][xPoz-1].xRodzica = xPoz;
                    info[yPoz][xPoz-1].yRodzica = yPoz;
                    info[yPoz][xPoz-1].kosztDojscia = info[yPoz][xPoz].kosztDojscia + kosztRuchu;
                    info[yPoz][xPoz-1].f = ObliczF(xPoz-1, yPoz, xCel, yCel, info[yPoz][xPoz].kosztDojscia) + kosztRuchu;
                }
            }
            else{
                info[yPoz][xPoz-1].status = 1;
                info[yPoz][xPoz-1].xRodzica = xPoz;
                info[yPoz][xPoz-1].yRodzica = yPoz;
                info[yPoz][xPoz-1].kosztDojscia = info[yPoz][xPoz].kosztDojscia + kosztRuchu;
                info[yPoz][xPoz-1].f = ObliczF(xPoz-1, yPoz, xCel, yCel, info[yPoz][xPoz].kosztDojscia) + kosztRuchu;
         //       cout << "Dodaje kratke y:" << yPoz << " x:" << xPoz-1 << " do listy otwartej. Rodzic y:" << info[yPoz][xPoz-1].yRodzica << " x:" << info[yPoz][xPoz-1].xRodzica << endl;
            }
        }



        if(xPoz != 19 && info[yPoz][xPoz+1].status != 5 && info[yPoz][xPoz+1].status != 2){
            if(info[yPoz][xPoz+1].status == 1){
                if(info[yPoz][xPoz+1].f > (ObliczF(xPoz+1, yPoz, xCel, yCel, info[yPoz][xPoz].kosztDojscia)) + kosztRuchu){
                    info[yPoz][xPoz+1].xRodzica = xPoz;
                    info[yPoz][xPoz+1].yRodzica = yPoz;
                    info[yPoz][xPoz+1].kosztDojscia = info[yPoz][xPoz].kosztDojscia + kosztRuchu;
                    info[yPoz][xPoz+1].f = ObliczF(xPoz+1, yPoz, xCel, yCel, info[yPoz][xPoz].kosztDojscia) + kosztRuchu;
                }
            }
            else{
                info[yPoz][xPoz+1].status = 1;
                info[yPoz][xPoz+1].xRodzica = xPoz;
                info[yPoz][xPoz+1].yRodzica = yPoz;
                info[yPoz][xPoz+1].kosztDojscia = info[yPoz][xPoz].kosztDojscia + kosztRuchu;
                info[yPoz][xPoz+1].f = ObliczF(xPoz+1, yPoz, xCel, yCel, info[yPoz][xPoz].kosztDojscia) + kosztRuchu;
       //         cout << "Dodaje kratke y:" << yPoz << " x:" << xPoz+1 << " do listy otwartej. Rodzic y:" << info[yPoz][xPoz+1].yRodzica << " x:" << info[yPoz][xPoz+1].xRodzica << endl;
            }
        }

        //DODAWANIE DO LISTY ZAMKNIETEJ
        double najmniejsza = NIESKONCZONOSC;
        int xPom = 0;
        int yPom = 0;


        for(int i = 0 ; i < wymiar ; i++){
            for(int j = 0 ; j < wymiar ; j++){
                // - tutaj
                if(info[i][j].status == 1){
                    if(info[i][j].f <= najmniejsza){
                        najmniejsza = info[i][j].f;
                        yPom = i;
                        xPom = j;

                    }
                }
            }
        }

        xPoz = xPom;
        yPoz = yPom;

        info[yPoz][xPoz].status = 2;

        int licznik = 0;

        for(int i=0; i<wymiar; i++) {
            for(int j=0; j<wymiar; j++) {
                if(info[i][j].status == 1) {
                    licznik=licznik+1;;
                }
            }
        }



        if((licznik == 0) && !((info[yPoz-1][xPoz].status == 0) || (info[yPoz+1][xPoz].status == 0) || (info[yPoz][xPoz-1].status == 0) || (info[yPoz][xPoz+1].status == 0))){
            cout << "Nie mozna znalezc drogi do celu." << endl;
            flaga = true;
            break;
        }


    }

    cout << endl;
    if (flaga == false) {
        cout << "Wracam do domu!" << endl;
        mapa[yStart][xStart] = 3;
        int zmiennaPomX = 0;
        int zmiennaPomY = 0;

        while(!((xPoz == xStart) && (yPoz == yStart))){
            mapa[yPoz][xPoz] = 3;


            zmiennaPomY = info[yPoz][xPoz].yRodzica;
            zmiennaPomX = info[yPoz][xPoz].xRodzica;

            xPoz = zmiennaPomX;
            yPoz = zmiennaPomY;
        }





        //WYPISANIE SCIEZKI WYNIKOWEJ
        for(int i = 0 ; i < wymiar ; i++){
            for(int j = 0 ; j < wymiar ; j++)
                cout << mapa[i][j] << " ";
            cout << endl;
        }

    }

    //#######################################################


}



//WCZYTYWANIE DANYCH Z PLIKU
int** Wczytaj(string nazwa, int rozmiar){

    cout << "Wczytywanie danych z pliku" << endl;

    int **tablica;
    tablica = new int*[rozmiar];

    for(int i = 0 ; i < rozmiar ; i++)
        tablica[i] = new int[rozmiar];

    cout << "Nacisnij ENTER, aby wczytac plik  " << nazwa;
    getchar();

    ifstream plik(nazwa.c_str());

    for(int i = 0 ; i < rozmiar ; i++){
        for(int j = 0 ; j < rozmiar ; j++)
            plik >> tablica[i][j];
    }

    plik.close();

    return tablica;
}

//LICZYMY wartosc funkcji F dla danej kratki
double ObliczF(int PozycjaX, int PozycjaY, int CelX, int CelY, int koszt){


    double F = sqrt(pow((PozycjaX - CelX), 2) + pow((PozycjaY - CelY), 2)) + koszt;

    return F;
}
