# Programowanie współbieżne - zrównoleglanie obliczeń z wykorzystaniem OpenMP

Programowanie współbieżne to zestaw technik programowania, mających na celu współbieżne (równoległe) przetwarzanie, dzięki jednoczesnym istnieniu wielu wątków lub procesów, operujących na współdzielonych danych.

Przetwarzanie współbieżne ma istotne zastosowanie w systemach czasu rzeczywistego, które muszą zarządzać wieloma procesami jednocześnie, bądź w serwerach, które muszą obsługiwać dużą liczbę zapytań od klientów. W zastosowaniach tych niedopuszczalne jest, żeby jedno zadanie paraliżowało pracę całego systemu.

W systemach wieloprocesorowych lub z procesorami wielordzeniowymi, przetwarzanie współbieżne pozwala także na osiągnięcie dużych korzyści w wydajności przetwarzania, o ile dane zadanie da się podzielić na mniejsze fragmenty, które mogą być wykonywane jednocześnie. Każdy z wątków może być wówczas wykonywany na oddzielnym rdzeniu.

Zarządzaniem dostępnymi zasobami zajmuje się system operacyjny i przydziela on poszczególnym wątkom czas przetwarzania na dostępnych rdzeniach, korzystając z algorytmów szeregowania charakterystycznych dla danego systemu. W sytuacji, kiedy wątków jest więcej niż rdzeni, są one cyklicznie wywłaszczane, tak aby uniknąć zagłodzenia oczekujących zadań.

W ramach kursu wykorzystamy dwa narzędzia:

* bibliotekę OpenMP służącą do zrównoleglania obliczeń - poprzez dodanie kilku odpowiednich dyrektyw możemy przerobić dzięki niej w łatwy sposób zrównoleglić istniejący kod np. przez zrównoleglenie operacji wykonywanych w pętlach

* klasę `std::thread` oraz powiązane obiekty z biblioteki standardowej służące do ręcznego tworzenia i zarządzania wątkami, dostępne w C++ od wersji C++11

## Konfiguracja środowiska dla OpenMP

Większość współczesnych kompilatorów wspiera OpenMP (w różnych wersjach), ale jego obsługa jest zazwyczaj domyślnie wyłączona. Stwórzmy nasz pierwszy projekt wykorzystujący OpenMP.

### Microsoft Visual Studio 2019

Utwórz projekt aplikacji typu **C++ Console App**.

Następnie włącz obsługę OpenMP w ustawieniach projektu. Otwórz właściwości projektu wybierając menu **Project** → **<nazwa_projektu> properties...**.

Przełącz edytor tak, aby zmienić opcje dla każdej z konfiguracji - z listy rozwijanej **Configuration:** wybierz **All Configurations**. 

Następnie w sekcji **Configuration Properties** → **C/C++** → **Language** → **OpenMP Support** na **Enable**. Dodatkowo, w przypadku kompilatora MSVC, który jest używany domyślnie w Visualu, konieczne będzie wyłączenie opcji *two-phase name lookup*. W sekcji **Configuration Properties** → **C/C++** → **Command Line** dodaj w polu **Additional Options** przełącznik `/Zc:twoPhase-`. Zatwierdź zmiany i zamknij konfigurator.

### Qt Creator

Stwórz projekt aplikacji C++ nie wykorzystującej Qt (**Non-Qt Project** → **Plain C++ Application**). W pliku **.pro** opisującym projekt dodaj linijkę:

#### Kompilator MSVC (Windows)

```qmake
QMAKE_CXXFLAGS += -openmp
```

#### Kompilatory GCC, clang (Windows)

```qmake
QMAKE_CXXFLAGS += -fopenmp

LIBS += -lgomp
```

#### Kompilatory GCC, clang (Linux, macOS)

```qmake
QMAKE_CXXFLAGS += -fopenmp

LIBS += -lomp
```

Wymuś ponowne przetworzenie pliku **.pro** klikając prawym przyciskiem na projekt w drzewie plików i wybierz **Run qmake**

### Inne kompilatory, środowiska programistyczne

W większości środowisk i kompilatorów włączenie wsparcia dla OpenMP sprowadza się do dodania odpowiedniej flagi do kompilatora (zazwyczaj `-openmp` lub `-fopenmp`) w ustawieniach projektu. W ramach kursu zakładamy korzystanie ze środowiska Visual Studio lub Qt Creator, w przypadku chęci skorzystania z innego środowiska, poszukaj pomocnych informacji w Internecie.

## Hello OpenMP!

Wklej poniższy tekst jako kod swojej aplikacji.

```cpp
#include <iostream>
#include <chrono>
#include <cmath>

int main() {
    int array[1000];
    int i = 0;

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

W zależności od posiadanego procesora, po wyłączeniu dyrektywy OpenMP powinien być zauważalny spadek wydajności - zazwyczaj 2-3 krotny. Jeśli nie występuje różnica w wydajności, upewnij się, że projekt został dobrze skonfigurowany. Oczywiście przyrost wydajności będzie widoczny tylko w przypadku posiadania wielordzeniowego procesora, ale ostatnie jednordzeniowe modele w komputerach osobistych pojawiały się sporadycznie w okolicach 2010 roku. Liczbę rdzeni i wątków logicznych możesz sprawdzić w systemie Windows 10 otwierając **Menedżer Zadań**, w zakładce **Performance**, a w systemach Linux komendą `lscpu`.

**Uwaga!** Brak obsługi OpenMP przez kompilator lub jego niepoprawna konfiguracja nie spowoduje zazwyczaj żadnych widocznych błędów w kompilacji ani działaniu programu - dyrektywy `#pragma omp` zostaną po prostu zignorowane, generując co najwyżej ostrzeżenie preprocesora. Stąd ważna jest weryfikacja czy nasze środowisko faktycznie wykorzystuje OpenMP.

### Dyrektywy OpenMP

Powyższy program tworzy tablicę liczb, a następnie wypełnia każdy element tablicy wartością szeregu opisanego wzorem:

![Wzór na szereg](_images/09/series.svg),

gdzie ![Wzór na szereg](_images/09/a_i.svg) to i-ty element tablicy.

Ponieważ każdy element tablicy jest liczony niezależnie (nie zależy od wyników uzyskanych w pozostałych przebiegach pętli `for(i)`), w prosty sposób można wykorzystać dyrektywy OpenMP do zrównoleglenia głównej pętli.

W ogólnym przypadku dyrektywy te mają następującą postać:
```
#pragma omp dyrektywy opcje_i_parametry
```

Wyraz `omp` jest słowem kluczowym OpenMP. Dyrektywa `parallel` wskazuje kompilatorowi obszar kodu, który będzie zrównoleglany, kolejna dyrektywa `for` powoduje podział pracy na wątki w obrębie następującej po niej pętli for.

W opcjach i parametrach określamy przede wszystkim to, które zmienne są unikalne dla każdego wątku, a które mogą być współdzielone. Dla dyrektywy `for` zmienna iterowana w następującej pętli - w tym przypadku `i` - jest domyślnie prywatna (`private`) - **każdy wątek ma własną jej kopię**, którą wewnętrznie iteruje. Wszystkie zmienne zewnętrzne (np. `array`) są domyślnie **współdzielone** (`shared`), natomiast zmienne deklarowane lokalnie wewnątrz pętli (np. `j`) - muszą być prywatne, ponieważ są tworzone dopiero wewnątrz wątku. Zachowanie domyślne możemy modyfikować podając nazwy zmiennych do opcji `private` lub `shared`, wymienione po przecinku, w nawiasach okrągłych. Tablica wynikowa może być tutaj bez obaw współdzielona, ponieważ każdy z wyników zapisywany jest w odrębne miejsce.

Opcje pozwalają także określić sposób rozdzielania pracy na poszczególne wątki klauzulą `schedule`. Domyślny tryb zależy od konkretnej implementacji, ale zazwyczaj używane jest przydzielanie statyczne (`static`), dzielące pracę na możliwie równe "kawałki" (ang. `chunk`), po jednym dla każdego wątku. W uproszczeniu, jeśli w takim przypadku będziemy mieli do dyspozycji 4 wątki, to każdy z nich będzie miał do wykonania 250 iteracji głównej pętli - pierwszy dla `i` od 0 do 249, drugi od 250 do 499 itd:

```
schedule(static):
thread 1: ****************                                                
thread 2:                 ****************                                
thread 3:                                 ****************                
thread 4:                                                 ****************
```

Możliwa jest modyfikacja rozmiaru "kawałka" opcjonalnym argumentem `chunk-size`. Przykładowo, dla `chunk-size` równego 4 wątek pierwszy otrzyma do wykonania iteracje dla `i` równego 0, 1, 2, 3, 16, 17, 18, 19..., drugi 4, 5, 6, 7, 20, 21, 22, 23 itd.

```
schedule(static, 4):   
thread 1: ****            ****            ****            ****            
thread 2:     ****            ****            ****            ****        
thread 3:         ****            ****            ****            ****    
thread 4:             ****            ****            ****            ****
```

Dostępne są także alternatywne algorytmy rozdzielania pracy: `dynamic`, `guided`, `auto` i `runtime`. Wyjaśnienie sposobu ich działania możesz sprawdzić w dokumentacji lub przeczytać tutaj: http://jakascorner.com/blog/2016/06/omp-for-scheduling.html.

Jeśli mamy zatem program, w którym zaimplementowaliśmy algorytm składający się z wielokrotnie powtarzanej czynności, a kolejne jej iteracje nie zależą od wyniku poprzednich - możemy w ten sposób bardzo małym nakładem pracy spowodować, że nasz program będzie potrafił wykorzystywać wiele rdzeni procesora. Kluczowe jest tutaj określenie, **które zmienne będą prywatne, a które współdzielone**. Ważna jest też deklaracja wymaganych zmiennych ponad dyrektywą OpenMP - zwróć uwagę na wcześniejszą deklarację zmiennej `i`, zamiast zwyczajowego umieszczenia jej wewnątrz samej pętli `for`.

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

Pamiętaj, że program uzyska zupełnie różne wyniki w zależności od optymalizacji podczas kompilacji (wybór konfiguracji *Debug* lub *Release*) oraz sposobu uruchomienia - zwykłego lub w trybie debugowania. Porównując wyniki należy uruchamiać tę samą konfigurację, w ten sam sposób.

## Zadania do samodzielnego wykonania

### 1. Operacje na macierzach

W plikach [`Matrix.h`](../resources/Matrix.h) i  [`Matrix.cpp`](../resources/Matrix.cpp) dostarczono prostą bibliotekę pozwalającą na reprezentację macierzy w formie wektora wektorów. Działanie poszczególnych metod opisane jest w komentarzach przy ich deklaracjach.

Przykładowe użycie biblioteki:

```cpp
Matrix test1(3, 10);
Matrix test2(10, 5);
test1.rand();
test2.rand();

Matrix result = test1.multiply(test2);
```

Przeanalizuj działanie metody `multiply`. Na jej podstawie napisz własną, wielowątkową metodę `multiply_openmp` wykonującą mnożenie, dzieląc pracę ze względu na elementy macierzy wyjściowej. Sprawdź dla jakich rozmiarów macierzy opłacalne jest wywoływanie metody wielowątkowej.

### 2. Szukanie liczb pierwszych

> Liczby pierwsze często znajdują się w okolicy zbioru *2<sup>n</sup> - 1*, gdzie n ∈ N

Napisz program, który wykorzystując tę własność, będzie wyszukiwał liczby pierwsze. W tym celu wygeneruj wektor - serię wartości należących do powyższego zbioru, dla *n* od 2 do 63. Wykorzystaj typ liczbowy `uint64_t` aby zapewnić, że liczby da się poprawnie zapisać w zmiennej.

Następnie napisz funkcję, która rozpoczynając od przekazanej do niej pojedynczej liczby, będzie przeszukiwała zbiór liczb naturalnych w kierunku malejącym, aż do napotkania liczby pierwszej:

```cpp
uint64_t prime = find_prime_from(63); // zwroci 61
```

Przetestuj działanie funkcji dla liczb nieco większych od znanych sobie liczb pierwszych, aby zweryfikować poprawność jej implementacji.

Następnie wywołaj funkcję dla każdego elementu z uprzednio przygotowanego zbioru wartości początkowych, a wyniki umieszczaj w odpowiednim miejscu w uprzednio przygotowanym wektorze (pamiętaj, aby zmienić rozmiar wektora wynikowego przed uruchomieniem pętli - nie używaj metody `push_back()`!).

Dodaj do programu odpowiednie dyrektywy OpenMP, dzięki którym jednocześnie będzie uruchomionych wiele wątków z funkcją `find_prime_from`. Zastanów się, które zmienne powinny być współdzielone, a które prywatne.

Dodaj do programu funkcje pomiaru czasu i porównaj wynik działania na jednym wątku oraz wersji wielowątkowej. Czy widoczny jest przyrost wydajności?

Zwróć uwagę na fakt, że przetworzenie małych liczb zajmie funkcji `find_prime_from` znacznie mniej iteracji (poszukiwana liczba pierwsza będzie przeciętnie znajdowała się bliżej wartości startowej). Zastanów się jak w takim przypadku będą się zachowywać poszczególne wątki przy domyślnych ustawieniach *schedulera*.

Zmodyfikuj ustawienia *schedulera* ustawiając `chunk-size` równy 1 lub wybierając alternatywny tryb jego pracy i ponownie zbadaj szybkość działania programu.

***
Autor: *Jakub Tomczyński*
