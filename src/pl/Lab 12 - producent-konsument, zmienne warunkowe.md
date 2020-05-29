# Problem producenta i konsumenta

Problem producenta i konsumenta pojawia się, kiedy w systemie występują dane, które są generowane i przetwarzane asynchronicznie. W problemie występują dwa działające współbieżnie rodzaje procesów: producent i konsument, którzy dzielą wspólny zasób - bufor dla przetwarzanych jednostek danych. Producent nie powinien próbować umieszczać danych w buforze, kiedy ten jest pełny, a konsument nie powinien przetwarzać danych, kiedy bufor jest pusty.

Przykładem występowania problemu producenta i konsumenta może być np. obsługa klawiatury, gdzie sterownik klawiatury przechwytuje naciśnięcia klawiszy i umieszcza odpowiednie znaki w buforze klawiatury, a system operacyjny bądź program pobiera znaki z bufora i podejmuje odpowiednie akcje.

### ❗️Zadanie 1.❗️ - producent i konsument

Napisz program, który będzie pokazywał działanie producenta i konsumenta na przykładzie produktów - losowych ciągów znaków.

Producent i konsument powinni pracować w dwóch wątkach, wykonujących się w pętli nieskończoność:

* Producent: cyklicznie generuje losowe 10-znakowe ciągi i próbuje umieścić je w buforze, a następnie czeka przez losowy czas (ograniczony parametrami). Jeśli bufor jest pełny, wygenerowany „produkt” zostaje porzucony, a do konsoli, do strumienia błędów (*stderr*) zostaje wypisany odpowiedni komunikat, .

* Konsument: sprawdza czy w buforze są jakieś elementy, jeśli tak - pobiera je i zapisuje do pliku, każdy w oddzielnej linii, następnie czeka przez czas określony parametrem.

Program powinien mieć następujące parametry, definiowane stałymi w kodzie:

* minimalny okres pracy producenta
* maksymalny okres pracy producenta
* okres pracy konsumenta
* maksymalna głębokość kolejki

Jako bufor możesz wykorzystać kolejkę z biblioteki standardowej (`std::queue`), z odpowiednimi zabezpieczeniami. Zaobserwuj działanie programu w różnych warunkach, zmieniając opóźnienia/głębokość kolejki i doprowadzając do przepełnienia bufora.

*Przydatne funkcje:*

Generator liczb losowych, którego można używać bezpiecznie z poziomu wielu wątków (sam generator jest współdzielony między wszystkie wątki i wywołania funkcji (`static`), ale jego wywołanie jest chronione muteksem).

```cpp
int random_int(int min, int max) {
    static std::mt19937 random_number_engine;
    static std::mutex rng_mtx;
    std::uniform_int_distribution<int> distribution(min, max);
    rng_mtx.lock();
    int retval = distribution(random_number_engine);
    rng_mtx.unlock();
    return retval;
}
```

Generowanie ciągu znaków na podstawie powyższego:

```cpp
std::string temp;
temp.resize(10);
for (int i = 0; i < 10; i++) {
    temp[i] = random_int('a', 'z');
}
```

Napisz program w dwóch wersjach:

#### a) Kolejka FIFO 1-1

Uruchom jednego producenta i jednego konsumenta, działających w dwóch wątkach i wymieniających się danymi poprzez współdzielony bufor. 

#### b) Kolejka FIFO many-many

Rozbuduj program z podpunktu a) o możliwość uruchomienia dowolnej liczby producentów i konsumentów podanej przez użytkownika. Każdy konsument powinien zapisywać dane w pliku o nazwie zawierającej swój ID wątku.


## Bufor cykliczny

Rozwiązaniem pozwalającym na poprawną wymianę danych pomiędzy jednym producentem i jednym konsumentem, jest bufor cykliczny (zwany też kołowym).

Bufor cykliczny wykorzystuje stałą (o stałej długości i położeniu w pamięci) tablicę, która w praktyce jest buforem bez początku i końca - po ostatnim elemencie następuje przeskok na element pierwszy. W celu określenia, w które miejsce powinny zostać zapisywane dane, a z którego odczytywane, wykorzystuje się dwie dodatkowe zmienne (indeksy miejsc lub wskaźniki na miejsca). Wskaźnik zapisu (*head*) opisuje miejsce, w które powinny zostać zapisane dane, jeśli nadejdą. Wskaźnik odczytu (*tail*) opisuje miejsce, z którego powinny zostać odczytane dane, jeśli nastąpi takie żądanie. Każda operacja zapisu/odczytu powoduje przesunięcie odpowiedniego wskaźnika o 1 w przód. Kiedy *tail* „dogoni” *head* bufor jest pusty, a kiedy head dogoni tail, bufor jest pełny i dalszy zapis jest niemożliwy.

Ponieważ oba wskaźniki są modyfikowane tylko przez jeden proces, możliwa jest implementacja dla przypadku z jednym producentem i jednym konsumentem bez wykorzystania muteksów. Rozwiązanie to jest bardzo efektywne i często wykorzystywane w mikrokontrolerach i systemach wbudowanych.

Przykładowe stany bufora cyklicznego:

* Stan początkowy, bufor pusty:

![Bufor pusty](../images/cyclic_buffer_empty.svg)

* Dodano 5 elementów:

![Dodano 5 elementów](../images/cyclic_buffer_head_moved.svg)

* Pobrano 2 elementy, 3 zostały w buforze:

![Pobrano 2 elementy](../images/cyclic_buffer_tail_moved.svg)

* Dodano 4 elementy, bufor pełny:

![Dodano 5 elementów](../images/cyclic_buffer_full.svg)

### ❗️Zadanie 2.❗️ - bufor cykliczny

Napisz program analogiczny do **Zadania 1** tak, aby wykorzystywał bufor cykliczny.

# Zmienne warunkowe

Zmienne warunkowe (ang. *condition variables*) są jednym ze sposobów na synchronizację pomiędzy wątkami. Pozwalają na przysłanie sygnału z jednego wątku do pozostałych, które mogą na niego oczekiwać (np. sygnał o nadejściu danych, zwolnieniu bufora itp.). Dzięki temu nie jest konieczne np. okresowe sprawdzanie w pętli danego warunku, co mogłoby zwiększać czas reakcji na wystąpienie warunku lub powodować niepotrzebne użycie procesora. Zmienne warunkowe w C++11 są dostępne pod postacią klasy `std::condition_variable` po załączeniu nagłówka `<condition_variable>`. Zmienna warunkowa używana jest zawsze w połączeniu z muteksem oraz obiektem `std::unique_lock`, który zarządza blokadą muteksu. Oczekiwanie na sygnał od zmiennej warunkowej (dodanie wątku do kolejki oczekujących) rozpoczynamy metodą `wait()`. Pozostałe wątki korzystające z danej zmiennej warunkowej mogą powiadomić jeden z kolejki oczekujących wątków metodą `notify_one()` lub wszystkie oczekujące metodą `notify_all()`. Przykładowe użycie zmiennej warunkowej pokazano na poniżej:

```cpp
std::condition_variable cv; // zmienna warunkowa
std::mutex cv_mtx;          // mutex chroniacy zmienna warunkowa oraz stan
int state = 0;               // zmienna przekazujaca informacje o stanie

void do_work() {
    for (int i = 1; i <= 5; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        {
            std::unique_lock<std::mutex> lock(cv_mtx);
            // tworzy blokade na muteksie
            state = i; // zmien stan
        } // koniec scope - obiekt lock zostaje zniszczony -
        // nastepuje automatyczne zwolnienie muteksu
        cv.notify_all(); // powiadom wszystkie watki oczekujace w cv.wait()
    }
}

int main (){
    std::unique_lock<std::mutex> lock(cv_mtx); // tworzy blokade na muteksie
    std::thread work_thread(do_work);
    while(state != 5) {
        cv.wait(lock); // zdejmuje blokade, czeka na powiadomienie
        // po nadejsciu powiadomienia ponownie zaklada blokade
        std::cout << state << std::endl;
    }
    work_thread.join();
    return 0;
}
```

**Uwaga!** Wyjście z metody `wait()` nie musi oznaczać sygnału od innego wątku - w programie mogą wystąpić spontaniczne wybudzenia wątków (ang. *spurious wakeups*), stąd faktyczne źródło wybudzenia należy zweryfikować np. korzystając z dodatkowej zmiennej (tutaj: `state`).

### ❗️Zadanie 3.❗️ - komunikacja z czujnikami

W pewnym systemie występuje wiele czujników zwracających dane w nieregularnych odstępach czasu. Napisz program, który będzie cyklicznie pobierał dane ze wszystkich czujników (każdy czujnik w oddzielnym wątku), a po uzbieraniu w buforze 5 wartości, wypisywał dane na ekranie w postaci:

```
New data!: ID:wartosc ID:wartosc ID:wartosc ID:wartosc ID:wartosc
```

Użytkownik wprowadza liczbę czujników *n* przy uruchomieniu programu, a ID przyjmują wartości od 0 do *n*-1.

Do powiadamiania wątku realizującego wyświetlanie o nowych danych wykorzystaj zmienną warunkową. Komunikację z czujnikiem oraz odczyt danych symuluj dostarczoną funkcją double `read_sensor(int id)`:

```cpp
double read_sensor(int id){
    static thread_local std::mt19937 generator(id);
    std::uniform_int_distribution<int> sleep_distribution(100,(id+1)*1000);
    std::uniform_real_distribution<double> value_distribution(id,id+0.99);
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_distribution(generator)));
    return value_distribution(generator);
}
```

***
Autor: *Jakub Tomczyński*
