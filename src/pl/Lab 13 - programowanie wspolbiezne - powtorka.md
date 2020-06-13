# Programowanie współbieżne - powtórka

## Zadanie 1

Poniżej umieszczono kod programu obliczającego punktację w grze Scrabble dla słów zaczytanych z pliku tekstowego.

Program wczytuje słowa do wektora `words`, a następnie umieszcza odpowiadające im wartości punktów w wektorze `scores`.

Wykorzystując wybrane mechanizmy C++, zrównolegl obliczanie punktów.

Przykładowy plik wejściowy możesz pobrać tutaj (słowa celowo są sztucznie posklejane w długie ciągi): [`gibberish.txt`](../resources/gibberish.txt)

```cpp
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

int calculate_points(const std::string& word, const std::map<char, unsigned int> &face_values) {
    int value = 0;
    for (const char &c : word) {
        value += face_values.at(c);
    }
    return value;
}

int main() {
    std::map<char, unsigned int> face_values_english = {
        {'a', 1}, {'e', 1}, {'i', 1}, {'o', 1}, {'u', 1}, {'l', 1}, {'n', 1}, {'s', 1}, {'t', 1}, {'r', 1},
        {'d', 2}, {'g', 2},
        {'b', 3}, {'c', 3}, {'m', 3}, {'p', 3},
        {'f', 4}, {'h', 4}, {'v', 4}, {'w', 4}, {'y', 4},
        {'k', 5},
        {'j', 8}, {'x', 8},
        {'q', 10}, {'z', 10}
    };

    std::ifstream file("gibberish.txt");
    if (file.fail()) {
        std::cerr << "Could not open file" << std::endl;
        return 1;
    }
    std::vector<std::string> words;
    while (file.good()) {
        std::string word;
        file >> word;
        words.push_back(word);
    }

    std::vector<int> scores;
    scores.reserve(words.size());
    for (const std::string &w : words) {
         scores.push_back(calculate_points(w, face_values_english));
    }
}
```

***

## Zadanie 2

Poniżej umieszczono kod programu odczytującego i przetwarzającego dane z symulowanego czujnika wilgotności.

W programie uruchomione są (poza wątkiem głównym) wątki:
* czujnika - gdzie dane pobierane są z czujnika z maksymalną prędkością (jeden odczyt trwa ok. 100 ms)
* filtra - gdzie dane są poddawane filtracji na podstawie ostatnich 10 próbek, co 1 sekundę

Poza tym w wątku głównym aktualne dane (po filtracji) są wyświetlane w konsoli, co 3 sekundy. Przerwanie programu możliwe jest kombinacją klawiszy Ctrl-C.

Wymiana danych pomiędzy wątkami używa współdzielonych zmiennych. Zmodyfikuj program tak, aby wyeliminować możliwość pojawienia się związanych z tym błędów.

**Uwaga!**
Nie modyfikuj klas `HumiditySensor` ani `Filter`

```cpp
#include <csignal>
#include <iostream>
#include <string>
#include <thread>
#include <random>
#include <vector>
#include <queue>

class HumiditySensor {
public:
    HumiditySensor() {
        rng_.seed(static_cast<unsigned int>(std::time(nullptr)));
    }
    
    double blocking_read() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return dist_(rng_);
    }
private:
    std::mt19937 rng_;
    std::normal_distribution<double> dist_{40.0, 10.0};
};

class Filter {
public:
    Filter(size_t buffer_length = 10) : buffer_length_(buffer_length) {
        buffer_.reserve(buffer_length);
    }
    void add_data(double data) {
        if (buffer_.size() < buffer_length_) {
            buffer_.push_back(data);
        } else {
            buffer_[buffer_idx_] = data;
            buffer_idx_ = (buffer_idx_ + 1) % buffer_length_;
        }
    }
    double output() {
        return std::accumulate(buffer_.begin(), buffer_.end(), 0.0) / buffer_.size();
    }
private:
    std::vector<double> buffer_;
    size_t buffer_idx_ = 0;
    size_t buffer_length_;
};

void sensor_thread_func(bool& cont, std::queue<double> &raw_data) {
    HumiditySensor sensor;
    while (cont) {
        raw_data.push(sensor.blocking_read());
    }
}

void filter_thread_func(bool& cont, std::queue<double> &raw_data, std::vector<double> &filtered_data) {
    Filter filter;
    while (cont) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        while (!raw_data.empty()) {
            filter.add_data(raw_data.back());
            raw_data.pop();
        }
        filtered_data.push_back(filter.output());
    }
}

bool cont = true;

int main() {
    std::signal(SIGINT, [](int){std::cout << "Quitting"; cont = false;});
    
    std::queue<double> raw_data_;
    std::vector<double> filtered_data_;
    
    std::thread sensor_thread(sensor_thread_func, std::ref(cont), std::ref(raw_data_));
    std::thread filter_thread(filter_thread_func, std::ref(cont), std::ref(raw_data_), std::ref(filtered_data_));
    
    while (cont) {
        if (!filtered_data_.empty()) {
            std::cout << "filtered data:";
            for (double v : filtered_data_) {
                std::cout << " " << v;
            }
            std::cout << std::endl;
            filtered_data_.clear();
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    sensor_thread.join();
    filter_thread.join();
}
```

***

## Zadanie 3

Dostarczono klasę `TemperatureSensor`, która odpowiada za komunikację z symulowanym czujnikiem temperatury oraz funkcję `do_magic_processing()`, która dokonuje czasochłonnego przetwarzania danych.

Utwórz instancję klasy `TemperatureSensor`. Uruchom w oddzielnym wątku metodę `TemperatureSensor::run()`, powodując start "odczytu". Klasa zapisuje dane do wewnętrznego bufora.

W kolejnym wątku cyklicznie (co 1000 ms) sprawdzaj dostępne dane i pobieraj je do własnego bufora (metoda `TemperatureSensor::get_data()`).

Uruchom równoległe wątki, które będą wykonywały przetwarzanie danych za pomocą funkcji `do_magic_processing()`. Wątków powinno być tyle, ile oferuje platforma, na której został uruchomiony program.

Zastosuj mechanizm, który będzie powodował, że wszystkie wątki przetwarzające będą miały na bieżąco przydzielane dane z bufora. Wynik przetwarzania wyświetlaj na ekranie.

Dodaj do programu możliwość poprawnego zamknięcia po naciśnięciu klawisza Enter w terminalu. Koniec transmisji z czujnikiem możesz wywołać poprzez `TemperatureSensor::stop()`. Dokończ przetwarzanie danych zebranych w buforach i zakończ program.

```cpp
class TemperatureSensor {
public:
    TemperatureSensor() {
    value_distribution_ = std::uniform_int_distribution<int>(-5,5);
    }
    void run() {
        start_ = std::chrono::system_clock::now();
        while (continue_) {
            double value = 36.6 + value_distribution_(value_generator_);
            buffer_mutex_.lock();
            if (buffer_.size() > 1000) {
                std::cerr << "TemperatureSensor::run(): ERROR: internal buffer full" << std::endl;
            } else {
                buffer_.push_back(value);
            }
            buffer_mutex_.unlock();
            std::chrono::duration<double> elapsed_time = std::chrono::system_clock::now() - start_;
            double sleep_time = (std::sin(elapsed_time.count() * 3.14 * 2.0 * freq) + 1.0) / 2.0 + 0.1;
            std::cout << sleep_time << std::endl;
            std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
        }
    }
    void stop() {
        continue_ = false;
    }
    std::vector<double> get_data() {
        buffer_mutex_.lock();
        std::vector<double> retval = buffer_;
        buffer_.clear();
        buffer_mutex_.unlock();
        return retval;
    }
private:
    std::vector<double> buffer_;
    std::mutex buffer_mutex_;
    bool continue_ = true;
    std::chrono::time_point<std::chrono::system_clock> start_;
    std::mt19937 value_generator_;
    std::uniform_int_distribution<int> value_distribution_;
    const double freq = 0.5;
};
```

```cpp
double do_magic_processing(double input) {
    double sum = 0;
    for (int k = 1; k < 100; k++) {
        sum += std::pow(-1.0, k+1) / (2.0 * double(k) - 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    return input * sum / 0.437735 + 32.0;
}
```

**Podpowiedź:** aby utworzyć wątek, który będzie wykonywał metodę klasy inną niż statyczną, należy przekazać do konstruktora `std::thread` wskaźnik do metody (`&nazwa_klasy::nazwa_metody`) jako funkcję, a następnie dodatkowy argument będący wskaźnikiem do konkretnej instancji obiektu:

Z zewnątrz:
```cpp
class SomeClass {
    /* ... */

    void some_method(int p1, int p2) {
        /* do stuff */
    }
}
```

```cpp
SomeClass obj;
std::thread some_thread(&SomeClass::some_method, &obj, 1, -100);
```

Z poziomu klasy:
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

***
Autor: *Jakub Tomczyński*
