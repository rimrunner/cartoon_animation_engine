/*

Copyright Jari Sihvola 2019, unreleased

kääntäminen: g++ anim_engine.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -std=gnu++11 -o anim_engine

--

VAIHTUVAT X-ARVOT:
-waypointeja käsittelevät funktiot: suunnanlasku, hahmonpiirto
-Mihin x-arvot ja waypoint-arvot viittaavat?
-kamerax1:n ja kamrectin suhde pitäisi selvittää
-kamrect, on rect, jonka perusteella taustakuva renderoidaan
-eli kamrectin muuttuva arvo kertoo, mistä kohti X-akselia taustakuvaa aletaan rendata
-kamerax1 on taas se kamrectin muuttuva arvo itsessään
-eli vaikuttaa väärältä, jos näitä molempia arvoja manipuloidaan heti alussa. Pitäisi riittää, että kamerax1 on kalibroitu, kamrectia varmaan ei pitäisi muuttaa.
-Veikkaisin, että suunnanlasku-funktiossa on se syy, miksi hahmot ei liiku vasemmalle, kun skrollataan vasemmalle
-kamrect tosiaan asettaa ehdot sille, missä koordinaatit ovat
-joten, jos hahmo liikkuu näkyvän alueen vasemmalle puolelle, waypoint-koordinaatit ovat negatiivisia
--
-negatiivisen waypointin lasku: katso kuvasta suoraan koordinaatit ja vähennä kamerax1
--
-vaikka suunnanlaskun loppuun forcettaisi miinus-arvot, ilmeisesti lähinnä tuo pelkällä x-akselilla liikkuva kulkee miinusta
--
-nyt tarkkaillaan tilannetta pelkän näädän kanssa. vaikuttaa, että jo suunta-arvot ovat väärät, koska tulee positiivisia, vaikka pitäisi olla kai negatiivisia.
-Testattu, että suunta.firstin pitäisi suunnanlaskussa olla negatiivinen, jos liikutaan vasemmalle. Nyt se ei ole sitä.
--
uusi teoria:
-sijainti syötetään taustakuvan koordinaatteihin
-renderoitaessa vähennetään kamerax1
-waypoint syötetään taustakuvan koordinaatteihin
-tarvitseeko tällöin kameran vaikutusta huomioida ollenkaan?
-suunnanlaskun voi ainakin tehdä pelkästään kuvan koordinaattien perusteella
-sitten kun ruutu liikkuu...
-sijainti päivittyy automaattisesti, kun siitä vähennetään rendatessa se kamerax-1n

TESTAUS-ONGELMAT:
-tarkista, että tarinafunktiossa ensin if, sitten loput else iffejä
-kamrectin eka arvo ja kamerax1 pitää molemmat muuttaa

KOODATTAVAA:
-taukoa insert_kupiin
-sivulle laitettavaan puhekuplaan osoitin oikeaan kohtaan
-ratkaisu tuohon päällekkäisyyteen keittiöön siirryttäessä. esim. hahmoluokkaan muuttuja, joka muuttaa ruudulla arvoa waypointissa
-puhekuplien reunat jotenkin järkevästi
-puhekupla tarvitsee selkeästi ääriviivat

VIIMEKSI:
-Ilme laitettu
-Laitettu sopiva kuplanpituus yhden rivin kuplille
-Korjattu bugi, että kutsutaan autowrap, vaikkei olisi kuin yksi rivi
-Korjattu, että tarinakeskeytys menee päälle hahmonpiirrosta, kun jompi kumpi waypoint menee nollaan
-Esine laitettu
--
-C++:n castit kannattaisi kai olla muodossa static_cast<int>(expression), voisi loputkin vaihtaa. Niihin vaan pitää lisätä aina sulkeet. Ja pitää olla tarkka, että sulkeet menee juuri seuraavan luvun kohdalle, ei pidemmälle (seuraavaa lukua noissa c-casteissa kai aina muunnetaan). Ja tdb sanoi, että pärjäisi vähemmilläkin
--
-IRCistä puuttuu käyttäjän string kursoreineen ja palkeista merkit, värien hienosäätö, kanavalle joinatessa tulevat rivit
-Ehkä ennen tarinakeskeytykseen menemistä voisi aina odottaa, että joku yleis viivytysarvo odotellaan, mikäli sinne on laitettu jotain, niin ei tarvitse tehdä viivytyksistä omaa kohtaansa
-Typedefeillä voisi tehdä tarinafunktioon selkeämmän käyttöliittymän erityisesti puhekuplalle ehkä
-Ongelma tyhjän oikean laidan suhteen olikin tavutusongelma, joka on PC-lamereissakin, ei sitä ehkä tarvitse korjata, mutta voi tarvittaessa lievittää

SYYSKUU: LIIKKUMISEN ONGELMAT
-Waypoint1.firstin konvertoinnin sijaan hahmonpiirto()on ja suunnanlasku()un tehtiin nyt vain kamerax1 -muuttujan vähennykset sopiviin kohtiin
-Toisin kuin luulin, tällä kertaa ongelmana olikin tilanne, jossa molemmat waypointit tulevat maaliin. Tällaisessa tilanteessa aiheutti ylimääräisen väärän framenpiirron framesuunta(), mutta tuli lisättyä siihen myös, että framesuunta()a ei myöskään kutsuta, jos toinen waypoint tulee maaliin ja toinen on vain 5 maalista. Epävarmaa, oliko tämä hyvä lisäys.
-Piti palauttaa framesuunta() -tarkistus myös waypointtien yhteyteen, ei olisi pitänyt poistaa sitä.
-Toisen waypointin mennessä nollaan suunnanlasku antoi hölmöjä tuloksia ja hahmo alkoi sutimaan. Tämä on nyt korjattu (tangenttia ei tarvitse, kun toinen waypoint on 0). Aluksi korjauksessa sattui paha virhe, jolloin waypoint2 ei toiminut ollenkaan, mutta tämäkin korjattiin. Lisäksi hahmonpiirtoon piti lisätä kaksi uutta kutsua framesuunta()an, koska suunta voi muuttua oleellisesti, kun toinen waypoint menee nollaan. HUOM. Tilanteessa, jossa vain toinen waypoint on nolla, voisi suunnan kyllä ottaa hyvinkin yksinkertaisesti.
-Juuri nyt vaikuttaa, että jumitusbugi katosi, kun waypointtien perille pääsyn skuuppia laajennettiin 1 -> 2
-Löytyi uusi bugi: kääntyessä seuraaviin waypointteihin vilahti väärä frame. Tämä taisi korjaantua tekemällä erillinen framesuunta() suunnanlaskun lopun pohjalta ja kutsumalla sitä suunnanlaskun jälkeen sekä bugin korjaamiseksi myös waypointteja vaihdettaessa.
((Tämä taisi olla väärien oletusten vallassa kirjoitettu: -hahmot jäävät jumittamaan ilmeisesti, koska sijainti.first ja wayp1 lähentyvät. pitää selvittää mikä logiikka niissä on (esim. laita pointit ruudulle, jotka otetaan mainfunktiossa suoraan koira1:n arvoista). X-waypointtihan on nyt konvertoitu aina hahmonpiirron alussa. sen voi suht helposti palauttaa. Tarinafunktiossa annetut X-waypointit nyt siis viittaavat aina taustakuvan eikä ruudun pisteisiin.))
-Nyt suunta lasketaan aina (mutta suunnanmuutos() ei koske frameen, jos ollaan jo kävelyframeissa) ja hahmo pysähtyy, kun se on +-1 pikselin päässä waypointista.
-kävely pitäisi synkata skrollauksen kanssa siten, että vähennetään waypoint.firstistä aina skrollinopeus (pitää selvittää missä tahdissa ne kasvaa)
-Aikaisemmin oli pohdittu, että skroliter-iteraattorin looppiin myös, että hahmon paikkaa ei muuteta skrollatessa, jos hahmo liikkuu. Toisaalta vaikuttaisi, että jos tässä liikutellaan hahmoja vain skrollin suuntaisesti, ei tuo skrollaus + kävely näytä nyt kovin huonolta ilman mitään sopeutustakaan.


KESKEYTYS:
(-hahmo-luokkaan saisi helposti muuttujan, että tarkistetaan tarinakeskeytys liikkumisen loputtua)
(-puhekeskeytyksen saisi hoidettua ilmeisesti siten, että on erikseen muuttuja, joka laittaa keskeytyksen, jos puhevektori loppuu)
-mutta ehkä pitäisi tehdä se hahmoihin pointterit omaava vektori kuitenkin. siitä voi olla muutenkin hyötyä. saisi kätevästi nuo piirtokutsut ja sitten ei tarvitsisi tehdä tuota puhekeskeytystä ja liikkumiskeskeytystä erikseen
-IRC vaihe ei tarvitse erikseen keskeytyksiä. Siihen pitää laittaa joku systeemi, että se keskeyttää, kun vuorosanat loppuu
-keskeytystarkistuksen (joka käyttää hahmojen pointterivektoria ja tarkistaa, liikkuuko joku, onko yleistimeria laitettu ja onko puhetta menossa) voisi kutsua aina, kun liikkuminen loppuu, yleistimeri loppuu ja puhuminen loppuu)

POINTTERIVEKTORI-BUGI
-tämä bugi taisi kadota tuntemattomasta syystä (voisiko liittyä tuohon, että spritesheettien määrittelyt erotettiin omaan funktioonsa)
--
-pointteri hahmoluokka-olioon push_backataan konstruktorissa eikä int_mainissa, niin ongelma tulee
-valgrind tuntui pitävän syyllisenä *spritet -viittausta hahmonpiirto-funktiossa (cout edellistä riviä, missä piirtorect)
-piirtorect, spritet ovat kai ne ongelmalliset pointteri-muuttujat hahmoluokassa
-ratkaisuehdotus: Nämä int mainin rivit olisiko mahdollista siirtää aiempaan vaiheeseen? Vaatii ehkä ylimääräisten julistusten tekoa tms. voi olla, että näiden rivien esiintyminen int mainissa vaatii, että hahmoluokka-olion pointterikin on push_backattu sieltä
koira2.yhdista_tekstuuri(&koirateks2);
koira2.yhdista_rect(koira2_arkki, (sizeof(koira2_arkki)/sizeof(*koira2_arkki)), sizeof(koira2_arkki));
-toinen ehdotus: Voisikohan tämän ongelman kiertää myös siten, että push_back funktio ajetaan sekä konstruktorissa, että mainissa
-valgrind:
==1803== Invalid read of size 4
==1803==    at 0x804A472: hahmoluokka::hahmonpiirto(bool, int) (anim_engine.cpp:559)
==1803==    by 0x804F0AD: main (anim_engine.cpp:1542)
==1803==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
==1803== 
==1803== 
==1803== Process terminating with default action of signal 11 (SIGSEGV)
==1803==  Access not within mapped region at address 0x0
==1803==    at 0x804A472: hahmoluokka::hahmonpiirto(bool, int) (anim_engine.cpp:559)
==1803==    by 0x804F0AD: main (anim_engine.cpp:1542)
==1803==  If you believe this happened as a result of a stack
==1803==  overflow in your program's main thread (unlikely but
==1803==  possible), you can try to increase the size of the
==1803==  main thread stack using the --main-stacksize= flag.
==1803==  The main thread stack size used in this run was 8388608.


FRAMEKUVIEN ONGELMAT:
-oli framejen väliset x-erot epäsymmetriset arkissa ja yhdessä joku ruumiinosa doublepikselin väärässä kohti. Huom. Jos kuvassa on ylimääräisiä frame-settejä, vahingossa voi ruveta editoimaan niitä
-oli määritelty puhe-sheet liian pieneksi, niin se ei näkynyt sitten, kun sitä oli siirretty oikealle

PUHEKUPLA:
Mitkä jutut voi koodata nyt:
-Borderit (ylä- ja ala = 5, sivut = 15, kuplan hahmon pään yläpuolellenosto = 30) voisi ehkä tehdä consteiksi. Tarvitseeko oikean puolen border olla yhtä iso?
--
-Halutessa on helppo luoda "useita defaultteja" kuplanpituuteen, esim luvut 1, 2, 3... voi laittaa tarkoittamaan jotain muuta
-Kunhan hahmottuu, mitä kaikkea puhekuplaan pitää aina syöttää, voisi miettiä, miten ne saisi kaikki kätevästi yhdessä tarinafunktiosta
-Pitäisikö puhekuplan olla yleisesti ottaen leveämpi, kuten PC-lamereissa? Ehkä se ei sovi niin hyvin tähän grafiikkatyyliin, missä hahmot ovat pienempiä eikä ole zoomia
-Pitäisikö kuplan sijainnin mennä ylös, jos siellä on tilaa ja muussa tapauksessa alas? Tämä tuntuisi ihan hyvältä vaihtoehdolta. Jospa katsottaisiin ensin käytännön myötä, tarvitaanko sitä alempana olevaa kuplaa ollenkaan.
--
-pitäisikö puhekuplan piirrosta tehdä hahmoluokan funktio? ei ehkä, koska se ei tarvitse hahmosta muuta tietoa kuin sijainnin ...mutta sijaintia pitää heittää jatkuvasti? joten ehkä sen voisi yhdistää hahmoluokkaan. näin voisi toteuttaa myös helpommin sen, että kuplia voi olla useita samaan aikaan. vuorosanankin voisi sitten heittää sinne. lopuksi puhekupla sulkee itsensä.


RUUDUN KOKO JA ASPECT RATIO:
-Youtuben aspect ratio (16:9) on aika leveä, mutta siinä on varmaan ideana, että siinä on sivusuuntaan otettu pelivaraa ja se ilmeisesti ei häiritse, että monet videot näkyy kapeampina (pc -lamerit ei taida olla noin leveä kuin tuo aspect)
-Ruudun koko sinänsä on varmaan ihan OK, sitä voi zoomata tarvittaessa
-Edellinen ruudun koko oli ehkä turhan korkea ajatellen jotain sisälläolomaisemaa. Ehkä parempi viedä hiukan youtuben ratioon päin

PUHEEN ATTRIBUOINTI HAHMOLLE:
-yksi mahdollisuus on että on kuppointteri ja sitten hahmonpiirto-funktiossa kysytään, jos puhekupla on true, niin onko hahmon sijainti ja kuppointteri samat... no oikeestaan, eihän tämä olisi niin kuormittava
-toinen olisi ollut, että hahmoluokkaan erikseen puhe. ja puhekuplafunktio hahmoluokkaan. tällöin ei tarvitse sijaintipointteria. mutta sitten on ehkä hankalampi kutsua puhkuplafunktiota tarinafunktiosta käsin, kun pitää olla se hahmonnimi siinä kutsussa ja se kutsu olisi kuitenkin hyvä tehdä pääloopista käsin

IRC-NÄKYMÄ:
-Eri rivit alkuun, mitä tulee kanavalle joinatessa. Näitä varten pitää tehdä sitten uusia tekstuureja, koska värit on eri
*pääfunktioon: jos IRC -päällä
*musta tausta ensin - tulee nyt defaulttina
*wrap, joka automaattisesti katkoo rivit kirjaimen kohdalta (ei sanan) niin on realistisempaa
*vuorosanat voi syöttää tarinafunktiosta ja irc-funktio laittaa ne tekstuurivektoreiksi
*eli funktio, johon syötetään kellonaika, puhuja, vuorosana ja se lisää ne vektoriin
-pitää olla monospace-fontti kaiketi? näköjään xfce-terminaalissa on kaiketi defaultina fontti "monospace"
*vektorin elementtiä piirrettäessä lisätään nuo tummanharmaat < > -merkit, jotka ovat omat tekstuurinsa (ja alkurivien vaatimat värilliset merkit)
-käyttäjärivin on alettava oikeaan aikaan. Sen voisi varmaan tehdä niin, että se laitetaan vektoriin normaalisti, mutta vektorista sitä ei piirretä rivien sekaan vaan siihen käyttäjä-stringiin vähitellen (sen mukaan mitä on jo piirretty). ja ehkä sen tyypin voisi erikois-ajastimen mentyä normaaliksi, jolloin se piirtyy normaalina
-käyttäjärivin tunnistaa tyypistä (struct irc_rivi), sillä on ylimääräinen aika-arvo-muuttuja (tehdään structiin) joka kertoo, miten paljon kestää seuraavan merkin ilmaantumisessa (tähän voi halutessaan pistää, että sitä randomisti muutetaan)
-kirjoitusnopeuden ja stringin perusteella pystyisi ajastamaan käyttäjärivin etukäteen
-erivärinen teksti vaatii omat surfacensa. Vektorien määrä voisi olla vakio riippuen merkinkoosta (jos tulee tyhjiä rivejä (niitä ei kyllä kanavilla ole), voi jättää sen tyhjäksi). Ei tarvita muistiin y-akseleita vaan riittää merkinkorkeuteen perustuva systeemi.

TEKSTISKROLLI:
-kuplantilaan uusi kohta, joka on, että jos kupla skrollaa. Vai pitäisikö olla joku erillinen delimiter-muuttuja, jossa on kerrottu sekin, missä rivillä se raja on (0, jos kaikki mahtuu)
-kuplanluonnissa pitää ensin laskea, onko kuplankorkeus suurempi kuin rivit
-kun kursori saavuttaa viimeisen näkyvän rivin, skrollataankin tekstin puuttuva osa. sen nykyisen tekstiskrollausarvon rinnalle joku toinen arvo
-voisi laittaa sen rectin, joka on kai nyt koko ajan päällä, että se on päällä, jos delimiter on tms.

LIIKKUMISBUGI:
-voisiko epätarkkuus liikkumisessa liittyä siihen, että lasketaan irrationaaliluvuilla?
-HUOM. Yksi mahdollinen tapa tehdä liikkumisen laskeminen tarkemmaksi olisi, että pelkän plussaamisen sijaan laskettaisiin aina siinä kohti sijainti uudestaan
-toinen potentiaalinen tapa tarkentaa, olisi laskea suunta uudestaan tietyn etenemismäärän jälkeen (sen hetkinen sijainti ja waypoint)
-HUOM. Tuossa kohti, kun asetetaan toinen waypointti nollaan, voisi heittää funktiokutsun, joka asettaa framen (sijainnin?) oikein, mikäli tuota liikkumista ei muuten korjata tarkemmaksi
-Liikkuminen waypointtiin toimii, mutta siinä voi tulla tuommoista, että lopussa liikutaan yhteen suuntaan. Olisiko kyse pyöristysvirheistä suunnan hakemisessa?

vanhempia muistiinpanoja:
-laittaisko nyt sitten niin, että skrollausarvon osuessa kuplan ylälaitaan, seuraa aina keskeytys. Muuten vois laittaa, mutta ei ole päätetty, tuleeko tuota skrollausta yleensä lopulliseen engineen. Eli voihan tämän enginen lopun toteuttaa myöhemminkin.
-muissa tapauksissa keskeytyksen ehto voisi olla tarinafunktion jotenkin määrittelemä, tosin vähän epäselvää ehkä, miten tämä tehdään. Ainakin liikkumisissa voisi olla ehton määritelty helposti vaikka joka hahmoille erikseen liikkumiskeskeytys tarinafunktiosta käsin universaali-muuttujalla
-laittaisko siten, että keskeytykset aina lisää yhden desimaalin ja jos tarinafunktiosta ei löydy if-lausetta siihen lukuun, siirrytään seuraavaan kokonaislukuun ja kutsutaan se funktio uudestaan. Tällöin tarinafunktiosta pitäisi tehdä boolelainen ilmeisesti
-yhden puhekuplan ratkaisussa riittäisi pointterimuuttujat, joille laitetaan osoite hahmon sijainti-muuttujaan?

-ohjelmaa on helppo testata muuttamalla tarinanvaihe-muuttujan lähtöarvoa, niin se alkaa suoraan jostain tietystä kohdasta

--

//Tarinafunktio
//Tarinanvaihe kertoo, missä vaiheessa tarinaa ollaan menossa. Float, koska juoksevat numerot sopivat hyvin, jos haluaa lisätä lisää vaiheita jälkikäteen ja muutenkin pätkittää tehokkaasti
//Jos tarinakeskeytys on true, päälooppi käy hakemassa uusia tietoja tarinafunktiosta, jolloin uudet muuttujien arvot määräytyvät tarinanvaiheen mukaan. Lisäksi ilmeisesti määrittyy, milloin seuraava keskeytys tulee. Keskeytyksiä voi tehdä useamman tai esim liikkumisille omia, jos se sujuvoittaa asioita.
-jotain ehtoa sen ohjelman pitää kai koko ajan tarkistaa ja sitten se ehto itsessään on joku muuttuja, jota joku tarinafunktio muuttelee
-esim kun hahmon x pitää kävellä paikkaan y, päälooppi kysyy tarkistusfunktiosta, täyttyykö jokin keskeytys-ehto. näitä keskeytys-ehtoja voisiko olla muutama kerrallaan, niin saa kätevämmin hallittua, jos tapahtuu useampi juttu.
-tarinanhallinnasta käsin taas asetetaan, milloin ehdot täyttyvät
-pitäisikö resetoida arvot (esim. onko hahmo x näkyvissä) jollain funktiolla välillä vai pitääkö aina erikseen säätää hahmot pois näkyvistä, kun vaihtuu kohtaus? ehkä resetointi-funktion voisi tehdä, sitähän ei ole pakko kutsua jatkuvasti

tarinafunktion muokkaamat muuttujat kertovat:
-mikä taustakuva on päällä
-mitkä hahmot ovat päällä
-miten hahmoja liikutellaan
-onko puhekupla päällä ja mitä tekstiä siihen tulee
-mikä musiikki soi

hahmojen liikutusfunktio:
-framet ja suunta
-liikkuessa pitää aina myös vaihtaa framea (framen vaihtonopeus varmaan sidotaan yleisempään liikkumisnopeuteen mutta ne voi erottaa)
-tarvitaanko hahmoluokkaan myös muuttuja, että missä waypointissa ollaan menossa? ehkä ei, koska jos esim ykkönen muutetaan nollaksi, kun sinne ollaan päästy, niin sitä ei sitten enää käytetään
-tämä pitää ehkä ajastaa nopeuden säätämiseksi. vai pitääkö laittaa hahmoluokkaan ne nopeudet
-funktio joka tarkistaa, onko hahmoilla liikutus päällä ja jos on, mihin suuntaan ja muuttaa sitten sen mukaan arvoa (ottaen huomioon mahdollinen nopeus) ja tarkistaa myös, onko päästy jo kohteeseen ja laittaa sitten liikutuksen pois tai ohjaa seuraavaan waypointtiin.
-pitää laskea (erillisellä funktiolla ehkä), mitä spritejä käytetään riippuen liikkumisen suunnasta

pääloopin kysymykset:
mikä on taustakuva (yksi vaihtoehto: musta ruutu)
vaihtuuko musiikki
skrollataanko taustaa
ollaanko kuvaa feidaamassa pois
ollaanko puhekuplaa luomassa
onko puhekupla aktiivinen
onko kursori aktiivinen - tämä on pääloopissa puhekuplan if-rakenteen alla
piirretäänkö hahmoja (ja mihin)
liikutetaanko hahmoja
puhuuko joku hahmo - tämä tapahtuu samaan aikaan kursorin kanssa.

OSOITIN:
Ratkaisut:
-osoittimen vasen-oikea suunnan saa muutettua, kun for-sulkeiden laskurin laittaa dekrementoimaan ja jatkoehdoksi > -20
-alaspäin avautuu, jos laittaa koko y7 ja y8 kaavan negaatioksi JA vaihtaa laskuri3:n ja 2:n paikkaa keskenään loopin lopussa (eli kummasta dekrementoidaan 1 ja kummasta 2). Tämä alaspäin on käytössä kaiketi myös sivulle päin tehtäessä.
-sivukupla: vaihdetaan X ja Y viivanpiirtokäskyssä
kupasema: ylä & ala, sivuk, sivuy, sivua
http://www.protutorcompany.com/how-do-i-know-which-direction-the-parabola-opens/
Mitkä on kuplan eri tilanteet?
yläkupla vasen  
yläkupla oikea
alakupla vasen  alakuplaan kai kuplan arvot parempia kuin kuppointer?
alakupla oikea
sivukupla ylä
sivukupla keski
sivukupla ala

MUUTA
-vektorilla, jossa on pointtereita olioihin voisi kutsua esim funktiota kaikista olioista, joilla on muuttuja x arvossa y. Eli voisi liikkumisfunktiota kutsua hahmoista, jotka liikkuu sen sijaan, että laittaa ne kaikki hahmo-objektien nimet siihen listaksi

*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <unistd.h>

using namespace std;

const int SCREEN_WIDTH = 680;
const int SCREEN_HEIGHT = 480;
const int KNOPEUS = 3; //Kävelynopeuden kerroin //oli 3
int kamerax1 = 530; //1280, 530
int kamerax2 = SCREEN_WIDTH;
int kamskrolli = 0; //0 = kameran liikutus pois päältä, 1 = vasemmalle, 2 = oikealle
SDL_Rect kamrect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; //530 ekaan
//DEBUG  SDL_Rect kamrect = {1200, 0, SCREEN_WIDTH+1200, SCREEN_HEIGHT};
int skrollihalt;  //Mihin skrollaus pysähtyy. Maksimiarvo tälle on kuvan koko X-akselilla miinus SCREEN_WIDTH (eli 1280 tässä produktiossa, jossa 1960 on taustakuvan X-koko)

//Tarinanvaihe kertoo, missä vaiheessa tarinaa ollaan menossa.
//Jos tarinakeskeytys on true, päälooppi käy hakemassa uusia tietoja tarinafunktiosta, jolloin uudet muuttujien arvot määräytyvät tarinanvaiheen mukaan.
int tarinanvaihe = 3; //Arvo oltava pienempi kuin ensimmäinen koodissa käytettävä arvo
bool tarinakeskeytys = true;
int tarina_t = 0; //Tarinafunktion yleistimer, montako sekunttia odotellaan (muunnetaan millisekunneiksi)
int tarina_kt = 0; //Kuplanluonnista asetettava timer-arvo, joka siirtää arvonsa tarina_t:lle
int ruokaa = 0; //0 = off, 1 = on, 2 = stop;
bool drop = false;
bool kesktark();

SDL_Rect kissa_arkki[20];
SDL_Rect kissak_arkki[20];
SDL_Rect koira2_arkki[19];
SDL_Rect koira1_arkki[21];
SDL_Rect naata_arkki[18];
SDL_Rect mayra_arkki[28];
SDL_Rect drops_arkki[16];
SDL_Rect piirtorect;

//Oikeat arvot merkinpituudelle- ja korkeudelle saa, kun katsoo coutilla, mitä tekstirend-funktion surfacen antamat w & h -arvot ovat. Korkeus tulee suoraan, merkinleveyden saa jakamalla jotain riviä kuvaavan surfacen leveyden jonkun rivin merkkien määrällä
const int merkinpituus = 9;
const int merkinkorkeus = 18;
const int RIVIVALI = 0;
//puhekuplan muuttujat
string vuorosana1;
vector<string> vuorosanavec;
int kuplantila = 0; //onko puhekupla päällä (0 = ei kuplaa, 1 = alustusvaihe, 2 = puhekupla on päällä, 3 = tekstiskrolli, 4 = perään toinen kupla, 5 = kuplan poistaminen, 6 = tauko kuplan jälkeen)
double ylirivit = 0;
struct kupla {
  int timer = 0;
  int kuka = 0;
  int pituus = 0;
  int korkeus = 0;
  string puhe = "";
  int asemointi = 1; //kupla voi olla ylä- (1), alapuolella (2) hahmoa tai hahmon yläsivulla (3), sivulla keskellä (4) tai alasivulla (5)
  int vilme = 1;
};
vector<kupla> kuplavec; //Vektori, että voi laittaa kerralla perättäisiä puhekuplia
int bubblew; //kuplan vasen laita
int bubbleh; //kuplan ylälaita
int bubblew2; //oli 360, tämä on siis kuplan leveys (ml. borderit). Kuplanpituus myöhemmin tulee ilmeisesti tähän kohtaan määriteltäväksi, kunhan autowrapistä tehdään oma funktionsa
int bubbleh2; //kuplan korkeus ylälaidasta laskien
int kupasema;
int puhilme = 1; //puhujan ilme
const int OLETUSKUPLANPITUUS = 360;
const int OLETUSKUPLANKORKEUS = 140;
const int KUPYLABORDER = 5;
const int KUPALABORDER = 5;
const int KUPVASBORDER = 15;
const int KUPOIKBORDER = 15;
//Puhekuplassa pituus on primäärinen, jonka perusteella katkotaan vuorosanan rivit ja sitten korkeus voidaan 0-arvolla sopeuttaa automaattisesti oikeaksi pituuden ja rivien avulla
//Tarinafunktiossa säädettävät kuplan pituus ja korkeus
int kuplanpituus = 0; //0 = OLETUSKUPLANPITUUS, muut positiiviset arvot on x-akselin arvoja
int kuplankorkeus = 0; //0 = säädetään tekstin pituuden mukaan, 1= laitetaan OLETUSKUPLANKORKEUS, muut positiiviset arvot viittaa y-koordinaatteihin
//Muut väliaikaisemmat kuplanpituus ja korkeus
int tempkuppituus = 0;
int tempkupkorkeus = 0;
int kuplavo = 0; //1=kupla vasemmalle hahmosta, 2=kupla oikealle, 0=sijainnin mukaan (jos hahmo kuvaruudun vasemmalla puoliskolla, kupla oikealle ja toisin päin)
SDL_Rect kupla1; //kuplan leveämpi pala
SDL_Rect kupla2; //kuplan korkeampi pala
SDL_Rect cliprect; //Neliö, jolla peitetään alueet, joihin rendataan, kun muualle ei. Käytetään ainakin puhekuplan tekstiskrollissa, että teksti katoaisi kuplan ylälaidassa
pair<double, double> *kuppoint; //pointteri hahmon sijaintiin, josta kuplan koordinaatit otetaan

//Tekstin rendauksessa ja järjestelyssä käytettyjä muuttujia
TTF_Font* fontti;
SDL_Surface* pinta2;
SDL_Texture* tekstuuri2;
//int tekstinleveys;
//int tekstinkorkeus;
int rivinpituus;

//Muuttujat, joilla puhekuplan kursoria ohjaillaan (nämä ovat default-arvot, joita ei saa tässä vaiheessa muuttaa). Hmmm... tarvitsisivatkohan nämä arvot jonkun resetointi-funktion
pair<int, int> mikarivi; //Ensimmäinen luku on millä kuplan rivillä kursori menee ja toinen on, millä vuorosanavektorin rivillä kursori menee. Nämä voivat erkaantua tekstiä skrollatessa.
int moneskirjain = 0;
int kursoripaalla = 0; //0 = ei kursoria, 1 = kursori juoksee, 2 = kursori odottaa, että skrollataan puhekuplantekstiä ylemmäs, 3 = kursori maalissa
int kursorikountteri = 0;
int pkupkounter = 0;
SDL_Color vaeri = {0, 0, 0}; //tekstinväri

//Tekstin skrollauksen muuttujat
bool tekstiskrolli = false; //onko skrollaus menossa vai ei
int skrollausarvo = 0; //jotkut funktiot lisää skrollausarvon tekstiä sijoittaessaan. jos se on muuta kuin nolla, teksti skrollaa

//Hahmot
bool hahmoja = false;
bool init();
bool lataamedia();
void sammutus();

bool IRC = false;
SDL_Color ircvari = {255, 255, 255};
SDL_Color joinvari = {255, 255, 255};
SDL_Color nimilista = {255, 255, 255};
struct irc_rivi {
  SDL_Texture *aikaleima;
  SDL_Texture *nikki;
  SDL_Texture *sanoma;
  int npituus;
  int vpituus;
  int ajastin = 0;
  string tyyppi = "normaali"; //muut: nimilista, join, kayttaja
};
irc_rivi generic_irc_rivi;
vector<irc_rivi> irc_teksturet;  //En tiedä, pitääkö tuon olla pointteri, ei käänny ilman
string irc_aika;
const int irc_MERKINPITUUS = 9;
const int irc_MERKINKORKEUS = 18;
const int irc_RIVIVALI = 0;
TTF_Font* irc_fontti;
int aika_leveys;
int as_leveys;
int ks_leveys;
SDL_Texture* aika1;
SDL_Texture* aika2;
SDL_Texture* aika3;

//Pääikkuna
SDL_Window* ikkuna = NULL;
//Ikkunan renderöijä-olio
SDL_Renderer* piirturi = NULL;

class tekstuuriluokka {
  int leveys;
  int korkeus;
public:
  SDL_Texture* tekstuuri;  //Tämä on itse hardwaretekstuuri
  tekstuuriluokka(); //Muuttujien initialisointi
  ~tekstuuriluokka(); //De-allokoidaan muisti
  bool tiedostonlataus(string filename);
  void erase(); //tekstuurin de-allokointi
  void renderfunktio(int x, int y, SDL_Rect* temprect = NULL, bool flip = false); //Renderöi tekstuurin annettuun kohtaan
  int haeleveys();
  int haekorkeus();
  void aseta_blend(SDL_BlendMode blend_arvo);
  void alphafunktio(Uint8 alpha_arvo);
};

tekstuuriluokka taustakuva; //tämä on muuttuva luokka
tekstuuriluokka pimee;
tekstuuriluokka koti;
tekstuuriluokka wc;
//
tekstuuriluokka koirateks1;
tekstuuriluokka koirateks2;
tekstuuriluokka mayrateks;
tekstuuriluokka kissateks;
tekstuuriluokka kissakteks;
tekstuuriluokka naatateks;
tekstuuriluokka dropsteks;
//
tekstuuriluokka poytateks;
tekstuuriluokka kpoytateks;
tekstuuriluokka rvteks;
tekstuuriluokka tuoliteks;
tekstuuriluokka ruokateks;

//Constructor on datatyypitön ja luokan kanssa samanniminen funktio, joka ajetaan aina, kun luokasta luodaan luodaan olio. Voi olla parametrejä.
tekstuuriluokka::tekstuuriluokka() {
  tekstuuri = NULL;
  leveys = 0;
  korkeus = 0;
}

//Destructorin nimi on tilde + luokannimi, ajetaan, jos olio menee ulos scoopistaan tai olioon osoittava pointteri deletoidaan. 
tekstuuriluokka::~tekstuuriluokka() {
  erase();
}

bool tekstuuriluokka::tiedostonlataus(string filename) {
  erase(); //olemassa oleva tekstuuri pois
  SDL_Texture* uusitekstuuri = NULL;
  SDL_Surface* pinta = IMG_Load(filename.c_str());
  if(pinta == NULL) {
    cerr << "Image load fail: " << filename.c_str() << " " << IMG_GetError() << endl;
  }
  else {
    SDL_SetColorKey(pinta, SDL_TRUE, SDL_MapRGB(pinta->format, 0, 0xFF, 0xFF));
    //Tässä kuva colorkeyätään, ennen kuin siitä luodaan tekstuuri. Ensimmäinen argumentti on, mikä surface colorkeyätään, seuraava argumentti laittaa colorkeyingin päälle, kolmas argumentti on pikseli, joka halutaan colorkeyätä. Tähän laitetaan sitten toinen funktio, jossa ensimmäinen argumentti on formaatti pikseleille (käytetyssä oliossa on formaatin kertova jäsenmuuttuja), seuraavat muuttujat ovat väri, tässä tapauksessa syaani.

    //Luodaan surfacen pikseleistä tekstuuri
    uusitekstuuri = SDL_CreateTextureFromSurface(piirturi, pinta);
    if(uusitekstuuri == NULL) {
      cerr << "Texture creation fail - " << SDL_GetError << endl;
    }
    else {
      //tekstuurin leveys ja korkeus talteen
      leveys = pinta->w;
      korkeus = pinta->h;
    }
    //Eroon ladatusta surfacesta
    SDL_FreeSurface(pinta);
  }
  tekstuuri = uusitekstuuri;

  return tekstuuri != NULL;
}

void tekstuuriluokka::erase() {
  if(tekstuuri != NULL) {
    SDL_DestroyTexture(tekstuuri);
    tekstuuri = NULL;
    leveys = 0;
    korkeus = 0;
  }
}

//Full-screen imagejen kanssa riitti SDL_RenderCopy ja kaksi viimmeistä argumenttia NULLiksi, mutta nyt tiettyyn paikkaan renderöitäessä tarvitaan alkuperäisen kuvan dimensiot leveyden ja korkeuden spesifioimiseen eli luodaan ensin tuommoinen neliskulmio, johon luokasta leveys ja korkeus ja se sitten annetaan RenderCopyyn
void tekstuuriluokka::renderfunktio(int x, int y, SDL_Rect* temprect, bool flip) {
  SDL_Rect rendnelio = {x,y,leveys,korkeus};
  if(temprect != NULL) {
    rendnelio.w = temprect->w;
    rendnelio.h = temprect->h;
  }
  SDL_RenderCopyEx(piirturi, tekstuuri, temprect, &rendnelio, 0.0, NULL, (flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
  //  SDL_RenderCopy(piirturi, tekstuuri, temprect, &rendnelio);
}

void tekstuuriluokka::aseta_blend(SDL_BlendMode blend_arvo) {
  SDL_SetTextureBlendMode(tekstuuri, blend_arvo);
}

void tekstuuriluokka::alphafunktio(Uint8 alpha_arvo) {
  SDL_SetTextureAlphaMod(tekstuuri, alpha_arvo);
}

int tekstuuriluokka::haeleveys() {return leveys;}
int tekstuuriluokka::haekorkeus() {return korkeus;}


/*
Suunnat
789
456
123
5 = ei liiku
8.0 on suoraan ylös, 9 on suoraan oikeaan yläviistoon, 8.1-8.9 on siinä välissä olevat, 9.1-9.9 on 9:n ja 6:n välissä olevat
Mitä kuvakulmaa käytetään riippuu kokonaisluvusta, paitsi että 7.0 on eri kuin >7, 3.0 on eri kuin >3, 1.0 on eri kuin >1
-suunta lasketaan lähtöpisteen ja waypointin mukaan eikä siis ole suoraan aktuaalinen suunta vaan on pyöristettynä ohjeena

seuraavaksi hahmonpiirtoon tuo, että suunnasta katsotaan frame.

...hetkinen, vai pitäiskö suunta katsoa sittenkin tarkkaan waypointista. koska pitäähän se tarkka suunta olla, että osataan muuttaa sijaintia oikein

eli on ne koordinaatit, esim
120, 140
sitten waypoint 230, 170
katsotaan etäisyys molempiin arvoihin (tässä +110, +30)

eikös noiden w:n ja h:n muutoksen suhde kerro, miten paljon aina muutetaan arvoa

näin näyttäisi olevan. mutta tarvitaan erikseen X:lle ja Y:lle arvot tai ainakin etumerkit vaihtelee
110 / 30 = 3.667

eli toiseen lisätään sitten 0.3677 ja toiseen 1-0.3677

pitäisikö liikkuminenkin olla pair?
se voisi olla paras ratkaisu, doubleja

tarvitaanko sitten erikseen suuntaa? tai siis ennemminkin vain muuttuja, että mitä frame-sarjaa käytetään
*/


class hahmoluokka {
public:
  int ruudulla; //Onko hahmo ruudulla ja monennessako hahmonpiirtokerroksessa
  bool symmetrinen;
  bool flip;
  bool steadyflip; //Flip päällä, kun hahmo on paikallaan
  pair<double, double> sijainti;  //Viitataan sijainti.first, sijainti.second
  pair<int,int> waypoint1;
  pair<int,int> waypoint2;
  pair<int,int> waypoint3;
  pair<int,int> waypoint4;
  int wp1muutos;  //Muuttuuko ruudulla-arvo waypointissa (0 = ei muutu, muut arvot viittavat uuteen ruudulla-arvoon)
  int wp2muutos;
  int wp3muutos;
  int wp4muutos;
  pair<double, double> liikesuunta;
  int rintamasuunta; //Kun hahmo on pysähdyksissä
  int frame;
  tekstuuriluokka *spritet;
  SDL_Rect *hahmorect;
  int ilme; //vai string
  hahmoluokka();
  ~hahmoluokka();
  void hahmonpiirto(bool, int);
  void yhdista_tekstuuri(tekstuuriluokka*);
  void yhdista_rect(SDL_Rect *rect, int, unsigned int);
  pair<double,double> suunnanlasku();
  void arkit(); //Asettaa spritearkkien koordinaatit
  void framesuunta();
};

vector<hahmoluokka*> hahmovektori; //Vektori, johon liitetään hahmoluokan jäseniin osoittavat pointterit

void hahmoluokka::yhdista_tekstuuri(tekstuuriluokka *olio) {spritet = olio;}

hahmoluokka::hahmoluokka():
  ruudulla(0), symmetrinen(true), flip(false), steadyflip(false), rintamasuunta(2), frame(3), spritet(NULL), ilme(1), wp1muutos(0), wp2muutos(0), wp3muutos(0), wp4muutos(0) { //Pair-muuttujat puuttuvat tästä, niihin tulisi vain nollaa
  hahmovektori.push_back(this);
}

hahmoluokka::~hahmoluokka() {
  delete[] hahmorect;
  //Tekstuurin tuhoaminen tähän ...tosin se erase() on tekstuuriluokassa
}

//Hahmon rect yhdistetään sen spritesheettiin
void hahmoluokka::yhdista_rect(SDL_Rect *rect, int arraynpituus, unsigned int arraynkoko) {  //Funktioon lähetetään array, jolloin sinne lähtee arrayn ensimmäiseen elementtiin osoittava pointteri ja näin ollen parametrin täytyy olla pointteri-tyyppinen
  hahmorect = new SDL_Rect[arraynkoko]; //Dynaamisesti allokoituvaan arrayseen muistit
  for(int counter = 0; counter != arraynpituus; counter++) { //Assignataan sisältö
    hahmorect[counter] = rect[counter];
  }
}

//HUOM! Tähän tulee kaiketi myös liikuutus
//xcv
void hahmoluokka::hahmonpiirto(bool kavelynyt, int puheajastin) {
  //drops
  if(ilme == 4) {
    piirtorect = *(hahmorect + frame);  //Viittaus pointterin läpi arrayn indeksiin (hahmorect tässä on itse pointteri ensimmäiseen elementtiinsä, johtuisiko siitä, että se on * -operaattorin argumentti)
    spritet->renderfunktio(sijainti.first-kamerax1, sijainti.second, &piirtorect, flip);
    return;
  }

  bool tarkistus = false; //Tarkistus on vain sen takia, ettei turhaan renkata keskeytys-funktiota kokoajan

  if(frame >= 18) rintamasuunta = 0;
  else if(rintamasuunta == 0 and frame < 18) {
    rintamasuunta = 2;
  }
  
  if(waypoint1.first != 0 and waypoint1.second != 0) {
    liikesuunta = suunnanlasku();  //Ilmeisesti siinä tulee se akkumulaatiovirhe, jos lisäilee niitä doubleja päällekäin, joten tämä pitää kutsua täällä
    //Hahmon framet synkkaan kävelysuunnan kanssa, jos ei olla kävelyframeissa
    if(frame < 3 or frame > 14) framesuunta();
  }

  //Kun toinen waypoint tulee maaliin, mutta toinen waypoint on vielä kaukana, kutsutaan framesuunta(). Tämä voi tapahtua, jos toinen waypoint on jo lähtökohtaisesti hyvin lähellä maalia. Mutta jos toinenkin waypoint on nolla tai jos toinenkin on hyvin lähellä maalia (+-5), silloin ei kutsuta. On tosin epäselvää, tarvitaanko tuota hyvin lähellä juttua ollenkaan vai riittäisikö, että se ei ole nolla.
  if(waypoint1.first != 0) { 
    if(sijainti.first > waypoint1.first-2 and sijainti.first < waypoint1.first+2) {
      waypoint1.first = 0;
      tarkistus = true;
      if(waypoint1.second != 0 and (waypoint1.second - sijainti.second > 5 or waypoint1.second - sijainti.second < -5)) framesuunta();
    } 
    else if(liikesuunta.first > 0 and (sijainti.first < waypoint1.first)) sijainti.first += liikesuunta.first;
    else if(liikesuunta.first < 0 and (sijainti.first > waypoint1.first)) sijainti.first += liikesuunta.first; //Oleellinen rivi
    else {cout << "liikesuunta.first, sijainti.first, waypoint1.first, waypoint1.first-kamerax1" << " " << liikesuunta.first << " " << sijainti.first << " " << waypoint1.first << " " << waypoint1.first << endl; waypoint1.first = 0;} //Tänne todellakin mennään
  }
  if(waypoint1.second != 0) {
    if(sijainti.second > waypoint1.second-2 and sijainti.second < waypoint1.second+2) {
      if(waypoint1.first != 0 and (waypoint1.first - sijainti.first > 5 or waypoint1.first - sijainti.first < -5)) framesuunta();
      waypoint1.second = 0;
      tarkistus = true;
    }
    else if(liikesuunta.second > 0 and (sijainti.second < waypoint1.second)) sijainti.second += liikesuunta.second;
    else if(liikesuunta.second < 0 and (sijainti.second > waypoint1.second)) sijainti.second += liikesuunta.second;
    else waypoint1.second = 0;
  }
  //    cout << setprecision(17) << "sijainti: " << sijainti.first << " " << sijainti.second << endl;

  if(kavelynyt == true and (waypoint1.first != 0 or waypoint1.second != 0)) {
    //cout << "FRAMEFRAMEFRAME: " << frame << endl;
    frame++;
    // cout << "FRAMEFRAMEFRAME2: " << frame << endl;
    if(frame == 7 or frame == 11 or frame == 15) frame -= 4;
    //  cout << "FRAMEFRAMEFRAME3: " << frame << endl;
    //0,1|2345|6789|10,11,12,13|
  }
  if(waypoint1.first == 0 and waypoint1.second == 0 and waypoint2.first == 0 and waypoint2.second == 0) { //Ilman waypoint2:ia välähtää kääntyessä seisomisframe
    switch(rintamasuunta) {
    case 0: break;
    case 2: frame = 0; break;
    case 8: frame = 2; break;
    case 6: frame = 1; break;
    case 4: {
      if(!symmetrinen) frame = 15; //HUOM!!!! TÄMÄ PITÄÄ MUUTTAA
      else {
	frame = 1;
	flip = true;
      }
      break;
    }
    }
  }
  //Seuraava rivi aiheuttaa segfaultin, jos viitataan konstruktorista laitetun vektorin kautta
  piirtorect = *(hahmorect + frame);  //Viittaus pointterin läpi arrayn indeksiin (hahmorect tässä on itse pointteri ensimmäiseen elementtiinsä, johtuisiko siitä, että se on * -operaattorin argumentti)

  //Renderfunktiokutsun molemmin puolin on flip-tarkistukset. Riippuu rintamasuunnasta, johon hahmo jää, pitääkö tarkistus tehdä ennen vain jälkeen renderfunktion.
  if(tarkistus and rintamasuunta == 2 and waypoint1.first == 0 and waypoint1.second == 0 and waypoint2.first == 0 and waypoint2.second == 0) flip = false; //Ilman tätä riviä hahmoon voi jäädä flip-päälle, kun se pysähtyy.
  spritet->renderfunktio(sijainti.first-kamerax1, sijainti.second, &piirtorect, flip);

  //Huom. seuraava kohta oli aiemmin yllä olevan renderfunktio-kutsun yläpuolella, mutta siitä tuli ylimääräinen väärin flipattu frame
  if(tarkistus and waypoint1.first == 0 and waypoint1.second == 0 and waypoint2.first == 0 and waypoint2.second == 0) flip = false;
  if(steadyflip == true) flip = true;

  
  //Puhe katsotaan sen perusteella, onko kuplan kuppointit säädetty tälle hahmolle
  //Toiseksi viimeinen ehto sulkee pois selinkävelyframet ja viimeinen selinpaikallaanframen
  //Rintamasuuntaa ei voi käyttää ehtona seuraavassa, koska hahmo voi kävellä ihan eri suuntaan, kuin mitä se pysähtymiseen viittaava rintamasuunta on
  //Puheen skrollin aikana saisi helposti pois, jos halutaan
  if(kuplantila == 2 and puheajastin%10 > 5 and kuppoint->first == sijainti.first and kuppoint->second == sijainti.second and (static_cast<double>(frame-2) / 12) * 4 != 3 and frame != 2) {
    //1: 3-6 sivukävely, 2: 7-10 alaskävely, 3: 11-14 ylöskävely (-2 niin tulee 1-12 asteikko)
    //logiikka: (positio/koko rangen koko)*rangen välimatka
    //    if((double(frame-2) / 12) * 4 == 1) piirtorect = *(hahmorect + 16); //Flip on jo valmiiksi päälle laitettu tässä funktiossa, jos sitä tarvitaan
    //    else if ((double(frame-2) / 12) * 4 == 2) piirtorect = *(hahmorect + 15);
    if(frame >= 3 and frame <= 6) piirtorect = *(hahmorect + 16); //Flip on jo valmiiksi päälle laitettu tässä funktiossa, jos sitä tarvitaan
    else if (frame >= 7 and frame <= 10) piirtorect = *(hahmorect + 15);
    else if (frame == 0) piirtorect = *(hahmorect + 17); //Suoraanseisomisframe
    else if (frame == 1) piirtorect = *(hahmorect + 16); //Sivulleseisomisframe
    
    spritet->renderfunktio(sijainti.first-kamerax1, sijainti.second, &piirtorect, flip);
  }

  if(ilme > 1 or puhilme > 1) {
    if(frame == 0 or (frame >= 7 and frame <= 10) and (ilme == 2 or puhilme == 2)) piirtorect = *(hahmorect + 18);
    if(frame == 1 or (frame >= 3 and frame <= 6) and (ilme == 2 or puhilme == 2)) piirtorect = *(hahmorect + 20);
    if(frame == 0 and (ilme == 3 or puhilme == 3)) piirtorect = *(hahmorect + 19);
    spritet->renderfunktio(sijainti.first-kamerax1, sijainti.second, &piirtorect, flip);
  }

  //Päivitetään waypoint, jos sinne saavutaan
  if(waypoint1.first == 0 and waypoint1.second == 0 and (waypoint2.first != 0 or waypoint2.second != 0)) { //jos ei liikuta, mutta waypoint2:ssa on jotain
    //  if(round(sijainti.first) == waypoint1.first and round(sijainti.second) == waypoint1.second) {  //JOS tämä ei riitä voi aikaisemmin laittaa suunnan perusteella semmoisen, että yksittäistä sijaintia (first tai second) ei enää muuteta, jos se on sama tai yli kuin vastaava waypoint)

    if(wp1muutos != 0 or wp2muutos != 0 or wp3muutos != 0 or wp4muutos != 0) {    //Päivitetään ruudulla-arvo (hahmon näkyvyyskerros), jos se on säädettyä päivittyväksi
      if(wp1muutos != 0) ruudulla = wp1muutos;
      wp1muutos = wp2muutos;
      wp2muutos = wp3muutos;
      wp3muutos = wp4muutos;
      wp4muutos = 0;
    }
    
    waypoint1.first = waypoint2.first;
    waypoint1.second = waypoint2.second;
    waypoint2.first = waypoint3.first;
    waypoint2.second = waypoint3.second;
    waypoint3.first = waypoint4.first;
    waypoint3.second = waypoint4.second;
    waypoint4.first = 0;
    waypoint4.second = 0;
    liikesuunta = suunnanlasku();
    framesuunta();
  }

  if(tarkistus == true and waypoint1.first == 0 and waypoint1.second == 0 and kesktark()) tarinakeskeytys = true;
}

//Funktio joka laskee hahmon kävelysuunnan perusteella siihen sopivat kävelyframet. HUOM!!! frame = 15 taitaa olla vasta oletus
void hahmoluokka::framesuunta() {
  cout << "framesuunta, liikesuunta.first & liikesuunta.second: " << liikesuunta.first << ", " << liikesuunta.second << endl;
  if(liikesuunta.second < 0) { //jos mennään ylös
    if(liikesuunta.second <= -0.55) frame = 11;  //Kun pienentää tuon 0.4:n itseisarvoa (ja sama tuossa alempana), laajenee se ala, että käytetään ylös/alas -meno frameja
    else {
      frame = 3;
      if(liikesuunta.first < 0) symmetrinen ? flip = true : frame = 15;
    }
  }
  else { //jos mennään alas
    if(liikesuunta.second >= 0.55) frame = 7;
    else { //DEBUG: JOS KONSTRUKTORISSA ARVO EI OLE VALMIIKSI 3, SE MENEE SITTEN TÄNNE
      frame = 3;
      if(liikesuunta.first < 0) symmetrinen ? flip = true : frame = 15;
    }
  }
  cout << "FRAMESUUNTAFRAME: " << frame << endl;
}

pair<double,double> hahmoluokka::suunnanlasku() {

  pair<double,double> suunta;

  //Jos kävellään sivulle ja vasemmalle, niin flippaus päälle
  //Tässä on sopiva hetki nollata flippaus. Ainakin puhe-framejen kanssa voi tulla ongelmia, jos flippaus jää päälle turhaan.
  (frame >= 3 and frame <= 6 and waypoint1.first < sijainti.first) ? flip = true : flip = false;  //HUOM.! Tähän pitää lisätä myös ne epäsymmetriset frame

  //Ensin tarvitaan suuntakulman selvittämiseksi molempien kolmion kateettien pituudet, ne ovat x:n ja y:n muutosten itseisarvot. Ne saadaan kun pienempi x:n ja y:n arvo (lähtöpaikka ja päämäärä) vähennetään aina suuremmasta.

  int xsuunta = 1;
  int ysuunta = 1;

  if(waypoint1.first < sijainti.first) suunta.first *= -1;
  if(waypoint1.second < sijainti.second) suunta.second *= -1;
  
  //Funktion loppu ei toimi, jos vain toinen waypoint on 0. Tällöin ei tarvitse muutenkaan laskea
  if(waypoint1.first == 0 and waypoint1.second != 0) {
    suunta.first = 0;
    suunta.second = KNOPEUS;
    if(waypoint1.second < sijainti.second) suunta.second *= -1;
    if(frame < 3 or frame > 14) framesuunta();
    return suunta;
  }
  if(waypoint1.first != 0 and waypoint1.second == 0) {
    suunta.first = KNOPEUS;
    suunta.second = 0;
    if(waypoint1.first < sijainti.first) suunta.first *= -1;
    if(frame < 3 or frame > 14) framesuunta();
    return suunta;
  }

  double x_itmuutos = (sijainti.first > waypoint1.first ? sijainti.first - waypoint1.first : waypoint1.first - sijainti.first);
  double y_itmuutos = (sijainti.second > waypoint1.second ? sijainti.second - waypoint1.second : waypoint1.second - sijainti.second);
  //Saadut arvot ovat siis aina positiiviset
  //suuntaluku = kulma / 90 ja jos tulos on > 0.5, se tulee sille arvolle, joka muuttuu enemmän (ja toiselle 1 - suuntaluku)
  double suuntaluku = (atan(y_itmuutos/x_itmuutos)); //Y-arvo on aina vastainen kateetti ja X-arvo viereinen kateetti. atan-funktio = tan^-1
  //Suuntaluku on kulman asteet/radiaanit. Tämä pitää jakaa suorakulmalla, jotta saadaan, kuinka monta prosenttia haluttu muutos on eli asteet/radiaanit tässä muunnetaan koordinaatiston kielelle, joka on 0-1 eikä esim 0-90. Toisen muutoksen prosentit saadaan tietysti vähentämällä se 1:stä.
  suuntaluku /= M_PI_2;  //atan antaa tuloksen radiaaneina. Radiaaneina suorakulma on pii/2 (M_PI_2 = pii/2)
  //suuntaluku = suuntaluku*180/M_PI; //atan palauttaa radiaaneina, konversio takaisin asteiksi
  //cout << "suuntaluku (deg, kulma?): " << suuntaluku << endl;
  //suuntaluku /= 90;

  //Mihin suuntaan ollaan enemmän menossa, sille isompi kulma
  if(x_itmuutos > y_itmuutos) { //Jos x:n muutos suurempi kuin y
    if(suuntaluku > 0.5) {
      suunta.first = suuntaluku;
      suunta.second = 1-suuntaluku;
    }
    else {
      suunta.first = 1-suuntaluku;
      suunta.second = suuntaluku;
    }
  }
  else { //jos y:n muutos on suurempi kuin x
    if(suuntaluku > 0.5) {
      suunta.second = suuntaluku;
      suunta.first = 1-suuntaluku;
    }
    else {
      suunta.second = 1-suuntaluku;
      suunta.first = suuntaluku;
    }
  }

  //Etumerkit miinukseksi tarvittaessa, tätä ennen on laskettu itseisarvoilla

  if(waypoint1.first < sijainti.first) suunta.first *= -1;
  if(waypoint1.second < sijainti.second) suunta.second *= -1;

  suunta.first *= KNOPEUS; //Kävelynopeuden kerroin
  suunta.second *= KNOPEUS;
  //  cout << "SUUNTA: " << suunta.first << " " << suunta.second << endl;
  return suunta;
}

void hahmoluokka::arkit() {
//NÄITÄ ARKKIEN MÄÄRIÄ KUN MUUTTELEE PITÄÄ SITTEN MYÖS MUUTTAA VASTAAVIEN SDL_RECT-ARRAYIDEN KOKOJA KOODIN ALUSSA. AINA EI TULE EDES VIRHEILMOITUSTA.

  //Suoraan edestä
  kissa_arkki[0].x = 0;
  kissa_arkki[0].y = 0;
  kissa_arkki[0].w = 79;
  kissa_arkki[0].h = 139;
  //Sivulta
  kissa_arkki[1].x = 80;
  kissa_arkki[1].y = 0;
  kissa_arkki[1].w = 51;
  kissa_arkki[1].h = 139;
  //Selin
  kissa_arkki[2].x = 0;
  kissa_arkki[2].y = 0;
  kissa_arkki[2].w = 0;
  kissa_arkki[2].h = 0;
  //Sivukävely
  kissa_arkki[3].x = 0;
  kissa_arkki[3].y = 140;
  kissa_arkki[3].w = 53;
  kissa_arkki[3].h = 135;
  kissa_arkki[4].x = 54;
  kissa_arkki[4].y = 140;
  kissa_arkki[4].w = 53;
  kissa_arkki[4].h = 135;
  kissa_arkki[5].x = 108;
  kissa_arkki[5].y = 140;
  kissa_arkki[5].w = 53;
  kissa_arkki[5].h = 135;
  kissa_arkki[6].x = 170;
  kissa_arkki[6].y = 140;
  kissa_arkki[6].w = 53;
  kissa_arkki[6].h = 135;
  //Kävely alas
  kissa_arkki[7].x = 0;
  kissa_arkki[7].y = 276;
  kissa_arkki[7].w = 75;
  kissa_arkki[7].h = 135;
  kissa_arkki[8].x = 76;
  kissa_arkki[8].y = 276;
  kissa_arkki[8].w = 75;
  kissa_arkki[8].h = 135;
  kissa_arkki[9].x = 152;
  kissa_arkki[9].y = 276;
  kissa_arkki[9].w = 75;
  kissa_arkki[9].h = 135;
  kissa_arkki[10].x = 228;
  kissa_arkki[10].y = 276;
  kissa_arkki[10].w = 75;
  kissa_arkki[10].h = 135;
  //Kävely ylös
  kissa_arkki[11].x = 0;
  kissa_arkki[11].y = 412;
  kissa_arkki[11].w = 75;
  kissa_arkki[11].h = 141;
  kissa_arkki[12].x = 76;
  kissa_arkki[12].y = 412;
  kissa_arkki[12].w = 75;
  kissa_arkki[12].h = 141;
  kissa_arkki[13].x = 152;
  kissa_arkki[13].y = 412;
  kissa_arkki[13].w = 75;
  kissa_arkki[13].h = 141;
  kissa_arkki[14].x = 228;
  kissa_arkki[14].y = 412;
  kissa_arkki[14].w = 75;
  kissa_arkki[14].h = 141;
  //Puhe alas, sivulta, liikkumatta alas
  kissa_arkki[15].x = 0;
  kissa_arkki[15].y = 0;
  kissa_arkki[15].w = 0;
  kissa_arkki[15].h = 0;
  kissa_arkki[16].x = 0;
  kissa_arkki[16].y = 0;
  kissa_arkki[16].w = 0;  //Tärkeää, että flipattava puheframe on yhtä leveä kuin vastaava koko hahmon frame
  kissa_arkki[16].h = 0;
  kissa_arkki[17].x = 208;
  kissa_arkki[17].y = 0;
  kissa_arkki[17].w = 75;
  kissa_arkki[17].h = 51;
  //Ilmeet:
  //Seisotaan kasvot kameraa kohden, surullinen hämmästys
  kissa_arkki[18].x = 208;
  kissa_arkki[18].y = 56;
  kissa_arkki[18].w = 75;
  kissa_arkki[18].h = 51;
  //Sad
  kissa_arkki[19].x = 274;
  kissa_arkki[19].y = 0;
  kissa_arkki[19].w = 45;
  kissa_arkki[19].h = 51;
  
  //Suoraan edestä
  koira1_arkki[0].x = 0;
  koira1_arkki[0].y = 0;
  koira1_arkki[0].w = 65;
  koira1_arkki[0].h = 141;
  //Sivulta
  koira1_arkki[1].x = 66;
  koira1_arkki[1].y = 0;
  koira1_arkki[1].w = 123-66;
  koira1_arkki[1].h = 141;
  //Selin
  koira1_arkki[2].x = 128;
  koira1_arkki[2].y = 0;
  koira1_arkki[2].w = 177-124;
  koira1_arkki[2].h = 141;
  //Sivukävely
  koira1_arkki[3].x = 0;
  koira1_arkki[3].y = 142;
  koira1_arkki[3].w = 61;
  koira1_arkki[3].h = 141;
  koira1_arkki[4].x = 62;
  koira1_arkki[4].y = 142;
  koira1_arkki[4].w = 61;
  koira1_arkki[4].h = 141;
  koira1_arkki[5].x = 124;
  koira1_arkki[5].y = 142;
  koira1_arkki[5].w = 61;
  koira1_arkki[5].h = 141;
  koira1_arkki[6].x = 186;
  koira1_arkki[6].y = 142;
  koira1_arkki[6].w = 61;
  koira1_arkki[6].h = 141;
  //Kävely alas
  koira1_arkki[7].x = 0;
  koira1_arkki[7].y = 284;
  koira1_arkki[7].w = 65;
  koira1_arkki[7].h = 143;
  koira1_arkki[8].x = 66;
  koira1_arkki[8].y = 284;
  koira1_arkki[8].w = 65;
  koira1_arkki[8].h = 143;
  koira1_arkki[9].x = 132;
  koira1_arkki[9].y = 284;
  koira1_arkki[9].w = 65;
  koira1_arkki[9].h = 143;
  koira1_arkki[10].x = 198;
  koira1_arkki[10].y = 284;
  koira1_arkki[10].w = 65;
  koira1_arkki[10].h = 143;
  //Kävely ylös
  koira1_arkki[11].x = 0;
  koira1_arkki[11].y = 428;
  koira1_arkki[11].w = 51;
  koira1_arkki[11].h = 143;
  koira1_arkki[12].x = 54;
  koira1_arkki[12].y = 428;
  koira1_arkki[12].w = 51;
  koira1_arkki[12].h = 143;
  koira1_arkki[13].x = 106;
  koira1_arkki[13].y = 428;
  koira1_arkki[13].w = 51;
  koira1_arkki[13].h = 143;
  koira1_arkki[14].x = 160;
  koira1_arkki[14].y = 428;
  koira1_arkki[14].w = 51;
  koira1_arkki[14].h = 143;
  //Puhe alas, sivulta, liikkumatta alas
  koira1_arkki[15].x = 0;
  koira1_arkki[15].y = 0;
  koira1_arkki[15].w = 0;
  koira1_arkki[15].h = 0;
  koira1_arkki[16].x = 174;
  koira1_arkki[16].y = 0;
  koira1_arkki[16].w = 57;  //Tärkeää, että flipattava puheframe on yhtä leveä kuin vastaava koko hahmon frame
  koira1_arkki[16].h = 45;
  koira1_arkki[17].x = 174;
  koira1_arkki[17].y = 46;
  koira1_arkki[17].w = 65;
  koira1_arkki[17].h = 46;
  //Ilme, sad sivulta
  koira1_arkki[18].x = 0;
  koira1_arkki[18].y = 0;
  koira1_arkki[18].w = 0;
  koira1_arkki[18].h = 0;
  koira1_arkki[19].x = 0;
  koira1_arkki[19].y = 0;
  koira1_arkki[19].w = 0;
  koira1_arkki[19].h = 0;
  koira1_arkki[20].x = 236; //oli 254
  koira1_arkki[20].y = 0;
  koira1_arkki[20].w = 123-66;
  koira1_arkki[20].h = 40;
  
  //Suoraan edestä
  koira2_arkki[0].x = 0;
  koira2_arkki[0].y = 0;
  koira2_arkki[0].w = 57;
  koira2_arkki[0].h = 131;
  //Sivulta
  koira2_arkki[1].x = 58;
  koira2_arkki[1].y = 0;
  koira2_arkki[1].w = 45;
  koira2_arkki[1].h = 131;
  //Selin
  koira2_arkki[2].x = 0;
  koira2_arkki[2].y = 0;
  koira2_arkki[2].w = 0;
  koira2_arkki[2].h = 0;
  //Sivukävely
  koira2_arkki[3].x = 0;
  koira2_arkki[3].y = 132;
  koira2_arkki[3].w = 49;
  koira2_arkki[3].h = 131;
  koira2_arkki[4].x = 50;
  koira2_arkki[4].y = 132;
  koira2_arkki[4].w = 49;
  koira2_arkki[4].h = 131;
  koira2_arkki[5].x = 104;
  koira2_arkki[5].y = 132;
  koira2_arkki[5].w = 49;
  koira2_arkki[5].h = 131;
  koira2_arkki[6].x = 154;
  koira2_arkki[6].y = 132;
  koira2_arkki[6].w = 49;
  koira2_arkki[6].h = 131;
  //Kävely alas
  koira2_arkki[7].x = 0;
  koira2_arkki[7].y = 264;
  koira2_arkki[7].w = 57;
  koira2_arkki[7].h = 135;
  koira2_arkki[8].x = 58;
  koira2_arkki[8].y = 264;
  koira2_arkki[8].w = 57;
  koira2_arkki[8].h = 135;
  koira2_arkki[9].x = 116;
  koira2_arkki[9].y = 264;
  koira2_arkki[9].w = 57;
  koira2_arkki[9].h = 135;
  koira2_arkki[10].x = 174;
  koira2_arkki[10].y = 264;
  koira2_arkki[10].w = 57;
  koira2_arkki[10].h = 135;
  //Kävely ylös
  koira2_arkki[11].x = 0;
  koira2_arkki[11].y = 400;
  koira2_arkki[11].w = 55;
  koira2_arkki[11].h = 139;
  koira2_arkki[12].x = 56;
  koira2_arkki[12].y = 400;
  koira2_arkki[12].w = 55;
  koira2_arkki[12].h = 139;
  koira2_arkki[13].x = 112;
  koira2_arkki[13].y = 400;
  koira2_arkki[13].w = 55;
  koira2_arkki[13].h = 139;
  koira2_arkki[14].x = 168;
  koira2_arkki[14].y = 400;
  koira2_arkki[14].w = 55;
  koira2_arkki[14].h = 139;
  //Puhe alas, sivulta, liikkumatta alas
  koira2_arkki[15].x = 156;  //oli 160
  koira2_arkki[15].y = 0;
  koira2_arkki[15].w = 38;
  koira2_arkki[15].h = 40;
  koira2_arkki[16].x = 208;
  koira2_arkki[16].y = 0;
  koira2_arkki[16].w = 57;  //Tärkeää, että flipattava puheframe on yhtä leveä kuin vastaava koko hahmon frame
  koira2_arkki[16].h = 46;
  koira2_arkki[17].x = 248; //oli 254
  koira2_arkki[17].y = 0;
  koira2_arkki[17].w = 57;
  koira2_arkki[17].h = 44;
  //Ilmeet, sad edestä
  koira2_arkki[18].x = 316; //oli 254
  koira2_arkki[18].y = 0;
  koira2_arkki[18].w = 57;
  koira2_arkki[18].h = 40;

  //Suoraan edestä
  naata_arkki[0].x = 0;
  naata_arkki[0].y = 0;
  naata_arkki[0].w = 51;
  naata_arkki[0].h = 127;
  //Sivulta
  naata_arkki[1].x = 52;
  naata_arkki[1].y = 0;
  naata_arkki[1].w = 53;
  naata_arkki[1].h = 127;
  //Selin
  naata_arkki[2].x = 106;
  naata_arkki[2].y = 0;
  naata_arkki[2].w = 51;
  naata_arkki[2].h = 127;
  //Sivukävely
  naata_arkki[3].x = 0;
  naata_arkki[3].y = 128;
  naata_arkki[3].w = 53;
  naata_arkki[3].h = 127;
  naata_arkki[4].x = 56;
  naata_arkki[4].y = 128;
  naata_arkki[4].w = 53;
  naata_arkki[4].h = 127;
  naata_arkki[5].x = 111;
  naata_arkki[5].y = 128;
  naata_arkki[5].w = 53;
  naata_arkki[5].h = 127;
  naata_arkki[6].x = 168;
  naata_arkki[6].y = 128;
  naata_arkki[6].w = 53;
  naata_arkki[6].h = 127;
  //Kävely alas
  naata_arkki[7].x = 0;
  naata_arkki[7].y = 256;
  naata_arkki[7].w = 53;
  naata_arkki[7].h = 131;
  naata_arkki[8].x = 54;
  naata_arkki[8].y = 256;
  naata_arkki[8].w = 53;
  naata_arkki[8].h = 131;
  naata_arkki[9].x = 108;
  naata_arkki[9].y = 256;
  naata_arkki[9].w = 53;
  naata_arkki[9].h = 131;
  naata_arkki[10].x = 162;
  naata_arkki[10].y = 256;
  naata_arkki[10].w = 53;
  naata_arkki[10].h = 131;
  //Kävely ylös
  naata_arkki[11].x = 0;
  naata_arkki[11].y = 388;
  naata_arkki[11].w = 51;
  naata_arkki[11].h = 131;
  naata_arkki[12].x = 52;
  naata_arkki[12].y = 388;
  naata_arkki[12].w = 51;
  naata_arkki[12].h = 131;
  naata_arkki[13].x = 104;
  naata_arkki[13].y = 388;
  naata_arkki[13].w = 51;
  naata_arkki[13].h = 131;
  naata_arkki[14].x = 156;
  naata_arkki[14].y = 388;
  naata_arkki[14].w = 51;
  naata_arkki[14].h = 131;
  //Puhe alas, sivulta, liikkumatta alas
  naata_arkki[15].x = 0;
  naata_arkki[15].y = 0;
  naata_arkki[15].w = 0;
  naata_arkki[15].h = 0;
  naata_arkki[16].x = 210;
  naata_arkki[16].y = 0;
  naata_arkki[16].w = 255-210;  //Tärkeää, että flipattava puheframe on yhtä leveä kuin vastaava koko hahmon frame
  naata_arkki[16].h = 42;
  naata_arkki[17].x = 158; 
  naata_arkki[17].y = 0;
  naata_arkki[17].w = 209-158;
  naata_arkki[17].h = 45;

  //Suoraan edestä
  mayra_arkki[0].x = 0;
  mayra_arkki[0].y = 0;
  mayra_arkki[0].w = 57;
  mayra_arkki[0].h = 119;
  //Sivulta
  mayra_arkki[1].x = 58;
  mayra_arkki[1].y = 0;
  mayra_arkki[1].w = 51;
  mayra_arkki[1].h = 119;
  //Selin
  mayra_arkki[2].x = 0;
  mayra_arkki[2].y = 0;
  mayra_arkki[2].w = 0;
  mayra_arkki[2].h = 0;
  //Sivukävely
  mayra_arkki[3].x = 0;
  mayra_arkki[3].y = 120;
  mayra_arkki[3].w = 57;
  mayra_arkki[3].h = 121;
  mayra_arkki[4].x = 58;
  mayra_arkki[4].y = 120;
  mayra_arkki[4].w = 57;
  mayra_arkki[4].h = 121;
  mayra_arkki[5].x = 122;
  mayra_arkki[5].y = 120;
  mayra_arkki[5].w = 57;
  mayra_arkki[5].h = 121;
  mayra_arkki[6].x = 180;
  mayra_arkki[6].y = 120;
  mayra_arkki[6].w = 57;
  mayra_arkki[6].h = 121;
  //Kävely alas
  mayra_arkki[7].x = 0;
  mayra_arkki[7].y = 242;
  mayra_arkki[7].w = 59;
  mayra_arkki[7].h = 121;
  mayra_arkki[8].x = 60;
  mayra_arkki[8].y = 242;
  mayra_arkki[8].w = 59;
  mayra_arkki[8].h = 121;
  mayra_arkki[9].x = 118;
  mayra_arkki[9].y = 242;
  mayra_arkki[9].w = 59;
  mayra_arkki[9].h = 121;
  mayra_arkki[10].x = 176;
  mayra_arkki[10].y = 242;
  mayra_arkki[10].w = 59;
  mayra_arkki[10].h = 121;
  //Kävely ylös
  mayra_arkki[11].x = 0;
  mayra_arkki[11].y = 364;
  mayra_arkki[11].w = 57;
  mayra_arkki[11].h = 123;
  mayra_arkki[12].x = 58;
  mayra_arkki[12].y = 364;
  mayra_arkki[12].w = 57;
  mayra_arkki[12].h = 123;
  mayra_arkki[13].x = 116;
  mayra_arkki[13].y = 364;
  mayra_arkki[13].w = 57;
  mayra_arkki[13].h = 123;
  mayra_arkki[14].x = 174;
  mayra_arkki[14].y = 364;
  mayra_arkki[14].w = 57;
  mayra_arkki[14].h = 123;
  //Puhe alas, sivulta, liikkumatta alas
  mayra_arkki[15].x = 0;
  mayra_arkki[15].y = 0;
  mayra_arkki[15].w = 0;
  mayra_arkki[15].h = 0;
  mayra_arkki[16].x = 164;
  mayra_arkki[16].y = 0;
  mayra_arkki[16].w = 51;  //Tärkeää, että flipattava puheframe on yhtä leveä kuin vastaava koko hahmon frame
  mayra_arkki[16].h = 42;
  mayra_arkki[17].x = 110; 
  mayra_arkki[17].y = 0;
  mayra_arkki[17].w = 52;
  mayra_arkki[17].h = 48;
  //Ilmeet: ahdistunut (edestä), hämmästynyt (edestä), ahdistunut (sivulta)
  mayra_arkki[18].x = 271;
  mayra_arkki[18].y = 0;
  mayra_arkki[18].w = 53;
  mayra_arkki[18].h = 47;
  mayra_arkki[19].x = 214; //oli 216
  mayra_arkki[19].y = 0;
  mayra_arkki[19].w = 53;
  mayra_arkki[19].h = 47;
  mayra_arkki[20].x = 330;
  mayra_arkki[20].y = 0;
  mayra_arkki[20].w = 381-330;
  mayra_arkki[20].h = 26;
  //Istuminen, kaksi framea koneen kanssa ja yksi ilman
  mayra_arkki[21].x = 286;
  mayra_arkki[21].y = 366;
  mayra_arkki[21].w = 342-286;
  mayra_arkki[21].h = 477-366;
  mayra_arkki[22].x = 0;
  mayra_arkki[22].y = 0;
  mayra_arkki[22].w = 0;
  mayra_arkki[22].h = 0;
  mayra_arkki[23].x = 232;
  mayra_arkki[23].y = 364;
  mayra_arkki[23].w = 287-232;
  mayra_arkki[23].h = 477-364;
  //Ruokailu
  mayra_arkki[24].x = 0;
  mayra_arkki[24].y = 488;
  mayra_arkki[24].w = 67;
  mayra_arkki[24].h = 557-488;
  mayra_arkki[25].x = 68;
  mayra_arkki[25].y = 488;
  mayra_arkki[25].w = 135-68;
  mayra_arkki[25].h = 553-488;
  mayra_arkki[26].x = 136;
  mayra_arkki[26].y = 488;
  mayra_arkki[26].w = 203-136;
  mayra_arkki[26].h = 551-488;
  mayra_arkki[27].x = 204;
  mayra_arkki[27].y = 488;
  mayra_arkki[27].w = 271-204;
  mayra_arkki[27].h = 551-488;

  drops_arkki[0].x = 1;
  drops_arkki[0].y = 1;
  drops_arkki[0].w = 159;
  drops_arkki[0].h = 41;
  drops_arkki[1].x = 1;
  drops_arkki[1].y = 44;
  drops_arkki[1].w = 159;
  drops_arkki[1].h = 41;
  drops_arkki[2].x = 1; 
  drops_arkki[2].y = 87;
  drops_arkki[2].w = 159;
  drops_arkki[2].h = 41;
  drops_arkki[3].x = 1; 
  drops_arkki[3].y = 130;
  drops_arkki[3].w = 159;
  drops_arkki[3].h = 41;
  drops_arkki[4].x = 1; 
  drops_arkki[4].y = 173;
  drops_arkki[4].w = 159;
  drops_arkki[4].h = 41;
  drops_arkki[5].x = 1; 
  drops_arkki[5].y = 216;
  drops_arkki[5].w = 159;
  drops_arkki[5].h = 41;
  drops_arkki[6].x = 1; 
  drops_arkki[6].y = 259;
  drops_arkki[6].w = 159;
  drops_arkki[6].h = 41;
  drops_arkki[7].x = 1;
  drops_arkki[7].y = 302;
  drops_arkki[7].w = 159;
  drops_arkki[7].h = 41;
  drops_arkki[8].x = 0;
  drops_arkki[8].y = 0;
  drops_arkki[8].w = 0;
  drops_arkki[8].h = 0;
  drops_arkki[9].x = 0;
  drops_arkki[9].y = 0;
  drops_arkki[9].w = 0;
  drops_arkki[9].h = 0;
  drops_arkki[10].x = 0;
  drops_arkki[10].y = 0;
  drops_arkki[10].w = 0;
  drops_arkki[10].h = 0;
  drops_arkki[11].x = 0;
  drops_arkki[11].y = 0;
  drops_arkki[11].w = 0;
  drops_arkki[11].h = 0;
  drops_arkki[12].x = 0;
  drops_arkki[12].y = 0;
  drops_arkki[12].w = 0;
  drops_arkki[12].h = 0;
  drops_arkki[13].x = 0;
  drops_arkki[13].y = 0;
  drops_arkki[13].w = 0;
  drops_arkki[13].h = 0;
  drops_arkki[14].x = 0;
  drops_arkki[14].y = 0;
  drops_arkki[14].w = 0;
  drops_arkki[14].h = 0;
  drops_arkki[15].x = 0;
  drops_arkki[15].y = 0;
  drops_arkki[15].w = 0;
  drops_arkki[15].h = 0;

  //Suoraan edestä
  kissak_arkki[0].x = 0;
  kissak_arkki[0].y = 0;
  kissak_arkki[0].w = 79;
  kissak_arkki[0].h = 139;
  //Sivulta
  kissak_arkki[1].x = 80;
  kissak_arkki[1].y = 0;
  kissak_arkki[1].w = 155-80;
  kissak_arkki[1].h = 139;
  //Selin
  kissak_arkki[2].x = 0;
  kissak_arkki[2].y = 0;
  kissak_arkki[2].w = 0;
  kissak_arkki[2].h = 0;
  //Sivukävely
  kissak_arkki[3].x = 0;
  kissak_arkki[3].y = 140;
  kissak_arkki[3].w = 71;
  kissak_arkki[3].h = 135;
  kissak_arkki[4].x = 72;
  kissak_arkki[4].y = 140;
  kissak_arkki[4].w = 143-72;
  kissak_arkki[4].h = 135;
  kissak_arkki[5].x = 144;
  kissak_arkki[5].y = 140;
  kissak_arkki[5].w = 217-144;
  kissak_arkki[5].h = 135;
  kissak_arkki[6].x = 218;
  kissak_arkki[6].y = 140;
  kissak_arkki[6].w = 287-218;
  kissak_arkki[6].h = 135;
  //Kävely alas
  kissak_arkki[7].x = 0;
  kissak_arkki[7].y = 276;
  kissak_arkki[7].w = 75;
  kissak_arkki[7].h = 135;
  kissak_arkki[8].x = 76;
  kissak_arkki[8].y = 276;
  kissak_arkki[8].w = 75;
  kissak_arkki[8].h = 135;
  kissak_arkki[9].x = 152;
  kissak_arkki[9].y = 276;
  kissak_arkki[9].w = 75;
  kissak_arkki[9].h = 135;
  kissak_arkki[10].x = 228;
  kissak_arkki[10].y = 276;
  kissak_arkki[10].w = 75;
  kissak_arkki[10].h = 135;
  //Kävely ylös
  kissak_arkki[11].x = 0;
  kissak_arkki[11].y = 412;
  kissak_arkki[11].w = 75;
  kissak_arkki[11].h = 141;
  kissak_arkki[12].x = 76;
  kissak_arkki[12].y = 412;
  kissak_arkki[12].w = 75;
  kissak_arkki[12].h = 141;
  kissak_arkki[13].x = 152;
  kissak_arkki[13].y = 412;
  kissak_arkki[13].w = 75;
  kissak_arkki[13].h = 141;
  kissak_arkki[14].x = 228;
  kissak_arkki[14].y = 412;
  kissak_arkki[14].w = 75;
  kissak_arkki[14].h = 141;
  //Puhe alas, sivulta, liikkumatta alas
  kissak_arkki[15].x = 0;
  kissak_arkki[15].y = 0;
  kissak_arkki[15].w = 0;
  kissak_arkki[15].h = 0;
  kissak_arkki[16].x = 0;
  kissak_arkki[16].y = 0;
  kissak_arkki[16].w = 0;  //Tärkeää, että flipattava puheframe on yhtä leveä kuin vastaava koko hahmon frame
  kissak_arkki[16].h = 0;
  kissak_arkki[17].x = 208;
  kissak_arkki[17].y = 0;
  kissak_arkki[17].w = 75;
  kissak_arkki[17].h = 51;
  //Ilmeet:
  //Seisotaan kasvot kameraa kohden, surullinen hämmästys
  kissak_arkki[18].x = 208;
  kissak_arkki[18].y = 56;
  kissak_arkki[18].w = 75;
  kissak_arkki[18].h = 51;
  //Sad
  kissak_arkki[19].x = 274;
  kissak_arkki[19].y = 0;
  kissak_arkki[19].w = 45;
  kissak_arkki[19].h = 51;
}

hahmoluokka kissa;
hahmoluokka kissak;
hahmoluokka koira1;
hahmoluokka koira2;
hahmoluokka naata;
hahmoluokka mayra;
hahmoluokka drops;

class esineluokka {
public:
  int ruudulla; //Onko esine ruudulla ja monennessako hahmonpiirtokerroksessa
  pair<double, double> sijainti;  //Viitataan sijainti.first, sijainti.second
  //  int frame;
  //  int framecount;
  tekstuuriluokka *tekstuuri;
  //  tekstuuriluokka *tekstuuri;
  //  SDL_Rect *esinerect;
  esineluokka();
  ~esineluokka();
  void esineenpiirto();
  //  void yhdista_tekstuuri(tekstuuriluokka*);
  //  void yhdista_rect(SDL_Rect *rect, int, unsigned int);
  //  void arkit(); //Asettaa spritearkkien koordinaatit
};

vector<esineluokka*> esinevektori; //Vektori, johon liitetään esineluokan jäseniin osoittavat pointterit

//void esineluokka::yhdista_tekstuuri(tekstuuriluokka *olio) {spritet = olio;}  JOS TULEE FRAMET

esineluokka::esineluokka():
  ruudulla(0), tekstuuri(NULL) { //Pair-muuttujat puuttuvat tästä, niihin tulisi vain nollaa
  esinevektori.push_back(this);
}

esineluokka::~esineluokka() {
  //  delete[] esinerect; PALAUTA TÄMÄ RIVI, JOS FRAMET TULEE ESINELUOKKAAN!!!!
  //Tuleeko tähän joku tekstuurin tuhoaminen vai onko se jo siellä muualla
}

/*
//Esineen rect yhdistetään sen spritesheettiin
void esineluokka::yhdista_rect(SDL_Rect *rect, int arraynpituus, unsigned int arraynkoko) {  //Funktioon lähetetään array, jolloin sinne lähtee arrayn ensimmäiseen elementtiin osoittava pointteri ja näin ollen parametrin täytyy olla pointteri-tyyppinen
  esinerect = new SDL_Rect[arraynkoko]; //Dynaamisesti allokoituvaan arrayseen muistit
  for(int counter = 0; counter != arraynpituus; counter++) { //Assignataan sisältö
    hahmorect[counter] = rect[counter];
  }
}
*/

void esineluokka::esineenpiirto() {
  tekstuuri->renderfunktio(sijainti.first-kamerax1,sijainti.second);
}

/*
void esineluokka::arkit() {
}
*/

esineluokka poyta;
esineluokka kpoyta;
esineluokka rv;
esineluokka ruoka;
esineluokka tuoli;

bool init() {
  bool success = true;
  if(SDL_Init(SDL_INIT_VIDEO)<0) {
    cerr << "SDL init fail - " << SDL_GetError() << endl;
    success = false;
  }
  else {
    //tekstuurien filtteröinti lineaariseksi
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
      cerr << "Linear filtering failed" << endl;
    }
    //Ikkunan luonti
    ikkuna = SDL_CreateWindow("Testi-ikkuna", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(ikkuna == NULL) {
      cerr << "Window creation failed - " << SDL_GetError() << endl;
      success = false;
    }
    else {
      //Renderöijän luonti
      piirturi = SDL_CreateRenderer(ikkuna, -1, SDL_RENDERER_ACCELERATED);
      SDL_RenderSetLogicalSize(piirturi, SCREEN_WIDTH, SCREEN_HEIGHT);
      SDL_RendererInfo piirturi_info;
      SDL_GetRendererInfo(piirturi, &piirturi_info);
      cout << "Rendereroija: " << piirturi_info.name << endl;
      cout << "Renderoijan maksimi tekstuurinleveys: " << piirturi_info.max_texture_width << endl;
      cout << "Renderoijan maksimi tekstuurinkorkeus: " << piirturi_info.max_texture_height << endl;
      if(piirturi == NULL) {
	cerr << "Renderer creation fail - " << SDL_GetError() << endl;
	success = false;
      }
      else {
	//Initialisoidaan renderöijän väri
	SDL_SetRenderDrawColor(piirturi, 0xFF, 0xFF, 0xFF, 0xFF);
	//Initialisoidaan png:n lataus
	int imgFlags = IMG_INIT_PNG;
	if(!(IMG_Init(imgFlags)&imgFlags)) {
	  cerr << "SDL_image fail - " << IMG_GetError() << endl;
	  success = false;
	}
	else {
	  if(TTF_Init() == -1) {
	    cerr << "TTF Init fail" << TTF_GetError() << endl;
	  }
	}
      }
    }
  }
  return success;
}


bool lataamedia() {
  bool success = true;
  if(!kissateks.tiedostonlataus("animpng/hk3.png")) {
    cerr << "Texture image load fail" << endl;
    success = false;
  }
  if(!kissakteks.tiedostonlataus("animpng/hkk.png")) {
    cerr << "Texture image load fail" << endl;
    success = false;
  }
  if(!koirateks1.tiedostonlataus("animpng/te-frames.png")) {
    cerr << "Texture image load fail" << endl;
    success = false;
  }
  if(!koirateks2.tiedostonlataus("animpng/val-frames.png")) {
    cerr << "Texture image load fail" << endl;
    success = false;
  }
  if(!naatateks.tiedostonlataus("animpng/ma-frames.png")) {
    cerr << "Texture image load fail" << endl;
    success = false;
  }
  if(!mayrateks.tiedostonlataus("animpng/bad-frames.png")) {
    cerr << "Texture image load fail" << endl;
    success = false;
  }
    if(!dropsteks.tiedostonlataus("animpng/sweat2.png")) {
    cerr << "Texture image load fail" << endl;
    success = false;
  }
  if(!koti.tiedostonlataus("animpng/koti.png")) {
    cerr << "Texture background koti load fail" << endl;
    success = false;
  }
    if(!wc.tiedostonlataus("animpng/wc.png")) {
    cerr << "Texture background wc load fail" << endl;
    success = false;
  }
  if(!poytateks.tiedostonlataus("animpng/poyta.png")) {
    cerr << "Object pöytä load fail" << endl;
    success = false;
  }
  if(!kpoytateks.tiedostonlataus("animpng/poyta2.png")) {
    cerr << "Object kpöytä load fail" << endl;
    success = false;
    }
  if(!rvteks.tiedostonlataus("animpng/rv.png")) {
    cerr << "Object rv load fail" << endl;
    success = false;
    }
  if(!tuoliteks.tiedostonlataus("animpng/tuoli.png")) {
    cerr << "Object tuoli load fail" << endl;
    success = false;
    }
  if(!ruokateks.tiedostonlataus("animpng/ruoka5.png")) {
    cerr << "Object ruoka load fail" << endl;
    success = false;
    }
  if(!pimee.tiedostonlataus("animpng/pimee.png")) {
    cerr << "Pimee.png load fail" << endl;
    success = false;
  }
  else pimee.aseta_blend(SDL_BLENDMODE_BLEND);
  
  return success;
}

void sammutus() {
  wc.erase();
  koti.erase();
  pimee.erase();
  koirateks1.erase();
  koirateks2.erase();
  kissateks.erase();
  kissakteks.erase();
  naatateks.erase();
  mayrateks.erase();
  dropsteks.erase();
  poytateks.erase();
  kpoytateks.erase();
  rvteks.erase();
  ruokateks.erase();
  tuoliteks.erase();
  
  SDL_DestroyRenderer(piirturi);
  SDL_DestroyWindow(ikkuna);
  ikkuna = NULL;
  piirturi = NULL;

  IMG_Quit();
  SDL_Quit();
}

//DEVDEVDEVDEV
//puhekuplan osoittimien piirtofunktio. Tätä käytetään sekä piirtäessä se muoto, että täyttäessä osoitin valkoisella (jolloin funktio kertoo, miten y-pisteet rajautuvat). Toisen tällaisen voisi ehkä tehdä, jos haluaa toisenkin kaaren siihen osoittimeen

//puhekuplan osoitinosan piirto toisen asteen yhtälöä käyttämällä. kupasema == ala --> ylöspäin aukeavat kaaret
void osoitin(int bubw, int bubh) {
  //  int kpituus;
  //  kuplanpituus == 0 ? kpituus = OLETUSKUPLANPITUUS : kpituus = kuplanpituus;
  //  if(kuplavo == 1 or (kuplavo == 0 and kuppoint->first > SCREEN_WIDTH/2)) bubw += kpituus-100;

  //  -osoittimen vasen-oikea suunnan saa muutettua, kun for-sulkeiden laskurin laittaa dekrementoimaan ja jatkoehdoksi > -20
  bool vasen = false; //Kupla vasemmalle (vai oikealle) päin?
  if(kuplavo == 1 or kuppoint->first-kamerax1 > SCREEN_WIDTH/2) vasen = true;  //Eikö tämä pitäisi ottaa hahmon sijainnin eikä kuplan mukaan? Tärkeää joka tapauksessa, että otetaan samalla tavalla kuin kuplanpiirtofunktiossa (osoittaako kupla vasemmalle vai oikealle)

  if(kupasema == 2) bubh += 5;
  //(ainakin) Yläkupla-h
  else bubh = kuppoint->second-30;

  //Yläkupla-w vasemmalle
  if(vasen)  bubw = kuppoint->first+10-kamerax1;
  //Yläkupla-w oikealle
  else bubw = kuppoint->first+40-kamerax1;

  //Näissä kupasema > 2 määrittelyissä w ja h vaihtavat paikkaa, kun kaari keikautetaan
  if(!vasen and kupasema > 2) {
    bubh += 20;
    bubw += 70;
  }
  else if (kupasema > 2) {
    bubw = kuppoint->second+50;
    bubh = kuppoint->first-32-kamerax1;
  }
  
  int y7, y8;
  int laskuri2 = 30;
  int laskuri3 = 30; //Huom! Tämä oli double ja double-arvot (1.9 jne.) sopivat myös
  int inkdek;
  if(vasen) inkdek = -1;
  else inkdek = 1;
  for(int laskuri = 0; laskuri > -20 and laskuri < 20; laskuri += inkdek) {  //Vasemmalle päin kaartuva osoitin dekrementoi laskuria ja jatkoehto on laskuri > -20
    //  for(int laskuri = 0; laskuri < 20; laskuri++) {  //Tämä antaa oikealle kaartuvan osoittimen
    y7 = pow((0.2*laskuri2),2)+(-0.3*laskuri2)-1; //nämäkin voisi upottaa ilmeisesti suoraan noihin piirtokutsuihin vai meneekö liian sotkuksi
    y8 = pow((0.2*laskuri3),2)+(-0.3*laskuri3)-1;
    if(kupasema == 2 or (!vasen and kupasema > 2)) {
      y7 *= -1;
      y8 *= -1;
    }

    //Jos kupla sivulla, X ja Y vaihtaa paikkaa, niin saadaan sivusuuntaan keikautettu osoitin
    if(kupasema > 2) SDL_RenderDrawLine(piirturi, bubh+y7, bubw+laskuri, bubh-3+y8, bubw+laskuri);
    else SDL_RenderDrawLine(piirturi, bubw+laskuri, bubh+y7, bubw+laskuri, bubh-3+y8);

    /*
-alaspäin avautuu, jos laittaa koko y7 ja y8 kaavan negaatioksi JA vaihtaa laskuri3:n ja 2:n paikkaa keskenään loopin lopussa (eli kummasta dekrementoidaan 1 ja kummasta 2). Tämä alaspäin on käytössä kaiketi myös sivulle päin tehtäessä.
    */
    
    
    //    SDL_RenderDrawPoint(piirturi, 120+laskuri, bubh+y7);
    //    SDL_RenderDrawPoint(piirturi, 120+laskuri, bubh-3+y8); //Tämän piirto pitäisi lopettaa jossain vaiheessa (samoin värittävien poikkiviivojen)
    //    SDL_RenderDrawLine(piirturi, (120+laskuri), (bubh-2+y8), (120+laskuri), (bubh+y7));
    //kolmas argumentti oli bubh+y7 ja sitten bubh-10+y7
    //    SDL_RenderDrawLine(piirturi, (bubw+40+laskuri), (bubh+bubh2), (bubw+40+laskuri), (bubh+bubh2+y7));   //Samaan tapaan tekisi helposti toisenkin kaaren tuohon, joka rajoittuisi ylä- eikä alasuunnasta, jos jaksaisi vielä kokeilla sille sopivan yhtälön
    
    if(kupasema == 2 or (!vasen and kupasema > 2)) {
      laskuri2 -= 2;
      laskuri3--;
    }
    else {
      laskuri2--;
      laskuri3 -= 2; //oli 1.5 ja oli double
    }
  }
}
//DEVDEVDEVDEVDEVDEV

/*
//puhekuplan osoittimien piirtofunktio. Tätä käytetään sekä piirtäessä se muoto, että täyttäessä osoitin valkoisella (jolloin funktio kertoo, miten y-pisteet rajautuvat). Toisen tällaisen voisi ehkä tehdä, jos haluaa toisenkin kaaren siihen osoittimeen
int funktio1(int num) {
  return (pow((+0.2*num),2)+(-0.3*num)-1); //Toisen asteen yhtälö
}

//puhekuplan osoitinosan piirto
void osoitin(int bubw, int bubh, int bubh2) {
  int kpituus;
  kuplanpituus == 0 ? kpituus = OLETUSKUPLANPITUUS : kpituus = kuplanpituus;
  if(kuplavo == 1 or (kuplavo == 0 and kuppoint->first > SCREEN_WIDTH/2)) bubw += kpituus-100;
int y7;
  int laskuri2 = 30;
  for(int laskuri = 0; laskuri < 30; laskuri++) {
    y7 = (funktio1((laskuri2)));
    SDL_RenderDrawLine(piirturi, (kuppoint->first+40+laskuri), (bubh+bubh2), (kuppoint->first+40+laskuri), (bubh+bubh2+y7));
    //    SDL_RenderDrawLine(piirturi, (bubw+40+laskuri), (bubh+bubh2), (bubw+40+laskuri), (bubh+bubh2+y7));   //Samaan tapaan tekisi helposti toisenkin kaaren tuohon, joka rajoittuisi ylä- eikä alasuunnasta, jos jaksaisi vielä kokeilla sille sopivan yhtälön
    laskuri2--;
  }
}
*/

//Mitä tietoa tähän funktioon pitää syöttää?
//vaerin määrittely siirretty alkuun
//muokattu tekstirivi - tämä ehkä pitäisi syöttää
//bubblew, bubbleh
//pitäisikö laittaa parametri, josta tulee tekstin väri?
void tekstirend(int bw, int bh, int moneskorivi, string teksti, TTF_Font* kirjaisinlaji) {
  int rivinasetus = (5+((moneskorivi-1)*merkinkorkeus)); //Arvo 5 on väli yläreunasta. Voisi ehkä laittaa constiksi? Kursorirendissä on vastaava (bubh+5)
  SDL_Surface* pinta2 = TTF_RenderText_Solid(kirjaisinlaji, teksti.c_str(), vaeri);
  int tekstinleveys = pinta2->w;
  //  int tekstinkorkeus = pinta2->h;  //Tämä onkin sama kuin merkinkorkeus --> ei ilmeisesti tarvita
  //  cout << teksti << endl;
  //  cout << "tekstinleveys & -korkeus: " << tekstinleveys << " - " << tekstinkorkeus << endl;
  SDL_Texture* tekstuuri2 = SDL_CreateTextureFromSurface(piirturi, pinta2);
  //  SDL_Rect tekstinelio = {bw+5, bh+rivinasetus-skrollausarvo, tekstinleveys, tekstinkorkeus}; //skrollausarvo lisätty text_scroll.cpp:hen
  SDL_Rect tekstinelio = {bw+5, bh+rivinasetus-skrollausarvo+RIVIVALI*(moneskorivi-1), tekstinleveys, merkinkorkeus}; //skrollausarvo lisätty text_scroll.cpp:hen. Onko tässäkin 5 rako kuplan yläreunan ja tekstin välillä?
  SDL_RenderCopy(piirturi, tekstuuri2, NULL, &tekstinelio);
}

void kursorirend(int bubw, int bubh, int rivinumero, int kirjaimennumero, string merkki, TTF_Font* kirjaisinlaji) {
  //bubh on kuplan sivuttain menevän palkin yläreuna eli siitä alkaa se teksti suoraan
  rivinumero--;
  bubh += merkinkorkeus*rivinumero;
  kirjaimennumero--;
  bubw += merkinpituus*kirjaimennumero;
  SDL_Surface* pinta3 = TTF_RenderText_Solid(kirjaisinlaji, merkki.c_str(), vaeri);      
  SDL_Texture* peitekirjain = SDL_CreateTextureFromSurface(piirturi, pinta3); 
  SDL_Rect tekstinelio = {bubw+5, bubh+5+RIVIVALI*(rivinumero), merkinpituus, merkinkorkeus};  //bubh+5:ssa 5 on rako kuplan yläreunan ja alkavan tekstin välillä
  SDL_SetRenderDrawColor(piirturi, 0xFF, 0xA5, 0x00, 0x00);
  SDL_RenderFillRect(piirturi, &tekstinelio);   //kursori pitää rendata tässä eri väristen tekstien välissä
  SDL_RenderCopy(piirturi, peitekirjain, NULL, &tekstinelio);
}

//Funktio, joka palauttaa stringinä kirjainmerkin, joka pitää sillä kertaa näyttää kursorissa (sama aina mikä jää kursorin alle)
string kursorikirjain(int rivinum, int kirjainnum) {
  string kirjain;
  vector<string>::iterator kuriter = vuorosanavec.begin()+(rivinum-1);
  kirjain = (*kuriter)[kirjainnum-1];
  return kirjain;
}

bool onkoriviloppu(int rivinum, int kirjainnum) {
  string rimpsu;
  vector<string>::iterator onkoiter = vuorosanavec.begin()+(rivinum-1);
  rimpsu = *onkoiter;
  return rimpsu.length() == kirjainnum; 
}

//Tämä funktio pitäisi ajaa puhekuplan käytön jälkeen. Koodille on tärkeää tietää, mitkä rivit ovat tyhjiä. Ei ole käytössä tässä koodissa
void riviennollaus() {
  for(vector<string>::iterator eraser_iter = vuorosanavec.begin(); eraser_iter != vuorosanavec.end(); ++eraser_iter) {
    vuorosanavec.erase(eraser_iter);
    --eraser_iter;
  }
}

//Vuorosanan sopivanpituisiin riveihin pätkivä funktio
void autowrap(int rivinpituus, string vuorosana1) {
//looppeja, jotka käy vuorosanaa alusta aina rivinpituuden kohdalta läpi taaksepäin ja ensimmäisen välilyönnin kohdalla laittaa seuraavaksi riviksi sitä edeltävät merkit ja poistaa ne vuorosanasta
  string tempstring;
  int marker = 0; //Tämän pitäisi merkata koko stringin viimeinen välilyönti
  for(int m = vuorosana1.length(); marker == 0;  m--)
    if(vuorosana1[m] == ' ') marker = vuorosana1.length()-m;
  for(int i = rivinpituus; vuorosana1.length() > marker; i--) {
    if(vuorosana1[i] == ' ') {
      //looppi joka tekee tempstringiin stringin joka lisätään elementiksi vuorosanavec:iin
      tempstring = "";
      for(int j = 0; j <= i; j++) tempstring += vuorosana1[j];
      vuorosanavec.push_back(tempstring);
      //Poistetaan yhtä pitkä pätkä (se vektoriin lisätty) vuorosana1:stä, deletoidaan aina alusta merkki
      for(int k = 0; k <= i; k++) vuorosana1.erase(0,1);
      i = rivinpituus;
    }
  }
  //Edelliset loopit perustuu välilyöntiin, joten viimeinen sana jää yleensä yli ilman välilyöntiä ja pitää lisätä erikseen
  if(vuorosana1 != "" or vuorosana1 != " ") {
    vector<string>::iterator iter = --vuorosanavec.end();
    //    iter--;  postfix-- on siirretty tästä tuohon ylemmäs, näyttäisi toimivan
    //Mahtuuko viimeinen sana samalle riville
    if((*iter).length() + vuorosana1.length() <= rivinpituus) *iter += vuorosana1; //Tuohon *iterin ja vuorosana1:n vertailuu näyttää sisältyvän niiden välissä oleva välilyöntikin korrektisti
    else vuorosanavec.push_back(vuorosana1);
  }
  //DEBUG koko loppu
  //  cout << "autowrap output" << endl;
  //0  for(vector<string>::iterator clear_iter = vuorosanavec.begin(); clear_iter != vuorosanavec.end(); ++clear_iter) {cout << *clear_iter << endl;}
}

void tyhjenna_vektori() {
  for(vector<string>::iterator clear_iter = vuorosanavec.begin(); clear_iter != vuorosanavec.end(); ++clear_iter) {
    vuorosanavec.erase(clear_iter);
    --clear_iter;
  }
}

void puhekulmat(int bubblew, int bubblew2, int bubbleh, int bubbleh2) {  //jostain syystä nämä arvot ei tule oikein, jos niitä ei laita parametreinä
	SDL_SetRenderDrawColor(piirturi, 0xFF, 0xFF, 0xFF, 0xFF);
        //vasen yläkulma
	SDL_RenderDrawLine(piirturi, bubblew, (bubbleh + 11), bubblew, (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + 1), (bubbleh + 8), (bubblew + 1), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + 2), (bubbleh + 7), (bubblew + 2), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + 3), (bubbleh + 5), (bubblew + 3), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + 4), (bubbleh + 4), (bubblew + 4), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + 5), (bubbleh + 3), (bubblew + 5), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + 6), (bubbleh + 3), (bubblew + 6), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + 7), (bubbleh + 2), (bubblew + 7), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + 8), (bubbleh + 1), (bubblew + 8), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + 9), (bubbleh + 1), (bubblew + 9), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + 10), (bubbleh + 1), (bubblew + 10), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + 11), bubbleh, (bubblew + 11), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + 12), bubbleh, (bubblew + 12), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + 13), bubbleh, (bubblew + 13), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + 14), bubbleh, (bubblew + 14), (bubbleh + 15));
	//oikea yläkulma
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 1), (bubbleh + 11), (bubblew + bubblew2 - 1), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 2), (bubbleh + 8), (bubblew + bubblew2 - 2), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 3), (bubbleh + 7), (bubblew + bubblew2 - 3), (bubbleh + 15));	
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 4) , (bubbleh + 6), (bubblew + bubblew2 - 4), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 5), (bubbleh + 4), (bubblew + bubblew2 - 5), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 6), (bubbleh + 3), (bubblew + bubblew2 - 6), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 7), (bubbleh + 3), (bubblew + bubblew2 - 7), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 8), (bubbleh + 2), (bubblew + bubblew2 - 8), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 9), (bubbleh + 1), (bubblew + bubblew2 - 9), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 10), (bubbleh + 1), (bubblew + bubblew2 - 10), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 11), (bubbleh + 1), (bubblew + bubblew2 - 11), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 12), bubbleh, (bubblew + bubblew2 - 12), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 13), bubbleh, (bubblew + bubblew2 - 13), (bubbleh + 15));	
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 14), bubbleh, (bubblew + bubblew2 - 14), (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 15), bubbleh, (bubblew + bubblew2 - 15), (bubbleh + 15));	
	//vasen alakulma
	SDL_RenderDrawLine(piirturi, bubblew, (bubbleh + bubbleh2 - 10), bubblew, (bubbleh + 15));
	SDL_RenderDrawLine(piirturi, (bubblew + 1), (bubbleh + bubbleh2 - 9), (bubblew + 1), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + 2), (bubbleh + bubbleh2 - 7), (bubblew + 2), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + 3), (bubbleh + bubbleh2 - 6), (bubblew + 3), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + 4), (bubbleh + bubbleh2 - 5), (bubblew + 4), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + 5), (bubbleh + bubbleh2 - 4), (bubblew + 5), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + 6), (bubbleh + bubbleh2 - 4), (bubblew + 6), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + 7), (bubbleh + bubbleh2 - 3), (bubblew + 7), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + 8), (bubbleh + bubbleh2 - 2), (bubblew + 8), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + 9), (bubbleh + bubbleh2 - 2), (bubblew + 9), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + 10), (bubbleh + bubbleh2 -1), (bubblew + 10), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + 11), (bubbleh + bubbleh2 -1), (bubblew + 11), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + 12), (bubbleh + bubbleh2 -1), (bubblew + 12), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + 13), (bubbleh + bubbleh2 -1), (bubblew + 13), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + 14), (bubbleh + bubbleh2 -1), (bubblew + 14), (bubbleh + bubbleh2 - 16));
	//oikea alakulma
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 1), (bubbleh + bubbleh2 - 12), (bubblew + bubblew2 - 1), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 2), (bubbleh + bubbleh2 - 9), (bubblew + bubblew2 - 2), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 3), (bubbleh + bubbleh2 - 8), (bubblew + bubblew2 - 3), (bubbleh + bubbleh2 - 16));	
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 4) , (bubbleh + bubbleh2 - 7), (bubblew + bubblew2 - 4), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 5), (bubbleh + bubbleh2 - 5), (bubblew + bubblew2 - 5), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 6), (bubbleh + bubbleh2 - 4), (bubblew + bubblew2 - 6), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 7), (bubbleh + bubbleh2 - 4), (bubblew + bubblew2 - 7), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 8), (bubbleh + bubbleh2 - 3), (bubblew + bubblew2 - 8), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 9), (bubbleh + bubbleh2 - 2), (bubblew + bubblew2 - 9), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 10), (bubbleh + bubbleh2 -2), (bubblew + bubblew2 - 10), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 11), (bubbleh + bubbleh2 - 2), (bubblew + bubblew2 - 11), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 12), (bubbleh + bubbleh2 - 1), (bubblew + bubblew2 - 12), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 13), (bubbleh + bubbleh2 - 1), (bubblew + bubblew2 - 13), (bubbleh + bubbleh2 - 16));	
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 14), (bubbleh + bubbleh2 - 1), (bubblew + bubblew2 - 14), (bubbleh + bubbleh2 - 16));
	SDL_RenderDrawLine(piirturi, (bubblew + bubblew2 - 15), (bubbleh + bubbleh2 - 1), (bubblew + bubblew2 - 15), (bubbleh + bubbleh2 - 16));
}

void aseta_puhuja(int puhuja) {
  //  cout << "aseta_puhuja -funktion alku" << endl;
  //Nyt oletuksena on, että puhuja -arvot alkavat nollasta (size alkaa 1:stä)
  if(puhuja >= hahmovektori.size()) {
    cerr << "Liian suuri arvo aseta_puhuja -vektoriin" << endl;
  }
  //  cout << "aseta_puhuja -funktion keskiväli" << endl;
  vector<hahmoluokka*>::iterator p_iter = hahmovektori.begin()+puhuja;
  //  cout << "aseta_puhuja -funktion kohta x" << endl;
  kuppoint = &(*p_iter)->sijainti;
  //  cout << "aseta_puhuja -funktion loppu" << endl;
}

//Funktio, joka luo seuraavan (vektorin ensimmäisen) kuplan kuplavektorista. Määritellään kuplakohtaiset kuplan mitat (joiden arvot varsinaisesti annetaan tarinafunktiossa), kutsutaan autowrap jakamaan teksti, tarkistetaan, pitäisikö tekstin skrollata, vai mahtuuko se kuplaan
void kuplanluonti(int frame_nyt) {
  if(kuplavec.size() == 0) cerr << "Kuplavektori on tyhjä, vaikka yritetään luoda kuplaa" << endl;
  vector<kupla>::iterator kupiter = kuplavec.begin();
  tarina_kt = (*kupiter).timer;
  aseta_puhuja((*kupiter).kuka);
  vuorosana1 = (*kupiter).puhe;
  kuplanpituus = (*kupiter).pituus;
  kuplankorkeus = (*kupiter).korkeus;
  mikarivi.first = 1;
  mikarivi.second = 1;
  skrollausarvo = 0;
  kupasema = (*kupiter).asemointi;
  puhilme = (*kupiter).vilme;

  kuplanpituus == 0 ? tempkuppituus = OLETUSKUPLANPITUUS : tempkuppituus = kuplanpituus; //Tähän voi laittaa myös kuplanpituudet 1, 2, 3... jos niihin haluaa "vaihtoehtoisia oletuksia"
  rivinpituus = ((tempkuppituus - KUPVASBORDER - KUPOIKBORDER) / merkinpituus); //30 on molemmat borderit (2 x 15). borderi-muuttujan voi tietysti tehdä tarvittaessa erikseen.

  if(rivinpituus < vuorosana1.length()) kursoripaalla = 1;

  if(vuorosana1.size() > rivinpituus) autowrap(rivinpituus, vuorosana1);  //Rivinkatkaisufunkioon
  else {
    vuorosanavec.push_back(vuorosana1); //Paitsi jos vuorosana on riviä lyhyempi
    tempkuppituus = vuorosana1.length()*merkinpituus+40; //Tällöin myös kupla laitetaan lyhyemmäksi leveydeltään.
    if(tempkuppituus < 80) tempkuppituus = 80;
  }

  //jos kuplankorkeus on 1, käytetään oletusarvoja, jos 0, lasketaan sopiva korkeus
  if(kuplankorkeus == 1) tempkupkorkeus = OLETUSKUPLANKORKEUS;
  else if(((*kupiter).asemointi == 3) and kuplankorkeus == 0) {
    if(kuppoint->second+40 - ((merkinkorkeus+RIVIVALI)*vuorosanavec.size()+KUPYLABORDER+KUPALABORDER) < 0) tempkupkorkeus = kuppoint->second+40; //Jos sivuyläkupla ei mahdu ruutuun, korkeus on kattoon asti
    else tempkupkorkeus = (merkinkorkeus+RIVIVALI)*vuorosanavec.size()+KUPYLABORDER+KUPALABORDER; //Jos taas mahtuu, lasketaan siihen koko kuplan koko
  }
  else if(((*kupiter).asemointi == 5) and kuplankorkeus == 0) {
    if(kuppoint->second+20 + ((merkinkorkeus+RIVIVALI)*vuorosanavec.size()+KUPYLABORDER+KUPALABORDER) > SCREEN_HEIGHT) tempkupkorkeus = kuppoint->second+40; //Jos sivualakupla ei mahdu ruutuun, korkeus on kattoon asti
    else tempkupkorkeus = (merkinkorkeus+RIVIVALI)*vuorosanavec.size()+KUPYLABORDER+KUPALABORDER; //Kuten edellä
  }
  else if(((*kupiter).asemointi == 4) and kuplankorkeus == 0) {
    if(((merkinkorkeus+RIVIVALI)*vuorosanavec.size()+KUPYLABORDER+KUPALABORDER) > SCREEN_HEIGHT-20) tempkupkorkeus = SCREEN_HEIGHT-20; //Jos sivualakupla ei mahdu ruutuun, ei ehkä kuitenkaan ihan ruudun kokoista kuplaa
    else tempkupkorkeus = (merkinkorkeus+RIVIVALI)*vuorosanavec.size()+KUPYLABORDER+KUPALABORDER;
  }
  else if(kuplankorkeus == 0) tempkupkorkeus = (merkinkorkeus+RIVIVALI)*vuorosanavec.size()+KUPYLABORDER+KUPALABORDER;
  else tempkupkorkeus = kuplankorkeus;

  //Lasketaan mahdolliset rivit, jotka eivät mahdu kuplaan
  ylirivit = (static_cast<double>(vuorosanavec.size())*(static_cast<double>(merkinkorkeus)+static_cast<double>(RIVIVALI)) - (static_cast<double>(tempkupkorkeus) - static_cast<double>(KUPYLABORDER) - static_cast<double>(KUPALABORDER))) / (static_cast<double>(merkinkorkeus) + static_cast<double>(RIVIVALI));
  //Tähän: jos rivinpituus suurempi kuin vuorosanan pituus
  kuplantila = 2;
  pkupkounter = frame_nyt;
}

//Tekee uuden kupla-structin kupvektoriin. Jos ei laita paikka-, pituus- ja korkeusarvoja, tulee samat kuin edellisessä kuplavektorin repliikissä
void insert_kup(int puhuja, string asia, int timer_kt = 0, int vilmepar = 1, int pit = 999, int kor = 999, int paikka = 6, int vo = 3) {
  kupla temp;

  if(timer_kt != 0) temp.timer = timer_kt;
  if(vilmepar != 1) temp.vilme = vilmepar;

  if(puhuja == 99) {
    if(kuplavec.size() == 0) puhuja = 0; //Oletus
    else {
      vector<kupla>::iterator samiter = kuplavec.end()-1;
      temp.kuka = (*samiter).kuka;
    }
  }
  else temp.kuka = puhuja;
  
  if(vo != 3) kuplavo = vo;
  temp.puhe = asia;
  if(paikka == 6) {
    if(kuplavec.size() == 0) paikka = 1; //Oletus
    else {
      vector<kupla>::iterator samiter = kuplavec.end()-1;
      temp.asemointi = (*samiter).asemointi;
    }
  }
  else temp.asemointi = paikka;
  
  if(pit == 999) {
    if(kuplavec.size() == 0) pit = 0;
    else {
      vector<kupla>::iterator samiter = kuplavec.end()-1;
      temp.pituus = (*samiter).pituus;
    }
  }
  else temp.pituus = pit;
  
  if(kor == 999) {
    if(kuplavec.size() == 0) kor = 0;
    else {
      vector<kupla>::iterator samiter2 = kuplavec.end()-1;
      temp.korkeus = (*samiter2).korkeus;
    }
  }
  else temp.korkeus = kor;
  
  kuplavec.push_back(temp);
}

void tee_irc_rivi(int sekunnit, SDL_Texture *aika, string nick, string repliikki, string tyyppi = "normaali") {
  int rivinpituus = floor((double)SCREEN_WIDTH/(double)irc_MERKINPITUUS)-1;
  if(nick[0] != ' ' and nick[0] != '@') nick.insert(0, " ");
  int rivimaara = ceil((double)repliikki.length()/(double)rivinpituus);
  cout << "rivimaara: " << rivimaara << endl;
  SDL_Surface* pinta7 = TTF_RenderText_Solid(irc_fontti, nick.c_str(), ircvari);
  SDL_Texture* nimi = SDL_CreateTextureFromSurface(piirturi, pinta7);

  string tempstring = "";
  for(int i = 1; i <= rivimaara; i++) {
    for(int j = 0; j <= rivinpituus; j++) {
      tempstring += repliikki[j];
      if(tempstring.length() == repliikki.length()) break;
      if(i == 1 and tempstring.length()+nick.length()+8 == rivinpituus) break;
    }
    cout << "tempstring" << tempstring << endl;
    SDL_Surface* pinta6 = TTF_RenderText_Solid(irc_fontti, tempstring.c_str(), ircvari);
    SDL_Texture* repla = SDL_CreateTextureFromSurface(piirturi, pinta6);
    irc_teksturet.push_back(generic_irc_rivi);
    vector<irc_rivi>::iterator irc_iter = --irc_teksturet.end();
    i == 1 ? (*irc_iter).aikaleima = aika : (*irc_iter).aikaleima = NULL; 
    (*irc_iter).nikki = nimi;
    (*irc_iter).sanoma = repla;
    (*irc_iter).vpituus = tempstring.length();
    (*irc_iter).npituus = nick.length();
    (*irc_iter).ajastin = sekunnit;
    for(int k = 0; (i == 1 ? k <= rivinpituus-nick.length()-9 : k <= rivinpituus); k++) repliikki.erase(0,1); //Ensimmäisellä rivillä katkotaan enemmän. 8 = etumerkit.
    tempstring = "";

  }
  //Alusta rivejä pois, jos ruutu tulee täyteen
  while(irc_teksturet.size() >= floor((double)SCREEN_HEIGHT/(double)(rivimaara*irc_MERKINKORKEUS-3*irc_MERKINKORKEUS))) {
    vector<irc_rivi>::iterator eraser = irc_teksturet.begin();
    irc_teksturet.erase(eraser);
  }
}

//Tästä ehkä int, joka palauttaa nollan, jos ei löytynyt sitä kohtaa, mihin inkrementoitiin
//zxc
bool tarinafunktio() {
  bool loytyiko = true;
  //ELSEIFFEINÄ KAIKKI (paitsi ylin if ja lopussa else)
  //Loppuun else, joka laittaa bool-arvon, että ei löytynyt mitään

  if(tarinanvaihe == 5) {
    tarinakeskeytys=true;
  }
  else if(tarinanvaihe == 100) {
    hahmoja = true;
    taustakuva = koti;
    koira1.ruudulla = 3;
    koira1.sijainti.first = 240;
    koira1.sijainti.second = 225;
    koira1.rintamasuunta = 4;

    poyta.ruudulla = 3;
    poyta.sijainti.first = 816;
    poyta.sijainti.second = 274;
    
    kissa.ruudulla = 4;
    kissa.sijainti.first = 800; //debug-arvo: 270
    kissa.sijainti.second = 175;

    mayra.ruudulla = 2; //oli 2
    mayra.sijainti.first = 110;
    mayra.sijainti.second = 230;
    mayra.rintamasuunta = 6;
      
    koira2.ruudulla = 4;
    koira2.flip = true;
    koira2.sijainti.first = 950;
    koira2.sijainti.second = 170; //170 & 200

    naata.ruudulla = 4;
    naata.sijainti.first = 670;
    naata.sijainti.second = 170;

    tarinakeskeytys = true;
  }

 else if(tarinanvaihe == 101) {
    kuplantila = 1;
    insert_kup(1, "Hei, Morpheus! Minä olen Tepsam0n ...tai siis ihan vaan Tepsa.");
    insert_kup(4, "Hei, oikea nimeni on Erno.");
    insert_kup(1, "Saavuitkin ajoissa paikalle.");
    insert_kup(4, "Bussi oli vähän myöhässä, mutta talo löytyi helposti.");
    insert_kup(1, "Tule vain peremmälle!");
  }
  else if(tarinanvaihe == 105) {
    koira1.waypoint1.second = 240;
    koira1.waypoint2.first = 640;
    koira1.waypoint2.second = 320;
    koira1.waypoint3.first = 1100;
    koira1.waypoint3.second = 320;
    koira1.waypoint4.first = 1100;
    koira1.waypoint4.second = 264;
    mayra.waypoint1.first = 300;
    mayra.waypoint1.second = 280;
    mayra.waypoint2.first = 655;
    mayra.waypoint2.second = 295;
    //    mayra.rintamasuunta = 4;
    kamskrolli = 2;
    skrollihalt = 530; //Maksimiarvo tälle on kuvan koko X-akselilla miinus SCREEN_WIDTH (eli 1280 tässä produktiossa, jossa 1960 on taustakuvan X-koko)
  }
  else if(tarinanvaihe == 107) {
    kuplantila = 1;
    insert_kup(1, "Tässä ovat siis Hanna-Kaisa...");
    insert_kup(2, "Hei!");
    insert_kup(1, "Valtsu...");
    insert_kup(0, "Hei!");
    insert_kup(1, "...ja Sirkku");
    insert_kup(3, "Hei!");
    insert_kup(1, "Muistat varmaan heidän nickinsä.");
    insert_kup(4, "Joo");
  }
  else if(tarinanvaihe == 108) {
    tarina_t = 2;
    //Rest of the story removed from the sample code
  }

void kuplanpiirto(int frame_nyt) {
  //jos kuplan lähtöpiste + pituus on suurempia kuin screen_width -> kupla vasemmalle
  //jos kuplan lähtöpiste on alle nollan -> kupla oikealle
  //Nämä kuplan lähtöpisteet pitää määritellä aina tässä funktiossa, jos halutaan, että hahmo voi liikkua puhuessaan
  //Kupla vasemmalle vai oikealle päin hahmosta
  bubblew = kuppoint->first-kamerax1;
  if(kuplavo == 2) {
    if(kupasema > 2) bubblew += 55+30; //55 on tuossa noin hahmonspriten leveys ja 30 osoitin ja muu väli
    //pelkkä kuplavo == 2 ilman sivuX:aa ei vaadi käsittelyä
  }
  else if (kuplavo == 1) {
    if(kupasema > 2) bubblew -= tempkuppituus+30;
    else bubblew -= tempkuppituus-60; //60 on suunnilleen hahmon leveys eli vasemmallekin osoittava kupla alkaa hahmon pään toiselta puolelta
  }
  else { //jos kuplavo == 0
    //jos hahmo on ruudun oikealla puolella, kupla vasemmalle ja toisin päin
    if(bubblew > SCREEN_WIDTH/2) {
      if(kupasema > 2) bubblew -= tempkuppituus+30;
      else bubblew -= tempkuppituus-60;
    }
    else if(kupasema > 2) bubblew += 55+30; //55 on tuossa noin hahmonspriten leveys ja 30 osoitin ja muu väli
    //pelkkä oikealla oleva ylä/alakupla ei vaadi käsittelyä
  }

  //Jos kupla menee yli ruudusta, se siirtyy sen verran, että mahtuu ruutuun
  if(bubblew+tempkuppituus > SCREEN_WIDTH) bubblew = SCREEN_WIDTH-tempkuppituus;
  else if(bubblew < 0) bubblew = 0;

  if(kupasema == 1) bubbleh = (kuppoint->second)-tempkupkorkeus-30; //-30 on lisäkorkeus, jota ilman kupla alkaisi suoraan hahmon päältä ja osoite uppoaisi liian alas
  else if(kupasema == 3) bubbleh = (kuppoint->second)-tempkupkorkeus+40; //sivuyläkuplan alalaita on jossain suun paikkeilla (+40) ja siitä miinus kuplankorkeus
  else if(kupasema == 5) bubbleh = (kuppoint->second)+20; //sivualakuplan ylälaita on jossain silmien kohdalla (+20)
  else if(kupasema == 4) bubbleh = (kuppoint->second)+20-(tempkupkorkeus/2); //sivukeskikuplan ylälaita on silmänkorkeudelta (+20) ja miinus puolet kuplankorkeudesta
  else bubbleh = (kuppoint->second)+150; //ala-asemassa kupla lähtee jostain hahmon jalkojen alta

  //sivuy: kuplan alalaita on vähän hahmon suun yläpuolella eli miinuksen sijasta plussaa
  
  bubblew2 = tempkuppituus; //oli 360, tämä on siis kuplan leveys (ml. borderit). Kuplanpituus myöhemmin tulee ilmeisesti tähän kohtaan määriteltäväksi, kunhan autowrapistä tehdään oma funktionsa
  bubbleh2 = tempkupkorkeus; //kuplan korkeus ylälaidasta laskien
  cliprect = {bubblew, bubbleh, bubblew2, static_cast<int>((KUPYLABORDER+(vuorosanavec.size() - ceil(ylirivit))*(merkinkorkeus+RIVIVALI)))};
  kupla1 = {bubblew, (bubbleh + 15), bubblew2, bubbleh2 - 30}; //kuplan leveämpi pala
  kupla2 = {(bubblew + 15), bubbleh, (bubblew2 - 30), bubbleh2}; //kuplan korkeampi pala

  SDL_RenderFillRect(piirturi, &kupla1); SDL_RenderFillRect(piirturi, &kupla2); //Piirretään puhekuplan molemmat kappaleet (suorapalkit). &-merkin avulla saadaan kuplan muistiosoite arvojen sijaan, koska SDL_Rect palauttaa pointterin eikä arvoja
  osoitin(bubblew, bubbleh);

  puhekulmat(bubblew, bubblew2, bubbleh, bubbleh2);  //Nämä kuplan kulmat rendataan ennen tekstiä, muuten tulevat tekstin päälle

  if(kuplantila == 3 or ylirivit > 0) {
    //Tekstiskrollia varten tarvitaan koko puhekuplan sisältävä neliö, jota ei leikata (jotta teksti leikkautuu skrollatessaan poistuessaan tältä alueelta)
    SDL_RenderSetClipRect(piirturi, &cliprect);  //Tässä funktiossa rect on const pointer. se ei kuitenkaan tarkoita, että objektin pitäisi olla const vaan funktio ei muuta sitä objektia. referenssinä argumentin laittaminen tekee siitä ilmeisesti käytännössä pointerin (pointteri ja referenssi on melko samanlaisia). kupla1 ei ole aivan sopivan kokoinen recti ilmeisesti tekstin feidaamiseen, joten sen tilalle voidaan laittaa toinen, vähän korkeampi, niin näyttäisi ehkä paremmalta
  }

  for(vector<string>::iterator renditer = vuorosanavec.begin(); renditer != vuorosanavec.end(); ++renditer) {
    tekstirend(bubblew, bubbleh, distance(vuorosanavec.begin(), renditer)+1, *renditer, fontti);
  }

  if(kuplantila == 3 or ylirivit > 0) SDL_RenderSetClipRect(piirturi, NULL);  //if on tässä vain, että vältetään turha funktiokutsu. kai sen voisi muuten NULLata aina
  //rivimuuttuja ja kirjainmuuttuja (monesko kirjain)

  
  if(kursoripaalla == 1) { //kursoripäällä voisi kyllä sisältyä puhekupla-muuttujaankin
    if(onkoriviloppu(mikarivi.second, moneskirjain) and kursoripaalla < 3) {  //Jos rivi loppuu, siirrytään seuraavalle riville ja rivin alkuun ...jos kursori ei ole maalissa
      if(mikarivi.second == vuorosanavec.size() and onkoriviloppu(mikarivi.second, moneskirjain) and kursorikountteri < 4) {  //Jos ollaan vikalla rivillä ja vikassa kirjaimessa.
	kursoripaalla = 3;
      }
      else {  //jos rivi on loppu ja kursori ei ole vielä maalissa, laitetaan kursori rivin alkuun
	mikarivi.first++;
	mikarivi.second++;
	moneskirjain = 0;
      }
    }
  }

  //mikarivi.first & mikarivi.second -muuttujat kasvaa aina yhdellä rivin loputtua, jos on uusi rivi tulossa
  //string kursorimerkki = (mikarivi.first, moneskirjain);
  if(ylirivit > 0 and kursoripaalla == 1 and mikarivi.first-1 == vuorosanavec.size()-ceil(ylirivit)) kursoripaalla = 2;
  if(kursoripaalla == 2) {
    skrollausarvo += 2;
    //jos skrollausarvon kanssa huomioitu korkeus on sellainen, että vika rivi on näkyvissä, sitten kursoripaalla = true ja mikarivi = ensimmäinen piilossa ollut, skrollausarvo = 0
    if(bubbleh-skrollausarvo+KUPYLABORDER+vuorosanavec.size()*(RIVIVALI+merkinkorkeus) <= bubbleh+KUPYLABORDER+(vuorosanavec.size() - ceil(ylirivit))*(merkinkorkeus+RIVIVALI)) { //Tässä hyvin oleellista on, että vertailun oikeanpuoleinen y-koordinaatti on laskettu samalla logiikalla kuin cliprectin korkeus
      kursoripaalla = 1;
      //skrollin jälkeinen rivi on kuplaan mahtuvien rivien määrä miinus uusien rivien määrä plus yksi
      mikarivi.first = ((tempkupkorkeus - KUPYLABORDER - KUPALABORDER) / (merkinkorkeus + RIVIVALI) - ceil(ylirivit)) + 1;
    }
  }
  
  if(kursoripaalla == 1) moneskirjain++; //jos kursori ei ole maalissa, lisätään rivi
  else if(kursoripaalla > 2 and frame_nyt%10 == 0) {kursoripaalla = 3; kursorikountteri++;}
  else if(kursoripaalla > 2 and frame_nyt%10 == 5) kursoripaalla = 4;
  if(kursoripaalla % 2 != 0) {kursorirend(bubblew, bubbleh, mikarivi.first, (kursoripaalla > 2 ? moneskirjain+1 : moneskirjain), (kursoripaalla == 3 ? " " : (kursorikirjain(mikarivi.second, moneskirjain))), fontti);}
  if(kursorikountteri == 4) {kursoripaalla = 0; kuplantila = 3;}  //Kun on välkytty neljä kertaa, lopetetaan. HUOMHUOM!! Lopullisessa koodissa kursorikountterikin pitää resetoida, mutta tässä koodissa sitä ei tarvitse (vaan se aiheuttaa itse asiassa kursorin palaamisen)
  //pinta2:sta (vuorosana1) otettu tekstinleveys on 1890 ja korkeus 15. merkkejä siinä on 269 eli 7 tuntuisi olevan merkinleveys.
  //if(tekstiskrolli == true and frame_nyt%10 < 9) {skrollausarvo++;}
  if(kuplantila == 3 and frame_nyt%10 <= 20) skrollausarvo +=10;  // tuo <= 9 ei hidasta ollenkaan eli hidastus on nyt pois päältä. Alemmat arvot hidastavat. HUOM! tässä voi nopeuttaa skrollausta myös lisäämällä skrollausarvoon ykköstä suurempia määriä

  if(kursoripaalla == 0 and pkupkounter+35 <= frame_nyt) kuplantila = 3;
  
  //Jos tekstin poistava tekstiskrolli on vienyt koko tekstin kuplan ylärajan yläpuolelle
  if(kuplantila == 3 and bubbleh-skrollausarvo+KUPYLABORDER+static_cast<signed int>(vuorosanavec.size())*(RIVIVALI+merkinkorkeus) <= bubbleh) {
    kursorikountteri = 0;
    pkupkounter = 0;
    moneskirjain = 0;
    tyhjenna_vektori();
    if(puhilme != 1) puhilme = 1;
    if(tarina_kt != 0) {
      tarina_t = tarina_kt*25;
      tarina_kt = -1;
      kuplantila = 6;
    }
    if(kuplantila != 6) {
      vector<kupla>::iterator eriter = kuplavec.begin();
      kuplavec.erase(eriter);
    }
    if(kuplavec.size() > 0 and tarina_kt == 0) kuplanluonti(frame_nyt);
    else if(tarina_kt == 0) {
      kuplantila = 0;
      if(kesktark()) tarinakeskeytys = true;
    }
  }
}

bool kesktark() {
  bool kesk = true;
  /*
  if(kuplavec.size() > 0) {
    kesk = false;
    return kesk;
  }
  */
  if(tarina_t != 0) {
    kesk = false;
    return kesk;
  }
  //Puhuuko joku
  if(kuplantila != 0 and kuplantila != 8) {
    kesk = false;
    return kesk;
  }
  //Skrollaako tausta?
  if(kamskrolli != 0) {
    kesk = false;
    return kesk;
  }
  //Ovatko kaikki hahmot pysähdyksissä?
  for(vector<hahmoluokka*>::iterator way_iter = hahmovektori.begin(); way_iter != hahmovektori.end(); ++way_iter) {
    if(((*way_iter)->waypoint1.first != 0 or (*way_iter)->waypoint1.second != 0)) {
      kesk = false;
      return kesk;
    }
  }
  cout << "KESKTARK!!!!!!!" << " " << kesk << endl;
  return kesk;
}


int main(int argc, char *args[]) {

  //  kissa.symmetrinen = false;
  tyhjenna_vektori();
  int frame_nyt = 0;
  bool feidaus = false; //Vai pitäisikö olla int, jolloin eri numeroilla saisi eri nopeuksia
  bool liikutaan = false;
  Uint8 alpha = 0;
  Uint32 ajastin1 = 0;
  Uint32 ajastin2 = 0;
  Uint32 ajastin_fr = 0;  //kävelyframejen nopeus
  Uint32 ajastin_r = 0;
  Uint32 irc_ajastin = 0;
  bool irc_sekunti = false;
  int rcount = 0;
  SDL_Rect irc_ylapalkki = {0, 0, SCREEN_WIDTH, irc_MERKINKORKEUS};  //Näihin palkkeihin ei taida tulla riviväliä
  SDL_Rect irc_alapalkki = {0, SCREEN_HEIGHT-2*irc_MERKINKORKEUS, SCREEN_WIDTH, irc_MERKINKORKEUS};

  /*
  hahmovektori.push_back(&koira2);
  hahmovektori.push_back(&koira1);
  hahmovektori.push_back(&kissa);
  hahmovektori.push_back(&naata);
  hahmovektori.push_back(&mayra);
  hahmovektori.push_back(&kissak);
  hahmovektori.push_back(&drops);
  */
  
  //Käynnistetään SDL ja luodaan ikkuna
  if(!init()) {
    cerr << "Init fail" << endl;
  }
  else {
    //Ladataan media
    if(!lataamedia()) {
    cerr << "Media load fail" << endl;
    }

    else {

      kissa.arkit();  //Tämän funktion voi kutsua minkä tahansa hahmoluokan olion kautta
      
      kissa.yhdista_tekstuuri(&kissateks);
      kissa.yhdista_rect(kissa_arkki, (sizeof(kissa_arkki)/sizeof(*kissa_arkki)), sizeof(kissa_arkki));

      kissak.yhdista_tekstuuri(&kissakteks);
      kissak.yhdista_rect(kissak_arkki, (sizeof(kissak_arkki)/sizeof(*kissak_arkki)), sizeof(kissak_arkki));
      
      koira1.yhdista_tekstuuri(&koirateks1);
      koira1.yhdista_rect(koira1_arkki, (sizeof(koira1_arkki)/sizeof(*koira1_arkki)), sizeof(koira1_arkki));
      
      koira2.yhdista_tekstuuri(&koirateks2);
      koira2.yhdista_rect(koira2_arkki, (sizeof(koira2_arkki)/sizeof(*koira2_arkki)), sizeof(koira2_arkki)); //Kun array lähetetään funktioon (ensimmäinen parametri), se on pointteri arrayn ensimmäiseen elementtiin, toisessa parametrissä lasketaan arrayn pituus eli se arkkien määrä, kolmas parametri on arrayn koko tavuina (?)

      naata.yhdista_tekstuuri(&naatateks);
      naata.yhdista_rect(naata_arkki, (sizeof(naata_arkki)/sizeof(*naata_arkki)), sizeof(naata_arkki));

      mayra.yhdista_tekstuuri(&mayrateks);
      mayra.yhdista_rect(mayra_arkki, (sizeof(mayra_arkki)/sizeof(*mayra_arkki)), sizeof(mayra_arkki));

      drops.yhdista_tekstuuri(&dropsteks);
      drops.yhdista_rect(drops_arkki, (sizeof(drops_arkki)/sizeof(*drops_arkki)), sizeof(drops_arkki));

      poyta.tekstuuri = &poytateks;
      kpoyta.tekstuuri = &kpoytateks;
      rv.tekstuuri = &rvteks;
      tuoli.tekstuuri = &tuoliteks;
      ruoka.tekstuuri = &ruokateks;
      
      bool quit = false; //Pääloopin lippu
      SDL_Event e; //Event handler

      
      ///usr/share/fonts/truetype/ttf-dejavu/DejaVuSansMono-Bold.ttf
      ///usr/share/matplotlib/mpl-data/fonts/ttf/DejaVuSansMono-Bold.ttf

         //Thinkcentre
	 fontti = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 15);  //Parameters: path to the font, font size. Jos polku on väärin, tulee segmentation fault
	 irc_fontti = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 15);  //Parameters: path to the font, font size. Jos polku on väärin, tulee segmentation fault

	 
	 //Thinkpad
      //fonttikoko oli 12
      //fontti = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSansMono-Bold.ttf", 15);  //Parameters: path to the font, font size. Jos polku on väärin, tulee segmentation fault
      //irc_fontti = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSansMono-Bold.ttf", 15);  //Parameters: path to the font, font size. Jos polku on väärin, tulee segmentation fault

      SDL_Surface* kello1 = TTF_RenderText_Solid(irc_fontti, "18:41", {255, 255, 255});
      SDL_Surface* kello2 = TTF_RenderText_Solid(irc_fontti, "18:42", {255, 255, 255});
      SDL_Surface* kello3 = TTF_RenderText_Solid(irc_fontti, "18:43", {255, 255, 255});
      SDL_Surface* pinta4 = TTF_RenderText_Solid(irc_fontti, " <", {70, 70, 70});
      SDL_Surface* pinta5 = TTF_RenderText_Solid(irc_fontti, ">", {70, 70, 70});
      aika_leveys = kello1->w;
      cout << "kello w & h" << kello1->w << " " << kello1->h << endl;
      aika_leveys = kello1->w;
      //      as_leveys = pinta4->w; //Käytetäänkö näitä???
      //      ks_leveys = pinta5->w; //Hetkinen, eikö tähän riitä merkinpituus!!!!???? Ja eikö nämä muutenkin saa laskettua merkinpituuden pohjalta muillekin?
      //  int tekstinkorkeus = pinta2->h;  //Tämä onkin sama kuin merkinkorkeus --> ei ilmeisesti tarvita
      aika1 = SDL_CreateTextureFromSurface(piirturi, kello1);
      aika2 = SDL_CreateTextureFromSurface(piirturi, kello2);
      aika3 = SDL_CreateTextureFromSurface(piirturi, kello3);
      SDL_Texture* sulut_auki = SDL_CreateTextureFromSurface(piirturi, pinta4);
      SDL_Texture* sulut_kiinni = SDL_CreateTextureFromSurface(piirturi, pinta5);
  //  SDL_Rect  = {bw+5, bh+rivinasetus-skrollausarvo+RIVIVALI*(moneskorivi-1), leveys, irc_MERKINKORKEUS}; //skrollausarvo lisätty text_scroll.cpp:hen. Onko tässäkin 5 rako kuplan yläreunan ja tekstin välillä?

      //Ohjelman ollessa päällä
      while(!quit) {
	ajastin1 = SDL_GetTicks();

	//Käsittele jonon tapahtumat
	while(SDL_PollEvent(&e)!=0) {
	  //käyttäjä quittaa
	  if(e.type == SDL_QUIT) {
	    quit = true;
	  }
	}

	if(tarinakeskeytys) {
  	  tarinakeskeytys = false;
	  do tarinanvaihe++; while (!tarinafunktio());
	  if(tarina_t > 0 and tarina_kt == 0) tarina_t *= 25;
	}

	/*
	cout << "mayra flip: " << mayra.flip << endl;
	cout << "kissa flip: " << kissa.flip << endl;
	cout << "koira1 flip: " << koira1.flip << endl;
	cout << "koira2 flip: " << koira2.flip << endl;
	cout << "naata flip: " << naata.flip << endl;
	*/
	//	cout << "koira1.ruudulla: " << koira1.ruudulla << endl;
	//	cout << "koira1.wp1-4muutos: " << koira1.wp1muutos << koira1.wp2muutos << koira1.wp3muutos << koira1.wp4muutos << endl;
	
	//Ruudun tyhjennys
      	SDL_SetRenderDrawColor(piirturi, 0x0, 0x0, 0x0, 0x0);
	SDL_RenderClear(piirturi);
      	SDL_SetRenderDrawColor(piirturi, 0xFF, 0xFF, 0xFF, 0xFF);

	//Renderoi tausta, taustan liike, hahmojen sijaintien päivitys
	if(taustakuva.tekstuuri != NULL) {
	  if(kamskrolli != 0) {
	    //	    cout << kamerax1 << " " << kamerax2 << endl;
	    //	    kamskrolli == 2 ? kamrect = {(kamerax1 += 3), 0, (kamerax2 += 3), SCREEN_HEIGHT} : kamrect = {(kamerax1 -= 3), 0, (kamerax2 -= 3), SCREEN_HEIGHT} ;
   	    kamskrolli == 2 ? kamrect = {(kamerax1 += 3), 0, SCREEN_WIDTH, SCREEN_HEIGHT} : kamrect = {(kamerax1 -= 3), 0, SCREEN_WIDTH, SCREEN_HEIGHT} ;

	    //	    if(kamskrolli == 2 and kamerax2 >= skrollihalt) { //Jos skrollataan oikealle, pysähdytään kun kamerax2 ylitetään ja laitetaan sen mukaan uusi paikka
	    if(kamskrolli == 2 and kamerax1 >= skrollihalt) {
	      //	      kamerax2 = skrollihalt;
	      //	      	      kamerax1 = skrollihalt - SCREEN_WIDTH;
	      kamerax1 = skrollihalt;
	      kamskrolli = 0;
	      //	      kamrect = {(kamerax1), 0, (kamerax2), SCREEN_HEIGHT};
	      kamrect = {(kamerax1), 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	    }
	    else if(kamskrolli == 1 and kamerax1 <= skrollihalt) {
	      kamerax1 = skrollihalt;
	      //	      kamerax2 = skrollihalt+SCREEN_WIDTH;
	      kamskrolli = 0;
	    }
	  }
	  //	  taustakuva.renderfunktio(0,0, &kamrect);
	  taustakuva.renderfunktio(0,0, &kamrect);
	}

	
	if(feidaus) {
	  alpha += 3;
	  if(alpha == 255) {tarinakeskeytys = true; alpha = 0; feidaus = false;}
	  pimee.renderfunktio(0,0);       	  
	}
	//	SDL_RenderDrawPoint(piirturi, 20, bubbleh-skrollausarvo+KUPYLABORDER+vuorosanavec.size()*(RIVIVALI+merkinkorkeus));
	//	SDL_RenderDrawPoint(piirturi, 20, bubbleh+bubbleh2-KUPALABORDER);

	if(IRC) {
	  if(irc_sekunti) {
	    for(vector<irc_rivi>::iterator seku_iter = irc_teksturet.begin(); seku_iter != irc_teksturet.end(); ++seku_iter) {
	      if(seku_iter->ajastin > 0) seku_iter->ajastin--;
	    }
	    irc_sekunti = false;
	  }
	  
	  SDL_SetRenderDrawColor(piirturi, 0x00, 0x00, 0xFF, 0xFF);
 	  SDL_RenderFillRect(piirturi, &irc_ylapalkki);
	  SDL_RenderFillRect(piirturi, &irc_alapalkki);

	  int rivi = irc_MERKINKORKEUS;
	  int monesrivi = 1;

	  //Tähän pitäisi lisätä irc_RIVIVALI
	  for(vector<irc_rivi>::iterator irc_rendit = irc_teksturet.begin(); irc_rendit != irc_teksturet.end(); ++irc_rendit) {
	    if(irc_rendit -> aikaleima != NULL and irc_rendit -> ajastin == 0) {
	      SDL_Rect rect1 = {0, monesrivi*irc_MERKINKORKEUS, 5*irc_MERKINPITUUS, irc_MERKINKORKEUS}; //5 == aikaleiman merkkien lukumäärä (esim. 15:03)
	      SDL_RenderCopy(piirturi, irc_rendit->aikaleima, NULL, &rect1);
	      SDL_Rect rect5 = {5*irc_MERKINPITUUS, monesrivi*irc_MERKINKORKEUS, 2*irc_MERKINPITUUS, irc_MERKINKORKEUS};
	      SDL_RenderCopy(piirturi,sulut_auki, NULL, &rect5);
      	      SDL_Rect rect2 = {7*irc_MERKINPITUUS, monesrivi*irc_MERKINKORKEUS, irc_rendit->npituus*irc_MERKINPITUUS, irc_MERKINKORKEUS};
	      SDL_RenderCopy(piirturi, irc_rendit->nikki, NULL, &rect2);
	      SDL_Rect rect6 = {irc_rendit->npituus*irc_MERKINPITUUS+7*irc_MERKINPITUUS, monesrivi*irc_MERKINKORKEUS, 1*irc_MERKINPITUUS, irc_MERKINKORKEUS};
	      SDL_RenderCopy(piirturi,sulut_kiinni, NULL, &rect6);
	    }
	    if(irc_rendit -> aikaleima == NULL and irc_rendit -> ajastin == 0) {
	      SDL_Rect rect3 = {0, monesrivi*irc_MERKINKORKEUS, irc_rendit->vpituus*irc_MERKINPITUUS, irc_MERKINKORKEUS};
	      SDL_RenderCopy(piirturi, irc_rendit->sanoma, NULL, &rect3);
	    }
	    else if (irc_rendit -> ajastin == 0) {
	      SDL_Rect rect4 = {irc_rendit->npituus*irc_MERKINPITUUS+9*irc_MERKINPITUUS, monesrivi*irc_MERKINKORKEUS, irc_rendit->vpituus*irc_MERKINPITUUS, irc_MERKINKORKEUS};
	      SDL_RenderCopy(piirturi, irc_rendit->sanoma, NULL, &rect4);
	    }
	    monesrivi++;
	    //	    SDL_SetRenderDrawColor(piirturi, 0xFF, 0x00, 0x00, 0xFF);
	    //	    SDL_RenderDrawPoint(piirturi, 40, monesrivi*irc_MERKINKORKEUS);
	    //    	    SDL_RenderDrawPoint(piirturi, 39, monesrivi*irc_MERKINKORKEUS);
	    //	    SDL_RenderDrawPoint(piirturi, 41, monesrivi*irc_MERKINKORKEUS);
	    //	    SDL_RenderDrawPoint(piirturi, 40, monesrivi*irc_MERKINKORKEUS+1);
	    //	    SDL_RenderDrawPoint(piirturi, 40, monesrivi*irc_MERKINKORKEUS-1);
    	  }
	}

	//Hahmot
	if(hahmoja) {
          if(SDL_GetTicks() - ajastin_fr > 57) { //Mitä suurempi tuo viimeinen luku, sitä hitaammin kävelyframet vaihtuvat
	    ajastin_fr = SDL_GetTicks();
	    liikutaan = true;
	  }
	  if(drop) {
	    //	    cout << "DROP" << endl;
	    if(SDL_GetTicks() - ajastin_r > 80) {
	      //	      cout << "DROP2" << endl;
	      ajastin_r = SDL_GetTicks();
	      drops.frame += 1;
	      if(drops.frame == 16) drops.frame = 0;
	    }
	  }
          if(ruokaa > 0) {
	    if(SDL_GetTicks() - ajastin_r > 130) {
	      ajastin_r = SDL_GetTicks();
	      if(rcount == 0) mayra.frame = 24;
	      if(rcount == 1) mayra.frame = 25;
	      if(rcount == 2) mayra.frame = 26;
	      if(rcount == 3) mayra.frame = 27;
	      if(rcount == 4) mayra.frame = 26;
	      if(rcount == 5) mayra.frame = 25;
	      rcount++;
	      if(rcount == 6) rcount = 0;
	      if(ruokaa == 2) {
		ruokaa = 0;
		mayra.frame = 0;
	      }
	    }
	  }


	  
	  /*
  	  if(kamskrolli > 0) {
	    for(vector<hahmoluokka*>::iterator skroliter = hahmovektori.begin(); skroliter != hahmovektori.end(); ++skroliter) {
	      kamskrolli == 1 ? (*skroliter)->sijainti.first += 3 : (*skroliter)->sijainti.first -= 3;
	    }
	    for(vector<esineluokka*>::iterator skroliter2 = esinevektori.begin(); skroliter2 != esinevektori.end(); ++skroliter2) {
	      kamskrolli == 1 ? (*skroliter2)->sijainti.first += 3 : (*skroliter2)->sijainti.first -= 3;
	    }
	  }
	  */

	  //	  for(vector<string>::iterator ruudulla1 = hahmovektori.begin(); ruudulla1 != vuorosanavec.end(); ++ruudulla1) {
	  //	    if((&ruudulla1)->ruudulla == 1) (&ruudulla1).hahmonpiirto(liikutaan, frame_nyt);
	  //	  }

	  //UUSI PÄTKÄ!! (muutettu myös luokan konstruktoria ja tehty siihen se funktio)

	  //hahmovektori.push_back(&koira2);
	  for(vector<esineluokka*>::iterator r4_iter2 = esinevektori.begin(); r4_iter2 != esinevektori.end(); ++r4_iter2) {
	    if((*r4_iter2)->ruudulla == 4) (**r4_iter2).esineenpiirto();
	  }
	  for(vector<hahmoluokka*>::iterator r4_iter = hahmovektori.begin(); r4_iter != hahmovektori.end(); ++r4_iter) {
	    if((*r4_iter)->ruudulla == 4) (**r4_iter).hahmonpiirto(liikutaan, frame_nyt);
	  }
	  for(vector<esineluokka*>::iterator r3_iter2 = esinevektori.begin(); r3_iter2 != esinevektori.end(); ++r3_iter2) {
	    if((*r3_iter2)->ruudulla == 3) (**r3_iter2).esineenpiirto();
	  }
	  for(vector<hahmoluokka*>::iterator r3_iter = hahmovektori.begin(); r3_iter != hahmovektori.end(); ++r3_iter) {
	    if((*r3_iter)->ruudulla == 3) (**r3_iter).hahmonpiirto(liikutaan, frame_nyt);
	  }
	  for(vector<esineluokka*>::iterator r2_iter2 = esinevektori.begin(); r2_iter2 != esinevektori.end(); ++r2_iter2) {
	    if((*r2_iter2)->ruudulla == 2) (**r2_iter2).esineenpiirto();
	  }
	  for(vector<hahmoluokka*>::iterator r2_iter = hahmovektori.begin(); r2_iter != hahmovektori.end(); ++r2_iter) {
	    if((*r2_iter)->ruudulla == 2) (**r2_iter).hahmonpiirto(liikutaan, frame_nyt);
	  }
	  for(vector<esineluokka*>::iterator r1_iter2 = esinevektori.begin(); r1_iter2 != esinevektori.end(); ++r1_iter2) {
	    if((*r1_iter2)->ruudulla == 1) (**r1_iter2).esineenpiirto();
	  }
	  for(vector<hahmoluokka*>::iterator r1_iter = hahmovektori.begin(); r1_iter != hahmovektori.end(); ++r1_iter) {
	    if((*r1_iter)->ruudulla == 1) (**r1_iter).hahmonpiirto(liikutaan, frame_nyt);
	  }
	  
	  /*
	  //VANHA!!
	  //if(kissa.ruudulla == 1) koira2.hahmonpiirto(liikutaan, frame_nyt);
	  if(koira2.ruudulla == 1) koira2.hahmonpiirto(liikutaan, frame_nyt);
	  
	  //if(kissa.ruudulla == 2) koira2.hahmonpiirto(liikutaan, frame_nyt);
  	  if(koira2.ruudulla == 2) koira2.hahmonpiirto(liikutaan, frame_nyt);
	  
 	  if(koira2.ruudulla == 3) koira2.hahmonpiirto(liikutaan, frame_nyt);
	  
	  if(koira2.ruudulla == 4) koira2.hahmonpiirto(liikutaan, frame_nyt);
	  //VANHA LOPPUU

	  //if(koira2.ruudulla) koira2.spritet->renderfunktio(0,0);  //koira2.spritet viittaa siis koira2 -olion jäseneen spritet. Yleensä renderfunktioon viitataan member access operaattorilla (piste), mutta nyt kun "spritet" on pointteri, käytetäänkin nuolta, joka on pointterien kanssa käytettävä operaattori
	  */

	  liikutaan = false; //Ennen tätä riviä kaikki hahmonpiirto-kutsut
	}

	if(kuplantila == 1) kuplanluonti(frame_nyt);
	if(kuplantila == 2 or kuplantila == 3) kuplanpiirto(frame_nyt);
	
	//	koira2.spritet->renderfunktio(0,0);  //koira2.spritet viittaa siis koira2 -olion jäseneen spritet. Yleensä renderfunktioon viitataan member access operaattorilla (piste), mutta nyt kun "spritet" on pointteri, käytetäänkin nuolta, joka on pointterien kanssa käytettävä operaattori

	//DEVDEVDEV
	/*, int fa = 0.2, int fb = 2, int fc = -0.3, int fd = -1) {
  int
*/
	//osoitin2(5, 40, 40);

	if(feidaus) {
	pimee.alphafunktio(alpha);
        pimee.renderfunktio(0,0);
	}
	
	SDL_RenderPresent(piirturi);

	if(tarina_t > 0 and kuplantila != 6) {
	  //	if(tarina_t > 0 and kuplantila != 6 and kuplantila != 7) {
	  //	  SDL_Delay(tarina_t*1000);
	  tarina_t--;
	  if(tarina_t == 0) {
	    if(kesktark()) tarinakeskeytys = true;
	    if(kuplantila == 7) {
	      tarina_kt = 0;
	      vector<kupla>::iterator eriter = kuplavec.begin();
	      kuplavec.erase(eriter);
	      if(kuplavec.size() > 0) kuplanluonti(frame_nyt);
	      else {
		tarinakeskeytys = true;
		kuplantila = 0;
	      }
	    }
	  }
	}
	if(kuplantila == 6) kuplantila++;
	//	if(kuplantila == 6 or kuplantila == 7) kuplantila++;
	if(IRC and SDL_GetTicks() - irc_ajastin >= 1000) {
	  irc_ajastin = SDL_GetTicks();
	  irc_sekunti = true;
	}

	frame_nyt++;
	//FRAMERATE
	ajastin2 = (SDL_GetTicks() - ajastin1);
	if(ajastin2 < 40) SDL_Delay(40 - ajastin2); //FPS:n asetus. 40 = 1000 millisekunttia / 25 FPS
	
      }
    }
  }
  //Vapauta resurssit ja sulje SDL
  sammutus();
  return 0;
}
