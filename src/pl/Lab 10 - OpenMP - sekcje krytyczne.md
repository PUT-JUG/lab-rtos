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

## Synchronizacja wątków w OpenMP
W odróżnieniu od sekcji krytycznych (lub semaforów - omawianych później, gdzie jednoczesny dostęp do zasobów jest ograniczony do jednego lub większej liczby wątków, czasem istnieje konieczność synchronizacji wątków, szczególnie jeśli realizują zadanie, które skłąda się z faz, a każda z nich wymaga wyniku z pozostałych wątków. Zadanie to można rozwiązać na kilka sposobów.
//rysunek

W OpenMP istnieje mechanizm bariery (```barier```), który powoduje że wszystkie uruchomione wątki oczekują aż każdy z nich osiągnie barierę a dopiero potem są wznawiane. Dzięki temu mamy pewność, że wyniki, które miały zostać wygenerowane w pierwszej fazie (przed barierą) przez każdy z wątków są dostępne dla pozostałych wątków, gdy rozpoczynają wykonywanie kodu znajdującego się za barierą.

```cpp
#define MAX_NT 4

void
thread_work(int id)
{
    clock_t t1, t2;

    printf("Watek %d zaczyna prace...\n", id - 1);

    t1 = clock();

    do {
        t2 = clock();
    }
    while ((((double)t2 - t1) / CLOCKS_PER_SEC) < id);
}

int
main(void)
{
    int id;

    #pragma omp parallel num_threads(MAX_NT) private(id)
    {
        id = omp_get_thread_num();
        thread_work(id + 1);
        printf("Watek %d zakonczyl prace i czeka przy barierze...\n", id); 

        #pragma omp barrier

        printf("Watek %d juz poza bariera.\n", id); 
    }

    printf("Nacisnij dowolny klawisz...."); 
    getchar();

    return 0;
}
```
Dyrektywa ```#pragma omp parallel num_threads(MAX_NT)``` uruchamia ```MAX_NT``` wątków, gdzie scope wątku określony jest nawiasami klamrowymi. Daje to możliwość odpalenia danej sekcji kodu w kilku instancjach wątków mimo, że nie ma tu pętli. 

Funkcja ``omp_get_thread_num()`` zwraca identyfikator wątku. Gdzie wartość 0 oznacza wątek główny programu (wątek ten nie wykonuje sekcji ``#opm parallel...``, tylko czeka na jej zakończenie), natomiast uruchamiane wątki mają kolejne numery porządkowe.

``#pragma omp barrier`` tworzy barierę, która zatrzymuje dalsze wykonanie każdego z wątków, do czasu aż wszystkie z nich nie osiągną bariery.

W przykładzie powyżej w każdym z wątków uruchamiana jest funkcja ``thread_work``, której czas trwania jest zależny od numeru wątku (trwa tyle sekund ile wynosi id wątku). Uruchamiając kod zwróć uwagę w jakiej kolejności uruchamiane są wątki, oraz w jakiej kolejności wątki dochodzą do bariery. 

Zwróć uwagę na kolejność w której pojawiają się komunikaty za barierą, czy jest ona ustalona i zawsze taka sama?

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

### Zadanie 3 - synchronizacja katalogów
Załóżmy że mamy `N` serwerów, między którymi chcielibyśmy synchronizować pliki. W przypadku standardowego procesu (bez zrównoleglania) działanie wyglądałoby następująco:
1. dla serwera id pobierz dane
2. wyślij je na serwery wszystkie pozostałe serwery
3. weź kolejny serwer i idź do kroku 1.

W przypadku zrównoleglania procesu schemat mógłby mieś postać
1. uruchom wątek dla każdego serwera
2. pobierz dane z serwera id i zapisz je do pliku
3. czekaj aż wszystkie wątki zakończą fazę 2
4. Rozpocznij przesyłanie wszystkich plików na serwer id
zauważ że operacje 2 i 4 odbywają się równolegle i w danym wątku realizowana jest komunikacja z pojedynczym serwerem.


Napisz program, który połączy się ze stworzonym przez Ciebie serwerem ssh i dokona synchronizacji plików między różnymi zasobami sieciowymi (możesz również w ten sposób synchronizować dane między różnymi serwerami). W tym celu:
- stwórz na serwerze w znanym zasobie sieciowym plik `status` (o rozmiarze 10-300MB).
- wygeneruj klucz `rsa`, który umożliwi Ci logowanie się do serwera ssh bez podawania hasła. Pod windowsem użyj winscp jeśli używasz linuxa lub MacOs użyj `ssh-keygen`
- W winscp wprowadż dane serwera, użytkownika oraz hasło, przejdź do [Advanced->Authentication](https://winscp.net/eng/docs/ui_login_authentication) i klikając na przycisk tools wybierz ``Generate new key pair` i wygeneruj klucz RSA zapisz klucz publiczny i prywatny w znanej lokalizacji (https://www.ssh.com/ssh/putty/windows/puttygen)
- zainstaluj swój klucz publiczny na serwerze, tak żeby była możliwa autoryzacja: pod Windowsem możesz użyć winscp: zainstaluj go, przejdź do zakładki [authentication](https://winscp.net/eng/docs/ui_login_authentication), z kliknij na przycisku ``Tools`` wybierz: ``Install Public Key into Server`` podając klucz publiczny, który zostanie zainstalowany na serwerze i powiązany z twoim loginem. Od teg momentu możesz logować się do serwera bez podawania hasła, a jedynie załączając w opcją -i ścieżkę dostępu do klucza prywatnego.   Jeśli używasz Linuxa lub MacOs wystarczy użyci polecenia ssh-copy-id
- z linii poleceń systemu przetestuj wywołanie komendy `scp`,  które umożliwi upload i download pliku między twoim komputerem i zasobem sieciowym serwera i wykorzystuje autoryzację za pomocą klucza (przełącznik `-i`)
- napisz program, który równolegle pobierze dane ze wszystkich serwerów (zasobów sieciowych) i zapisze je do tymczasowej lokalizacji  na twoim komputerze dodając do nazwy pliku identyfikator serwera (np. `status_serwer_id`)
- wyśle wszystkie pobrane plik ``statusów`` do wszystkich serwerów/zasobów sieciowych. Pamiętaj, że każdy wątek przed rozpoczęciem ładowania aktualizacji plików na serwer musi poczekać do czasu, aż wszystkie wątki zakończą pobieranie pliku ``status`` 

*Uwaga* w celu wywołania polecenia systemowego (np. komendy `scp`) użyj funkcji `system`. Jeśli chcesz przygotować ciąg znaków generujących polecenie (zawierających np. nazwę pliku zależną od id wątku itp. użyj komendy `sprintf` lub `sprintf_s`.
np. 
```cpp
char exec[180];
sprintf(exec, "scp  -i%s %s/%s %s@%s:/home", priv_key, current_path, filename, destination_user, dest_ip);
std::cout << exec << std::endl;
if (system(exec) == 0)
    std::cout << "File " << file << "moved successfully" << std::endl;
else
    std::cout << "File " << file << "not moved successfully" << std::endl;
```

W tym przypadku

***
Autor: *Jakub Tomczyński*, *Piotr Kaczmarek*
