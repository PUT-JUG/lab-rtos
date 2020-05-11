# Programowanie współbieżne - OpenMP - sekcje krytyczne

Równoległa praca wielu wątków, które operują na wspólnych danych może prowadzić do nieoczekiwanych rezultatów. Należy pamiętać, że wątki mogą zostać wywłaszczane w dowolnych momentach, co bez odpowiednich zabezpieczeń może spowodować, że dane będące w trakcie modyfikacji przez jeden wątek zostaną błędnie wykorzystane lub nadpisane przez inny.

## Problem równoległego dostępu do zasobów

Skompiluj i uruchom poniższy program:

```cpp
#include <iostream>

int main() {
    uint64_t accumulator = 0;
    int i;

#pragma omp parallel for private(i)
    for (i = 0; i < 1000; i++) {
        for (volatile int j = 0; j < 1000; j++) { // volatile dla uniknięcia optymalizacji
            accumulator = accumulator + 1;
        }
    }

    std::cout << accumulator << std::endl;

    return 0;
}
```

Sprawdź wynik dodawania. Uruchom program kilkukrotnie. Czy wynik jest powtarzalny i zgodny ze spodziewanym? Zastanów się jakie operacje elementarne musi wykonać komputer, aby zwiększyć wartość zmiennej zapisanej w pamięci o daną wielkość. Przeanalizuj sytuację, kiedy sekwencja tych operacji zostanie przerwana przez inny wątek.

## Sekcje krytyczne w OpenMP

Aby zapobiec podobnym sytuacjom, w programach, gdzie zasoby są współdzielone pomiędzy wiele wątków, wprowadza się sekcje krytyczne. Sekcja krytyczna to fragment programu, w którym odbywa się dostęp do zasobów współdzielonych, i który nie może być wykonywany przez więcej niż jeden proces jednocześnie.

Najczęściej dostęp do sekcji krytycznych kontroluje się za pomocą *semaforów*, czyli specjalnych zmiennych lub typów danych, które regulują dostęp do wspólnych zasobów. Więcej o semaforach dowiesz się w kolejnej części kursu. W OpenMP do oznaczenia sekcji krytycznej wystarczy jednak prosta dyrektywa `#pragma omp critical`. Jeśli jakikolwiek wątek znajduje się już w sekcji krytycznej, wszystkie pozostałe będą musiały poczekać na wejście do niej.

Aby nie spowalniać niepotrzebnie programu poprzez zmuszanie wątków do oczekiwania na dostęp do zasobów, należy dbać o to, aby sekcje krytyczne były możliwie krótkie - wszelkie obliczenia, oczekiwanie na nowe dane itp. powinny odbywać się poza sekcją krytyczną.

Powyższy program możemy spróbować naprawić oznaczając miejsce, gdzie odwołujemy się do współdzielonej zmiennej `accumulator` jako krytyczne:

```cpp
#include <iostream>

int main() {
    uint64_t accumulator = 0;
    int i;

#pragma omp parallel for private(i)
    for (i = 0; i < 1000; i++) {
        for (volatile int j = 0; j < 1000; j++) { // volatile dla uniknięcia optymalizacji
#pragma omp critical
            accumulator = accumulator + 1;
        }
    }

    std::cout << accumulator << std::endl;

    return 0;
}
```

Tak zmodyfikowany program traci jednak sens w kontekście wielowątkowości - całe obliczenia znajdują się w sekcji krytycznej, a do tego dokładany jest narzut związany z przełączaniem kontekstu i oczekiwaniem na wejście do sekcji krytycznej, co powoduje, że działa on wolniej niż program jednowątkowy.

### Zadanie 1 - akumulacja

Napraw powyższy program tak, aby sekcja krytyczna nie obejmowała całych obliczeń.

Zauważ, że każda z iteracji głównej pętli (iterującej po `i`) mogłaby akumulować dane w swoim wewnętrznym liczniku, który dopiero na końcu iteracji byłby dodawany do głównego akumulatora.

Sprawdź czy modyfikacja poprawiła czas działania programu. Jeśli uzyskiwane czasy są bardzo krótkie, a co za tym idzie obarczone dużym błędem pomiaru, możesz zwiększyć liczbę wewnętrznych iteracji.

### Zadanie 2 - całkowanie metodą trapezów

Poniżej umieszczono funkcję, która pozwala na całkowanie numeryczne funkcji przekazanej jako argument `func`:

```cpp
double integrate(double x_min, double x_max, double step, std::function<double(double)> func) {
    double retval = 0;
    double half_step = step / 2.0;
    for (double x = x_min; x+step <= x_max; x += step) {
        double x1 = x;
        double x2 = std::min(x+step, x_max);
        retval += (func(x1) + func(x2)) * half_step;
    }
    return retval;
}
```

Przyjmuje ona zakres argumentów, krok całkowania oraz obiekt typu `std::function` wskazujący na określoną funkcję, której wartości mają zostać zcałkowane.

Przykładowe jej użycie może wyglądać następująco - potrzebujemy funkcji, która obliczy swoją wartość dla danego argumentu:

```cpp
double sin_poly(double arg) {
    return pow(sin(arg), 2.0) * arg + pow(sin(arg), 3.0) * arg + pow(sin(arg), 4.0) * arg + pow(sin(arg), 5.0) * arg;
}
```

W tym przypadku `sin_poly` opisuje funkcję ![sin_poly](../images/10/sin_poly.svg),

Następnie wywołujemy funkcję `integrate` przekazując jej odpowiednie argumenty i zdefiniowaną uprzednio funkcję `sin_poly`:

```cpp
double result = integrate(0, 100*M_PI, 0.00001, sin_poly);
```

Zbadaj działanie powyższego kodu. Na jego podstawie napisz własną funkcję `integrate_openmp`, w której wykorzystasz możliwości wielowątkowe dla przyspieszenia obliczeń. Sprawdź poprawność uzyskanego wyniku porównując go z wersją jednowątkową.

**Podpowiedzi:**

Do sprawdzenia obliczeń możesz wykorzystać serwis wolframalpha.com
Przykładowe zapytanie może mieć postać:

> integrate (sin(x)^2 * x + sin(x)^3 * x + sin(x)^4 * x + sin(x)^5 * x) from 0 to 100*pi

Czasem przydatne jest sprawdzenie iloma wątkami dysponujemy w systemie - wewnątrz kodu oznaczonego dyrektywą `omp parallel` możemy wykorzystać funkcje:

`omp_get_num_threads()` - zwraca liczbę używanych wątków
`omp_get_thread_num()` - zwraca indeks aktualnego wątku (w zakresie od 0 do `omp_get_num_threads()`-1)

***
Autor: *Jakub Tomczyński*
