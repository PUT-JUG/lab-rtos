# Programowanie współbieżne - powtórka

## Zadanie

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

***
Autor: *Jakub Tomczyński*

Data ostatniej modyfikacji: {JUG:MODIFICATION_DATE}
