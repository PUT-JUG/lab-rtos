# Powtórka przed Kolokwium 1

## Przykładowe zadania:

1. Napisz skrypt, który znajdzie w katalogu podanym jako argument do skryptu wszystkie pliki z rozszerzeniem *sh*, modyfikowane nie dawniej niż 7 dni temu i nada im prawo do wykonywania.

2. Napisz skrypt, który na podstawie pliku wejściowego wskazanego pierwszym argumentem wyświetli nazwy trzech planet o największej liczbie księżycy, w kolejności alfabetycznej. Przykładowy plik wejściowy: [planets.txt](../resources/planets.txt).

3. Napisz skrypt, który sklei zawartość wszystkich plików przekazanych jako argumenty i wypisze w konsoli zgodnie z następującym przykładem:


*Zawartość plik1.txt*:
```
abc
xyz
```

*Zawartość inny_plik.txt*:
```
123
456
```

```bash
./task_3.sh plik1.txt inny_plik.txt
```

*Wyjście*:
```
plik1.txt: abc
plik1.txt: xyz
inny_plik.txt: 123
inny_plik.txt: 456
```

4. Napisz skrypt, który zliczy i wypisze sumę znaków we wszystkich plikach podanych jako argumenty wywołania. 

5. Napisz skrypt, który utworzy w katalogu domowym folder *pictures_backup* i skopiuje do niego wszystkie pliki z rozszerzeniem *jpg* znajdujące się w bieżącym katalogu, a następnie zmieni nowym plikom prawa dostępu na tylko do odczytu.

6. Napisz skrypt, który przydzieli plikom odpowiednie prawa dostępu na podstawie par argumentów *plik* - *prawa dostępu w notacji numerycznej*. Przykładowo:

```bash
./task_6.sh plik1.txt 467 plik2.txt 777 plik3.txt 577
```

7. Napisz skrypt, który dopisze tekst określony jako pierwszy argument wywołania na końcu wszystkich plików z rozszerzeniem zdefiniowanym jako drugi argument i znajdujących się w bieżącym katalogu.

8. Napisz skrypt, który zsumuje wielkości plików w bieżącym katalogu dla każdego rozszenia podanego jako argument.
Przykładowo:

```bash
./task_8.sh txt jpg rar
txt: 173
jpg: 3748909
rar: 0
```

9. Napisz skrypt, który wyświetli pierwszą linię od końca z pliku podanego jako pierwszy argument, drugą linię od końca z pliku podanego jako drugi argument itd.

    *Wersja rozszerzona:*
    
    Jeżeli dany plik jest zbyt krótki, wyświetl stosowny komunikat.

10. Napisz skrypt, który w pętli wczytuje z klawiatury numer (PID) procesu, numer sygnału a następnie wysyła wskazany sygnał do określonego procesu. Wpisanie słowa `EXIT` kończy pracę skryptu.


***
Autorzy: *Jakub Tomczyński*, *Adam Bondyra*

Data ostatniej modyfikacji: {JUG:MODIFICATION_DATE}
