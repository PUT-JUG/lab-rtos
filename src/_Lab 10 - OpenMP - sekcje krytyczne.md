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

Przyjmuje ona zakres argumentów, krok całkowania oraz obiekt typu `std::function` wskazujący na określoną funkcję, której wartości mają zostać zcałkowane. Przykładowe jej użycie może wyglądać następująco - potrzebujemy funkcji, która obliczy swoją wartość dla danego argumentu:

```cpp
double sin_poly(double arg) {
    return pow(sin(arg), 2.0) * arg + pow(sin(arg), 3.0) * arg + pow(sin(arg), 4.0) * arg + pow(sin(arg), 5.0) * arg;
}
```

W tym przypadku `sin_poly` opisuje funkcję ![sin_poly](_images/10/sin_poly.svg). Następnie wywołujemy funkcję `integrate` przekazując jej odpowiednie argumenty i zdefiniowaną uprzednio funkcję `sin_poly`:

```cpp
double result = integrate(0, 100*M_PI, 0.00001, sin_poly);
```

Zbadaj działanie powyższego kodu. Na jego podstawie napisz własną funkcję `integrate_openmp`, w której wykorzystasz możliwości wielowątkowe dla przyspieszenia obliczeń. Sprawdź poprawność uzyskanego wyniku porównując go z wersją jednowątkową.

**Podpowiedzi:**

Do sprawdzenia obliczeń możesz wykorzystać serwis www.wolframalpha.com
Przykładowe zapytanie może mieć postać:

> integrate (sin(x)^2 * x + sin(x)^3 * x + sin(x)^4 * x + sin(x)^5 * x) from 0 to 100*pi

Czasem przydatne jest sprawdzenie iloma wątkami dysponujemy w systemie - wewnątrz kodu oznaczonego dyrektywą `omp parallel` możemy wykorzystać funkcje:

* `omp_get_num_threads()` - zwraca liczbę używanych wątków
* `omp_get_thread_num()` - zwraca indeks aktualnego wątku (w zakresie od 0 do `omp_get_num_threads()`-1)

## Synchronizacja wątków w OpenMP

W odróżnieniu od sekcji krytycznych (lub semaforów - omawianych później), gdzie jednoczesny dostęp do zasobów jest ograniczony do jednego lub większej liczby wątków, czasem istnieje konieczność synchronizacji wątków, szczególnie jeśli realizują zadanie, które składa się z faz, a każda z nich wymaga wyniku z pozostałych wątków. Zadanie to można rozwiązać na kilka sposobów.

W OpenMP istnieje mechanizm bariery (`barrier`), który powoduje że wszystkie uruchomione wątki oczekują, do czasu aż każdy z nich osiągnie barierę a dopiero potem są wznawiane. Dzięki temu mamy pewność, że wyniki, które miały zostać wygenerowane w pierwszej fazie (przed barierą) przez każdy z wątków są dostępne dla pozostałych wątków, gdy rozpoczynają wykonywanie kodu znajdującego się za barierą.

```cpp
#include <omp.h>

#define MAX_NT 4


void thread_work(int id)
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    std::cout << "Watek " << id << "zaczyna prace..." << std::endl;

    std::chrono::duration<double> elapsed_time;
    do {
        end = std::chrono::system_clock::now();
        elapsed_time = end - start;
    } while (elapsed_time.count() < id);
}

int main(void)
{
    int id;

#pragma omp parallel num_threads(MAX_NT) private(id)
    {
        id = omp_get_thread_num();
        thread_work(id + 1);
        std::cout << "Watek " << id << "zakonczyl prace i czeka przy barierze..." << std::endl;

#pragma omp barrier

        std::cout << "Watek " << id << "juz poza bariera." << std::endl;
    }

    return 0;
}
```
Dyrektywa `#pragma omp parallel num_threads(MAX_NT)` uruchamia `MAX_NT` wątków, gdzie scope wątku określony jest nawiasami klamrowymi. Daje to możliwość uruchomienia danej sekcji kodu w kilku instancjach wątków mimo, że nie ma tu pętli. 

Funkcja `omp_get_thread_num()` zwraca identyfikator wątku, gdzie wartość 0 oznacza wątek główny programu (wątek ten nie wykonuje sekcji `#omp parallel...`, tylko czeka na jej zakończenie), natomiast uruchamiane wątki mają kolejne numery porządkowe.

`#pragma omp barrier` tworzy barierę, która zatrzymuje dalsze wykonanie każdego z wątków, do czasu aż wszystkie z nich nie osiągną bariery.

W przykładzie powyżej w każdym z wątków uruchamiana jest funkcja `thread_work`, której czas trwania jest zależny od numeru wątku (trwa tyle sekund ile wynosi `id` wątku). Uruchamiając kod zwróć uwagę w jakiej kolejności uruchamiane są wątki, oraz w jakiej kolejności wątki dochodzą do bariery. 

Przeanalizuj i uruchom powyższy kod. Zwróć uwagę na kolejność w której pojawiają się komunikaty za barierą, czy jest ona ustalona i zawsze taka sama?

### Zadanie 3 - synchronizacja katalogów

Załóżmy, że mamy `N` serwerów o adresach `serwer_1`, `serwer_2` itd., między którymi chcielibyśmy synchronizować pliki. W przypadku standardowego procesu (bez zrównoleglania) działanie wyglądałoby następująco:

zaczynając z *i = 1*:
1. dla serwera `serwer_i` pobierz dane
2. wyślij je na wszystkie pozostałe serwery
3. weź kolejny serwer (zwiększ *i*) i idź do kroku 1.

W przypadku zrównoleglania procesu schemat mógłby mieś postać:

1. uruchom wątek dla każdego serwera, każdy wątek z inną wartością *i = 1...N*
2. w danym wątku pobierz dane z serwera `serwer_i` i zapisz je do pliku
3. czekaj aż wszystkie wątki zakończą fazę 2
4. w każdym wątku rozpocznij przesyłanie wszystkich plików na serwer `serwer_i`

Zwróć uwagę, że operacje 2 i 4 odbywają się w każdym z wątków równolegle i w danym wątku realizowana jest komunikacja tylko i wyłącznie z pojedynczym serwerem.

Napisz program, który połączy się ze stworzonym przez Ciebie serwerem ssh i dokona synchronizacji plików między różnymi zasobami sieciowymi (możesz również w ten sposób synchronizować dane między różnymi serwerami).

W tym celu:
- stwórz na serwerze 4-8 katalogów i w każdym z nich umieść pliki `status` (o różnym rozmiarze 10-300MB).
- w systemie w którym piszesz program, wygeneruj klucz `RSA`, który umożliwi Ci logowanie się do serwera ssh oraz używanie scp bez podawania hasła
* Dla Windows:
    * zainstaluj program WinSCP, uruchom go i wprowadź adres serwera, użytkownika oraz hasło ssh
    * przejdź do [Advanced->Authentication](https://winscp.net/eng/docs/ui_login_authentication) i klikając na przycisk `Tools` wybierz **Generate new key pair** i wygeneruj klucz RSA - zapisz w znanej tobie lokalizacji, dostępnej tylko dla twojego użytkownika (np. w c:\Users\user\Documents): 
        * klucz publiczny (klikając przyciski **Save public key**) dodaj rozszerzenie .ppk
        * prywatny w standardzie openssh **Conversions-> Export OpenSSH key (new format)**
    * zainstaluj swój klucz publiczny na serwerze ssh, tak żeby była możliwa autoryzacja bez użycia hasła: w WinSCP przejdź do zakładki [authentication](https://winscp.net/eng/docs/ui_login_authentication), z kliknij na przycisku `Tools` wybierz: `Install Public Key into server` podając klucz publiczny, który zostanie zainstalowany na serwerze i powiązany z twoim loginem
    * od tego momentu możesz logować się do serwer bez podawania hasła, a jedynie załączając w poleceniu opcją `-i` ścieżkę dostępu do klucza prywatnego.
```
    ssh -i c:\sciezka\klucz user@ip_servera
```

* Dla Linux lub macOS:
    * klucz jest zazwyczaj domyślnie wygenerowany jako `~/.ssh/id_rsa` oraz  `~/.ssh/id_rsa.pub`, jeśli go nie ma możesz go wygenerować poleceniem `ssh-keygen`
    * skopiuj klucz na serwer poleceniem `ssh-copy-id` (składnia identyczna jak dla polecenia `ssh`)
- z linii poleceń systemu przetestuj wywołanie komendy `scp`,  które umożliwi upload i download pliku między twoim komputerem i zasobem sieciowym serwera i wykorzystuje autoryzację za pomocą klucza (w Windowsie użyj przełącznika `-i` aby wskazać klucz)

Napisz program, który równolegle pobierze dane ze wszystkich serwerów (zasobów sieciowych) i zapisze je do tymczasowej lokalizacji na twoim komputerze dodając do nazwy pliku identyfikator serwera (np. `status_serwer_id`), a następnie wyśle wszystkie pobrane pliki `status` do wszystkich serwerów/zasobów sieciowych. Pamiętaj, że każdy wątek przed rozpoczęciem ładowania aktualizacji plików na serwer musi poczekać do czasu, aż wszystkie wątki zakończą pobieranie.

**Uwaga** w celu wywołania polecenia systemowego (np. komendy `scp`) użyj funkcji `system`. Jeśli chcesz przygotować ciąg znaków generujących polecenie (zawierających np. nazwę pliku zależną od id wątku, nazwę zasobu sieciowego itp. użyj komendy `sprintf` lub `sprintf_s`.
np. 
```cpp
char exec[180];
sprintf(exec, "scp  -i %s %s/%s %s@%s:%s", private_key_path, current_path, filename, destination_user, destination_ip, destination_path);
std::cout << exec << std::endl;
if (system(exec) == 0) {
    std::cout << "File " << file << "moved successfully" << std::endl;
} else {
    std::cout << "File " << file << "not moved successfully" << std::endl;
}
```

**Uwaga 2**

W systemie Windows może wystąpić problem z dostępem do pliku wykonywalnego `scp.exe`, ponieważ znajduje się on w katalogach systemowych.

Możesz ominąć ten problem kopiując pliki `scp.exe` oraz `ssh.exe` z ich oryginalnej lokalizacji do katalogu uruchomieniowego projektu (domyślnie jest to katalog, gdzie znajdują się pliki źródłowe):

```
<lokalizacja_projektu>/<nazwa_projektu>/<nazwa_projektu>
```

Oryginalną lokalizację pliku `scp.exe` znajdziesz wpisując w terminalu polecenie `where scp`.

***
Autor: *Jakub Tomczyński*, *Piotr Kaczmarek*
