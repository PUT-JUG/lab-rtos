# Programowanie współbieżne - współdzielenie zasobów, semafory

Równoległa praca wielu wątków, które operują na wspólnych danych może prowadzić do nieoczekiwanych rezultatów. Należy pamiętać, że wątki mogą zostać wywłaszczane w dowolnych momentach, co bez odpowiednich zabezpieczeń może spowodować, że dane będące w trakcie modyfikacji przez jeden wątek zostaną błędnie wykorzystane lub nadpisane przez inny.

### Problem równoległego dostępu do zasobów

Stwórz w programie zmienną całkowitą. Utwórz 10000 wątków, z których każdy odczeka 10 milisekund, a następnie zwiększy wartość stworzonej zmiennej o 100.

**Podpowiedź:** Wstrzymanie wątku możliwe jest dzięki funkcji `std::this_thread::sleep_for(std::chrono::duration sleep_duration)`. Obiekt typu `std::chrono::duration` reprezentujący czas wstrzymania można stworzyć dzięki typom pomocniczym, takim jak `std::chrono::milliseconds` lub `std::chrono::seconds`, na przykład:

```cpp
// pausing current thread for 500 milliseconds
std::this_thread::sleep_for(std::chrono::milliseconds(500));
```

Poczekaj na zakończenie wszystkich wątków i sprawdź wynik dodawania. Uruchom program kilkukrotnie. Czy wynik jest powtarzalny i zgodny ze spodziewanym? Zastanów się jakie operacje elementarne musi wykonać komputer, aby zwiększyć wartość zmiennej zapisanej w pamięci o daną wielkość. Przeanalizuj sytuację, kiedy sekwencja tych operacji zostanie przerwana przez inny wątek.

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

### Zadanie 1: prosty muteks

Zastanów się, które operacje w programie inkrementującym zmienną wielowątkowo, powinny należeć do sekcji krytycznej. Stwórz muteks, który będzie odpowiadał za kontrolę dostępu do zwiększanej zmiennej. Zmodyfikuj program tak, aby działał poprawnie.

## Problem producenta i konsumenta

Problem producenta i konsumenta pojawia się, kiedy w systemie występują dane, które są generowane i przetwarzane asynchronicznie. W problemie występują dwa działające współbieżnie rodzaje procesów: producent i konsument, którzy dzielą wspólny zasób - bufor dla przetwarzanych jednostek danych. Producent nie powinien próbować umieszczać danych w buforze, kiedy ten jest pełny, a konsument nie powinien przetwarzać danych, kiedy bufor jest pusty.

Przykładem występowania problemu producenta i konsumenta może być np. obsługa klawiatury, gdzie sterownik klawiatury przechwytuje naciśnięcia klawiszy i umieszcza odpowiednie znaki w buforze klawiatury, a system operacyjny bądź program pobiera znaki z bufora i podejmuje odpowiednie akcje.


### Zadanie 2: producent i konsument

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

- generowanie ciągu znaków

Napisz program w dwóch wersjach:

a) Kolejka FIFO 1-1
Uruchom jednego producenta i jednego konsumenta, działających w dwóch wątkach i wymieniających się danymi poprzez współdzielony bufor. 

b) Kolejka FIFO many-many
Rozbuduj program z podpunktu a) o możliwość uruchomienia dowolnej liczby producentów i konsumentów podanej przez użytkownika. Każdy konsument powinien zapisywać dane w pliku o nazwie zawierającej swój ID wątku.


***
Autor: *Jakub Tomczyński*

Data ostatniej modyfikacji: {JUG:MODIFICATION_DATE}
