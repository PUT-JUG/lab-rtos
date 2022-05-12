# Filtry, strumienie oraz przetwarzanie potokowe

## Standardowe wejście/wyjście
Każdy proces domyślnie korzysta ze standardowych strumieni danych, będących abstrakcją źródła lub ujścia danych. Dla każdego procesu system tworzy standardowy strumień wejściowy (ang. *standard input*), reprezentujący urządzenia wejściowe, np. klawiaturę i dysk, standardowy strumień wyjściowy (ang. *standard output*), którym może być terminal (monitor komputera) lub plik oraz standardowy strumień diagnostyczny (ang. *standard error*). Strumienie wejściowy, wyjściowy i diagnostyczny oznaczane są odpowiednio: `stdin`, `stdout`, `stderr`. Każdy strumień ma odpowiadające mu wartości: `stdin` - 0, `stdout` - 1 i `stderr` - 2.

![Standardowe wejscie-wyjscie procesu](_images/lab_04_proc_std.png)

Standardowe strumienie procesów charakteryzują się następującymi cechami:

* Dane odczytywane są poprzez standardowe wejście.
* Dane wypisywane są na standardowym wyjściu lub standardowym wyjściu diagnostycznym.
* Raz przeczytanych danych nie można ponownie przeczytać.
  
Działanie standardowych strumieni ilustruje program `cat`. Uruchomienie tego programu bez argumentów powoduje przepisanie tego, co zostanie wpisane z wejścia standardowego (klawiatury) na wyjście standardowe (okno terminala). Wpisywanie danych z klawiatury można zakończyć za pomocą kombinacji *Ctrl-D* (reprezentowane w listingach jako `^D`).

## Przekierowanie wejścia/wyjścia procesów
Istnieje możliwość przeadresowania strumieni wyjściowych i wejściowych. Zmianę standardowego wejścia, wyjścia i wyjścia diagnostycznego można dokonać za pomocą operatorów: `>`, `<`, `>>`, `<<`.

Operator `>` powoduje przeadresowanie standardowego wyjścia, czyli utworzenie pliku i zapisanie w nim tego, co proces wypisałby na standardowym wyjściu. Jeśli wskazany plik już istnieje, zostanie on usunięty i utworzony na nowo.

```bash
cat > plik.txt
To jest plik.
Ala ma kota.
^D
```
Operator `<` powoduje przeadresowanie standardowego wejścia procesu, czyli pobranie danych wejściowych ze wskazanego pliku:

```bash
cat < plik.txt
To jest plik.
Ala ma kota.
```

Operatory `>` i `<` można używać jednocześnie, przeadresowując zarówno wyjście jak i wejście, co spowoduje, że zawartość pliku `plik.txt` zostaje skopiowana do pliku `plik_nowy.txt`:

```bash
cat < plik.txt > plik_nowy.txt
```

Operator `>>` przeadresowuje standardowe wyjście, dopisując wyniki działania programu na końcu istniejącego pliku:

```bash
cat >> plik.txt
Kot ma Ale.
^D
```

Operator `<<` powoduje, że do procesu zostaną przekazane dane ze standardowego wejścia aż do napotkania wskazanego napisu:

```bash
cat << przerwa
> Ala ma kota
> Kot ma Ale
> przerwa
Ala ma kota
Kot ma Ale.
```

Niektóre polecenia równolegle z wyświetlanymi na standardowym wyjściu informacjami wysyłają dodatkowe informacje informujące o błędach przetwarzania na standardowe wyjście diagnostyczne. Istnieje możliwość niezależnego przekierowania strumienia diagnostycznego, poprzez operator `>` poprzedzony numerem wyjścia diagnostycznego, czyli `2`:

```bash
cat plik1.txt plik2.txt 2> plik3.err
```

Polecenie to spowoduje wyświetlenie zawartości plików `plik1.txt` i `plik2.txt` oraz zapisanie informacji o błędach do pliku `plik3.err`.

W celu pominięcia komunikatów o błędach, wyjście diagnostyczne można przeadresować do pliku `/dev/null`. Wszystko co zostaje wysłane do pliku `null`, znajdującego się w katalogu `/dev` zostanie utracone:

```bash
cat plik1.txt plik2.txt> plik3.txt 2> /dev/null
```

Polecenie to spowoduje zapisanie kolejno zawartości plików `plik1.txt` i `plik2.txt` do pliku `plik3.txt` oraz jednocześnie zignoruje komunikaty o błędach.

W przypadku gdy strumień diagnostyczny ma trafiać tam, gdzie strumień wyjściowy, należy użyć zapisu `2>&1`:

```bash
cat plik1.txt plik2.txt> plik3.txt 2>&1
```

Wiele programów konsolowych działających na strumieniach może przyjąć również nazwę pliku jako wejście: polecenie `cat < plik.txt` wygeneruje taki sam efekt jak `cat plik.txt`, a w przypadku skomplikowanego potoku wersja druga może okazać się znacząco czytelniejsza. Dodatkowo, wiele programów akceptuje przekazanie `-` jako nazwy pliku, co w zależności od kontekstu oznacza wejście lub wyjście standardowe.

## Przetwarzanie potokowe
Standardowe wyjście jednego procesu może być połączone ze standardowym wejściem innego procesu, tworząc tzw. potok pomiędzy tymi procesami.

![Standardowe wejscie-wyjście procesu](_images/lab_04_stream.png)

Przetwarzanie potokowe polega na buforowaniu przez system danych produkowanych przez pierwszy proces i następnie odczytywaniu tych danych przez drugi proces. Innymi słowy proces w potoku czyta dane z wejścia, które zostało przeadresowane na wyjście procesu poprzedniego. W potoku może brać udział jednocześnie kilka procesów. Do przekierowania danych do kolejnego procesu używany jest znak `|`. Poniżej podano przykłady potoków:

Proces `ls` podaje wynik procesowi `more`, który w efekcie wyświetla listing strona po stronie:
```bash
ls -al | more
```
Proces `who` podaje wynik procesowi `sort`, podając posortowaną listę pracowników pracujących w systemie:
```bash
who | sort
```
Proces `ps` podaje wynik procesowi `grep`, wyszukując na liście procesów linii zawierających słowo `csh`:
```bash
ps -ef | grep csh
```
Proces `ls` podaje wynik procesowi `sort`, który następnie podaje wynik procesowi `head`, wyświetlając pierwszych 10 największych plików:
```bash
ls -l /usr/bin | sort -bnr +4 -5 | head
```
## Filtry
Istnieją programy, których zadaniem jest odczyt danych ze standardowego wejścia, przetworzenie tych danych i ich zapis na standardowe wyjście. Programy takie nazywane są filtrami i są szeroko wykorzystywane w przetwarzaniu potokowym. Poniżej przedstawiono najczęściej wykorzystywane filtry:

* `cat` - najprostszy filtr, nie wprowadzający zmian do przetwarzanych danych. Użyteczne przełączniki:
  * `-s` z paru pustych linii robi jedną
  * `-n` numeruje wszystkie linie
  * `-b` numeruje niepuste linie
  * `-A` pokazuje znaki specjalne
* `head` - wyświetla początkową część pliku o podanej nazwie lub danych wejściowych otrzymanych z potoku gdy nazwa pliku nie jest podana. Standardowo wyświetlanych jest pierwszych 10 linii odczytanych danych. Używając przełączników liczbę tą można zmienić:
  * `-c` pozwala określić liczbę wyświetlanych znaków
  * `-n` pozwala określić liczbę wyświetlanych linii
* `tail` - wyświetla końcową część pliku o podanej nazwie lub danych wejściowych otrzymanych z potoku, gdy nazwa pliku nie jest podana,. Standardowo wyświetlanych jest ostatnich 10 linii danych. Używając przełączników liczbę tą można zmienić:
  * `-c` pozwala określić liczbę wyświetlanych znaków
  * `-n` pozwala określić liczbę wyświetlanych linii
  * `-f` powoduje, że dane po zapisaniu ich do pliku są wyświetlane na standardowym wyjściu
* `sort` - służy do sortowania danych wejściowych, które to domyślnie sortowane są leksykograficznie. Sortowanie danych odbywa się liniami. Najważniejsze przełączniki:
  * `-n` pozwala sortować numerycznie
  * `-b` ignoruje przy sortowaniu spacje znajdujące się na początku linii
  * `-t` pozwala zmienić domyślny separator kolumn, którym są znaki tabulacji lub spacji
  * `-f` pozwala ignorować przy sortowaniu wielkość liter
  * `-r` odwraca kolejność sortowania
  * `+liczba` pozwala by sortowanie odbywało się względem dowolnej kolumny (a nie początku linii). Liczba określa liczbę kolumn pominiętych przy sortowaniu
  * `-o nazwa_pliku` zapisuje rezultat sortowania do pliku o podanej nazwie
* `uniq` - umożliwia usunięcie powtarzających się, sąsiadujących linii danych wejściowych.
  * `-d` wyświetla z danych wejściowych tylko linie powtarzające się
  * `-u` wyświetla z danych wejściowych tylko linie unikalne
  * `-c` zlicza liczbę powtórzeń
* `wc` - zlicza znaki, słowa i linie w podanych danych wejściowych. Standardowo wyświetlane są wszystkie trzy wartości, ale można to zmienić:
  * `-l` pozwala zliczać tylko linie
  * `-w` pozwala zliczać tylko słowa
  * `-c` pozwala zliczać tylko znaki
* `tr` - pozwala zamienić łańcuchy tekstowe, które podawane są jako argumenty wejściowe. Znaki z pierwszego łańcuch zamieniane są na znaki z drugiego łańcucha. Dodatkowo, dzięki przełącznikom możliwe jest następujące przetwarzanie:
  * `-d` usuwa podane po przełączniku znaki
  * `-s` usuwa powtarzające się sąsiednie znaki
* `cut` - pozwala wyświetlić fragmenty wierszy danych wejściowych. Zwykle jest to wycinanie odpowiednich kolumn
  * `-c` pozwala określić pozycję znakowe wycinanych fragmentów wierszy, np. `-c 1-72` wyświetla pierwsze 72 znaki każdego wiersza
  * `-f` pozwala określić numery wycinanych kolumn, np. `-f1,3-5,10` wyświetla pierwszą kolumnę, kolumny od 3 do 5 oraz kolumnę 10.
  * `-d` pozwala zmienić domyślny separator kolumn, którym jest znak tabulacji
* `grep [opcje] wyrażenie [lista_plików]` - przeszukuje dane pochodzące ze standardowego wejścia lub pliki wyszczególnione na liście plików, wypisując tylko linie zawierające szukane wyrażenie. Szukane wyrażenie zapisywane jest za pomocą wyrażenia regularnego. Najważniejsze przełączniki:
  * `-v` wyszukuje linie nie zawierające szukanego wzorca
  * `-c` podaje liczbę odszukanych wyrażeń
  * `-i` ignoruje wielkość liter przy wyszukiwaniu
  * `-n` wyświetla numery linii zawierających dany wzorzec
  * `-h` przy wyświetlaniu linii zawierających szukany wzorzec pomija nazwy plików
  * `-r` pozwala na przeszukiwanie rekurencyjne, np. `grep wzorzec -r katalog`
  * `-l` pokazuje nazwy plików zawierających określony wzorzec
  * `-L` pokazuje nazwy plików nie zawierających określonego wzorca

Zasady konstrukcji podstawowych wyrażeń regularnych opisujących szukany wzorzec są następujące:
* `.` - reprezentuje dowolny znak
* `[abc]` - oznacza jeden ze znaków a, b lub c
* `[a-z]` - oznacza jeden ze znaków z podanego zakresu
* `[^0-9]` - oznacza dopełnienie podanego zbioru (znaki inne niż od `0` do `9`)
* `*` - reprezentuje powtórzenie dowolną liczbę razy (0 lub więcej) wyrażenia znajdującego się bezpośrednio po lewej stronie np:
    * `.*` oznacza dowolny ciąg znaków
    * `A[a]*` określa A, Aa, Aaa, Aaaaaaaaa, itd.
* `^` - reprezentuje początek linii
* `$` - reprezentuje koniec linii

Domyślnie `grep` pracuje w uproszczonym trybie, gdzie znaki `?`, `+`, `{`, `}`, `|`, `(` oraz `)` traktowane są dosłownie. Aby uzyskać zgodność z wyrażeniami regularnymi stosowanymi w innych językach (*extended regular expressions*), należy stosować `grep` z przełącznikiem `-E` lub używać polecenia `egrep` (które wewnętrznie wywołuje `grep` z przełącznikiem `-E`). Rozbudowuje to dostępne znaczniki między innymi o:

* `+` - jedno lub więcej wystąpień poprzedzającego znaku
* `{n}` - n-krotne wystąpienie poprzedzającego znaku
* `{n,}` - oznacza co najmniej n-krotne wystąpienie znaku występującego bezpośrednio po lewej stronie nawiasów
* `{,m}` - oznacza co najwyżej m-krotne wystąpienie znaku występującego po lewej stronie nawiasów
* `{n,m}` - oznacza co najmniej n-krotne i co najwyżej m-krotne wystąpienie znaku występującego po lewej stronie nawiasów

Jeśli chcemy, aby znak mający znaczenie specjalne w wyrażeniach regularnych (np.  `.`, `*`, `{`, `}`,  `(`, `)`,  `^`,  `[`,  `]`,  `\`,  `<`,  `>`,  `$`) został potraktowany dosłownie, musimy poprzedzić go backslashem (`\`). Ponadto, ponieważ wiele z wymienionych znaków ma w powłoce `bash` znaczenie specjalne, całe wyrażenie wpisane w konsoli należy umieścić w pojedynczym cudzysłowie (`'`), aby uniknąć ich interpretacji przez powłokę.

### Przykłady potoków z wyrażeniami regularnymi:

Proste wyrażenie regularne - przeszukuje plik */etc/group* pod kątem słowa *student*:

 ```bash
 grep student /etc/group
 ```
 
 Zaawansowane wyrażenia:
 
 Wyświetla elementy znajdujące się w głównym katalogu, o nazwach zaczynających się na `s` i minimum trzech następujących dowolnych znakach:

```bash
ls / | egrep '^s.{3,}'
```

Listuje zawartość pliku */etc/hosts*, sortuje ją, a następnie przeszukuje pod kątem wystąpienia *adresu IP* - liczby w formacie *w.x.y.z*, gdzie *w*, *x*, *y* oraz *z* to liczby o od 1 do 3 cyfr. Zwróć uwagę na backslash przed kropkami powodujący, że kropka jest traktowana dosłownie, a nie jako dowolny znak:

```bash
cat /etc/hosts | sort | egrep '[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}'
```

## Nowa linia na końcu pliku

Zwyczajowo w systemach uniksowych pliki tekstowe (konfiguracyjne, źródłowe, skrypty itp.) zakończone są znakiem nowej linii. Niektóre edytory (np. `nano`) same dodają znak nowej linii przy każdej edycji pliku.

Niektóre z przedstawionych poleceń mogą wymagać obecności znaku końca linii w ostatnim wierszu. W przypadku problemów z wczytywaniem ostatniego wiersza możesz to zweryfikować wyświetlając plik np. `cat`:

```bash
student@vbox-xubuntu-rtos:~$ cat zly_plik 
To jest linia tekstu
To jest kolejna liniastudent@vbox-xubuntu-rtos:~$ cat dobry_plik 
A tu jest zawartosc innego pliku
Zakonczonego znakiem nowej linii
student@vbox-xubuntu-rtos:~$ 
```

Zwróć uwagę, że po wyświetleniu pliku nie zakończonego znakiem nowej linii znak zachęty (`student@vbox...`) wyświetla się w tym samym wierszu, co zawartość pliku.

## Zadania do samodzielnego wykonania
1. Wyświetl plik */etc/passwd* z podziałem na strony przyjmując, że strona ma 5 linii tekstu. **Podpowiedź:** sprawdź program `more`
2. Stwórz pliki *tekst1* oraz *tekst2*, wypełnij kilkoma linijkami tekstu. Korzystając z polecenia `cat` utwórz plik *tekst3*, który będzie składał się z zawartości plików *tekst1* oraz *tekst2*.
3. Wyświetl po 5 pierwszych linii wszystkich plików w swoim katalogu domowym w taki sposób, aby nie były wyświetlane ich nazwy. **Podpowiedź:** pamiętaj, że z programami, które przyjmują jako argumenty nazwy plików możesz używać **wzorców**.
4. Wyświetl linie o numerach 3, 4 i 5 z pliku */etc/passwd*
5. Wyświetl linie o numerach 7, 6 i 5 licząc od końca pliku */etc/passwd* (czyli kolejno 7. od końca, 6. od końca i 5. od końca)
6. Wyświetl zawartość */etc/passwd* w jednej linii
7. Za pomocą filtru `tr` wykonaj modyfikację pliku, polegającą na umieszczeniu każdego słowa (oddzielonych spacją) w osobnej linii. **Podpowiedź:** aby przekazać znak spacji jako argument, musisz umieścić go w cudzysłowie
8. Zlicz wszystkie pliki znajdujące się w katalogu */etc* i jego podkatalogach
9. Napisz polecenie zliczające sumę znaków z pierwszych trzech linii pliku */etc/passwd*
10. Wyświetl listę plików z aktualnego katalogu, zamieniając wszystkie małe litery na duże.
11. Wyświetl listę praw dostępu do plików w aktualnym katalogu, ich rozmiar i nazwę
12. Wyświetl listę plików w aktualnym katalogu, posortowaną według rozmiaru pliku
13. Wyświetl zawartość pliku */etc/passwd* posortowaną wg numerów *UID* w kolejności od największego do najmniejszego
14. Wyświetl zawartość pliku */etc/passwd* posortowaną najpierw wg numerów *GID* w kolejności od największego do najmniejszego, a następnie *UID*
15. Podaj nazwy trzech najmniejszych plików w katalogu posortowane wg nazwy
16. W pliku `/etc/services` przechowywana jest lista popularnych usług sieciowych, wraz z numerami portów i protokołem. Wylistuj (tylko) nazwy usług, które korzystają z protokołu `UDP`.
17. Wyświetl, ile wirtualnych terminali (`dev/tty`) o numerach z zakresu *50-69* znajduje się w systemie.
18. Zbuduj potok, który wyświetli w terminalu PID procesu **cupsd**.

***
Autorzy: *Adam Bondyra, Jakub Tomczyński*

Opracowano na podstawie materiałów projektu *Otwartych Studiów Informatycznych (http://wazniak.mimuw.edu.pl/)*.
