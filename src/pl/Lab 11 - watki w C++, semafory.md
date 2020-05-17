# Programowanie współbieżne - wątki w C++, semafory

## Wątki w C++11 – klasa `std::thread`

Biblioteka OpenMP sprawdza się przy *zrównleglaniu* wykonywanych operacji, jednak w wielu sytuacjach chcemy mieć ręczną kontrolę nad tym, kiedy wątki są uruchamiane, jakie realizują funkcje i kiedy są kończone - na przykład, kiedy chcemy asynchronicznie oczekiwać i reagować na zdarzenia bądź oddzielić warstwę interfejsu aplikacji od jej "rdzenia" - tak aby przetwarzanie danych mogło odbywać się w tle i nie powodowało "zawieszenia" interfejsu.

W ramach kursu wykorzystamy język C++ i mechanizmy dostarczane przez bibliotekę standardową w ramach rozszerzeń C++11. Rozszerzenia te są dostępne w większości współczesnych kompilatorów, np. w Visual Studio od wersji 2013. Niektóre środowiska/kompilatory wymagają włączenia dodatkowych opcji w celu uaktywnienia standardu C++11 (np. flaga `-std=c++11` dla kompilatora `gcc`).

Analogiczną funkcjonalność można osiągnąć za pomocą bibliotek takich *pthreads* (w systemach zgodnych z *POSIX*, np. Linux), bądź rozwiązań dedykowanych dostarczanych przez poszczególne systemy (np. wątki w *Windows API*). Istnieje też szereg języków projektowanych z myślą o przetwarzaniu współbieżnym, np. Go, Erlang, Ada.

## Pierwszy program wielowątkowy z `std::thread`

W celu wykorzystania wątków dołącz nagłówek `<thread>`. Pojedynczy wątek reprezentowany jest poprzez obiekt klasy `std::thread`. Konstruktor klasy przyjmuje wskaźnik do funkcji, która ma zostać uruchomiona w wątku oraz opcjonalnie argumenty, które mają zostać przekazane do uruchamianej funkcji:

```cpp
// tworzy wątek first wykonujący funkcję foo
std::thread first(foo);
// tworzy watek second wykonujacy funkcje bar, z argumentami
std::thread second(bar, 0, 0.2);
```

Wartość zwracana przez funkcje wykonywane w wątku (tutaj: `foo` i `bar`) jest ignorowana. W celu zwrócenia wyników z funkcji wykonywanych w wątku musimy się posłużyć zmienną globalną, polem w klasie, bądź przekazywaniem zmiennych przez wskaźnik. Przekazywanie zmiennych do wątku przez referencję jest zabronione (chyba, że skorzystamy z wrappera `std::ref()` lub `std::cref()`).

W celu poczekania na zakończenie danego wątku wywołujemy na nim metodę join():

```cpp
first.join(); // czeka na zakonczenie wykonywania wątku w obiekcie first
```

Przykładowy program uruchamiający dwa wątki może zatem wyglądać następująco:

```cpp
#include <iostream>
#include <thread>

void foo() {
    for (int i = 0; i < 5; i++) {
        std::cout << "hello foo" << std::endl;
    }
}

void bar(int x, float y) {
    for (int i = 0; i < 5; i++) {
        std::cout << "hello bar " << x << " " << y << std::endl;
    }
}

int main(int argc, const char * argv[]) {
    std::thread first(foo);
    std::thread second(bar, 0, 0.2);

    std::cerr << "threads started" << std::endl;

    // wait for threads to finish:
    first.join();
    second.join();

    std::cerr << "threads finished" << std::endl;

    return 0;
}
```

Uruchom powyższy program i przeanalizuj wyniki uzyskane w konsoli.

## Przydatne funkcje i mechanizmy

### Sprawdzenie liczby dostępnych rdzeni

Liczbę dostępnych w systemie równoległych sprzętowo wątków można sprawdzić wykorzystując funkcję `std::thread::hardware_concurrency()`. W przypadku procesorów z technologią Hyper-threading, każdy rdzeń fizyczny widziany jest w systemie jako dwa rdzenie logiczne, stąd np. dla 2-rdzeniowego procesora funkcja może zwrócić wartość 4.

**Uwaga!** Jeżeli dany system operacyjny nie oferuje możliwości sprawdzenia liczby równoległych wątków, funkcja zwróci 0. Poprawnie napisany program powinien uwzględniać tę ewentualność.

Pamiętaj, że tworzenie wątków i przełączanie kontekstu pomiędzy nimi (wywłaszczanie) to operacja zajmująca czas procesora, zatem tworzenie zbyt wielu wątków (więcej niż dostępnych sprzętowo), lub wątków działających zbyt krótko, może się okazać nieopłacalne.

### Wstrzymywanie wątku (sleep)

Wstrzymanie wątku (również w standardowym programie jednowątkowym) możliwe jest dzięki funkcji `std::this_thread::sleep_for(std::chrono::duration sleep_duration)`. Obiekt typu `std::chrono::duration` reprezentujący czas wstrzymania można stworzyć dzięki typom pomocniczym, takim jak `std::chrono::milliseconds` lub `std::chrono::seconds`, na przykład:

```cpp
// pausing current thread for 500 milliseconds
std::this_thread::sleep_for(std::chrono::milliseconds(500));
```

### Wyrażenia lambda

Kiedy nie chcemy deklarować nowej funkcji tylko po to, żeby na przykład uruchomić fragment kodu w wątku, możemy wykorzystać wyrażenia lambda. Wyrażenia lambda działają jak funkcje definiowane w miejscu, w którym chcemy ich użyć. Zamiast wskaźnika do funkcji (nazwy), którą chcielibyśmy uruchomić w wątku, wpisujemy kod:

```cpp
[](){ /* cialo lambdy */ }
```

Przykładowe wykorzystanie wyrażenia lambda wygląda następująco:

```cpp
std::thread watek1([](){ std::cout << "kod w watku" << std::endl; });
```
Jeśli chcemy, aby kod wewnątrz wyrażenia lambda miał dostęp do zewnętrznych zmiennych, możemy podać ich nazwy w tzw. *capture-list* w nawiasach kwadratowych, oddzielone przecinkami:

```cpp
int zmienna = 5;
std::thread watek2([zmienna](){
    std::cout << "kod w watku, zmienna = " << zmienna << std::endl;
});
```

Jeśli chcemy modyfikować zewnętrzne zmienne z poziomu wyrażenia lambda, musimy przekazać je przez referencję:

```cpp
int zmienna2 = -1;
std::thread watek3([&zmienna2](){
    zmienna2 = 10;
});
```

### ❗️Zadanie 1.❗️ - kolekcja wątków

Zmodyfikuj pierwszy przykład tak, aby uruchamiał podaną przez użytkownika liczbę wątków, wykonujących jedną funkcję. Funkcja ma wyświetlić w konsoli wiadomość zawierającą unikalne *ID* wątku. *ID* można pobrać z poziomu wątku dzięki funkcji `std::this_thread::get_id()`. Zastanów się jak możesz wygodnie przechowywać informacje o kolekcji wątków i czekać na zakończenie wszystkich.

**Uwaga!** obiekt `std::thread` nie jest kopiowalny (nie jest *CopyConstructible* ani *CopyAssignable*), co oznacza, że nie można w trywialny sposób najpierw utworzyć tymczasowego obiektu, a następnie umieścić go w kolekcji np. metodą `push_back`).

Dlatego też możemy tworząc wątek od razu umieszczać go w kolekcji:

```cpp
collection.push_back(std::thread(foo));
```

lub wymusić przeniesienie korzystając z narzędzia `std::move` i metod typu `emplace`:

```cpp
std::thread temporary(foo);
collection.emplace_back(std::move(temporary);
```

### ❗️Zadanie 2.❗️ - szukanie liczb pierwszych

Wykorzystując kod z zadania 2. z *Lab 09 - zrównoleglanie obliczeń OpenMP* przenieś obliczenia do wielu wątków z wykorzystaniem `std::thread` (zamiast OpenMP). Zastanów się jak uzyskać wartość zwróconą przez funkcję `find_prime_from` uruchomioną w wątku. W razie konieczności użyj funkcji lambda lub funkcji pośredniczącej w wywołaniu.

* Wersja podstawowa: uruchom tyle wątków, ile jest liczb do znalezienia

* Wersja zaawansowana: postaraj się samodzielnie podzielić pracę w zależności od liczby rdzeni dostępnych w komputerze (patrz: **Przydatne funkcje** powyżej).

## Problem równoległego dostępu do zasobów

Analogicznie jak w przypadku OpenMP, równoległa praca wielu wątków, które operują na wspólnych danych może prowadzić do nieoczekiwanych rezultatów. Należy pamiętać, że wątki mogą zostać wywłaszczane w dowolnych momentach, co bez odpowiednich zabezpieczeń może spowodować, że dane będące w trakcie modyfikacji przez jeden wątek zostaną błędnie wykorzystane lub nadpisane przez inny.

### ❗️Zadanie 3.❗️ - licznik

Stwórz w programie zmienną całkowitą. Utwórz 1000 wątków, z których każdy odczeka 10 milisekund, a następnie zwiększy wartość stworzonej zmiennej o 100. Poczekaj na zakończenie wszystkich wątków i sprawdź wynik dodawania. Uruchom program kilkukrotnie.

Czy wynik jest powtarzalny i zgodny ze spodziewanym? Zastanów się jakie operacje elementarne musi wykonać komputer, aby zwiększyć wartość zmiennej zapisanej w pamięci o daną wielkość. Przeanalizuj sytuację, kiedy sekwencja tych operacji zostanie przerwana przez inny wątek.

## Semafory, muteksy, operacje atomowe

Aby zapobiec podobnym sytuacjom, w programach, gdzie zasoby są współdzielone pomiędzy wiele wątków, wprowadza się sekcje krytyczne. Sekcja krytyczna to fragment programu, w którym odbywa się dostęp do zasobów współdzielonych, i który nie może być wykonywany przez więcej niż jeden proces jednocześnie.

Dostęp do sekcji krytycznych może być kontrolowany przez *semafory*, czyli specjalne zmienne lub typy danych, które regulują dostęp do wspólnych zasobów. Semafor może być licznikiem, który określa liczbę wolnych zasobów, jakie mogą zostać rozdysponowane procesom. Można to rozpatrywać przez analogię do restauracji, gdzie zasoby to stoliki, a klienci to procesy. Początkowo semafor ma wartość równą liczbie stolików w restauracji. Każde nadejście klienta (procesu) powoduje przydzielenie mu stolika (zasobów) i zmiejszenie wartości semafora o 1. Osiągnięcie przez semafor wartości 0 oznacza, że wszystkie zasoby są zajęte, a procesy muszą oczekiwać w kolejce na obsłużenie. Kiedy proces zakończy pracę i zwalnia zasoby, wartość semafora jest inkrementowana.

Szczególnym, często używanym przypadkiem semafora jest semafor binarny, zwany też *muteksem*. Przyjmuje on wartości 0 i 1 i używamy go w sytuacji, kiedy do danych zasobów dostęp może mieć tylko jeden proces. Przed wejściem do sekcji krytycznej wątek powinien sprawdzić czy inny proces nie „przejął” muteksu, ewentualnie poczekać na jego zwolnienie (lub wykonywać inne działania nie wymagające dostępu do zasobów współdzielonych), a następnie zablokować muteks na czas wykonywania sekcji krytycznej.

Aby nie spowalniać niepotrzebnie programu poprzez zmuszanie wątków do oczekiwania na dostęp do zasobów, należy dbać o to, aby sekcje krytyczne były możliwie krótkie.

Operacja sprawdzania i blokowania muteksu musi być operacją atomową, czyli jej wykonywanie nie może być przerwane przez inny wątek. Z tego względu muteks musi być obsługiwany na poziomie systemu operacyjnego i uwzględniany przez *scheduler*, który przełącza kontekst pomiędzy procesami.

## Muteksy w C++

Muteksy w C++ dostępne są pod postacią klasy `std::mutex`, dostępnej po załączeniu nagłówka `<mutex>`.

Na obiekcie mutex możemy wykonać trzy metody:
* `lock()`: blokuje muteks; jeśli jest już zablokowany, to czeka na odblokowanie
* `trylock()`: próbuje zablokować muteks, zwraca true jeśli blokada się powiodła
* `unlock()`: odblokowuje muteks

Przykładowo, jeśli wiele wątków ma pracować na zmiennej `counter`:

```cpp
int result = do_stuff();
counter_mtx.lock();
counter += result();
counter_mtx.unlock();
```

Pamiętaj, że aby blokada miała sens, muteks - zmienna `counter_mtx` również musi być współdzielona przez wątki.

### ❗️Zadanie 4.❗️ - prosty muteks

Zastanów się, które operacje w **Zadaniu 3** powinny należeć do sekcji krytycznej. Stwórz muteks, który będzie odpowiadał za kontrolę dostępu do zwiększanej zmiennej. Zmodyfikuj program tak, aby działał poprawnie.

## Problem producenta i konsumenta

Problem producenta i konsumenta pojawia się, kiedy w systemie występują dane, które są generowane i przetwarzane asynchronicznie. W problemie występują dwa działające współbieżnie rodzaje procesów: producent i konsument, którzy dzielą wspólny zasób - bufor dla przetwarzanych jednostek danych. Producent nie powinien próbować umieszczać danych w buforze, kiedy ten jest pełny, a konsument nie powinien przetwarzać danych, kiedy bufor jest pusty.

Przykładem występowania problemu producenta i konsumenta może być np. obsługa klawiatury, gdzie sterownik klawiatury przechwytuje naciśnięcia klawiszy i umieszcza odpowiednie znaki w buforze klawiatury, a system operacyjny bądź program pobiera znaki z bufora i podejmuje odpowiednie akcje.

### ❗️Zadanie 5.❗️ - producent i konsument

Napisz program, który będzie pokazywał działanie producenta i konsumenta na przykładzie produktów - losowych ciągów znaków.

Producent i konsument powinni pracować w dwóch wątkach, wykonujących się w pętli nieskończoność:

* Producent: cyklicznie generuje losowe 10-znakowe ciągi i próbuje umieścić je w buforze. Jeśli bufor jest pełny, wygenerowany „produkt” zostaje porzucony, a do konsoli, do strumienia błędów (*stderr*) zostaje wypisany odpowiedni komunikat, następnie czeka przez losowy czas (określony parametrami).

* Konsument: sprawdza czy w buforze są jakieś elementy, jeśli tak - pobiera je i zapisuje do pliku, każdy w oddzielnej linii, następnie czeka przez czas określony parametrem.

Program powinien mieć następujące parametry, definiowane stałymi w kodzie:

* minimalny okres pracy producenta
* maksymalny okres pracy producenta
* okres pracy konsumenta
* maksymalna głębokość kolejki

Jako bufor możesz wykorzystać kolejkę z biblioteki standardowej (`std::queue`), z odpowiednimi zabezpieczeniami. Zaobserwuj działanie programu w różnych warunkach, zmieniając opóźnienia/głębokość kolejki i doprowadzając do przepełnienia bufora.

*Przydatne funkcje:*

Generowanie ciągu znaków:

```cpp
std::string temp;
temp.resize(10);
for (int i = 0; i < 10; i++) {
    temp[i] = rand()%('z'-'a'+1)+'a';
}
```

Napisz program w dwóch wersjach:

a) Kolejka FIFO 1-1
Uruchom jednego producenta i jednego konsumenta, działających w dwóch wątkach i wymieniających się danymi poprzez współdzielony bufor. 

b) Kolejka FIFO many-many
Rozbuduj program z podpunktu a) o możliwość uruchomienia dowolnej liczby producentów i konsumentów podanej przez użytkownika. Każdy konsument powinien zapisywać dane w pliku o nazwie zawierającej swój ID wątku.


***
Autor: *Jakub Tomczyński*
