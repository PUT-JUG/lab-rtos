# Procesy w systemie Linux, edytory tekstu

## Procesy

### Pojęcie procesu

Każdy uruchomiony w systemie Unix program nosi nazwę procesu. Na proces składają się następujące elementy:

* Kod binarny procesu załadowany z pliku.
* Dane programu: struktury danych zadeklarowane w programie oraz pamięć dynamicznie przydzielona do procesu w trakcie jego działania.
* Dane systemowe: informacja o procesie utrzymywana przez system.
  
Dodatkowo, podczas tworzenia procesu system inicjalizuje systemowe struktury danych opisujące proces, które następnie są aktualizowane podczas wykonania tego procesu. Do danych systemowych należą:

* Identyfikator procesu (PID) - unikalna liczba całkowita jednoznacznie identyfikująca proces.
* Identyfikator procesu macierzystego (PPID) - wartość PID procesu, który stworzył dany proces.
* Środowisko procesu - zbiór zmiennych środowiskowych. Każdy proces ma swoje, niezależne od innych procesów środowisko wykonania, które początkowo jest kopiowane z procesu-rodzica a następnie jest modyfikowane niezależnie od innych procesów.
* Standardowe strumienie danych.
  
Oprócz procesów, w systemie Unix, jak w większości nowoczesnych systemów, wyróżnia się wątki (ang. thread), będące najmniejszymi aktywnymi elementami systemu. Wątek jest rodzajem procesu, który dzieli przestrzeń adresową z innym procesem - każdemu wątkowi jest więc przydzielony niezależny identyfikator.

### Procesy w systemie

Listę procesów dla aktualnej powłoki otrzymamy wywołując polecenie `ps` (ang. processes).

```bash
ps
PID             TTY             TIME               CMD
14285           pts/0           0:00               -csh
14286           pts/0           0:00               ps
^^^^^^          ^^^^^^          ^^^^^              ^^^^^
numer procesu   terminal        czas aktywności    nazwa
```

Pełne informacje o procesach aktualnej powłoki podaje polecenie `ps -f` (ang. *full*)

```bash
ps -f
UID           PID        PPID     C    STIME        TTY         TIME        CMD
student       1633       1625     0    06:00        pts/0       00:00:00    bash
student       2666       1633     0    06:14        pts/0       00:00:00    nano plik.txt
student       2855       1633     0    11:26        pts/0       00:00:00    ps -f
^^^^^^        ^^^^^      ^^^^^^^^      ^^^^^^^      ^^^^^^      ^^^^^       ^^^^^
nazwa         numer      numer         czas         terminal    czas        nazwa procesu
właściciela   procesu    procesu       uruchomienia
                         nadrzędnego
```
Pełne informacje o wszystkich procesach uzyskamy łącząc opcję `-f` z `-e` (ang. *every process*). Poniżej podano inne przydatne przełączniki polecenia `ps`:
*  `-w` powoduje rozszerzenie wyświetlanej listy
*  `-l` (ang. *long*) pozwala wyświetlić dodatkowe informacje o każdym procesie - w rezultacie wyświetlane są dodatkowe kolumny zawierające następujące informacje: 
   *  `S` - status procesu ( `S` - proces jest uśpiony; `R` - proces jest aktualnie wykonywany, `T` - zatrzymany) 
   *  `UID` - indentyfikator właściciela procesu 
   *  `PPID` - identyfikator procesu macierzystego 
   *  `PRI` i `NI` - opisują priorytety procesów 
   *  `WCHAN` - pozwala sprawdzić jaką funkcję systemową jądra wywołał proces

Wiele z procesów uruchamianych jest przy starcie systemu, pozostałe są uaktywniane przez użytkowników w momencie zlecenia wywołania programów. Dowolny proces może uruchomić kolejny proces potomny i stać się macierzystym (nadrzędnym) wobec tego procesu potomnego. W momencie zarejestrowania się użytkownika w systemie uruchomiony zostaje jego pierwszy proces, czyli powłoka interpretująca polecenia użytkownika.

Wszystkie procesy pracujące w systemie tworzą hierarchiczną strukturę, na szczycie której stoi proces `init`, będący rodzicem wszystkich procesów. Proces `init` ma zawsze wartość `PID` równą 1. Hierarchię procesów można obserwować korzystając z programu `pstree`. W systemie Ubuntu, na szczycie drzewa wyświetlanego przez `pstree` znajdziemy proces *systemd*, a nie *init*. Wynika to z faktu, że *init* jest w tym systemie aliasem (dowiązaniem symbolicznym) do programu *systemd* - możesz to potwierzuć komendą `ls -l /sbin/init`.

Istnieje także interaktywna wersja komendy `ps` - program `top`, gdzie oprócz wartości zwracanych przez polecenie `ps` wyświetlane są też inne informacje: aktualna liczbę użytkowników w systemie i średnie obciażenie (linia pierwsza), liczba procesów i ich stany(linia druga), obciążenie procesora (linia trzecia) i informację nt. dostępnej pamięci w systemie (linie czwarta i piąta).

### Usuwanie procesów

Dowolny proces może zostać usunięty z systemu przez jego właściciela. Służy do tego polecenie `kill`, wysyłające do procesu o podanym identyfikatorze sygnał przerwania pracy:

```bash
kill [ -nazwa_lub_numer_sygnału ] identyfikator_procesu
```
Domyślnie, jeśli nie podano numeru sygnału, wysłany zostanie sygnał `TERM`, powodujący zatrzymanie procesu. Aktualnie uruchomiony proces można również przerwać naciskając w konsoli kombinację **Ctrl-C**, co również powoduje wysłanie sygnału `TERM`. Gdy wysłanie sygnału `TERM` jest niewystarczające do zatrzymania procesu, należy wtedy wysłać sygnał `KILL`, który powoduje bezwarunkowe przerwanie procesu:

```bash
kill -KILL identyfikator_procesu
```

Sygnały mają przypisane numeryczne identyfikatory. Identyfikator sygnału `TERM` wynosi 2, natomiast sygnału `KILL` jest równy 9. W poleceniu `kill` można korzystać także z wartości numerycznych synałów:

```bash
kill -9 identyfikator_procesu
```

Zatrzymanie wszystkich procesów o danej nazwie powoduje polecenie `killall`. Przykładowo:

```bash
killall find
```
powoduje zatrzymanie wszystkich programów `find`.

Szczegółową listę sygnałów wraz z ich wartościami numerycznymi zawiera strona pomocy systemowej *signal(7)* (komenda `man 7 signal`). Skróconą listę dostępnych sygnałów można uzyskać poprzez wywołanie `kill -l`. 

### Priorytety procesów

Każdy proces wykonywany w systemie posiada przypisany mu priorytet, który można odczytać w wyniku wywołania polecenia `ps` z przełącznikiem `-l`.

Kolumna `PRI` wyświetlana w wyniku tego polecenia zawiera informacje o wartości priorytetu określonego procesu, nadanej mu poprzez system operacyjny. Wartość ta nie może być bezpośrednio zmieniana przez użytkownika. Jednakże użytkownik może wpłynąć na wartość `PRI`, zmnieniając tzw. liczbę *nice*, której aktualna wartość znajduje się w kolumnie `NI`. Wartość liczby nice należy do przedziału: od -20 do 19 i początkowo przyjmuje wartość 0. Im mniejsza wartość liczby *nice* tym wyższy priorytet procesu. Dla działającego procesu liczbę *nice* można zmienić poleceniem:

```bash
renice zmiana_priorytetu [ -p ] pid [ -u użytkownik ]
```
Na przykład:
```bash
renice +10 3442
```
zwiększa liczbe *nice* o 10, co powoduje zmniejszenie priorytetu tego zadania. Zwykli użytkownicy mogą jedynie zwiększać liczbę *nice*, czyli obniżać priorytet wykonania swoich zadań, natomiast użytkownik *root* jest uprawniony do wykonywania wszelkich zmian na wartości *nice*.

Możliwe jest uruchamianie nowych procesów z ustawionym już nowym priorytetem:

```bash
nice -n zmiana_priorytetu polecenie
```

### Zarządzanie procesami

Procesy uruchamiane z klawiatury terminala są nazywane pierwszoplanowymi. Powłoka czeka na zakończenie wykonywania procesu i dopiero wtedy jest gotowa na przyjęcie kolejnych poleceń od użytkownika.

Proces można jednak uruchomić w tle. Wówczas powłoka utworzy nowy proces potomny, będący powłoką, której nakaże wykonanie zadanego polecenia, a sama powróci do stanu gotowości na kolejne polecenia. W rezultacie proces, który został uruchomiany w tle zaczyna pracować równolegle z interpreterem poleceń.

Warto zaznaczyć, że praca w tle ma sens jedynie w przypadku programów nieinteraktywnych, czyli takich, które do swojej pracy nie potrzebują interakcji za strony użytkownika. W przypadku uruchamiania programu w tle interpreter poleceń natychmiast przechodzi w stan oczekiwania na następne zlecenie, czyli rozpoczyna czytanie danych z klawiatury. Podobnie, programy działające w tle nie powinny wypisywać informacji na ekranie, bo będą one wypisywane asynchronicznie w stosunku do aktualnie wykonywanych operacji. W tym przypadku, rozwiązaniem tego problemu może być przekierowanie wyników działania takiego programu do pliku i jego późniejsza analiza.

Polecenie jest uruchomione w tle, jeśli po ostatnim parametrze natępuje znak &:

```bash
polecenie &
```

Aktualnie uruchomiony proces można także zatrzymać wciskając kombinację **Ctrl-Z**. Spowoduje to wstrzymanie tego procesu. Wstrzymany proces istnieje w systemie, ale nie jest dla niego przydzielany procesor. Zastopowany proces można wprowadzić do wykonania (kontynuacji) w tle poleceniem `bg` (ang. *background*), a nawet przywrócić po dowolnym czasie z powrotem na pierwszy plan poleceniem `fg` (ang. *foreground*), pod warunkiem jednak, że pomiędzy tymi poleceniami nie uruchomimy w tle innego procesu. Listę aktualnie kontrolowanych zadań można wyświetlić poleceniem `jobs`.

Jeśli wstrzymano więcej niż jedno zadanie, niezbędna będzie ich identyfikacja. Interpreter poleceń wewnętrznie przydziela swoje identyfikatory i za pomocą polecenia `jobs` można wyświetlić ich wartości. Do konkretnego procesu można odwołać się korzystając z jego identyfikatora.
```bash
jobs
[1]- 	Stopped 	vim praca.html
[2]+	Stopped	find /usr -name signal.h
fg 1
```

### Status zakończenia procesu

Każdy proces w systemie Unix po zakończeniu swojej pracy przekazuje do systemu informację o tym jak zakończyło się przetwarzanie, określaną statusem zakończenia procesu. Status zakończenia jest liczbą jednobajtową, przy czym przyjęto, że wartość 0 oznacza poprawne zakończenie przetwarzania. Wartości różne od 0 oznaczają błąd. Status zakończenia ostatnio wykonywanego programu można uzyskać w następujący sposób:
```bash
echo $?
```
Status zakończenia procesu można wykorzystać do warunkowego uruchomiania poleceń. Fakt, że `polecenie_2` można wykonać tylko gdy `polecenie_1` zakończyło się sukcesem zapisujemy następująco:
```bash
polecenie_1 && polecenie_2 
```
Natomiast gdy `polecenie_2` może być wykonane tylko wtedy gdy polecenie `polecenie_1` zakończyło się niepowodzeniem:
```bash
polecenie_1 || polecenie_2
```
Ponadto w systemie UNIX możemy jednym poleceniem uruchomić kilka procesów jeden po drugim, nie zważając na wynik poprzednich, oddzielając poszczególne z nich średnikiem (jest to odpowiednik wpisania każdego polecenia w oddzielnej linii i zatwierdzania enterem):
```bash
polecenie_1; polecenie_2; polecenie_3
```
Taką sekwencję można również wprowadzić w tło:
```bash
(polecenie_1; polecenie_2; polecenie_3) &
```

---

### Zadania do samodzielnego wykonania

1. Wyświetl listę własnych procesów komendą `ps`. Porównaj wyniki z wynikami poleceń: `ps ­x` i `ps ­ax`.
2. Zaloguj się do systemu kilkukrotnie poprzez wirtualne konsole lub otwierając nowe okno w środowisku graficznym. Każdorazowo sprawdź poleceniem `tty` nazwę terminala, na którym pracujesz.
3. Wyświetl hierarchię procesów poleceniem `pstree`. 
4. Obejrzyj listę procesów poleceniem `top` sortując ją wg stopnia zajętości procesora i ilości zajętej pamięci (sprawdź przełącznik `-o`)
5. Wykonaj kolejno następujące kroki:
* Zmień priorytet powłoki `bash`, w której aktualnie się znajdujesz na 10.
* Uruchom polecenie `sleep` na 30 sekund. Od razu wstrzymaj je kombinacją **Ctrl-Z**.
* Uruchom *w tle* kolejne polecenie `sleep`, tym razem na 3600 sekund.
* Wyświetl aktywne zadania w bieżącej sesji komendą `jobs`.
* Sprawdź odpowiednim poleceniem `ps` priorytet i status (uruchomiony/wstrzymany) uruchomionych w bieżącej sesji programów.
* Przywróć w tle działanie wstrzymanego sleep.
* Sprawdzaj aktywne zadania poleceniem `jobs` aż do zakończenia `sleep 30`
* Zakończ `sleep 3600` przywracając go na pierwszy plan i zamykając kombinacją **Ctrl-C**. 
6. Uruchom w tle sekwencję  `sleep 1000 ; touch sleep_finished`. Sprawdź czy istnieje plik *sleep_finished*. Zakończ proces *sleep* sygnałem *TERM*. Sprawdź ponownie istnienie pliku *sleep_finished*.
7. Uruchom aplikację z GUI, np. edytor tekstu *Mousepad*. Sprawdź jego PID. Wyślij do jego procesu sygnał *STOP*, sprawdź czy aplikacja reaguje. Wyślij sygnał *CONT*.
8. Utwórz w swoim katalogu domowym folder o nazwie `readonly`. Usuń prawa do zapisu w nim. Następnie wykonaj komendę, która spróbuje utworzyć w nim plik, a w przypadku niepowodzenia wyświetli komunikat **ERROR** (polecenie `echo ERROR`).

---

## Edytory tekstu `nano`, `vim`

W pracy z systemami, które nie posiadają środowiska graficznego lub poprzez zdalny terminal często zachodzi konieczność edycji plików tekstowych z poziomu terminala. Sprawna edycja plików tekstowych możliwa jest dzięki konsolowym edytorom takim jak *Emacs*, *Vim* czy *Nano*.

Osoby posługujące się takimi edytorami na co dzień znają mnóstwo skrótów i trików, które powodują, że praca z takim programem może być sprawniejsza niż z edytorem graficznym.

Na potrzeby sporadycznej edycji plików, np. konfiguracyjnych, najprostszy w użyciu jest program *Nano*. Oferuje on quasi-graficzny interfejs z podpowiedziami skrótów klawiszowych.

Przykładowe użycie (jeśli plik nie istnieje, zostanie utworzony):

```bash
nano plik.txt
```

Najważniejsze skróty:
* **Ctrl-o** - zapisz
* **Ctrl-x** - wyjdź
* **Ctrl-k** - wytnij tekst (domyślnie bieżąca linia)
* **Ctrl-u** - wklej tekst

W przypadku konieczności edycji pliku pod systemem, gdzie program *Nano* jest niedostępny (i nie chcemy lub nie możemy go zainstalować), istnieje bardzo duża szansa, że zainstalowany jest program *Vim*.

Przykładowe użycie (jeśli plik nie istnieje, zostanie utworzony):

```bash
vim plik.txt
```

Najważniejsze skróty klawiszowe:

* `i` - przechodzi w tryb edycji, pozwala na wprowadzenie tekstu
* `Esc` - wychodzi z trybu edycji lub przerywa wpisywanie komendy

Komendy (zatwierdzane enterem):

* `:w` - zapisz
* `:q` - wyjdź
* `:q!` - wyjdź bez zapisywania
* `:x` - zapisz i wyjdź

---

### Zadania do samodzielnego wykonania

9. Korzystając z *Nano* zwiększ rozmiar przechowywanej historii *bash* (wartość `HISTSIZE` w pliku `.bashrc` w katalogu domowym)
10. Korzystając z *Vim*-a wyedytuj dowolny plik tekstowy.
11. Uruchom w pojedynczej konsoli, **w tle** trzy edytory nano, dla trzech różnych plików. Sprawdź procesy działające w tle w bieżącym terminalu komendą `jobs`. Naucz się przywracać wybrany proces na pierwszy plan.

***
Autorzy: *Adam Bondyra, Jakub Tomczyński*

Data ostatniej modyfikacji:   *12-03-2019*

Opracowano na podstawie materiałów projektu *Otwartych Studiów Informatycznych (http://wazniak.mimuw.edu.pl/*).
