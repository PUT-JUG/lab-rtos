

## Zadania do samodzielnego wykonania



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

Data ostatniej modyfikacji: {JUG:MODIFICATION_DATE}
