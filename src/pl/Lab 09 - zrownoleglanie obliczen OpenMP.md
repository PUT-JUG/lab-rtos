# Programowanie współbieżne - zrównoleglanie obliczeń z wykorzystaniem OpenMP

Programowanie współbieżne to zestaw technik programowania, mających na celu współbieżne (równoległe) przetwarzanie, dzięki jednoczesnym istnieniu wielu wątków lub procesów, operujących na współdzielonych danych.

Przetwarzanie współbieżne ma istotne zastosowanie w systemach czasu rzeczywistego, które muszą zarządzać wieloma procesami jednocześnie, bądź w serwerach, które muszą obsługiwać dużą liczbę zapytań od klientów. W zastosowaniach tych niedopuszczalne jest, żeby jedno zadanie paraliżowało pracę całego systemu.

W systemach wieloprocesorowych lub z procesorami wielordzeniowymi, przetwarzanie współbieżne pozwala także na osiągnięcie dużych korzyści w wydajności przetwarzania, o ile dane zadanie da się podzielić na mniejsze fragmenty, które mogą być wykonywane jednocześnie. Każdy z wątków może być wówczas wykonywany na oddzielnym rdzeniu.

Zarządzaniem dostępnymi zasobami zajmuje się system operacyjny i przydziela on poszczególnym wątkom czas przetwarzania na dostępnych rdzeniach, korzystając z algorytmów szeregowania charakterystycznych dla danego systemu. W sytuacji, kiedy wątków jest więcej niż rdzeni, są one cyklicznie wywłaszczane, tak aby uniknąć zagłodzenia oczekujących zadań.

W ramach kursu wykorzystamy dwa narzędzia:

* bibliotekę OpenMP służącą do zrównoleglania obliczeń - poprzez dodanie kilku odpowiednich dyrektyw możemy przerobić dzięki niej istniejący, jednowątkowy kod np. przez zrównoleglenie operacji wykonywanych w pętlach

* klasę `std::thread` oraz powiązane obiekty służące do ręcznego tworzenia i zarządzania wątkami, dostępne w C++ od wersji C++11

## Konfiguracja środowiska dla OpenMP

Większość współczesnych kompilatorów wspiera OpenMP (w różnych wersjach), ale jego obsługa jest zazwyczaj domyślnie wyłączona. Stworzymy pierwszy projekt wykorzystujący OpenMP

### Microsoft Visual Studio 2019

Utwórz projekt aplikacji typu **C++ Console App**.

Następnie włącz obsługę OpenMP w ustawieniach projektu. Otwórz właściwości projektu wybierając menu **Project** → **<nazwa_projektu> properties...**.

Przełącz edytor tak, aby zmienić opcje dla każdej z konfiguracji - z listy rozwijanej **Configuration:** wybierz **All Configurations**. 

Następnie w sekcji **Configuration Properties** → **C/C++** → **Language** → **OpenMP Support** na **Enable**. Dodatkowo, w przypadku kompilatora MSVC, który jest używany domyślnie w Visualu, konieczne będzie wyłączenie opcji *two-phase name lookup*. W sekcji **Configuration Properties** → **C/C++** → **Command Line** dodaj w polu **Additional Options** przełącznik `/Zc:twoPhase-`. Zatwierdź zmiany i zamknij konfigurator.

### Qt Creator z kompilatorem MSVC

W pliku **.pro** opisującym projekt dodaj linijkę:

```qmake
QMAKE_CXXFLAGS += -openmp
```

Wymuś ponowne przetworzenie pliku **.pro** klikając prawym przyciskiem na projekt w drzewie plików i wybierz **Run qmake**

### Inne kompilatory, środowiska programistyczne

W większości środowisk i kompilatorów włączenie wsparcia dla OpenMP sprowadza się do dodania odpowiedniej flagi do kompilatora w ustawieniach projektu. W ramach kursu zakładamy korzystanie ze środowiska Visual Studio lub Qt Creator, w przypadku chęci skorzystania z innego środowiska, poszukaj pomocnych informacji w Internecie.

## Hello OpenMP

Wklej poniższy tekst jako kod swojej aplikacji.

```cpp
#include <iostream>
#include <chrono>

int main() {
    int array[1000];
    int i;

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

#pragma omp parallel for private(i)
    for (i = 0; i < 1000; i++) {
        array[i] = 0;
        for (int j = 0; j < 10000; j++) {
            array[i] += pow(1.0 / i, j);
        }
    }

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;
    std::cerr << "elapsed time: " << elapsed_time.count() << std::endl;

    return 0;
}
```

Skompiluj i uruchom program w zwykłym trybie (bez debuggera):

* Visual Studio: wybierz menu **Debug** → **Start Without Debugging** lub skorzystaj ze sktótu `Ctrl`+`F5`
* Qt Creator: naciśnij zieloną strzałkę (Run) lub skrót `Ctrl`+`R`

Zaobserwuj czas, jaki wyświetlił program.

Następnie zakomentuj linijkę aktywującą użycie OpenMP: `#pragma omp parallel for private(i)`. Uruchom program raz jeszcze, w ten sam sposób i ponownie zaobserwuj zmierzony czas.

W zależności od posiadanego procesora, po wyłączeniu dyrektywy OpenMP powinien być zauważalny spadek wydajności - zazwyczaj 2-3 krotny. Jeśli nie występuje różnica w wydajności, upewnij się, że projekt został dobrze skonfigurowany. Oczywiście przyrost wydajności będzie widoczny tylko w przypadku posiadania wielordzeniowego procesora, ale ostatnie jednordzeniowe modele w komputerach osobistych pojawiały sporadycznie w okolicach 2010 roku. Liczbę rdzeni i wątków logicznych możesz sprawdzić w systemie Windows 10 otwierając **Menedżer Zadań**, w zakładce **Performance**.

### Dyrektywy OpenMP

Powyższy program tworzy tablicę liczb, a następnie wypełnia każdy element tablicy wartością szeregu opisanego wzorem:

a_i = \sum_{j=1}^{10000} \frac{1}{i^j},

gdzie a_i to i-ty element tablicy.

Ponieważ każdy element tablicy jest liczony niezależnie (nie zależy od wyników uzyskanych w pozostałych iteracjach), w prosty sposób można wykorzystać dyrektywy OpenMP do zrównoleglenia głównej pętli.

W ogólnym przypadku dyrektywy te mają następującą postać:
```
#pragma omp nazwa_dyrektywy opcje_i_parametry
```

# `TODO`
*Wyraz `omp` jest słowem kluczowym OpenMP. Programista wykorzystuje dyrektywę parallel, aby wskazać kompilatorowi obszar kodu, który będzie zrównoleglany. Kolejna dyrektywa - for - informuje kompilator, że zrównoleglana będzie pętle typu for. Następnie określa się, które zmienne, wykorzystane w pętli, będą zmiennymi wspólnymi (shared), a które prywatnymi (private). Zmienne wspólne są dostępne dla każdego wątku, natomiast do danej zmiennej prywatnej ma dostęp tylko jeden określony wątek. W naszym programie zmiennymi prywatnymi są i i k, czyli liczniki pętli. Uczynienie ich zmiennymi prywatnymi powoduje, że każdy wątek ma swój wewnętrzny licznik iteracji. Np.: jeśli pętla ma długość 100, to rozdzielając ją na cztery wątki, każdy wątek będzie miał do wykonania po 25 iteracji, a zmienne prywatne poinformują go, w którym miejscu wykonywania obliczeń się znajduje. Ściśle mówiąc, każdy wątek dostaje kopie zmiennych k oraz i i na tych kopiach pracuje. Jest to bardzo ważna część programu i użytkownik musi sam poprawnie określić, która zmienna ma być wspólna, a która prywatna.*

## Przydatne funkcje

### Pomiar czasu

W celu określenia czy wprowadzane przez nas rozwiązania przekładają się na zysk w wydajności możemy wykorzystać funkcje pomiaru czasu dostarczone w bibliotece `<chrono>`. W pierwszym przykładzie funkcje biblioteki `chrono` zostały wykorzystane do pomiaru czasu, jaki upłynął między dwoma miejscami w kodzie.

Poniżej zaprezentowano wyodrębniony fragment kodu realizujacy pomiar czasu, opatrzony dodatkowymi komentarzami:

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

## Zadania do samodzielnego wykonania `TODO`

### 1. Szukanie liczb pierwszych

Liczby pierwsze często znajdują się w okolicy zbioru *2<sup>n</sup> - 1*, gdzie n ∈ N.

Napisz program, który wykorzystując tę własność, będzie wyszukiwał liczby pierwsze. W tym celu wylosuj zbiór wartości początkowych spełniających powyższą zależność. Pamiętaj o ograniczeniach zakresu liczb, jakie mogą być przechowywane w wybranym typie zmiennej. Następnie dla każdej z wygenerowanych liczb, przeszukuj zbiór liczb naturalnych w kierunku malejącym, aż do napotkania liczby pierwszej. Znalezioną liczbę zapisz w przeznaczonym do tego wektorze.
Zastanów się jak podzielić pracę pomiędzy wiele wątków i jak przekazać każdemu wątkowi część zbioru do przeszukiwania. Porównaj wyniki wydajności ze względu na liczbę wątków oraz zadaną wielkość zbioru. Aby uzyskiwać te same wylosowane liczby przy każdym uruchomieniu, możesz zainicjować generator liczb losowych stałą wartością, np. `srand(0)`.

### 2. Operacje na macierzach

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

### 3. Dekodowanie wiadomości

W jednym z największych radioteleskopów na Ziemi w Parkes Observatory przechwycono pierwszą w historii ludzkości wiadomość od istot pozaziemskich:

```
NVD_NZJTDMJZIX^PSJ[ZROVEXOQEVUNYWBWNR^B_MIKOXU]APVBT]A__WM_AXBW_THQYWOCADCEIIUZODMWSD][RQZLSDKIARJ_AD_[SXUKEVVQ_QE_MDPPCMZD_MUGR^ZQNEXQER^[M
```

Wiadomość jest zakodowana, jednak analiza pozostałości po obcej cywilizacji pozwala przypuszczać, że zastosowano w tym przypadku rozbudowaną wersję *szyfru Cezara*: zamiast stałego przesunięcia dla wszystkich liter, dla każdego kolejnego znaku używane są kolejne przesunięcia z 4-elementowego zbioru będącego kluczem. Zbiór zapętla się dla kolejnych znaków, a dostępny alfabet zawiera znaki od `A` (ASCII *0x41*) do `_` (ASCII *0x5F*).

Przykładowo, dla klucza szyfrującego {3, 7, 11, 0}, zdanie `THIS_IS_A_TEST_MESSAGE` zostanie zakodowane w następujący sposób:
* znak 1: `T` zostanie przesunięty o 3,
* znak 2: `H` zostanie przesunięty o 7,
* znak 3: `I` zostanie przesunięty o 11,
* znak 4: `S` zostanie przesunięty o 0,
* znak 5: `_` zostanie przesunięty o 3,
* znak 6: `I` zostanie przesunięty o 7,
i tak dalej, tworząc zakodowaną wiadomość: `WOTSCP^_DG_EV[KMHZ^AJL`

Sygnał pochodzi z planety *URANTIA* i naukowcy są zgodni, że to słowo prawdopodobnie jest zawarte w tekście wiadomości. Wykorzystaj wszystkie rdzenie swojego komputera aby zidentyfikować nadawcę wiadomości i rozwiązać jedną z największych zagadek w historii ludzkości!

***
Autor: *Jakub Tomczyński*
