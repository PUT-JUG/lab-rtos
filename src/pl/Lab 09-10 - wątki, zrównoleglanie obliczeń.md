# Programowanie współbieżne - tworzenie wątków, zrównoleglanie obliczeń

Programowanie współbieżne to zestaw technik programowania, mających na celu współbieżne (równoległe) przetwarzanie, dzięki jednoczesnym istnieniu wielu wątków lub procesów, operujących na współdzielonych danych.

Przetwarzanie współbieżne ma istotne zastosowanie w systemach czasu rzeczywistego, które muszą zarządzać wieloma procesami jednocześnie, bądź w serwerach, które muszą obsługiwać dużą liczbę zapytań od klientów. W zastosowaniach tych niedopuszczalne jest, żeby jedno zadanie paraliżowało pracę całego systemu.

W systemach wieloprocesorowych lub z procesorami wielordzeniowymi, przetwarzanie współbieżne pozwala także na osiągnięcie dużych korzyści w wydajności przetwarzania, o ile dane zadanie da się podzielić na mniejsze fragmenty, które mogą być wykonywane jednocześnie. Każdy z wątków może być wówczas wykonywany na oddzielnym rdzeniu.

Zarządzaniem dostępnymi zasobami zajmuje się system operacyjny i przydziela on poszczególnym wątkom czas przetwarzania na dostępnych rdzeniach, korzystając z algorytmów szeregowania charakterystycznych dla danego systemu. W sytuacji, kiedy wątków jest więcej niż rdzeni, są one cyklicznie wywłaszczane, tak aby uniknąć zagłodzenia oczekujących zadań.

W ramach kursu wykorzystamy język C++ i mechanizmy dostarczane przez bibliotekę standardową w ramach rozszerzeń C++11. Rozszerzenia te są dostępne w większości współczesnych kompilatorów, np. w Visual Studio od wersji 2013. Niektóre środowiska/kompilatory wymagają włączenia dodatkowych opcji w celu uaktywnienia standardu C++11 (np. flaga `-std=c++11` dla kompilatora `gcc`).

Analogiczną funkcjonalność można osiągnąć za pomocą bibliotek takich *pthreads* (w systemach zgodnych z *POSIX*, np. Linux), bądź rozwiązań dedykowanych dostarczanych przez poszczególne systemy (np. wątki w *Windows API*). Istnieje też szereg języków projektowanych z myślą o przetwarzaniu współbieżnym, np. Go, Erlang, Ada.

## Pierwszy program wielowątkowy

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

## Przydatne funkcje

### Pomiar czasu

W celu określenia czy wprowadzane przez nas rozwiązania przekładają się na zysk w wydajności możemy wykorzystać funkcje pomiaru czasu dostarczone w bibliotece `<chrono>`. Przykładowe użycie mechanizmów chrono do pomiaru czasu, jaki upłynął między dwoma miejscami w kodzie wygląda następująco:

```cpp
// deklarujemy obiekty przechowujace moment rozpoczecia i zakonczenia
std::chrono::time_point<std::chrono::system_clock> start, end;
// zapisujemy biezacy czas jako moment rozpoczecia
start = std::chrono::system_clock::now();

/*
kod, ktorego wydajnosc badamy
*/

// zapisujemy biezacy czas jako moment zakonczenia
end = std::chrono::system_clock::now();
// obliczamy roznice
std::chrono::duration<double> elapsed_time = end - start;
// wypisanie czasu w sekundach
std::cerr << "czas: " << elapsed_time.count() << std::endl;
```

Pamiętaj, że uruchamianie programu skompilowanego w wersji *debug* może dawać inne wyniki wydajności od wersji *release*.

### Sprawdzenie liczby dostępnych rdzeni

Liczbę dostępnych w systemie równoległych sprzętowo wątków można sprawdzić wykorzystując funkcję `std::thread::hardware_concurrency()`. W przypadku procesorów z technologią Hyper-threading, każdy rdzeń fizyczny widziany jest w systemie jako dwa rdzenie logiczne, stąd np. dla 2-rdzeniowego procesora funkcja może zwrócić wartość 4.

**Uwaga!** Jeżeli dany system operacyjny nie oferuje możliwości sprawdzenia liczby równoległych wątków, funkcja zwróci 0. Poprawnie napisany program powinien uwzględniać tę ewentualność.

Pamiętaj, że tworzenie wątków i przełączanie kontekstu pomiędzy nimi (wywłaszczanie) to operacja zajmująca czas procesora, zatem tworzenie zbyt wielu wątków (więcej niż dostępnych sprzętowo), lub wątków działających zbyt krótko, może się okazać nieopłacalne.

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

## Zadania do samodzielnego wykonania

### 1. Kolekcja wątków

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

### 2. Szukanie liczb pierwszych

Liczby pierwsze często znajdują się w okolicy zbioru *2<sup>n</sup> - 1*, gdzie n ∈ N.

Napisz program, który wykorzystując tę własność, będzie wyszukiwał liczby pierwsze. W tym celu wylosuj zbiór wartości początkowych spełniających powyższą zależność. Pamiętaj o ograniczeniach zakresu liczb, jakie mogą być przechowywane w wybranym typie zmiennej. Następnie dla każdej z wygenerowanych liczb, przeszukuj zbiór liczb naturalnych w kierunku malejącym, aż do napotkania liczby pierwszej. Znalezioną liczbę zapisz w przeznaczonym do tego wektorze.
Zastanów się jak podzielić pracę pomiędzy wiele wątków i jak przekazać każdemu wątkowi część zbioru do przeszukiwania. Porównaj wyniki wydajności ze względu na liczbę wątków oraz zadaną wielkość zbioru. Aby uzyskiwać te same wylosowane liczby przy każdym uruchomieniu, możesz zainicjować generator liczb losowych stałą wartością, np. `srand(0)`.

### 3. Operacje na macierzach

W plikach [`Matrix.h`](../resources/Matrix.h) i  [`Matrix.cpp`](../resources/Matrix.cpp) dostarczono prostą bibliotekę pozwalającą na reprezentację macierzy w formie wektora wektorów. Działanie poszczególnych metod opisane jest w komentarzach przy ich deklaracjach.

Przykładowe użycie biblioteki:

```cpp
Matrix test1(3, 10);
Matrix test2(10, 5);
test1.rand();
test2.rand();

Matrix result = test1.multiply(test2);
```

Przeanalizuj działanie metody `multiply`. Na jej podstawie napisz własną, wielowątkową metodę wykonującą mnożenie. Sprawdź dla jakich rozmiarów macierzy opłacalne jest wywoływanie metody wielowątkowej.

**Podpowiedź:** aby utworzyć z poziomu metody klasy wątek, który będzie wykonywał inną metodę klasy (np. wykonującą zadany fragment mnożenia), należy przekazać ją do konstruktora `std::thread` w następujący sposób:

```cpp
class SomeClass {
    /* ... */

    void some_method(int p1, int p2) {
        /* do stuff */
    }

    void method_creating_threads() {
        int param1 = 100;
        int param2 = 1000;
        std::thread some_thread(&SomeClass::some_method, this, param1, param2);
    }

    /* ... */
}
```

Zauważ dodatkowy argument `this` przekazywany do konstruktora wątku, poprzedzający pozostałe parametry przekazywane do wywoływanej metody. Podobną funkcjonalność można uzyskać korzystając z wyrażeń lambda, metod statycznych lub zwykłych funkcji, zależnie od wygody bądź estetyki kodu.

### 4. Dekodowanie wiadomości

<!--

W jednym z największych radioteleskopów na Ziemi w Parkes Observatory przechwycono wiadomość przesłaną między kwaterą Kim Dzong Una w Pjongjangu a ośrodkiem rakietowym w Yongbyon:

`XS[_K][AXKCEEO[RDO[MEYZSD_^AXKPOYKDAOPVPPAKORTLMDQWSXPI_WZVWIKYARKZEW_IO]K_MTPIIEW_SXTY_Y^W_[TKHDLKOQ`
Wiadomość jest zakodowana, jednak wywiad dowiedział się, że Koreańczycy posługują się zmodyfikowaną wersją szyfru Cezara: zamiast stałego przesunięcia dla wszystkich liter, dla każdego kolejnego znaku używane są kolejne przesunięcia z 4-elementowego zbioru będącego kluczem. Zbiór zapętla się dla kolejnych znaków, a dostępny alfabet zawiera znaki od A (ASCII 0x41) do _ (ASCII 0x5F).
Przykładowo, dla klucza szyfrującego {3, 7, 11, 0}, zdanie THIS_IS_A_TEST_MESSAGE zostanie zakodowane w następujący sposób:
znak 1: T zostanie przesunięty o 3,
znak 2: H zostanie przesunięty o 7,
znak 3: I zostanie przesunięty o 11,
znak 4: S zostanie przesunięty o 0,
znak 5: _ zostanie przesunięty o 3,
znak 6: I zostanie przesunięty o 7,
i tak dalej, tworząc zakodowaną wiadomość: WOTSCP^_DG_EV[KMHZ^AJL
Dodatkowo, wiadomo, że każda wiadomość zawiera zwrot GREAT_LEADER. Wykorzystując te informacje pomóż agentom jak najszybciej rozkodować wiadomość wykorzystując wszystkie rdzenie procesora w Twoim komputerze!

-->

***
Autor: *Jakub Tomczyński*

Data ostatniej modyfikacji: {JUG:MODIFICATION_DATE}
