/*

Copyright Jari Sihvola 2019, unreleased

k��nt�minen: g++ anim_engine.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -std=gnu++11 -o anim_engine

--

TILANNE NYT:
-Bugi: koira1 ei k��ntynyt, kun laittoi rintamasuunta = 6, tarvittiin my�s flip = false
-pit�isi olla mahdollista aikaviive insert_kup()ista kunkin kohdan viimeisess�kin repliikiss�
-insert_kup()iin lis�tty aikaviiveen laittaminen
-puhuminen korjattu (siin� oli -kamerax1 liikaa)
-waypoint-ruudulla-vaihtumis-juttu tehty. Se n�ytt�� toimivan, mutta sen kanssa k�y helposti niin, ett� laittaa liian my�h�iseen kohtaan sen, eik� mit��n tapahdu 
-uuden teorian (pelk�st��n rendatessa huomioidaan kamerax1, ei edes s��det� arvoja ruudunvierityksen mukaan) mukaan siirtym� olohuoneesta keitti��n n�ytt�� toimivan, paitsi ett� puhekuplan osoitin ei toimi ...ja n�k�j��n useimmat puhekuplatkin menee v��r��n paikkaan
-Tuota m�yr�n takaperin k�vely� on tutkittu. DEBUG-riveill� on merkitty, mist� frame tulee. Mysteeri� voisi ehk� l�hesty� seuraavaksi siit� n�k�kulmasta, ett� miten asiat menee, silloin kun juttu toimii. Mist� se frame silloin tulee? Koska jos konstruktori laitttaa frameen 3:n valmiiksi, se ei jostain syyst� vaihdu. Pit�isi tiet��, miksi se ei vaihdu. Se k�ytt�� samoja k�velyframeja menip� mihin suuntaan tahansa eli siin� on ehk� joku ongelma, ett� ei hae sit� suuntaa. Jos kuitenkin laittaa framesuunta() keinotekoisesti, niin se n�k�j��n l�yt�� yl� ja ala -suunnat.

TEHT�V�T:
+ilmeet, tilanne: koira1:ll� on viel� punaiset kohdennuspisteet j�ljell�. tarvitsisiko kissa puhumiseen erikseen vakavan ilmeen. hk:n h�mm�stys alussa pit�isi tehd� uudestaan.
+hk:n ruumis
+hk:n p��
*hk kantaa l�pp�ri� (oma hahmonsa)
+istumiskuvat l�pp�rien kanssa (hahmoluokkaan)
*sy�minen
*pisarat
-WC:n oven avautuminen
-heti tuossa alussa k�velev�t tyhm�sti (viel�?)
-k�sien liike frameihin paremmaksi
-framevirheet pois k�velyst�
-nykyist� paremmat puheframet (koira1, koira2, mayra)
-puhumiseen peite-rect?
-taustagrafiikat ja p�yt� loppuun
-turhat tarina_t:t pois
-rappuk�yt�v�?
-koira1 -flip-bugi

VAIHTUVAT X-ARVOT:
-waypointeja k�sittelev�t funktiot: suunnanlasku, hahmonpiirto
-Mihin x-arvot ja waypoint-arvot viittaavat?
-kamerax1:n ja kamrectin suhde pit�isi selvitt��
-kamrect, on rect, jonka perusteella taustakuva renderoidaan
-eli kamrectin muuttuva arvo kertoo, mist� kohti X-akselia taustakuvaa aletaan rendata
-kamerax1 on taas se kamrectin muuttuva arvo itsess��n
-eli vaikuttaa v��r�lt�, jos n�it� molempia arvoja manipuloidaan heti alussa. Pit�isi riitt��, ett� kamerax1 on kalibroitu, kamrectia varmaan ei pit�isi muuttaa.
-Veikkaisin, ett� suunnanlasku-funktiossa on se syy, miksi hahmot ei liiku vasemmalle, kun skrollataan vasemmalle
-kamrect tosiaan asettaa ehdot sille, miss� koordinaatit ovat
-joten, jos hahmo liikkuu n�kyv�n alueen vasemmalle puolelle, waypoint-koordinaatit ovat negatiivisia
--
-negatiivisen waypointin lasku: katso kuvasta suoraan koordinaatit ja v�henn� kamerax1
--
-vaikka suunnanlaskun loppuun forcettaisi miinus-arvot, ilmeisesti l�hinn� tuo pelk�ll� x-akselilla liikkuva kulkee miinusta
--
-nyt tarkkaillaan tilannetta pelk�n n��d�n kanssa. vaikuttaa, ett� jo suunta-arvot ovat v��r�t, koska tulee positiivisia, vaikka pit�isi olla kai negatiivisia.
-Testattu, ett� suunta.firstin pit�isi suunnanlaskussa olla negatiivinen, jos liikutaan vasemmalle. Nyt se ei ole sit�.
--
uusi teoria:
-sijainti sy�tet��n taustakuvan koordinaatteihin
-renderoitaessa v�hennet��n kamerax1
-waypoint sy�tet��n taustakuvan koordinaatteihin
-tarvitseeko t�ll�in kameran vaikutusta huomioida ollenkaan?
-suunnanlaskun voi ainakin tehd� pelk�st��n kuvan koordinaattien perusteella
-sitten kun ruutu liikkuu...
-sijainti p�ivittyy automaattisesti, kun siit� v�hennet��n rendatessa se kamerax-1n

TESTAUS-ONGELMAT:
-tarkista, ett� tarinafunktiossa ensin if, sitten loput else iffej�
-kamrectin eka arvo ja kamerax1 pit�� molemmat muuttaa

KOODATTAVAA:
-taukoa insert_kupiin
-sivulle laitettavaan puhekuplaan osoitin oikeaan kohtaan
-ratkaisu tuohon p��llekk�isyyteen keitti��n siirrytt�ess�. esim. hahmoluokkaan muuttuja, joka muuttaa ruudulla arvoa waypointissa
-puhekuplien reunat jotenkin j�rkev�sti
-puhekupla tarvitsee selke�sti ��riviivat

VIIMEKSI:
-Ilme laitettu
-Laitettu sopiva kuplanpituus yhden rivin kuplille
-Korjattu bugi, ett� kutsutaan autowrap, vaikkei olisi kuin yksi rivi
-Korjattu, ett� tarinakeskeytys menee p��lle hahmonpiirrosta, kun jompi kumpi waypoint menee nollaan
-Esine laitettu
--
-C++:n castit kannattaisi kai olla muodossa static_cast<int>(expression), voisi loputkin vaihtaa. Niihin vaan pit�� lis�t� aina sulkeet. Ja pit�� olla tarkka, ett� sulkeet menee juuri seuraavan luvun kohdalle, ei pidemm�lle (seuraavaa lukua noissa c-casteissa kai aina muunnetaan). Ja tdb sanoi, ett� p�rj�isi v�hemmill�kin
--
-IRCist� puuttuu k�ytt�j�n string kursoreineen ja palkeista merkit, v�rien hienos��t�, kanavalle joinatessa tulevat rivit
-Ehk� ennen tarinakeskeytykseen menemist� voisi aina odottaa, ett� joku yleis viivytysarvo odotellaan, mik�li sinne on laitettu jotain, niin ei tarvitse tehd� viivytyksist� omaa kohtaansa
-Typedefeill� voisi tehd� tarinafunktioon selke�mm�n k�ytt�liittym�n erityisesti puhekuplalle ehk�
-Ongelma tyhj�n oikean laidan suhteen olikin tavutusongelma, joka on PC-lamereissakin, ei sit� ehk� tarvitse korjata, mutta voi tarvittaessa lievitt��

SYYSKUU: LIIKKUMISEN ONGELMAT
-Waypoint1.firstin konvertoinnin sijaan hahmonpiirto()on ja suunnanlasku()un tehtiin nyt vain kamerax1 -muuttujan v�hennykset sopiviin kohtiin
-Toisin kuin luulin, t�ll� kertaa ongelmana olikin tilanne, jossa molemmat waypointit tulevat maaliin. T�llaisessa tilanteessa aiheutti ylim��r�isen v��r�n framenpiirron framesuunta(), mutta tuli lis�tty� siihen my�s, ett� framesuunta()a ei my�sk��n kutsuta, jos toinen waypoint tulee maaliin ja toinen on vain 5 maalista. Ep�varmaa, oliko t�m� hyv� lis�ys.
-Piti palauttaa framesuunta() -tarkistus my�s waypointtien yhteyteen, ei olisi pit�nyt poistaa sit�.
-Toisen waypointin menness� nollaan suunnanlasku antoi h�lm�j� tuloksia ja hahmo alkoi sutimaan. T�m� on nyt korjattu (tangenttia ei tarvitse, kun toinen waypoint on 0). Aluksi korjauksessa sattui paha virhe, jolloin waypoint2 ei toiminut ollenkaan, mutta t�m�kin korjattiin. Lis�ksi hahmonpiirtoon piti lis�t� kaksi uutta kutsua framesuunta()an, koska suunta voi muuttua oleellisesti, kun toinen waypoint menee nollaan. HUOM. Tilanteessa, jossa vain toinen waypoint on nolla, voisi suunnan kyll� ottaa hyvinkin yksinkertaisesti.
-Juuri nyt vaikuttaa, ett� jumitusbugi katosi, kun waypointtien perille p��syn skuuppia laajennettiin 1 -> 2
-L�ytyi uusi bugi: k��ntyess� seuraaviin waypointteihin vilahti v��r� frame. T�m� taisi korjaantua tekem�ll� erillinen framesuunta() suunnanlaskun lopun pohjalta ja kutsumalla sit� suunnanlaskun j�lkeen sek� bugin korjaamiseksi my�s waypointteja vaihdettaessa.
((T�m� taisi olla v��rien oletusten vallassa kirjoitettu: -hahmot j��v�t jumittamaan ilmeisesti, koska sijainti.first ja wayp1 l�hentyv�t. pit�� selvitt�� mik� logiikka niiss� on (esim. laita pointit ruudulle, jotka otetaan mainfunktiossa suoraan koira1:n arvoista). X-waypointtihan on nyt konvertoitu aina hahmonpiirron alussa. sen voi suht helposti palauttaa. Tarinafunktiossa annetut X-waypointit nyt siis viittaavat aina taustakuvan eik� ruudun pisteisiin.))
-Nyt suunta lasketaan aina (mutta suunnanmuutos() ei koske frameen, jos ollaan jo k�velyframeissa) ja hahmo pys�htyy, kun se on +-1 pikselin p��ss� waypointista.
-k�vely pit�isi synkata skrollauksen kanssa siten, ett� v�hennet��n waypoint.firstist� aina skrollinopeus (pit�� selvitt�� miss� tahdissa ne kasvaa)
-Aikaisemmin oli pohdittu, ett� skroliter-iteraattorin looppiin my�s, ett� hahmon paikkaa ei muuteta skrollatessa, jos hahmo liikkuu. Toisaalta vaikuttaisi, ett� jos t�ss� liikutellaan hahmoja vain skrollin suuntaisesti, ei tuo skrollaus + k�vely n�yt� nyt kovin huonolta ilman mit��n sopeutustakaan.


KESKEYTYS:
(-hahmo-luokkaan saisi helposti muuttujan, ett� tarkistetaan tarinakeskeytys liikkumisen loputtua)
(-puhekeskeytyksen saisi hoidettua ilmeisesti siten, ett� on erikseen muuttuja, joka laittaa keskeytyksen, jos puhevektori loppuu)
-mutta ehk� pit�isi tehd� se hahmoihin pointterit omaava vektori kuitenkin. siit� voi olla muutenkin hy�ty�. saisi k�tev�sti nuo piirtokutsut ja sitten ei tarvitsisi tehd� tuota puhekeskeytyst� ja liikkumiskeskeytyst� erikseen
-IRC vaihe ei tarvitse erikseen keskeytyksi�. Siihen pit�� laittaa joku systeemi, ett� se keskeytt��, kun vuorosanat loppuu
-keskeytystarkistuksen (joka k�ytt�� hahmojen pointterivektoria ja tarkistaa, liikkuuko joku, onko yleistimeria laitettu ja onko puhetta menossa) voisi kutsua aina, kun liikkuminen loppuu, yleistimeri loppuu ja puhuminen loppuu)

POINTTERIVEKTORI-BUGI
-t�m� bugi taisi kadota tuntemattomasta syyst� (voisiko liitty� tuohon, ett� spritesheettien m��rittelyt erotettiin omaan funktioonsa)
--
-pointteri hahmoluokka-olioon push_backataan konstruktorissa eik� int_mainissa, niin ongelma tulee
-valgrind tuntui pit�v�n syyllisen� *spritet -viittausta hahmonpiirto-funktiossa (cout edellist� rivi�, miss� piirtorect)
-piirtorect, spritet ovat kai ne ongelmalliset pointteri-muuttujat hahmoluokassa
-ratkaisuehdotus: N�m� int mainin rivit olisiko mahdollista siirt�� aiempaan vaiheeseen? Vaatii ehk� ylim��r�isten julistusten tekoa tms. voi olla, ett� n�iden rivien esiintyminen int mainissa vaatii, ett� hahmoluokka-olion pointterikin on push_backattu sielt�
koira2.yhdista_tekstuuri(&koirateks2);
koira2.yhdista_rect(koira2_arkki, (sizeof(koira2_arkki)/sizeof(*koira2_arkki)), sizeof(koira2_arkki));
-toinen ehdotus: Voisikohan t�m�n ongelman kiert�� my�s siten, ett� push_back funktio ajetaan sek� konstruktorissa, ett� mainissa
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
-oli framejen v�liset x-erot ep�symmetriset arkissa ja yhdess� joku ruumiinosa doublepikselin v��r�ss� kohti. Huom. Jos kuvassa on ylim��r�isi� frame-settej�, vahingossa voi ruveta editoimaan niit�
-oli m��ritelty puhe-sheet liian pieneksi, niin se ei n�kynyt sitten, kun sit� oli siirretty oikealle

PUHEKUPLA:
Mitk� jutut voi koodata nyt:
-Borderit (yl�- ja ala = 5, sivut = 15, kuplan hahmon p��n yl�puolellenosto = 30) voisi ehk� tehd� consteiksi. Tarvitseeko oikean puolen border olla yht� iso?
--
-Halutessa on helppo luoda "useita defaultteja" kuplanpituuteen, esim luvut 1, 2, 3... voi laittaa tarkoittamaan jotain muuta
-Kunhan hahmottuu, mit� kaikkea puhekuplaan pit�� aina sy�tt��, voisi mietti�, miten ne saisi kaikki k�tev�sti yhdess� tarinafunktiosta
-Pit�isik� puhekuplan olla yleisesti ottaen leve�mpi, kuten PC-lamereissa? Ehk� se ei sovi niin hyvin t�h�n grafiikkatyyliin, miss� hahmot ovat pienempi� eik� ole zoomia
-Pit�isik� kuplan sijainnin menn� yl�s, jos siell� on tilaa ja muussa tapauksessa alas? T�m� tuntuisi ihan hyv�lt� vaihtoehdolta. Jospa katsottaisiin ensin k�yt�nn�n my�t�, tarvitaanko sit� alempana olevaa kuplaa ollenkaan.
--
-pit�isik� puhekuplan piirrosta tehd� hahmoluokan funktio? ei ehk�, koska se ei tarvitse hahmosta muuta tietoa kuin sijainnin ...mutta sijaintia pit�� heitt�� jatkuvasti? joten ehk� sen voisi yhdist�� hahmoluokkaan. n�in voisi toteuttaa my�s helpommin sen, ett� kuplia voi olla useita samaan aikaan. vuorosanankin voisi sitten heitt�� sinne. lopuksi puhekupla sulkee itsens�.


RUUDUN KOKO JA ASPECT RATIO:
-Youtuben aspect ratio (16:9) on aika leve�, mutta siin� on varmaan ideana, ett� siin� on sivusuuntaan otettu pelivaraa ja se ilmeisesti ei h�iritse, ett� monet videot n�kyy kapeampina (pc -lamerit ei taida olla noin leve� kuin tuo aspect)
-Ruudun koko sin�ns� on varmaan ihan OK, sit� voi zoomata tarvittaessa
-Edellinen ruudun koko oli ehk� turhan korkea ajatellen jotain sis�ll�olomaisemaa. Ehk� parempi vied� hiukan youtuben ratioon p�in

PUHEEN ATTRIBUOINTI HAHMOLLE:
-yksi mahdollisuus on ett� on kuppointteri ja sitten hahmonpiirto-funktiossa kysyt��n, jos puhekupla on true, niin onko hahmon sijainti ja kuppointteri samat... no oikeestaan, eih�n t�m� olisi niin kuormittava
-toinen olisi ollut, ett� hahmoluokkaan erikseen puhe. ja puhekuplafunktio hahmoluokkaan. t�ll�in ei tarvitse sijaintipointteria. mutta sitten on ehk� hankalampi kutsua puhkuplafunktiota tarinafunktiosta k�sin, kun pit�� olla se hahmonnimi siin� kutsussa ja se kutsu olisi kuitenkin hyv� tehd� p��loopista k�sin

IRC-N�KYM�:
-Eri rivit alkuun, mit� tulee kanavalle joinatessa. N�it� varten pit�� tehd� sitten uusia tekstuureja, koska v�rit on eri
*p��funktioon: jos IRC -p��ll�
*musta tausta ensin - tulee nyt defaulttina
*wrap, joka automaattisesti katkoo rivit kirjaimen kohdalta (ei sanan) niin on realistisempaa
*vuorosanat voi sy�tt�� tarinafunktiosta ja irc-funktio laittaa ne tekstuurivektoreiksi
*eli funktio, johon sy�tet��n kellonaika, puhuja, vuorosana ja se lis�� ne vektoriin
-pit�� olla monospace-fontti kaiketi? n�k�j��n xfce-terminaalissa on kaiketi defaultina fontti "monospace"
*vektorin elementti� piirrett�ess� lis�t��n nuo tummanharmaat < > -merkit, jotka ovat omat tekstuurinsa (ja alkurivien vaatimat v�rilliset merkit)
-k�ytt�j�rivin on alettava oikeaan aikaan. Sen voisi varmaan tehd� niin, ett� se laitetaan vektoriin normaalisti, mutta vektorista sit� ei piirret� rivien sekaan vaan siihen k�ytt�j�-stringiin v�hitellen (sen mukaan mit� on jo piirretty). ja ehk� sen tyypin voisi erikois-ajastimen menty� normaaliksi, jolloin se piirtyy normaalina
-k�ytt�j�rivin tunnistaa tyypist� (struct irc_rivi), sill� on ylim��r�inen aika-arvo-muuttuja (tehd��n structiin) joka kertoo, miten paljon kest�� seuraavan merkin ilmaantumisessa (t�h�n voi halutessaan pist��, ett� sit� randomisti muutetaan)
-kirjoitusnopeuden ja stringin perusteella pystyisi ajastamaan k�ytt�j�rivin etuk�teen
-eriv�rinen teksti vaatii omat surfacensa. Vektorien m��r� voisi olla vakio riippuen merkinkoosta (jos tulee tyhji� rivej� (niit� ei kyll� kanavilla ole), voi j�tt�� sen tyhj�ksi). Ei tarvita muistiin y-akseleita vaan riitt�� merkinkorkeuteen perustuva systeemi.

TEKSTISKROLLI:
-kuplantilaan uusi kohta, joka on, ett� jos kupla skrollaa. Vai pit�isik� olla joku erillinen delimiter-muuttuja, jossa on kerrottu sekin, miss� rivill� se raja on (0, jos kaikki mahtuu)
-kuplanluonnissa pit�� ensin laskea, onko kuplankorkeus suurempi kuin rivit
-kun kursori saavuttaa viimeisen n�kyv�n rivin, skrollataankin tekstin puuttuva osa. sen nykyisen tekstiskrollausarvon rinnalle joku toinen arvo
-voisi laittaa sen rectin, joka on kai nyt koko ajan p��ll�, ett� se on p��ll�, jos delimiter on tms.

LIIKKUMISBUGI:
-voisiko ep�tarkkuus liikkumisessa liitty� siihen, ett� lasketaan irrationaaliluvuilla?
-HUOM. Yksi mahdollinen tapa tehd� liikkumisen laskeminen tarkemmaksi olisi, ett� pelk�n plussaamisen sijaan laskettaisiin aina siin� kohti sijainti uudestaan
-toinen potentiaalinen tapa tarkentaa, olisi laskea suunta uudestaan tietyn etenemism��r�n j�lkeen (sen hetkinen sijainti ja waypoint)
-HUOM. Tuossa kohti, kun asetetaan toinen waypointti nollaan, voisi heitt�� funktiokutsun, joka asettaa framen (sijainnin?) oikein, mik�li tuota liikkumista ei muuten korjata tarkemmaksi
-Liikkuminen waypointtiin toimii, mutta siin� voi tulla tuommoista, ett� lopussa liikutaan yhteen suuntaan. Olisiko kyse py�ristysvirheist� suunnan hakemisessa?

vanhempia muistiinpanoja:
-laittaisko nyt sitten niin, ett� skrollausarvon osuessa kuplan yl�laitaan, seuraa aina keskeytys. Muuten vois laittaa, mutta ei ole p��tetty, tuleeko tuota skrollausta yleens� lopulliseen engineen. Eli voihan t�m�n enginen lopun toteuttaa my�hemminkin.
-muissa tapauksissa keskeytyksen ehto voisi olla tarinafunktion jotenkin m��rittelem�, tosin v�h�n ep�selv�� ehk�, miten t�m� tehd��n. Ainakin liikkumisissa voisi olla ehton m��ritelty helposti vaikka joka hahmoille erikseen liikkumiskeskeytys tarinafunktiosta k�sin universaali-muuttujalla
-laittaisko siten, ett� keskeytykset aina lis�� yhden desimaalin ja jos tarinafunktiosta ei l�ydy if-lausetta siihen lukuun, siirryt��n seuraavaan kokonaislukuun ja kutsutaan se funktio uudestaan. T�ll�in tarinafunktiosta pit�isi tehd� boolelainen ilmeisesti
-yhden puhekuplan ratkaisussa riitt�isi pointterimuuttujat, joille laitetaan osoite hahmon sijainti-muuttujaan?

-ohjelmaa on helppo testata muuttamalla tarinanvaihe-muuttujan l�ht�arvoa, niin se alkaa suoraan jostain tietyst� kohdasta

--

//Tarinafunktio
//Tarinanvaihe kertoo, miss� vaiheessa tarinaa ollaan menossa. Float, koska juoksevat numerot sopivat hyvin, jos haluaa lis�t� lis�� vaiheita j�lkik�teen ja muutenkin p�tkitt�� tehokkaasti
//Jos tarinakeskeytys on true, p��looppi k�y hakemassa uusia tietoja tarinafunktiosta, jolloin uudet muuttujien arvot m��r�ytyv�t tarinanvaiheen mukaan. Lis�ksi ilmeisesti m��rittyy, milloin seuraava keskeytys tulee. Keskeytyksi� voi tehd� useamman tai esim liikkumisille omia, jos se sujuvoittaa asioita.
-jotain ehtoa sen ohjelman pit�� kai koko ajan tarkistaa ja sitten se ehto itsess��n on joku muuttuja, jota joku tarinafunktio muuttelee
-esim kun hahmon x pit�� k�vell� paikkaan y, p��looppi kysyy tarkistusfunktiosta, t�yttyyk� jokin keskeytys-ehto. n�it� keskeytys-ehtoja voisiko olla muutama kerrallaan, niin saa k�tev�mmin hallittua, jos tapahtuu useampi juttu.
-tarinanhallinnasta k�sin taas asetetaan, milloin ehdot t�yttyv�t
-pit�isik� resetoida arvot (esim. onko hahmo x n�kyviss�) jollain funktiolla v�lill� vai pit��k� aina erikseen s��t�� hahmot pois n�kyvist�, kun vaihtuu kohtaus? ehk� resetointi-funktion voisi tehd�, sit�h�n ei ole pakko kutsua jatkuvasti

tarinafunktion muokkaamat muuttujat kertovat:
-mik� taustakuva on p��ll�
-mitk� hahmot ovat p��ll�
-miten hahmoja liikutellaan
-onko puhekupla p��ll� ja mit� teksti� siihen tulee
-mik� musiikki soi

hahmojen liikutusfunktio:
-framet ja suunta
-liikkuessa pit�� aina my�s vaihtaa framea (framen vaihtonopeus varmaan sidotaan yleisemp��n liikkumisnopeuteen mutta ne voi erottaa)
-tarvitaanko hahmoluokkaan my�s muuttuja, ett� miss� waypointissa ollaan menossa? ehk� ei, koska jos esim ykk�nen muutetaan nollaksi, kun sinne ollaan p��sty, niin sit� ei sitten en�� k�ytet��n
-t�m� pit�� ehk� ajastaa nopeuden s��t�miseksi. vai pit��k� laittaa hahmoluokkaan ne nopeudet
-funktio joka tarkistaa, onko hahmoilla liikutus p��ll� ja jos on, mihin suuntaan ja muuttaa sitten sen mukaan arvoa (ottaen huomioon mahdollinen nopeus) ja tarkistaa my�s, onko p��sty jo kohteeseen ja laittaa sitten liikutuksen pois tai ohjaa seuraavaan waypointtiin.
-pit�� laskea (erillisell� funktiolla ehk�), mit� spritej� k�ytet��n riippuen liikkumisen suunnasta

p��loopin kysymykset:
mik� on taustakuva (yksi vaihtoehto: musta ruutu)
vaihtuuko musiikki
skrollataanko taustaa
ollaanko kuvaa feidaamassa pois
ollaanko puhekuplaa luomassa
onko puhekupla aktiivinen
onko kursori aktiivinen - t�m� on p��loopissa puhekuplan if-rakenteen alla
piirret��nk� hahmoja (ja mihin)
liikutetaanko hahmoja
puhuuko joku hahmo - t�m� tapahtuu samaan aikaan kursorin kanssa.

OSOITIN:
Ratkaisut:
-osoittimen vasen-oikea suunnan saa muutettua, kun for-sulkeiden laskurin laittaa dekrementoimaan ja jatkoehdoksi > -20
-alasp�in avautuu, jos laittaa koko y7 ja y8 kaavan negaatioksi JA vaihtaa laskuri3:n ja 2:n paikkaa kesken��n loopin lopussa (eli kummasta dekrementoidaan 1 ja kummasta 2). T�m� alasp�in on k�yt�ss� kaiketi my�s sivulle p�in teht�ess�.
-sivukupla: vaihdetaan X ja Y viivanpiirtok�skyss�
kupasema: yl� & ala, sivuk, sivuy, sivua
http://www.protutorcompany.com/how-do-i-know-which-direction-the-parabola-opens/
Mitk� on kuplan eri tilanteet?
yl�kupla vasen  
yl�kupla oikea
alakupla vasen  alakuplaan kai kuplan arvot parempia kuin kuppointer?
alakupla oikea
sivukupla yl�
sivukupla keski
sivukupla ala

MUUTA
-vektorilla, jossa on pointtereita olioihin voisi kutsua esim funktiota kaikista olioista, joilla on muuttuja x arvossa y. Eli voisi liikkumisfunktiota kutsua hahmoista, jotka liikkuu sen sijaan, ett� laittaa ne kaikki hahmo-objektien nimet siihen listaksi

OPITTUA:
-kun l�hdet��n tekem��n jotain isompaa systeemi�, kuten t�ss� tuo puhekuplan toiminta, p��sisi kaiketi huomattavasti helpommalla, kun suunnittelisi alusta alkaen, mit� kaikkea toimintoja systeemin on tarkoitus sis�lt�� ja ottaisi ne sitten alusta pit�en huomioon
-kirjoita selke�t selitykset, ettei tarvitse selvitt�� koodin toimintaa my�hemmin
-Tarkkana -= -merkkien kanssa, ettei tule esim =-
-Rectien logiikka menee helposti sekaisin, eik� virhe n�yt� v�ltt�m�t�t heti niin suurelta k�yt�nn�ss�, kuin mit� se on. Rectin toiset arvot ovat rectin pituus eik� rectin toisen p��n koordinaatit.

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
const int KNOPEUS = 3; //K�velynopeuden kerroin //oli 3
int kamerax1 = 530; //1280, 530
int kamerax2 = SCREEN_WIDTH;
int kamskrolli = 0; //0 = kameran liikutus pois p��lt�, 1 = vasemmalle, 2 = oikealle
SDL_Rect kamrect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; //530 ekaan
//DEBUG  SDL_Rect kamrect = {1200, 0, SCREEN_WIDTH+1200, SCREEN_HEIGHT};
int skrollihalt;  //Mihin skrollaus pys�htyy. Maksimiarvo t�lle on kuvan koko X-akselilla miinus SCREEN_WIDTH (eli 1280 t�ss� produktiossa, jossa 1960 on taustakuvan X-koko)

//Tarinanvaihe kertoo, miss� vaiheessa tarinaa ollaan menossa.
//Jos tarinakeskeytys on true, p��looppi k�y hakemassa uusia tietoja tarinafunktiosta, jolloin uudet muuttujien arvot m��r�ytyv�t tarinanvaiheen mukaan.
int tarinanvaihe = 3; //Arvo oltava pienempi kuin ensimm�inen koodissa k�ytett�v� arvo
bool tarinakeskeytys = true;
int tarina_t = 0; //Tarinafunktion yleistimer, montako sekunttia odotellaan (muunnetaan millisekunneiksi)
int tarina_kt = 0; //Kuplanluonnista asetettava timer-arvo, joka siirt�� arvonsa tarina_t:lle
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

//Oikeat arvot merkinpituudelle- ja korkeudelle saa, kun katsoo coutilla, mit� tekstirend-funktion surfacen antamat w & h -arvot ovat. Korkeus tulee suoraan, merkinleveyden saa jakamalla jotain rivi� kuvaavan surfacen leveyden jonkun rivin merkkien m��r�ll�
const int merkinpituus = 9;
const int merkinkorkeus = 18;
const int RIVIVALI = 0;
//puhekuplan muuttujat
string vuorosana1;
vector<string> vuorosanavec;
int kuplantila = 0; //onko puhekupla p��ll� (0 = ei kuplaa, 1 = alustusvaihe, 2 = puhekupla on p��ll�, 3 = tekstiskrolli, 4 = per��n toinen kupla, 5 = kuplan poistaminen, 6 = tauko kuplan j�lkeen)
double ylirivit = 0;
struct kupla {
  int timer = 0;
  int kuka = 0;
  int pituus = 0;
  int korkeus = 0;
  string puhe = "";
  int asemointi = 1; //kupla voi olla yl�- (1), alapuolella (2) hahmoa tai hahmon yl�sivulla (3), sivulla keskell� (4) tai alasivulla (5)
  int vilme = 1;
};
vector<kupla> kuplavec; //Vektori, ett� voi laittaa kerralla per�tt�isi� puhekuplia
int bubblew; //kuplan vasen laita
int bubbleh; //kuplan yl�laita
int bubblew2; //oli 360, t�m� on siis kuplan leveys (ml. borderit). Kuplanpituus my�hemmin tulee ilmeisesti t�h�n kohtaan m��ritelt�v�ksi, kunhan autowrapist� tehd��n oma funktionsa
int bubbleh2; //kuplan korkeus yl�laidasta laskien
int kupasema;
int puhilme = 1; //puhujan ilme
const int OLETUSKUPLANPITUUS = 360;
const int OLETUSKUPLANKORKEUS = 140;
const int KUPYLABORDER = 5;
const int KUPALABORDER = 5;
const int KUPVASBORDER = 15;
const int KUPOIKBORDER = 15;
//Puhekuplassa pituus on prim��rinen, jonka perusteella katkotaan vuorosanan rivit ja sitten korkeus voidaan 0-arvolla sopeuttaa automaattisesti oikeaksi pituuden ja rivien avulla
//Tarinafunktiossa s��dett�v�t kuplan pituus ja korkeus
int kuplanpituus = 0; //0 = OLETUSKUPLANPITUUS, muut positiiviset arvot on x-akselin arvoja
int kuplankorkeus = 0; //0 = s��det��n tekstin pituuden mukaan, 1= laitetaan OLETUSKUPLANKORKEUS, muut positiiviset arvot viittaa y-koordinaatteihin
//Muut v�liaikaisemmat kuplanpituus ja korkeus
int tempkuppituus = 0;
int tempkupkorkeus = 0;
int kuplavo = 0; //1=kupla vasemmalle hahmosta, 2=kupla oikealle, 0=sijainnin mukaan (jos hahmo kuvaruudun vasemmalla puoliskolla, kupla oikealle ja toisin p�in)
SDL_Rect kupla1; //kuplan leve�mpi pala
SDL_Rect kupla2; //kuplan korkeampi pala
SDL_Rect cliprect; //Neli�, jolla peitet��n alueet, joihin rendataan, kun muualle ei. K�ytet��n ainakin puhekuplan tekstiskrollissa, ett� teksti katoaisi kuplan yl�laidassa
pair<double, double> *kuppoint; //pointteri hahmon sijaintiin, josta kuplan koordinaatit otetaan

//Tekstin rendauksessa ja j�rjestelyss� k�ytettyj� muuttujia
TTF_Font* fontti;
SDL_Surface* pinta2;
SDL_Texture* tekstuuri2;
//int tekstinleveys;
//int tekstinkorkeus;
int rivinpituus;

//Muuttujat, joilla puhekuplan kursoria ohjaillaan (n�m� ovat default-arvot, joita ei saa t�ss� vaiheessa muuttaa). Hmmm... tarvitsisivatkohan n�m� arvot jonkun resetointi-funktion
pair<int, int> mikarivi; //Ensimm�inen luku on mill� kuplan rivill� kursori menee ja toinen on, mill� vuorosanavektorin rivill� kursori menee. N�m� voivat erkaantua teksti� skrollatessa.
int moneskirjain = 0;
int kursoripaalla = 0; //0 = ei kursoria, 1 = kursori juoksee, 2 = kursori odottaa, ett� skrollataan puhekuplanteksti� ylemm�s, 3 = kursori maalissa
int kursorikountteri = 0;
int pkupkounter = 0;
SDL_Color vaeri = {0, 0, 0}; //tekstinv�ri

//Tekstin skrollauksen muuttujat
bool tekstiskrolli = false; //onko skrollaus menossa vai ei
int skrollausarvo = 0; //jotkut funktiot lis�� skrollausarvon teksti� sijoittaessaan. jos se on muuta kuin nolla, teksti skrollaa

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
vector<irc_rivi> irc_teksturet;  //En tied�, pit��k� tuon olla pointteri, ei k��nny ilman
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

//P��ikkuna
SDL_Window* ikkuna = NULL;
//Ikkunan render�ij�-olio
SDL_Renderer* piirturi = NULL;

class tekstuuriluokka {
  int leveys;
  int korkeus;
public:
  SDL_Texture* tekstuuri;  //T�m� on itse hardwaretekstuuri
  tekstuuriluokka(); //Muuttujien initialisointi
  ~tekstuuriluokka(); //De-allokoidaan muisti
  bool tiedostonlataus(string filename);
  void erase(); //tekstuurin de-allokointi
  void renderfunktio(int x, int y, SDL_Rect* temprect = NULL, bool flip = false); //Render�i tekstuurin annettuun kohtaan
  int haeleveys();
  int haekorkeus();
  void aseta_blend(SDL_BlendMode blend_arvo);
  void alphafunktio(Uint8 alpha_arvo);
};

tekstuuriluokka taustakuva; //t�m� on muuttuva luokka
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

//Constructor on datatyypit�n ja luokan kanssa samanniminen funktio, joka ajetaan aina, kun luokasta luodaan luodaan olio. Voi olla parametrej�.
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
    //T�ss� kuva colorkey�t��n, ennen kuin siit� luodaan tekstuuri. Ensimm�inen argumentti on, mik� surface colorkey�t��n, seuraava argumentti laittaa colorkeyingin p��lle, kolmas argumentti on pikseli, joka halutaan colorkey�t�. T�h�n laitetaan sitten toinen funktio, jossa ensimm�inen argumentti on formaatti pikseleille (k�ytetyss� oliossa on formaatin kertova j�senmuuttuja), seuraavat muuttujat ovat v�ri, t�ss� tapauksessa syaani.

    //Luodaan surfacen pikseleist� tekstuuri
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

//Full-screen imagejen kanssa riitti SDL_RenderCopy ja kaksi viimmeist� argumenttia NULLiksi, mutta nyt tiettyyn paikkaan render�it�ess� tarvitaan alkuper�isen kuvan dimensiot leveyden ja korkeuden spesifioimiseen eli luodaan ensin tuommoinen neliskulmio, johon luokasta leveys ja korkeus ja se sitten annetaan RenderCopyyn
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
8.0 on suoraan yl�s, 9 on suoraan oikeaan yl�viistoon, 8.1-8.9 on siin� v�liss� olevat, 9.1-9.9 on 9:n ja 6:n v�liss� olevat
Mit� kuvakulmaa k�ytet��n riippuu kokonaisluvusta, paitsi ett� 7.0 on eri kuin >7, 3.0 on eri kuin >3, 1.0 on eri kuin >1
-suunta lasketaan l�ht�pisteen ja waypointin mukaan eik� siis ole suoraan aktuaalinen suunta vaan on py�ristettyn� ohjeena

seuraavaksi hahmonpiirtoon tuo, ett� suunnasta katsotaan frame.

...hetkinen, vai pit�isk� suunta katsoa sittenkin tarkkaan waypointista. koska pit��h�n se tarkka suunta olla, ett� osataan muuttaa sijaintia oikein

eli on ne koordinaatit, esim
120, 140
sitten waypoint 230, 170
katsotaan et�isyys molempiin arvoihin (t�ss� +110, +30)

eik�s noiden w:n ja h:n muutoksen suhde kerro, miten paljon aina muutetaan arvoa

n�in n�ytt�isi olevan. mutta tarvitaan erikseen X:lle ja Y:lle arvot tai ainakin etumerkit vaihtelee
110 / 30 = 3.667

eli toiseen lis�t��n sitten 0.3677 ja toiseen 1-0.3677

pit�isik� liikkuminenkin olla pair?
se voisi olla paras ratkaisu, doubleja

tarvitaanko sitten erikseen suuntaa? tai siis ennemminkin vain muuttuja, ett� mit� frame-sarjaa k�ytet��n
*/


class hahmoluokka {
public:
  int ruudulla; //Onko hahmo ruudulla ja monennessako hahmonpiirtokerroksessa
  bool symmetrinen;
  bool flip;
  bool steadyflip; //Flip p��ll�, kun hahmo on paikallaan
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
  int rintamasuunta; //Kun hahmo on pys�hdyksiss�
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

vector<hahmoluokka*> hahmovektori; //Vektori, johon liitet��n hahmoluokan j�seniin osoittavat pointterit

void hahmoluokka::yhdista_tekstuuri(tekstuuriluokka *olio) {spritet = olio;}

hahmoluokka::hahmoluokka():
  ruudulla(0), symmetrinen(true), flip(false), steadyflip(false), rintamasuunta(2), frame(3), spritet(NULL), ilme(1), wp1muutos(0), wp2muutos(0), wp3muutos(0), wp4muutos(0) { //Pair-muuttujat puuttuvat t�st�, niihin tulisi vain nollaa
  hahmovektori.push_back(this);
}

hahmoluokka::~hahmoluokka() {
  delete[] hahmorect;
  //Tekstuurin tuhoaminen t�h�n ...tosin se erase() on tekstuuriluokassa
}

//Hahmon rect yhdistet��n sen spritesheettiin
void hahmoluokka::yhdista_rect(SDL_Rect *rect, int arraynpituus, unsigned int arraynkoko) {  //Funktioon l�hetet��n array, jolloin sinne l�htee arrayn ensimm�iseen elementtiin osoittava pointteri ja n�in ollen parametrin t�ytyy olla pointteri-tyyppinen
  hahmorect = new SDL_Rect[arraynkoko]; //Dynaamisesti allokoituvaan arrayseen muistit
  for(int counter = 0; counter != arraynpituus; counter++) { //Assignataan sis�lt�
    hahmorect[counter] = rect[counter];
  }
}

//HUOM! T�h�n tulee kaiketi my�s liikuutus
//xcv
void hahmoluokka::hahmonpiirto(bool kavelynyt, int puheajastin) {
  //drops
  if(ilme == 4) {
    piirtorect = *(hahmorect + frame);  //Viittaus pointterin l�pi arrayn indeksiin (hahmorect t�ss� on itse pointteri ensimm�iseen elementtiins�, johtuisiko siit�, ett� se on * -operaattorin argumentti)
    spritet->renderfunktio(sijainti.first-kamerax1, sijainti.second, &piirtorect, flip);
    return;
  }

  bool tarkistus = false; //Tarkistus on vain sen takia, ettei turhaan renkata keskeytys-funktiota kokoajan

  if(frame >= 18) rintamasuunta = 0;
  else if(rintamasuunta == 0 and frame < 18) {
    rintamasuunta = 2;
  }
  
  if(waypoint1.first != 0 and waypoint1.second != 0) {
    liikesuunta = suunnanlasku();  //Ilmeisesti siin� tulee se akkumulaatiovirhe, jos lis�ilee niit� doubleja p��llek�in, joten t�m� pit�� kutsua t��ll�
    //Hahmon framet synkkaan k�velysuunnan kanssa, jos ei olla k�velyframeissa
    if(frame < 3 or frame > 14) framesuunta();
  }

  //Kun toinen waypoint tulee maaliin, mutta toinen waypoint on viel� kaukana, kutsutaan framesuunta(). T�m� voi tapahtua, jos toinen waypoint on jo l�ht�kohtaisesti hyvin l�hell� maalia. Mutta jos toinenkin waypoint on nolla tai jos toinenkin on hyvin l�hell� maalia (+-5), silloin ei kutsuta. On tosin ep�selv��, tarvitaanko tuota hyvin l�hell� juttua ollenkaan vai riitt�isik�, ett� se ei ole nolla.
  if(waypoint1.first != 0) { 
    if(sijainti.first > waypoint1.first-2 and sijainti.first < waypoint1.first+2) {
      waypoint1.first = 0;
      tarkistus = true;
      if(waypoint1.second != 0 and (waypoint1.second - sijainti.second > 5 or waypoint1.second - sijainti.second < -5)) framesuunta();
    } 
    else if(liikesuunta.first > 0 and (sijainti.first < waypoint1.first)) sijainti.first += liikesuunta.first;
    else if(liikesuunta.first < 0 and (sijainti.first > waypoint1.first)) sijainti.first += liikesuunta.first; //Oleellinen rivi
    else {cout << "liikesuunta.first, sijainti.first, waypoint1.first, waypoint1.first-kamerax1" << " " << liikesuunta.first << " " << sijainti.first << " " << waypoint1.first << " " << waypoint1.first << endl; waypoint1.first = 0;} //T�nne todellakin menn��n
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
  if(waypoint1.first == 0 and waypoint1.second == 0 and waypoint2.first == 0 and waypoint2.second == 0) { //Ilman waypoint2:ia v�l�ht�� k��ntyess� seisomisframe
    switch(rintamasuunta) {
    case 0: break;
    case 2: frame = 0; break;
    case 8: frame = 2; break;
    case 6: frame = 1; break;
    case 4: {
      if(!symmetrinen) frame = 15; //HUOM!!!! T�M� PIT�� MUUTTAA
      else {
	frame = 1;
	flip = true;
      }
      break;
    }
    }
  }
  //Seuraava rivi aiheuttaa segfaultin, jos viitataan konstruktorista laitetun vektorin kautta
  piirtorect = *(hahmorect + frame);  //Viittaus pointterin l�pi arrayn indeksiin (hahmorect t�ss� on itse pointteri ensimm�iseen elementtiins�, johtuisiko siit�, ett� se on * -operaattorin argumentti)

  //Renderfunktiokutsun molemmin puolin on flip-tarkistukset. Riippuu rintamasuunnasta, johon hahmo j��, pit��k� tarkistus tehd� ennen vain j�lkeen renderfunktion.
  if(tarkistus and rintamasuunta == 2 and waypoint1.first == 0 and waypoint1.second == 0 and waypoint2.first == 0 and waypoint2.second == 0) flip = false; //Ilman t�t� rivi� hahmoon voi j��d� flip-p��lle, kun se pys�htyy.
  spritet->renderfunktio(sijainti.first-kamerax1, sijainti.second, &piirtorect, flip);

  //Huom. seuraava kohta oli aiemmin yll� olevan renderfunktio-kutsun yl�puolella, mutta siit� tuli ylim��r�inen v��rin flipattu frame
  if(tarkistus and waypoint1.first == 0 and waypoint1.second == 0 and waypoint2.first == 0 and waypoint2.second == 0) flip = false;
  if(steadyflip == true) flip = true;

  
  //Puhe katsotaan sen perusteella, onko kuplan kuppointit s��detty t�lle hahmolle
  //Toiseksi viimeinen ehto sulkee pois selink�velyframet ja viimeinen selinpaikallaanframen
  //Rintamasuuntaa ei voi k�ytt�� ehtona seuraavassa, koska hahmo voi k�vell� ihan eri suuntaan, kuin mit� se pys�htymiseen viittaava rintamasuunta on
  //Puheen skrollin aikana saisi helposti pois, jos halutaan
  if(kuplantila == 2 and puheajastin%10 > 5 and kuppoint->first == sijainti.first and kuppoint->second == sijainti.second and (static_cast<double>(frame-2) / 12) * 4 != 3 and frame != 2) {
    //1: 3-6 sivuk�vely, 2: 7-10 alask�vely, 3: 11-14 yl�sk�vely (-2 niin tulee 1-12 asteikko)
    //logiikka: (positio/koko rangen koko)*rangen v�limatka
    //    if((double(frame-2) / 12) * 4 == 1) piirtorect = *(hahmorect + 16); //Flip on jo valmiiksi p��lle laitettu t�ss� funktiossa, jos sit� tarvitaan
    //    else if ((double(frame-2) / 12) * 4 == 2) piirtorect = *(hahmorect + 15);
    if(frame >= 3 and frame <= 6) piirtorect = *(hahmorect + 16); //Flip on jo valmiiksi p��lle laitettu t�ss� funktiossa, jos sit� tarvitaan
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

  //P�ivitet��n waypoint, jos sinne saavutaan
  if(waypoint1.first == 0 and waypoint1.second == 0 and (waypoint2.first != 0 or waypoint2.second != 0)) { //jos ei liikuta, mutta waypoint2:ssa on jotain
    //  if(round(sijainti.first) == waypoint1.first and round(sijainti.second) == waypoint1.second) {  //JOS t�m� ei riit� voi aikaisemmin laittaa suunnan perusteella semmoisen, ett� yksitt�ist� sijaintia (first tai second) ei en�� muuteta, jos se on sama tai yli kuin vastaava waypoint)

    if(wp1muutos != 0 or wp2muutos != 0 or wp3muutos != 0 or wp4muutos != 0) {    //P�ivitet��n ruudulla-arvo (hahmon n�kyvyyskerros), jos se on s��detty� p�ivittyv�ksi
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

//Funktio joka laskee hahmon k�velysuunnan perusteella siihen sopivat k�velyframet. HUOM!!! frame = 15 taitaa olla vasta oletus
void hahmoluokka::framesuunta() {
  cout << "framesuunta, liikesuunta.first & liikesuunta.second: " << liikesuunta.first << ", " << liikesuunta.second << endl;
  if(liikesuunta.second < 0) { //jos menn��n yl�s
    if(liikesuunta.second <= -0.55) frame = 11;  //Kun pienent�� tuon 0.4:n itseisarvoa (ja sama tuossa alempana), laajenee se ala, ett� k�ytet��n yl�s/alas -meno frameja
    else {
      frame = 3;
      if(liikesuunta.first < 0) symmetrinen ? flip = true : frame = 15;
    }
  }
  else { //jos menn��n alas
    if(liikesuunta.second >= 0.55) frame = 7;
    else { //DEBUG: JOS KONSTRUKTORISSA ARVO EI OLE VALMIIKSI 3, SE MENEE SITTEN T�NNE
      frame = 3;
      if(liikesuunta.first < 0) symmetrinen ? flip = true : frame = 15;
    }
  }
  cout << "FRAMESUUNTAFRAME: " << frame << endl;
}

pair<double,double> hahmoluokka::suunnanlasku() {

  pair<double,double> suunta;

  //Jos k�vell��n sivulle ja vasemmalle, niin flippaus p��lle
  //T�ss� on sopiva hetki nollata flippaus. Ainakin puhe-framejen kanssa voi tulla ongelmia, jos flippaus j�� p��lle turhaan.
  (frame >= 3 and frame <= 6 and waypoint1.first < sijainti.first) ? flip = true : flip = false;  //HUOM.! T�h�n pit�� lis�t� my�s ne ep�symmetriset frame

  //Ensin tarvitaan suuntakulman selvitt�miseksi molempien kolmion kateettien pituudet, ne ovat x:n ja y:n muutosten itseisarvot. Ne saadaan kun pienempi x:n ja y:n arvo (l�ht�paikka ja p��m��r�) v�hennet��n aina suuremmasta.

  int xsuunta = 1;
  int ysuunta = 1;

  if(waypoint1.first < sijainti.first) suunta.first *= -1;
  if(waypoint1.second < sijainti.second) suunta.second *= -1;
  
  //Funktion loppu ei toimi, jos vain toinen waypoint on 0. T�ll�in ei tarvitse muutenkaan laskea
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
  //suuntaluku = kulma / 90 ja jos tulos on > 0.5, se tulee sille arvolle, joka muuttuu enemm�n (ja toiselle 1 - suuntaluku)
  double suuntaluku = (atan(y_itmuutos/x_itmuutos)); //Y-arvo on aina vastainen kateetti ja X-arvo viereinen kateetti. atan-funktio = tan^-1
  //Suuntaluku on kulman asteet/radiaanit. T�m� pit�� jakaa suorakulmalla, jotta saadaan, kuinka monta prosenttia haluttu muutos on eli asteet/radiaanit t�ss� muunnetaan koordinaatiston kielelle, joka on 0-1 eik� esim 0-90. Toisen muutoksen prosentit saadaan tietysti v�hent�m�ll� se 1:st�.
  suuntaluku /= M_PI_2;  //atan antaa tuloksen radiaaneina. Radiaaneina suorakulma on pii/2 (M_PI_2 = pii/2)
  //suuntaluku = suuntaluku*180/M_PI; //atan palauttaa radiaaneina, konversio takaisin asteiksi
  //cout << "suuntaluku (deg, kulma?): " << suuntaluku << endl;
  //suuntaluku /= 90;

  //Mihin suuntaan ollaan enemm�n menossa, sille isompi kulma
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

  //Etumerkit miinukseksi tarvittaessa, t�t� ennen on laskettu itseisarvoilla

  if(waypoint1.first < sijainti.first) suunta.first *= -1;
  if(waypoint1.second < sijainti.second) suunta.second *= -1;

  suunta.first *= KNOPEUS; //K�velynopeuden kerroin
  suunta.second *= KNOPEUS;
  //  cout << "SUUNTA: " << suunta.first << " " << suunta.second << endl;
  return suunta;
}

void hahmoluokka::arkit() {
//N�IT� ARKKIEN M��RI� KUN MUUTTELEE PIT�� SITTEN MY�S MUUTTAA VASTAAVIEN SDL_RECT-ARRAYIDEN KOKOJA KOODIN ALUSSA. AINA EI TULE EDES VIRHEILMOITUSTA.

  //Suoraan edest�
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
  //Sivuk�vely
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
  //K�vely alas
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
  //K�vely yl�s
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
  kissa_arkki[16].w = 0;  //T�rke��, ett� flipattava puheframe on yht� leve� kuin vastaava koko hahmon frame
  kissa_arkki[16].h = 0;
  kissa_arkki[17].x = 208;
  kissa_arkki[17].y = 0;
  kissa_arkki[17].w = 75;
  kissa_arkki[17].h = 51;
  //Ilmeet:
  //Seisotaan kasvot kameraa kohden, surullinen h�mm�stys
  kissa_arkki[18].x = 208;
  kissa_arkki[18].y = 56;
  kissa_arkki[18].w = 75;
  kissa_arkki[18].h = 51;
  //Sad
  kissa_arkki[19].x = 274;
  kissa_arkki[19].y = 0;
  kissa_arkki[19].w = 45;
  kissa_arkki[19].h = 51;
  
  //Suoraan edest�
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
  //Sivuk�vely
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
  //K�vely alas
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
  //K�vely yl�s
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
  koira1_arkki[16].w = 57;  //T�rke��, ett� flipattava puheframe on yht� leve� kuin vastaava koko hahmon frame
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
  
  //Suoraan edest�
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
  //Sivuk�vely
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
  //K�vely alas
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
  //K�vely yl�s
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
  koira2_arkki[16].w = 57;  //T�rke��, ett� flipattava puheframe on yht� leve� kuin vastaava koko hahmon frame
  koira2_arkki[16].h = 46;
  koira2_arkki[17].x = 248; //oli 254
  koira2_arkki[17].y = 0;
  koira2_arkki[17].w = 57;
  koira2_arkki[17].h = 44;
  //Ilmeet, sad edest�
  koira2_arkki[18].x = 316; //oli 254
  koira2_arkki[18].y = 0;
  koira2_arkki[18].w = 57;
  koira2_arkki[18].h = 40;

  //Suoraan edest�
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
  //Sivuk�vely
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
  //K�vely alas
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
  //K�vely yl�s
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
  naata_arkki[16].w = 255-210;  //T�rke��, ett� flipattava puheframe on yht� leve� kuin vastaava koko hahmon frame
  naata_arkki[16].h = 42;
  naata_arkki[17].x = 158; 
  naata_arkki[17].y = 0;
  naata_arkki[17].w = 209-158;
  naata_arkki[17].h = 45;

  //Suoraan edest�
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
  //Sivuk�vely
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
  //K�vely alas
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
  //K�vely yl�s
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
  mayra_arkki[16].w = 51;  //T�rke��, ett� flipattava puheframe on yht� leve� kuin vastaava koko hahmon frame
  mayra_arkki[16].h = 42;
  mayra_arkki[17].x = 110; 
  mayra_arkki[17].y = 0;
  mayra_arkki[17].w = 52;
  mayra_arkki[17].h = 48;
  //Ilmeet: ahdistunut (edest�), h�mm�stynyt (edest�), ahdistunut (sivulta)
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

  //Suoraan edest�
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
  //Sivuk�vely
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
  //K�vely alas
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
  //K�vely yl�s
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
  kissak_arkki[16].w = 0;  //T�rke��, ett� flipattava puheframe on yht� leve� kuin vastaava koko hahmon frame
  kissak_arkki[16].h = 0;
  kissak_arkki[17].x = 208;
  kissak_arkki[17].y = 0;
  kissak_arkki[17].w = 75;
  kissak_arkki[17].h = 51;
  //Ilmeet:
  //Seisotaan kasvot kameraa kohden, surullinen h�mm�stys
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

vector<esineluokka*> esinevektori; //Vektori, johon liitet��n esineluokan j�seniin osoittavat pointterit

//void esineluokka::yhdista_tekstuuri(tekstuuriluokka *olio) {spritet = olio;}  JOS TULEE FRAMET

esineluokka::esineluokka():
  ruudulla(0), tekstuuri(NULL) { //Pair-muuttujat puuttuvat t�st�, niihin tulisi vain nollaa
  esinevektori.push_back(this);
}

esineluokka::~esineluokka() {
  //  delete[] esinerect; PALAUTA T�M� RIVI, JOS FRAMET TULEE ESINELUOKKAAN!!!!
  //Tuleeko t�h�n joku tekstuurin tuhoaminen vai onko se jo siell� muualla
}

/*
//Esineen rect yhdistet��n sen spritesheettiin
void esineluokka::yhdista_rect(SDL_Rect *rect, int arraynpituus, unsigned int arraynkoko) {  //Funktioon l�hetet��n array, jolloin sinne l�htee arrayn ensimm�iseen elementtiin osoittava pointteri ja n�in ollen parametrin t�ytyy olla pointteri-tyyppinen
  esinerect = new SDL_Rect[arraynkoko]; //Dynaamisesti allokoituvaan arrayseen muistit
  for(int counter = 0; counter != arraynpituus; counter++) { //Assignataan sis�lt�
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
    //tekstuurien filtter�inti lineaariseksi
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
      cerr << "Linear filtering failed" << endl;
    }
    //Ikkunan luonti
    ikkuna = SDL_CreateWindow("Hanna-Kaisan tietokonekerho", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(ikkuna == NULL) {
      cerr << "Window creation failed - " << SDL_GetError() << endl;
      success = false;
    }
    else {
      //Render�ij�n luonti
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
	//Initialisoidaan render�ij�n v�ri
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
    cerr << "Object p�yt� load fail" << endl;
    success = false;
  }
  if(!kpoytateks.tiedostonlataus("animpng/poyta2.png")) {
    cerr << "Object kp�yt� load fail" << endl;
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
//puhekuplan osoittimien piirtofunktio. T�t� k�ytet��n sek� piirt�ess� se muoto, ett� t�ytt�ess� osoitin valkoisella (jolloin funktio kertoo, miten y-pisteet rajautuvat). Toisen t�llaisen voisi ehk� tehd�, jos haluaa toisenkin kaaren siihen osoittimeen

//puhekuplan osoitinosan piirto toisen asteen yht�l�� k�ytt�m�ll�. kupasema == ala --> yl�sp�in aukeavat kaaret
void osoitin(int bubw, int bubh) {
  //  int kpituus;
  //  kuplanpituus == 0 ? kpituus = OLETUSKUPLANPITUUS : kpituus = kuplanpituus;
  //  if(kuplavo == 1 or (kuplavo == 0 and kuppoint->first > SCREEN_WIDTH/2)) bubw += kpituus-100;

  //  -osoittimen vasen-oikea suunnan saa muutettua, kun for-sulkeiden laskurin laittaa dekrementoimaan ja jatkoehdoksi > -20
  bool vasen = false; //Kupla vasemmalle (vai oikealle) p�in?
  if(kuplavo == 1 or kuppoint->first-kamerax1 > SCREEN_WIDTH/2) vasen = true;  //Eik� t�m� pit�isi ottaa hahmon sijainnin eik� kuplan mukaan? T�rke�� joka tapauksessa, ett� otetaan samalla tavalla kuin kuplanpiirtofunktiossa (osoittaako kupla vasemmalle vai oikealle)

  if(kupasema == 2) bubh += 5;
  //(ainakin) Yl�kupla-h
  else bubh = kuppoint->second-30;

  //Yl�kupla-w vasemmalle
  if(vasen)  bubw = kuppoint->first+10-kamerax1;
  //Yl�kupla-w oikealle
  else bubw = kuppoint->first+40-kamerax1;

  //N�iss� kupasema > 2 m��rittelyiss� w ja h vaihtavat paikkaa, kun kaari keikautetaan
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
  int laskuri3 = 30; //Huom! T�m� oli double ja double-arvot (1.9 jne.) sopivat my�s
  int inkdek;
  if(vasen) inkdek = -1;
  else inkdek = 1;
  for(int laskuri = 0; laskuri > -20 and laskuri < 20; laskuri += inkdek) {  //Vasemmalle p�in kaartuva osoitin dekrementoi laskuria ja jatkoehto on laskuri > -20
    //  for(int laskuri = 0; laskuri < 20; laskuri++) {  //T�m� antaa oikealle kaartuvan osoittimen
    y7 = pow((0.2*laskuri2),2)+(-0.3*laskuri2)-1; //n�m�kin voisi upottaa ilmeisesti suoraan noihin piirtokutsuihin vai meneek� liian sotkuksi
    y8 = pow((0.2*laskuri3),2)+(-0.3*laskuri3)-1;
    if(kupasema == 2 or (!vasen and kupasema > 2)) {
      y7 *= -1;
      y8 *= -1;
    }

    //Jos kupla sivulla, X ja Y vaihtaa paikkaa, niin saadaan sivusuuntaan keikautettu osoitin
    if(kupasema > 2) SDL_RenderDrawLine(piirturi, bubh+y7, bubw+laskuri, bubh-3+y8, bubw+laskuri);
    else SDL_RenderDrawLine(piirturi, bubw+laskuri, bubh+y7, bubw+laskuri, bubh-3+y8);

    /*
-alasp�in avautuu, jos laittaa koko y7 ja y8 kaavan negaatioksi JA vaihtaa laskuri3:n ja 2:n paikkaa kesken��n loopin lopussa (eli kummasta dekrementoidaan 1 ja kummasta 2). T�m� alasp�in on k�yt�ss� kaiketi my�s sivulle p�in teht�ess�.
    */
    
    
    //    SDL_RenderDrawPoint(piirturi, 120+laskuri, bubh+y7);
    //    SDL_RenderDrawPoint(piirturi, 120+laskuri, bubh-3+y8); //T�m�n piirto pit�isi lopettaa jossain vaiheessa (samoin v�ritt�vien poikkiviivojen)
    //    SDL_RenderDrawLine(piirturi, (120+laskuri), (bubh-2+y8), (120+laskuri), (bubh+y7));
    //kolmas argumentti oli bubh+y7 ja sitten bubh-10+y7
    //    SDL_RenderDrawLine(piirturi, (bubw+40+laskuri), (bubh+bubh2), (bubw+40+laskuri), (bubh+bubh2+y7));   //Samaan tapaan tekisi helposti toisenkin kaaren tuohon, joka rajoittuisi yl�- eik� alasuunnasta, jos jaksaisi viel� kokeilla sille sopivan yht�l�n
    
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
//puhekuplan osoittimien piirtofunktio. T�t� k�ytet��n sek� piirt�ess� se muoto, ett� t�ytt�ess� osoitin valkoisella (jolloin funktio kertoo, miten y-pisteet rajautuvat). Toisen t�llaisen voisi ehk� tehd�, jos haluaa toisenkin kaaren siihen osoittimeen
int funktio1(int num) {
  return (pow((+0.2*num),2)+(-0.3*num)-1); //Toisen asteen yht�l�
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
    //    SDL_RenderDrawLine(piirturi, (bubw+40+laskuri), (bubh+bubh2), (bubw+40+laskuri), (bubh+bubh2+y7));   //Samaan tapaan tekisi helposti toisenkin kaaren tuohon, joka rajoittuisi yl�- eik� alasuunnasta, jos jaksaisi viel� kokeilla sille sopivan yht�l�n
    laskuri2--;
  }
}
*/

//Mit� tietoa t�h�n funktioon pit�� sy�tt��?
//vaerin m��rittely siirretty alkuun
//muokattu tekstirivi - t�m� ehk� pit�isi sy�tt��
//bubblew, bubbleh
//pit�isik� laittaa parametri, josta tulee tekstin v�ri?
void tekstirend(int bw, int bh, int moneskorivi, string teksti, TTF_Font* kirjaisinlaji) {
  int rivinasetus = (5+((moneskorivi-1)*merkinkorkeus)); //Arvo 5 on v�li yl�reunasta. Voisi ehk� laittaa constiksi? Kursorirendiss� on vastaava (bubh+5)
  SDL_Surface* pinta2 = TTF_RenderText_Solid(kirjaisinlaji, teksti.c_str(), vaeri);
  int tekstinleveys = pinta2->w;
  //  int tekstinkorkeus = pinta2->h;  //T�m� onkin sama kuin merkinkorkeus --> ei ilmeisesti tarvita
  //  cout << teksti << endl;
  //  cout << "tekstinleveys & -korkeus: " << tekstinleveys << " - " << tekstinkorkeus << endl;
  SDL_Texture* tekstuuri2 = SDL_CreateTextureFromSurface(piirturi, pinta2);
  //  SDL_Rect tekstinelio = {bw+5, bh+rivinasetus-skrollausarvo, tekstinleveys, tekstinkorkeus}; //skrollausarvo lis�tty text_scroll.cpp:hen
  SDL_Rect tekstinelio = {bw+5, bh+rivinasetus-skrollausarvo+RIVIVALI*(moneskorivi-1), tekstinleveys, merkinkorkeus}; //skrollausarvo lis�tty text_scroll.cpp:hen. Onko t�ss�kin 5 rako kuplan yl�reunan ja tekstin v�lill�?
  SDL_RenderCopy(piirturi, tekstuuri2, NULL, &tekstinelio);
}

void kursorirend(int bubw, int bubh, int rivinumero, int kirjaimennumero, string merkki, TTF_Font* kirjaisinlaji) {
  //bubh on kuplan sivuttain menev�n palkin yl�reuna eli siit� alkaa se teksti suoraan
  rivinumero--;
  bubh += merkinkorkeus*rivinumero;
  kirjaimennumero--;
  bubw += merkinpituus*kirjaimennumero;
  SDL_Surface* pinta3 = TTF_RenderText_Solid(kirjaisinlaji, merkki.c_str(), vaeri);      
  SDL_Texture* peitekirjain = SDL_CreateTextureFromSurface(piirturi, pinta3); 
  SDL_Rect tekstinelio = {bubw+5, bubh+5+RIVIVALI*(rivinumero), merkinpituus, merkinkorkeus};  //bubh+5:ssa 5 on rako kuplan yl�reunan ja alkavan tekstin v�lill�
  SDL_SetRenderDrawColor(piirturi, 0xFF, 0xA5, 0x00, 0x00);
  SDL_RenderFillRect(piirturi, &tekstinelio);   //kursori pit�� rendata t�ss� eri v�risten tekstien v�liss�
  SDL_RenderCopy(piirturi, peitekirjain, NULL, &tekstinelio);
}

//Funktio, joka palauttaa stringin� kirjainmerkin, joka pit�� sill� kertaa n�ytt�� kursorissa (sama aina mik� j�� kursorin alle)
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

//T�m� funktio pit�isi ajaa puhekuplan k�yt�n j�lkeen. Koodille on t�rke�� tiet��, mitk� rivit ovat tyhji�. Ei ole k�yt�ss� t�ss� koodissa
void riviennollaus() {
  for(vector<string>::iterator eraser_iter = vuorosanavec.begin(); eraser_iter != vuorosanavec.end(); ++eraser_iter) {
    vuorosanavec.erase(eraser_iter);
    --eraser_iter;
  }
}

//Vuorosanan sopivanpituisiin riveihin p�tkiv� funktio
void autowrap(int rivinpituus, string vuorosana1) {
//looppeja, jotka k�y vuorosanaa alusta aina rivinpituuden kohdalta l�pi taaksep�in ja ensimm�isen v�lily�nnin kohdalla laittaa seuraavaksi riviksi sit� edelt�v�t merkit ja poistaa ne vuorosanasta
  string tempstring;
  int marker = 0; //T�m�n pit�isi merkata koko stringin viimeinen v�lily�nti
  for(int m = vuorosana1.length(); marker == 0;  m--)
    if(vuorosana1[m] == ' ') marker = vuorosana1.length()-m;
  for(int i = rivinpituus; vuorosana1.length() > marker; i--) {
    if(vuorosana1[i] == ' ') {
      //looppi joka tekee tempstringiin stringin joka lis�t��n elementiksi vuorosanavec:iin
      tempstring = "";
      for(int j = 0; j <= i; j++) tempstring += vuorosana1[j];
      vuorosanavec.push_back(tempstring);
      //Poistetaan yht� pitk� p�tk� (se vektoriin lis�tty) vuorosana1:st�, deletoidaan aina alusta merkki
      for(int k = 0; k <= i; k++) vuorosana1.erase(0,1);
      i = rivinpituus;
    }
  }
  //Edelliset loopit perustuu v�lily�ntiin, joten viimeinen sana j�� yleens� yli ilman v�lily�nti� ja pit�� lis�t� erikseen
  if(vuorosana1 != "" or vuorosana1 != " ") {
    vector<string>::iterator iter = --vuorosanavec.end();
    //    iter--;  postfix-- on siirretty t�st� tuohon ylemm�s, n�ytt�isi toimivan
    //Mahtuuko viimeinen sana samalle riville
    if((*iter).length() + vuorosana1.length() <= rivinpituus) *iter += vuorosana1; //Tuohon *iterin ja vuorosana1:n vertailuu n�ytt�� sis�ltyv�n niiden v�liss� oleva v�lily�ntikin korrektisti
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

void puhekulmat(int bubblew, int bubblew2, int bubbleh, int bubbleh2) {  //jostain syyst� n�m� arvot ei tule oikein, jos niit� ei laita parametrein�
	SDL_SetRenderDrawColor(piirturi, 0xFF, 0xFF, 0xFF, 0xFF);
        //vasen yl�kulma
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
	//oikea yl�kulma
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
  //Nyt oletuksena on, ett� puhuja -arvot alkavat nollasta (size alkaa 1:st�)
  if(puhuja >= hahmovektori.size()) {
    cerr << "Liian suuri arvo aseta_puhuja -vektoriin" << endl;
  }
  //  cout << "aseta_puhuja -funktion keskiv�li" << endl;
  vector<hahmoluokka*>::iterator p_iter = hahmovektori.begin()+puhuja;
  //  cout << "aseta_puhuja -funktion kohta x" << endl;
  kuppoint = &(*p_iter)->sijainti;
  //  cout << "aseta_puhuja -funktion loppu" << endl;
}

//Funktio, joka luo seuraavan (vektorin ensimm�isen) kuplan kuplavektorista. M��ritell��n kuplakohtaiset kuplan mitat (joiden arvot varsinaisesti annetaan tarinafunktiossa), kutsutaan autowrap jakamaan teksti, tarkistetaan, pit�isik� tekstin skrollata, vai mahtuuko se kuplaan
void kuplanluonti(int frame_nyt) {
  if(kuplavec.size() == 0) cerr << "Kuplavektori on tyhj�, vaikka yritet��n luoda kuplaa" << endl;
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

  kuplanpituus == 0 ? tempkuppituus = OLETUSKUPLANPITUUS : tempkuppituus = kuplanpituus; //T�h�n voi laittaa my�s kuplanpituudet 1, 2, 3... jos niihin haluaa "vaihtoehtoisia oletuksia"
  rivinpituus = ((tempkuppituus - KUPVASBORDER - KUPOIKBORDER) / merkinpituus); //30 on molemmat borderit (2 x 15). borderi-muuttujan voi tietysti tehd� tarvittaessa erikseen.

  if(rivinpituus < vuorosana1.length()) kursoripaalla = 1;

  if(vuorosana1.size() > rivinpituus) autowrap(rivinpituus, vuorosana1);  //Rivinkatkaisufunkioon
  else {
    vuorosanavec.push_back(vuorosana1); //Paitsi jos vuorosana on rivi� lyhyempi
    tempkuppituus = vuorosana1.length()*merkinpituus+40; //T�ll�in my�s kupla laitetaan lyhyemm�ksi leveydelt��n.
    if(tempkuppituus < 80) tempkuppituus = 80;
  }

  //jos kuplankorkeus on 1, k�ytet��n oletusarvoja, jos 0, lasketaan sopiva korkeus
  if(kuplankorkeus == 1) tempkupkorkeus = OLETUSKUPLANKORKEUS;
  else if(((*kupiter).asemointi == 3) and kuplankorkeus == 0) {
    if(kuppoint->second+40 - ((merkinkorkeus+RIVIVALI)*vuorosanavec.size()+KUPYLABORDER+KUPALABORDER) < 0) tempkupkorkeus = kuppoint->second+40; //Jos sivuyl�kupla ei mahdu ruutuun, korkeus on kattoon asti
    else tempkupkorkeus = (merkinkorkeus+RIVIVALI)*vuorosanavec.size()+KUPYLABORDER+KUPALABORDER; //Jos taas mahtuu, lasketaan siihen koko kuplan koko
  }
  else if(((*kupiter).asemointi == 5) and kuplankorkeus == 0) {
    if(kuppoint->second+20 + ((merkinkorkeus+RIVIVALI)*vuorosanavec.size()+KUPYLABORDER+KUPALABORDER) > SCREEN_HEIGHT) tempkupkorkeus = kuppoint->second+40; //Jos sivualakupla ei mahdu ruutuun, korkeus on kattoon asti
    else tempkupkorkeus = (merkinkorkeus+RIVIVALI)*vuorosanavec.size()+KUPYLABORDER+KUPALABORDER; //Kuten edell�
  }
  else if(((*kupiter).asemointi == 4) and kuplankorkeus == 0) {
    if(((merkinkorkeus+RIVIVALI)*vuorosanavec.size()+KUPYLABORDER+KUPALABORDER) > SCREEN_HEIGHT-20) tempkupkorkeus = SCREEN_HEIGHT-20; //Jos sivualakupla ei mahdu ruutuun, ei ehk� kuitenkaan ihan ruudun kokoista kuplaa
    else tempkupkorkeus = (merkinkorkeus+RIVIVALI)*vuorosanavec.size()+KUPYLABORDER+KUPALABORDER;
  }
  else if(kuplankorkeus == 0) tempkupkorkeus = (merkinkorkeus+RIVIVALI)*vuorosanavec.size()+KUPYLABORDER+KUPALABORDER;
  else tempkupkorkeus = kuplankorkeus;

  //Lasketaan mahdolliset rivit, jotka eiv�t mahdu kuplaan
  ylirivit = (static_cast<double>(vuorosanavec.size())*(static_cast<double>(merkinkorkeus)+static_cast<double>(RIVIVALI)) - (static_cast<double>(tempkupkorkeus) - static_cast<double>(KUPYLABORDER) - static_cast<double>(KUPALABORDER))) / (static_cast<double>(merkinkorkeus) + static_cast<double>(RIVIVALI));
  //T�h�n: jos rivinpituus suurempi kuin vuorosanan pituus
  kuplantila = 2;
  pkupkounter = frame_nyt;
}

//Tekee uuden kupla-structin kupvektoriin. Jos ei laita paikka-, pituus- ja korkeusarvoja, tulee samat kuin edellisess� kuplavektorin repliikiss�
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
    for(int k = 0; (i == 1 ? k <= rivinpituus-nick.length()-9 : k <= rivinpituus); k++) repliikki.erase(0,1); //Ensimm�isell� rivill� katkotaan enemm�n. 8 = etumerkit.
    tempstring = "";

  }
  //Alusta rivej� pois, jos ruutu tulee t�yteen
  while(irc_teksturet.size() >= floor((double)SCREEN_HEIGHT/(double)(rivimaara*irc_MERKINKORKEUS-3*irc_MERKINKORKEUS))) {
    vector<irc_rivi>::iterator eraser = irc_teksturet.begin();
    irc_teksturet.erase(eraser);
  }
}

//T�st� ehk� int, joka palauttaa nollan, jos ei l�ytynyt sit� kohtaa, mihin inkrementoitiin
//zxc
bool tarinafunktio() {
  bool loytyiko = true;
  //ELSEIFFEIN� KAIKKI (paitsi ylin if ja lopussa else)
  //Loppuun else, joka laittaa bool-arvon, ett� ei l�ytynyt mit��n

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
    insert_kup(1, "Hei, Morpheus! Min� olen Tepsam0n ...tai siis ihan vaan Tepsa.");
    insert_kup(4, "Hei, oikea nimeni on Erno.");
    insert_kup(1, "Saavuitkin ajoissa paikalle.");
    insert_kup(4, "Bussi oli v�h�n my�h�ss�, mutta talo l�ytyi helposti.");
    insert_kup(1, "Tule vain peremm�lle!");
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
    skrollihalt = 530; //Maksimiarvo t�lle on kuvan koko X-akselilla miinus SCREEN_WIDTH (eli 1280 t�ss� produktiossa, jossa 1960 on taustakuvan X-koko)
  }
  else if(tarinanvaihe == 107) {
    kuplantila = 1;
    insert_kup(1, "T�ss� ovat siis Hanna-Kaisa...");
    insert_kup(2, "Hei!");
    insert_kup(1, "Valtsu...");
    insert_kup(0, "Hei!");
    insert_kup(1, "...ja Sirkku");
    insert_kup(3, "Hei!");
    insert_kup(1, "Muistat varmaan heid�n nickins�.");
    insert_kup(4, "Joo");
  }
  else if(tarinanvaihe == 108) {
    tarina_t = 2;
    //Rest of the story removed from the sample code
  }

void kuplanpiirto(int frame_nyt) {
  //jos kuplan l�ht�piste + pituus on suurempia kuin screen_width -> kupla vasemmalle
  //jos kuplan l�ht�piste on alle nollan -> kupla oikealle
  //N�m� kuplan l�ht�pisteet pit�� m��ritell� aina t�ss� funktiossa, jos halutaan, ett� hahmo voi liikkua puhuessaan
  //Kupla vasemmalle vai oikealle p�in hahmosta
  bubblew = kuppoint->first-kamerax1;
  if(kuplavo == 2) {
    if(kupasema > 2) bubblew += 55+30; //55 on tuossa noin hahmonspriten leveys ja 30 osoitin ja muu v�li
    //pelkk� kuplavo == 2 ilman sivuX:aa ei vaadi k�sittely�
  }
  else if (kuplavo == 1) {
    if(kupasema > 2) bubblew -= tempkuppituus+30;
    else bubblew -= tempkuppituus-60; //60 on suunnilleen hahmon leveys eli vasemmallekin osoittava kupla alkaa hahmon p��n toiselta puolelta
  }
  else { //jos kuplavo == 0
    //jos hahmo on ruudun oikealla puolella, kupla vasemmalle ja toisin p�in
    if(bubblew > SCREEN_WIDTH/2) {
      if(kupasema > 2) bubblew -= tempkuppituus+30;
      else bubblew -= tempkuppituus-60;
    }
    else if(kupasema > 2) bubblew += 55+30; //55 on tuossa noin hahmonspriten leveys ja 30 osoitin ja muu v�li
    //pelkk� oikealla oleva yl�/alakupla ei vaadi k�sittely�
  }

  //Jos kupla menee yli ruudusta, se siirtyy sen verran, ett� mahtuu ruutuun
  if(bubblew+tempkuppituus > SCREEN_WIDTH) bubblew = SCREEN_WIDTH-tempkuppituus;
  else if(bubblew < 0) bubblew = 0;

  if(kupasema == 1) bubbleh = (kuppoint->second)-tempkupkorkeus-30; //-30 on lis�korkeus, jota ilman kupla alkaisi suoraan hahmon p��lt� ja osoite uppoaisi liian alas
  else if(kupasema == 3) bubbleh = (kuppoint->second)-tempkupkorkeus+40; //sivuyl�kuplan alalaita on jossain suun paikkeilla (+40) ja siit� miinus kuplankorkeus
  else if(kupasema == 5) bubbleh = (kuppoint->second)+20; //sivualakuplan yl�laita on jossain silmien kohdalla (+20)
  else if(kupasema == 4) bubbleh = (kuppoint->second)+20-(tempkupkorkeus/2); //sivukeskikuplan yl�laita on silm�nkorkeudelta (+20) ja miinus puolet kuplankorkeudesta
  else bubbleh = (kuppoint->second)+150; //ala-asemassa kupla l�htee jostain hahmon jalkojen alta

  //sivuy: kuplan alalaita on v�h�n hahmon suun yl�puolella eli miinuksen sijasta plussaa
  
  bubblew2 = tempkuppituus; //oli 360, t�m� on siis kuplan leveys (ml. borderit). Kuplanpituus my�hemmin tulee ilmeisesti t�h�n kohtaan m��ritelt�v�ksi, kunhan autowrapist� tehd��n oma funktionsa
  bubbleh2 = tempkupkorkeus; //kuplan korkeus yl�laidasta laskien
  cliprect = {bubblew, bubbleh, bubblew2, static_cast<int>((KUPYLABORDER+(vuorosanavec.size() - ceil(ylirivit))*(merkinkorkeus+RIVIVALI)))};
  kupla1 = {bubblew, (bubbleh + 15), bubblew2, bubbleh2 - 30}; //kuplan leve�mpi pala
  kupla2 = {(bubblew + 15), bubbleh, (bubblew2 - 30), bubbleh2}; //kuplan korkeampi pala

  SDL_RenderFillRect(piirturi, &kupla1); SDL_RenderFillRect(piirturi, &kupla2); //Piirret��n puhekuplan molemmat kappaleet (suorapalkit). &-merkin avulla saadaan kuplan muistiosoite arvojen sijaan, koska SDL_Rect palauttaa pointterin eik� arvoja
  osoitin(bubblew, bubbleh);

  puhekulmat(bubblew, bubblew2, bubbleh, bubbleh2);  //N�m� kuplan kulmat rendataan ennen teksti�, muuten tulevat tekstin p��lle

  if(kuplantila == 3 or ylirivit > 0) {
    //Tekstiskrollia varten tarvitaan koko puhekuplan sis�lt�v� neli�, jota ei leikata (jotta teksti leikkautuu skrollatessaan poistuessaan t�lt� alueelta)
    SDL_RenderSetClipRect(piirturi, &cliprect);  //T�ss� funktiossa rect on const pointer. se ei kuitenkaan tarkoita, ett� objektin pit�isi olla const vaan funktio ei muuta sit� objektia. referenssin� argumentin laittaminen tekee siit� ilmeisesti k�yt�nn�ss� pointerin (pointteri ja referenssi on melko samanlaisia). kupla1 ei ole aivan sopivan kokoinen recti ilmeisesti tekstin feidaamiseen, joten sen tilalle voidaan laittaa toinen, v�h�n korkeampi, niin n�ytt�isi ehk� paremmalta
  }

  for(vector<string>::iterator renditer = vuorosanavec.begin(); renditer != vuorosanavec.end(); ++renditer) {
    tekstirend(bubblew, bubbleh, distance(vuorosanavec.begin(), renditer)+1, *renditer, fontti);
  }

  if(kuplantila == 3 or ylirivit > 0) SDL_RenderSetClipRect(piirturi, NULL);  //if on t�ss� vain, ett� v�ltet��n turha funktiokutsu. kai sen voisi muuten NULLata aina
  //rivimuuttuja ja kirjainmuuttuja (monesko kirjain)

  
  if(kursoripaalla == 1) { //kursorip��ll� voisi kyll� sis�lty� puhekupla-muuttujaankin
    if(onkoriviloppu(mikarivi.second, moneskirjain) and kursoripaalla < 3) {  //Jos rivi loppuu, siirryt��n seuraavalle riville ja rivin alkuun ...jos kursori ei ole maalissa
      if(mikarivi.second == vuorosanavec.size() and onkoriviloppu(mikarivi.second, moneskirjain) and kursorikountteri < 4) {  //Jos ollaan vikalla rivill� ja vikassa kirjaimessa.
	kursoripaalla = 3;
      }
      else {  //jos rivi on loppu ja kursori ei ole viel� maalissa, laitetaan kursori rivin alkuun
	mikarivi.first++;
	mikarivi.second++;
	moneskirjain = 0;
      }
    }
  }

  //mikarivi.first & mikarivi.second -muuttujat kasvaa aina yhdell� rivin loputtua, jos on uusi rivi tulossa
  //string kursorimerkki = (mikarivi.first, moneskirjain);
  if(ylirivit > 0 and kursoripaalla == 1 and mikarivi.first-1 == vuorosanavec.size()-ceil(ylirivit)) kursoripaalla = 2;
  if(kursoripaalla == 2) {
    skrollausarvo += 2;
    //jos skrollausarvon kanssa huomioitu korkeus on sellainen, ett� vika rivi on n�kyviss�, sitten kursoripaalla = true ja mikarivi = ensimm�inen piilossa ollut, skrollausarvo = 0
    if(bubbleh-skrollausarvo+KUPYLABORDER+vuorosanavec.size()*(RIVIVALI+merkinkorkeus) <= bubbleh+KUPYLABORDER+(vuorosanavec.size() - ceil(ylirivit))*(merkinkorkeus+RIVIVALI)) { //T�ss� hyvin oleellista on, ett� vertailun oikeanpuoleinen y-koordinaatti on laskettu samalla logiikalla kuin cliprectin korkeus
      kursoripaalla = 1;
      //skrollin j�lkeinen rivi on kuplaan mahtuvien rivien m��r� miinus uusien rivien m��r� plus yksi
      mikarivi.first = ((tempkupkorkeus - KUPYLABORDER - KUPALABORDER) / (merkinkorkeus + RIVIVALI) - ceil(ylirivit)) + 1;
    }
  }
  
  if(kursoripaalla == 1) moneskirjain++; //jos kursori ei ole maalissa, lis�t��n rivi
  else if(kursoripaalla > 2 and frame_nyt%10 == 0) {kursoripaalla = 3; kursorikountteri++;}
  else if(kursoripaalla > 2 and frame_nyt%10 == 5) kursoripaalla = 4;
  if(kursoripaalla % 2 != 0) {kursorirend(bubblew, bubbleh, mikarivi.first, (kursoripaalla > 2 ? moneskirjain+1 : moneskirjain), (kursoripaalla == 3 ? " " : (kursorikirjain(mikarivi.second, moneskirjain))), fontti);}
  if(kursorikountteri == 4) {kursoripaalla = 0; kuplantila = 3;}  //Kun on v�lkytty nelj� kertaa, lopetetaan. HUOMHUOM!! Lopullisessa koodissa kursorikountterikin pit�� resetoida, mutta t�ss� koodissa sit� ei tarvitse (vaan se aiheuttaa itse asiassa kursorin palaamisen)
  //pinta2:sta (vuorosana1) otettu tekstinleveys on 1890 ja korkeus 15. merkkej� siin� on 269 eli 7 tuntuisi olevan merkinleveys.
  //if(tekstiskrolli == true and frame_nyt%10 < 9) {skrollausarvo++;}
  if(kuplantila == 3 and frame_nyt%10 <= 20) skrollausarvo +=10;  // tuo <= 9 ei hidasta ollenkaan eli hidastus on nyt pois p��lt�. Alemmat arvot hidastavat. HUOM! t�ss� voi nopeuttaa skrollausta my�s lis��m�ll� skrollausarvoon ykk�st� suurempia m��ri�

  if(kursoripaalla == 0 and pkupkounter+35 <= frame_nyt) kuplantila = 3;
  
  //Jos tekstin poistava tekstiskrolli on vienyt koko tekstin kuplan yl�rajan yl�puolelle
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
  //Ovatko kaikki hahmot pys�hdyksiss�?
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
  bool feidaus = false; //Vai pit�isik� olla int, jolloin eri numeroilla saisi eri nopeuksia
  bool liikutaan = false;
  Uint8 alpha = 0;
  Uint32 ajastin1 = 0;
  Uint32 ajastin2 = 0;
  Uint32 ajastin_fr = 0;  //k�velyframejen nopeus
  Uint32 ajastin_r = 0;
  Uint32 irc_ajastin = 0;
  bool irc_sekunti = false;
  int rcount = 0;
  SDL_Rect irc_ylapalkki = {0, 0, SCREEN_WIDTH, irc_MERKINKORKEUS};  //N�ihin palkkeihin ei taida tulla riviv�li�
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
  
  //K�ynnistet��n SDL ja luodaan ikkuna
  if(!init()) {
    cerr << "Init fail" << endl;
  }
  else {
    //Ladataan media
    if(!lataamedia()) {
    cerr << "Media load fail" << endl;
    }

    else {

      kissa.arkit();  //T�m�n funktion voi kutsua mink� tahansa hahmoluokan olion kautta
      
      kissa.yhdista_tekstuuri(&kissateks);
      kissa.yhdista_rect(kissa_arkki, (sizeof(kissa_arkki)/sizeof(*kissa_arkki)), sizeof(kissa_arkki));

      kissak.yhdista_tekstuuri(&kissakteks);
      kissak.yhdista_rect(kissak_arkki, (sizeof(kissak_arkki)/sizeof(*kissak_arkki)), sizeof(kissak_arkki));
      
      koira1.yhdista_tekstuuri(&koirateks1);
      koira1.yhdista_rect(koira1_arkki, (sizeof(koira1_arkki)/sizeof(*koira1_arkki)), sizeof(koira1_arkki));
      
      koira2.yhdista_tekstuuri(&koirateks2);
      koira2.yhdista_rect(koira2_arkki, (sizeof(koira2_arkki)/sizeof(*koira2_arkki)), sizeof(koira2_arkki)); //Kun array l�hetet��n funktioon (ensimm�inen parametri), se on pointteri arrayn ensimm�iseen elementtiin, toisessa parametriss� lasketaan arrayn pituus eli se arkkien m��r�, kolmas parametri on arrayn koko tavuina (?)

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
      
      bool quit = false; //P��loopin lippu
      SDL_Event e; //Event handler

      
      ///usr/share/fonts/truetype/ttf-dejavu/DejaVuSansMono-Bold.ttf
      ///usr/share/matplotlib/mpl-data/fonts/ttf/DejaVuSansMono-Bold.ttf

         //Thinkcentre
	 fontti = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 15);  //Parameters: path to the font, font size. Jos polku on v��rin, tulee segmentation fault
	 irc_fontti = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf", 15);  //Parameters: path to the font, font size. Jos polku on v��rin, tulee segmentation fault

	 
	 //Thinkpad
      //fonttikoko oli 12
      //fontti = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSansMono-Bold.ttf", 15);  //Parameters: path to the font, font size. Jos polku on v��rin, tulee segmentation fault
      //irc_fontti = TTF_OpenFont("/usr/share/fonts/TTF/DejaVuSansMono-Bold.ttf", 15);  //Parameters: path to the font, font size. Jos polku on v��rin, tulee segmentation fault

      SDL_Surface* kello1 = TTF_RenderText_Solid(irc_fontti, "18:41", {255, 255, 255});
      SDL_Surface* kello2 = TTF_RenderText_Solid(irc_fontti, "18:42", {255, 255, 255});
      SDL_Surface* kello3 = TTF_RenderText_Solid(irc_fontti, "18:43", {255, 255, 255});
      SDL_Surface* pinta4 = TTF_RenderText_Solid(irc_fontti, " <", {70, 70, 70});
      SDL_Surface* pinta5 = TTF_RenderText_Solid(irc_fontti, ">", {70, 70, 70});
      aika_leveys = kello1->w;
      cout << "kello w & h" << kello1->w << " " << kello1->h << endl;
      aika_leveys = kello1->w;
      //      as_leveys = pinta4->w; //K�ytet��nk� n�it�???
      //      ks_leveys = pinta5->w; //Hetkinen, eik� t�h�n riit� merkinpituus!!!!???? Ja eik� n�m� muutenkin saa laskettua merkinpituuden pohjalta muillekin?
      //  int tekstinkorkeus = pinta2->h;  //T�m� onkin sama kuin merkinkorkeus --> ei ilmeisesti tarvita
      aika1 = SDL_CreateTextureFromSurface(piirturi, kello1);
      aika2 = SDL_CreateTextureFromSurface(piirturi, kello2);
      aika3 = SDL_CreateTextureFromSurface(piirturi, kello3);
      SDL_Texture* sulut_auki = SDL_CreateTextureFromSurface(piirturi, pinta4);
      SDL_Texture* sulut_kiinni = SDL_CreateTextureFromSurface(piirturi, pinta5);
  //  SDL_Rect  = {bw+5, bh+rivinasetus-skrollausarvo+RIVIVALI*(moneskorivi-1), leveys, irc_MERKINKORKEUS}; //skrollausarvo lis�tty text_scroll.cpp:hen. Onko t�ss�kin 5 rako kuplan yl�reunan ja tekstin v�lill�?

      //Ohjelman ollessa p��ll�
      while(!quit) {
	ajastin1 = SDL_GetTicks();

	//K�sittele jonon tapahtumat
	while(SDL_PollEvent(&e)!=0) {
	  //k�ytt�j� quittaa
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

	//Renderoi tausta, taustan liike, hahmojen sijaintien p�ivitys
	if(taustakuva.tekstuuri != NULL) {
	  if(kamskrolli != 0) {
	    //	    cout << kamerax1 << " " << kamerax2 << endl;
	    //	    kamskrolli == 2 ? kamrect = {(kamerax1 += 3), 0, (kamerax2 += 3), SCREEN_HEIGHT} : kamrect = {(kamerax1 -= 3), 0, (kamerax2 -= 3), SCREEN_HEIGHT} ;
   	    kamskrolli == 2 ? kamrect = {(kamerax1 += 3), 0, SCREEN_WIDTH, SCREEN_HEIGHT} : kamrect = {(kamerax1 -= 3), 0, SCREEN_WIDTH, SCREEN_HEIGHT} ;

	    //	    if(kamskrolli == 2 and kamerax2 >= skrollihalt) { //Jos skrollataan oikealle, pys�hdyt��n kun kamerax2 ylitet��n ja laitetaan sen mukaan uusi paikka
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

	  //T�h�n pit�isi lis�t� irc_RIVIVALI
	  for(vector<irc_rivi>::iterator irc_rendit = irc_teksturet.begin(); irc_rendit != irc_teksturet.end(); ++irc_rendit) {
	    if(irc_rendit -> aikaleima != NULL and irc_rendit -> ajastin == 0) {
	      SDL_Rect rect1 = {0, monesrivi*irc_MERKINKORKEUS, 5*irc_MERKINPITUUS, irc_MERKINKORKEUS}; //5 == aikaleiman merkkien lukum��r� (esim. 15:03)
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
          if(SDL_GetTicks() - ajastin_fr > 57) { //Mit� suurempi tuo viimeinen luku, sit� hitaammin k�velyframet vaihtuvat
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

	  //UUSI P�TK�!! (muutettu my�s luokan konstruktoria ja tehty siihen se funktio)

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

	  //if(koira2.ruudulla) koira2.spritet->renderfunktio(0,0);  //koira2.spritet viittaa siis koira2 -olion j�seneen spritet. Yleens� renderfunktioon viitataan member access operaattorilla (piste), mutta nyt kun "spritet" on pointteri, k�ytet��nkin nuolta, joka on pointterien kanssa k�ytett�v� operaattori
	  */

	  liikutaan = false; //Ennen t�t� rivi� kaikki hahmonpiirto-kutsut
	}

	if(kuplantila == 1) kuplanluonti(frame_nyt);
	if(kuplantila == 2 or kuplantila == 3) kuplanpiirto(frame_nyt);
	
	//	koira2.spritet->renderfunktio(0,0);  //koira2.spritet viittaa siis koira2 -olion j�seneen spritet. Yleens� renderfunktioon viitataan member access operaattorilla (piste), mutta nyt kun "spritet" on pointteri, k�ytet��nkin nuolta, joka on pointterien kanssa k�ytett�v� operaattori

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
