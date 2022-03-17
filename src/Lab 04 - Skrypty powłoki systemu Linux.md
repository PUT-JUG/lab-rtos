# Tworzenie skryptów powłoki systemu operacyjnego

## Interpreter poleceń oraz zmienne środowiskowe

Powłoka systemowa (ang. *shell*) to program komputerowy pełniący rolę pośrednika pomiędzy systemem operacyjnym lub aplikacjami a użytkownikiem, przyjmując jego polecenia i wyświetlając wyniki działania programów. W systemach linuksowych (w tym Ubuntu) najpopularniejsza obecnie powłoka to `bash` (ang. *Bourne Again Shell*) - dalsze przykłady będą prezentowane właśnie dla powłoki `bash`.

Podczas poprzednich zajęć, za każdym razem kiedy otwieraliśmy emulator terminala, uruchamiana została nowa instancja powłoki naszego użytkownika - program `bash`. Część poleceń to tzw. polecenia wbudowane (bezpośrednio w powłokę) - np. `cd`, `pwd`, inne to zupełnie oddzielne programy, które `bash` wywoływał - np. `ls`, `chmod` itd.

*Zmienne środowiskowe* to bardzo wygodny i uniwersalny sposób konfigurowania i parametryzowania powłok systemowych i - za ich pomocą - także innych programów. Dostępne zmienne środowiskowe tworzą tzw. *środowisko wykonania procesu* - środowisko to jest kopiowane do wszystkich nowych procesów, a więc modyfikacje zmiennych wykonane w powłoce są widoczne we wszystkich programach uruchomionych przy użyciu tej powłoki. Każdy użytkownik może definiować dowolną ilość własnych zmiennych oraz przypisywać im dowolne wartości. Aby zdefiniować zmienną środowiskową, należy zastosować operator przypisania (znak "=") w następujący sposób:

```bash
ZMIENNA=wartosc
```

W tym wypadku ciąg znaków `ZMIENNA` to nazwa zmiennej, a `wartosc` to jej wartość - należy zwrócić uwagę, że pomiędzy nazwą zmiennej, operatorem przypisania i wartością nie może być spacji. Odwołanie się do wartości zmiennej jest możliwe dzięki specjalnemu znakowi `$`; np. wyświetlenie wartości zmiennej na ekranie jest możliwe z wykorzystaniem polecenia `echo`, które wyświetla linię tekstu oraz wartości zmiennej pobranej znakiem `$`:

```bash
SYSTEM=Unix
echo $SYSTEM
```
Wyjście:
```
Unix
```

Polecenie systemowe `set` pozwala wyświetlić wartości wszystkich zmiennych środowiskowych, a polecenie
`unset` usuwa zmienną środowiskową, oto przykład:

```bash
unset SYSTEM
```

Jak wspomniano środowisko wykonania procesu jest przekazywane do procesów potomnych - jednak nie wszystkie zmienne powłoki muszą być przekazywane do uruchamianych programów. Zmienne, które są przekazywane nazywa się zmiennymi eksportowanymi, a zmienne, które nie są przekazywane, nazywa się zmiennymi lokalnymi. Z reguły nowo tworzone zmienne są początkowo zmiennymi lokalnymi i niezbędne jest jawne wskazanie, że mają być zmiennymi eksportowanymi. Nazywa się to eksportowaniem zmiennych i jest realizowane przez poleceniem:

```bash
export lista_zmiennych
```

Oto przykład tworzenia zmiennej i jej eksportowania:

```bash
SYSTEM=Unix
export SYSTEM
```

Powyższe polecenia można także zrealizować jednym poleceniem:

```bash
export SYSTEM=Unix
```

Wyświetlenie eksportowanych zmiennych środowiskowych możliwe jest poleceniem `env`.

Każdy użytkownik może łatwo (np. poleceniem `set`) zweryfikować, że w systemie domyślnie jest zdefiniowanych wiele zmiennych środowiskowych, oto znaczenie podstawowych z nich:

 * `HOME` - ścieżka i nazwa katalogu domowego użytkownika;
 * `USER` - nazwa zalogowanego użytkownika;
 * `PATH` - ścieżki poszukiwań programów;
 * `PS1` - postać znaku zachęty użytkownika;
 * `SHELL` - pełna ścieżka do domyślnego interpretatora poleceń użytkownika.
 
 Zmienne systemowe nie mają typu - wszystkie przechowywane są jako napis (ciąg znaków), niezależnie od zawartości.
 
 ## Zadania do samodzielnego wykonania
1. Zdefiniuj zmienną `IMIE` i przypisz jej swoje imię. Wyświetl zawartość tej zmiennej. Wyeksportuj tą zmienną i sprawdź, czy jest dostępna w nowym (potomnym) interpreterze.
2. Wyświetl listę zmiennych eksportowanych.
3. Zmień własny znak zachęty, modyfikując zmienną `PS1`.
  
## Skrypty i ich argumenty
Współczesne powłoki pozwalają także na tworzenie konstrukcji programistycznych takich jak instrukcje warunkowe czy pętle. Ponieważ kolejne ciągi poleceń możemy umieścić w pliku tekstowym, a następnie taki plik uruchomić w powłoce - w połączeniu ze zmiennymi środowiskowymi uzyskujemy możliwość pisania programów (skryptów) w języku `bash`.

Skrypty są bardzo pomocnym rozwiązaniem kiedy istnieje konieczność wykonywania złożonych poleceń i poleceń, które są powtarzane okresowo.

Skrypty mogą być parametryzowane argumentami ich wywołania - oznacza to, że podczas uruchamiania skryptu można przekazać do niego dane lub parametry. Do argumentów wywołania można się odwoływać w skryptach za pomocą tzw. zmiennych pozycyjnych, oznaczanych: *1*, *2*, *3*, ..., *9*. Każda zmienna pozycyjna przechowuje tekst przekazany za pośrednictwem odpowiadającemu jej argumentu wywołania - pierwszy argument dostępny jest w zmiennej pozycyjnej *1* itd. Oto przykład pierwszego skryptu, który wyświetla wartości pierwszych trzech argumentów jego wywołania:

```bash
#!/bin/bash
# pierwszy skrypt
echo "argument nr 1: $1"
echo "argument nr 2: $2"
echo "argument nr 3: $3"
```

Takie polecenia należy zapisać do dowolnego pliku (zaleca się, aby pliki skryptów miały rozszerzenie `.sh`). Pierwsza linia, zaczynająca się od ciągu `#!` pozwala wskazać jaki interpreter poleceń ma zostać wykorzystany do jego wykonania - w tym przypadku jest to powłoka `bash`. Druga linia prezentuje sposób umieszczania komentarzy: każda linia rozpoczynająca się od znaku `#`, jest traktowana jako komentarz. Kolejne trzy linie wyświetlają wartości pierwszych trzech argumentów wywołania skryptu z wykorzystaniem polecenia `echo`. **Aby uruchomić skrypt należy dla pliku, w którym jest on zapisany nadać prawo wykonywania.** Poniżej przedstawiono przykładowe wywołanie przedstawionego powyżej skryptu oraz jego wynik (przyjęto, że plik skryptu nazywa się `skrypt1.sh`):

```bash
./skrypt1.sh abc xyz 12345
```
Wyjście:
```
argument nr 1: abc
argument nr 2: xyz
argument nr 3: 12345
```

## Instrukcja warunkowa
Obecnie powłoki systemowe pozwalają na to, aby skrypty zawierały konstrukcje sterujące ich wykonaniem - podstawową instrukcją jest w tym przypadku instrukcja warunkowa. Składnia tej instrukcji jest następująca:

```bash
if warunek
then
    instrukcje wykonywane jeśli warunek zostanie spełniony
else
    instrukcje wykonywane jeśli warunek nie zostanie spełniony
fi
```

Warunek może być dowolnym poleceniem - warunek jest spełniony, jeśli program nim będący nie zwrócił błędu (miał kod wyjścia równy 0). Przykładowe wykorzystanie programu `ping` do sprawdzenia łączności z serwerem:

```bash
if ping -c 1 google.com
then
    echo "connected"
else
    echo "error"
fi
```

Najczęściej warunki konstruowane są z wykorzystaniem wbudowanego programu `test` i notacji używającej znaków `[` i `]` do realizacji testów warunków, co prezentuje kolejny przykład:

```bash
if [ $1 = xyz ]
then
    echo "arg nr 1 = xyz"
else
    echo "arg nr 1 <> xyz"
fi
```

Po znaku `[` i przed znakiem `]` konieczne jest wprowadzenie znaku spacji. Możliwe do sprawdzenia warunki z zastosowaniem programu `test` prezentuje tabela:


|                  Warunek                   |      Opis      |
|------------------------------------------|--------------|
|     `znaki1 = znaki2`      |      Weryfikacja równości dwóch łańcuchów znaków     |
| `znaki1 != znaki2` | Weryfikacja nierówności dwóch łańcuchów znaków |
| `-z znaki`     |     	Weryfikacja, czy łańcuch znaków ma zerową długość     |
| `-n znaki`    |     	Weryfikacja, czy łańcuch znaków ma niezerową długość     |
| `liczba1 -eq liczba2`    |     	Weryfikacja równości dwóch liczb     |
| `liczba1 -ne liczba2`     |     	Weryfikacja nierówności dwóch liczb     |
| `liczba1 -gt liczba2`   |     	Weryfikacja, czy `liczba1` jest większa od `liczba2`     |
| `liczba1 -lt liczba2`    |     	Weryfikacja, czy `liczba1` jest mniejsza od `liczba2`     |
| `-e nazwa`    |     	Weryfikacja, czy podany plik istnieje     |
| `-f nazwa`    |     	Weryfikacja, czy podany plik jest plikiem zwykłym     |
| `-d nazwa`   |     	Weryfikacja, czy podany plik jest katalogiem     |
| `-r nazwa`    |     	Weryfikacja, czy użytkownik ma prawo odczytu dla pliku o podanej nazwie     |
| `-w nazwa`    |     	Weryfikacja, czy użytkownik ma prawo zapisu dla pliku o podanej nazwie     |
| `-x nazwa`    |     	Weryfikacja, czy użytkownik ma prawo wykonywania dla pliku o podanej nazwie     |
| `warunek1 -a warunek2`   |     	Iloczyn logiczny warunków     |
| `warunek1 && warunek2`   |     	Iloczyn logiczny warunków     |
| `warunek1 -o warunek2`    |     	Suma logiczna warunków     |
| `warunek1 \|\| warunek2`    |     	Suma logiczna warunków     |
| `! warunek1`     |     	Negacja warunku     |

## Pętle
Skrypty powłoki mogą także zawierać pętle - podstawowe dwie z nich to pętla `for` oraz `while`. Pętla `for` wykonywana jest z góry określoną ilość razy, a jej ogólna składnia jest następująca:

```bash
for zmienna in lista
do
    instrukcje_do_wykonania
done
```

Wykonanie pętli powoduje przypisywanie zmiennej zmienna kolejnych wartości wymienionych na liście lista; ilość iteracji, jest zatem zależna od długości podanej listy. Jako listę można w pętli for można podawać wzorce uogólniające powłoki. Poniższy przykład skryptu prezentuje zastosowanie pętli for do usunięcia wszystkich plików z rozszerzeniem `.tmp` z katalogu bieżącego:

```bash
#!/bin/bash
for FILE in *.tmp
do
    rm -v $FILE
done
```

Ilość iteracji powyższej pętli bezie zatem determinowana ilością plików z rozszerzeniem `*.tmp`, które utworzą listę wartości dla zmiennej `FILE`.

Realizacja pętli numerycznej z zastosowaniem pętli `for` jest możliwa z użyciem programu zakresów definiowanych operatorem nawiasów klamrowych `{start..stop}`, np.:

```bash
for N in {1..10}
do
    echo $N
done
```
spowoduje dziesięciokrotne wykonanie pętli.

Pętla `while` pozawala na realizację pętli, dla których ilość iteracji nie jest znana z góry, jej składnia dla skryptów powłoki jest następująca:

```bash
while warunek
do
   instrukcje_do_wykonania
done
```

Warunek może być dowolnym poleceniem i najczęściej jest konstruowany - tak jak w przypadku instrukcji warunkowej - z zastosowaniem programu `test`.

Przykładem zastosowania pętli `while` może być skrypt wypisujący na ekranie wartości wszystkich argumentów wywołania skryptu (niezależnie od ich liczby). Pętla taka będzie wykorzystywała polecenie `shift`, które powoduje przesunięcie argumentów - oto skrypt:

```bash
#!/bin/bash
while [ -n "$1" ]
do
   echo $1
   shift
done
```

Warunkiem wykonania pętli jest sprawdzenie, czy pierwszy argument wywołania skryptu ma niezerową długość - jeśli skrypt został uruchomiony bez żadnych argumentów, to pętla nie zostanie wykonana. Jeśli natomiast skrypt został wykonany z argumentami, to w pierwszym wykonaniu pętli zostanie wyświetlona wartość pierwszego argumentu, a następnie nastąpi przesunięcie argumentów w lewo poleceniem `shift` (drugi argument stanie się pierwszym, trzeci drugim itd.). Pętla zakończy się jeśli zostaną wyświetlone i przesunięte wszystkie argumenty (zmienna pozycyjna `$1` będzie miała wówczas zerową długość).

Aby wyświetlić wszystkie argumenty skryptu bez korzystania z pętli wystarczy wykorzystać zmienną `$*`, natomiast ich liczbę - `$#`
```bash
#!/bin/bash
echo "Liczba argumentow: $#"
echo "Wszystkie argumenty: $*"
```
W ten sposób można również iterować po argumentach:
```bash
#!/bin/bash
for arg in $*
do
    echo "Argument: $arg"
done
```

Obie zaprezentowane pętle mogą zostać przerwane poleceniem `break` - oto przykład zastosowania przerywania pętli:

```bash
#!/bin/bash
for FILE in *.tmp
do
   if [ ! -f $FILE ]
   then
      echo "$FILE nie jest plikiem!"
      break
   fi
   rm -v $FILE
done
```

Jak widać pętla zostanie przerwana, jeśli pobrana nazwa z rozszerzeniem `*.tmp` nie będzie wskazywała na plik zwykły.

Wykonywanie całego skryptu można przerwać poleceniem `exit`.

## Pobieranie wartości do skryptów
Jeśli skrypt wymaga interakcji z użytkownikiem, to niezbędne staje się pobieranie wartości przekazywanych przez użytkownika. Służy do tego polecenie:

```bash
read [argumenty]
```

Argumentami są nazwy zmiennych środowiskowych, które przyjmą wartość odczytana ze standardowego wejścia (do napotkania znaku nowej linii). Jeśli jako argumenty podano kilka zmiennych, to są one inicjowane w ten sposób, że pierwsze słowo trafia do pierwszej zmiennej, drugie do drugiej itd. Polecenie to można przetestować wykonując następujące polecenia:

```bash
read X Y
echo $X
echo $Y
```
Wejście:
```
uzytkownik adam
```
Wyjście:
```
uzytkownik
adam
```

Polecenie `read` może również współpracować z pętlą `while` i potokiem, co pozwala na przetwarzanie wejściowego strumienia tekstu linia po linii:

```bash
cat file.txt | while read line
do
    echo Linia tekstu: $line
done
```

Ponieważ znak spacji jest w powłoce separatorem argumentów, problematyczne mogą stać się zawartości zmiennych bądź nazwy plików zawierające spacje. Umieszczenie tekstu w podwójnym cudzysłowie (`"`) spowoduje, że cała jego zawartość, wraz ze spacjami zostanie potraktowana jako jeden argument.

Przykładowo, przypisanie do zmiennej napisu zawierającego spacje:
```bash
LISTA="abc 123 456"
```

Umieszczenie napisu w pojedynczym cudzysłowie (`'`) spowoduje, że jego zawartość nie będzie interpretowana w żaden sposób (np. znaki `$` nie będą oznaczały odwołań do zmiennych).

Poniższe trzy pętle generują zatem różne rezultaty:

```bash
for ZMIENNA in $LISTA ; do
    echo $ZMIENNA
done
```

```bash
for ZMIENNA in "$LISTA" ; do
    echo $ZMIENNA
done
```

```bash
for ZMIENNA in '$LISTA' ; do
    echo $ZMIENNA
done
```

Jeśli chcemy natychmiast po zawartości zmiennej dokleić dodatkowy tekst, można ująć nazwę zmiennej w klamry, np:

```bash
echo ${A}hello
```

Umieszczenie tekstu w *grawisie* (`` ` ``) powoduje **uruchomienie** zawartego w nim tekstu, a następnie podstawienie w dane miejsce wyjścia uruchomionego programu. Przykładowo, zapisanie bieżącego katalogu roboczego do zmiennej:

```bash
CURRENT_DIR=`pwd`
```

## Funkcje
W skryptach bash możemy również definiować funkcje, które pozwalają na uproszczenie kodu. Przykład prostej funkcji oraz jej wywołania pokazano poniżej:

```bash
#!/bin/bash

function total_files {
        find $1 -type f | wc -l
}

katalog="`pwd`"
echo -n "Liczba plikow w katalogu $katalog = "
total_files $katalog
```
Wewnątrz definicji funkcji, którą podajemy w nawiasach klamrowych po nazwie funkcji, można wykorzystać argumenty przekazywane do funkcji analogicznie jak argumenty przekazywane do skryptu, tzn. za pomocą `$1`, `$2`, `$3` itd.
Funkcja total_files sprawdza liczbę plików w podanym katalogu. Aby użyć zdefiniowanej funkcji w dalszej części skryptu, wystarczy podać jej nazwę oraz po kolei argumenty przekazywane do funkcji. Jeśli chcemy przypisać wynik funkcji do zmiennej należy wykorzystać operator $:
```bash
files=$(total_files $katalog)
```

## Operacje matematyczne
W skryptach bash można wykonywać operacje matematyczne na zmiennych całkowitoliczbowych. Jest na to kilka sposobów. Pierwszy to wykorzystanie podwójnych nawiasów:
```bash
wynik=$((5+8))
```
Drugim sposobem jest wykorzystanie polecenia `expr`:
```bash
wynik=$(expr 5+8)
```
Inny sposób polega na użyciu polecenia `let`:
```bash
let wynik=5+8
```
Podstawowe operatory matematyczne w skryptach bash:
|                  Operator                   |      Opis      |
|:------------------------------------------:|:--------------:|
|     \+, \-, \*, /      |     suma, różnica, mnożenie, dzielenie     |
|     var\+\+      |     inkrementacja     |
|     var\-\-      |     dekrementacja     |
|     %      |     modulo     |

## Debugowanie skryptów

Skrypty mogą być także wykonywane w trybie debugowania, np. w celu testowania poprawności działania, warunków, pętli itp. Aby zrealizować wykonanie skryptu z wyświetlaniem informacji kontrolnych należy zastosować przełącznik `-x` wywołania interpretatora poleceń. Można to zrealizować na trzy sposoby:

* dopisać przełącznik w pierwszej linii skryptu:  `#!/bin/bash -x`
* uruchomić skrypt wywołując jawnie interpreter z parametrem `-x` oraz argumentem z nazwą skryptu, np .
```bash
bash -x nazwa_skryptu.sh
```
* wywołując wewnątrz skryptu polecenie `set -x`

Często ma miejsce sytuacja, kiedy niepowodzenie jakiegokolwiek z poleceń (np. utworzenie katalogu roboczego) powoduje, że dalsze operacje nie mają sensu. Przydatną opcją powłoki jest wtedy przełącznik `-e`, który powoduje, że powłoka (np. wykonująca skrypt) zostanie przerwana, w momencie kiedy którykolwiek z użytych w niej programów (poza warunkami w instrukcjach warunkowych/pętlach) zwróci błąd. Ustawienie przełącznika `-e` może odbyć się na sposoby analogiczne do opisanych powyżej. 

## Zadania do samodzielnego wykonania
4. Napisz skrypt, który dla każdego elementu (pliku, folderu) w bieżącym katalogu wyświetli jego nazwę wraz z informacją czy jest to plik czy katalog.
5. Napisz skrypt, który dla każdego z plików podanych jako argumenty wywołania wyświetli nazwę pliku, a następnie jego zawartość posortowaną alfabetycznie.
6. Napisz skrypt, który będzie kopiował plik podany jako pierwszy argument do wszystkich katalogów podanych jako kolejne argumenty wywołania.
7. Napisz skrypt, który wykona kopię zapasową plików podanych jako argumenty, do katalogu `backup` i dopisze do ich nazwy bieżącą datę:

Przykładowo:
```bash
./super_backup.sh notatki.txt zdjecia.tar.gz 
```
Skopiuje:

`notatki.txt` do `backup/notatki.txt_2021-10-29`

`zdjecia.tar.gz` do `backup/zdjecia.tar.gz_2021-10-29`

Jeśli folder `backup` nie istnieje, program powinien go utworzyć.
Jeśli plik docelowy już istnieje, program powinien wyświetlić stosowny komunikat i przerwać pracę.

Podpowiedź: bieżącą datę możesz uzyskać poleceniem `date '+%Y-%m-%d'`


8. Napisz skrypt, który będzie oczekiwał na pojawienie się pliku o nazwie wskazanej w argumencie. Skrypt powinien cyklicznie (co 5 sekund) sprawdzać istnienie pliku. Jeśli plik istnieje, skrypt powinien wyświetlić jego zawartość i zakończyć się. Uruchom skrypt, a z poziomu drugiego terminala utwórz monitorowany plik.

9. Utwórz skrypt i umieść w nim funkcję realizującą sumę dwóch argumentów (liczb) podawanych do skryptu.

***
Autorzy: \
*Adam Bondyra*, *Jakub Tomczyński*, *Bartłomiej Kulecki*


Opracowano na podstawie materiałów projektu *Otwartych Studiów Informatycznych (http://wazniak.mimuw.edu.pl/)*.
