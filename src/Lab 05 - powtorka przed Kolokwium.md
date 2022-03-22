# Powtórka przed Kolokwium (Linux)

## Dodatkowe zadania:

1. Napisz skrypt, który znajdzie w katalogu podanym jako argument do skryptu wszystkie pliki z rozszerzeniem *sh*, modyfikowane nie dawniej niż 7 dni temu i nada im prawo do wykonywania.

2. Napisz skrypt który policzy liczbę linii zawierających słowo “color” w pliku ~/.bashrc 

3. System linux loguje wiadomości w pliku tekstowym /var/log/kern.log. Wypisz z tego pliku 3 ostatnie zdarzenia dotyczące urządzeń USB.

4. Polecenie `ifconfig` pozwala zobaczyć konfigurację interfejsów sieciowych, m.in. adresy IP, MAC, liczby transferowanych danych (linia zaczynająca się od ‘RX packets’ wskaże ile danych zostało pobranych). Przykładowo, będąc podłączonym do WiFi z telefonu, można zobaczyć ile danych już zużyliśmy.
- Napisz skrypt, który wypisze ilość bajtów pobraną przez interfejs sieciowy, który pobrał tych danych najwięcej
- Napisz skrypt, który wypisze adresy MAC wszystkich interfejsów sieciowych

5. Suma kontrolna jest to liczba uzyskana według specjalnego algorytmu służąca do zapewnienia integralności danych. [https://pl.wikipedia.org/wiki/Suma_kontrolna]. 
Napisz skrypt, który stworzy plik raport.txt, zawierający w każdym wierszu nazwę pliku oraz jego sumę kontrolną obliczoną algorytmem md5, dla każdego pliku *.txt w aktualnym katalogu. nazwa pliku i suma kontrolna powinny być oddzielone spacją. 
Wykorzystaj program `md5sum`. 

6. Napisz program weryfikujący integralność plików w katalogu.
Dla każdego pliku *.txt w aktualnym katalogu, porównaj jego sumę kontrolną obliczoną za pomocą programu md5 z sumą kontrolną zapisaną w pliku raport.txt z zadania poprzedniego, oraz wyświetl ostrzegawczy komunikat w przypadku rozbieżności. 
- Przetestuj skrypt zmieniając treść któregoś z plików. 
- Zwróć uwagę, jak zmiana pojedynczego znaku wpływa na zmianę funkcji skrótu dla tego pliku.
- Czy program będzie działał, jeżeli któryś plik testowy będzie miał znak spacji w nazwie?


7. Napisz skrypt, który na podstawie pliku wejściowego wskazanego pierwszym argumentem wyświetli nazwy trzech planet o największej liczbie księżycy, w kolejności alfabetycznej. Przykładowy plik wejściowy: [planets.txt](_resources/planets.txt).

8. W pliku [trees.txt](_resources/trees.txt) zapisane są w formacie csv informacje o kilku drzewach rosnących w ogrodzie (wraz z nagłówkiem w pierwszej linii, informującym o zawartości kolumn pliku). Napisz skrypt, który zapisze do pliku output.txt 3 wysokości dwóch najwyższych brzóz o statusie “chronione”.
Weryfikacja: \
`./skrypt.sh` \
Oczekiwana zawartość pliku output.txt po uruchomieniu skryptu: \
11.0 \
10.8

9. Napisz skrypt, który sklei zawartość wszystkich plików przekazanych jako argumenty i wypisze w konsoli zgodnie z następującym przykładem:

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

10. Napisz skrypt, który zliczy i wypisze sumę znaków we wszystkich plikach podanych jako argumenty wywołania. 

11. Napisz skrypt, który utworzy w katalogu domowym folder *pictures_backup* i skopiuje do niego wszystkie pliki z rozszerzeniem *jpg* znajdujące się w bieżącym katalogu, a następnie zmieni nowym plikom prawa dostępu na tylko do odczytu.

12. Napisz skrypt, który przydzieli plikom odpowiednie prawa dostępu na podstawie par argumentów *plik* - *prawa dostępu w notacji numerycznej*. Przykładowo:

```bash
./task_6.sh plik1.txt 467 plik2.txt 777 plik3.txt 577
```

13. Napisz skrypt, który dopisze tekst określony jako pierwszy argument wywołania na końcu wszystkich plików z rozszerzeniem zdefiniowanym jako drugi argument i znajdujących się w bieżącym katalogu.

14. Napisz skrypt, który zsumuje wielkości plików w bieżącym katalogu dla każdego rozszerzenia podanego jako argument.
Przykładowo:

```bash
./task_8.sh txt jpg rar
txt: 173
jpg: 3748909
rar: 0
```

15. Napisz skrypt, który wyświetli pierwszą linię od końca z pliku podanego jako pierwszy argument, drugą linię od końca z pliku podanego jako drugi argument itd.

    *Wersja rozszerzona:*
    
    Jeżeli dany plik jest zbyt krótki, wyświetl stosowny komunikat.

16. Napisz skrypt przyjmujący dwa argumenty - dwie nazwy plikow, który porówna zawartość tych dwóch plików tekstowych. Jeżeli zawartość obu plików jest jednakowa skrypt powinien wypisać wiadomość: 
`pliki jednakowe`
Jeżeli pliki są różne, skrypt powinien wypisać który z nich ma więcej linii, np.:
`plik1.txt ma więcej linii niż plik2.txt`\
Do porównania zawartości plików możesz wykorzystać komendę
`diff plik1.txt plik2.txt`

17. Stwórz w pustym katalogu 3 pliki: plik1.txt, plik2.txt oraz plik3.txt. Umieść w każdym kilka słów. 
Napisz skrypt, który zamieni nazwy wszystkich plików w tym katalogu na liczbę znaków w danym pliku.


18. Napisz skrypt, który w pętli wczytuje z klawiatury numer (PID) procesu, numer sygnału a następnie wysyła wskazany sygnał do określonego procesu. Wpisanie słowa `EXIT` kończy pracę skryptu.

19. Stwórz kilka folderów i podfolderów, np.
```
mkdir -p studia/semestr6/scr/zadania && mkdir -p studia/semestr6/scr/kolokwium && mkdir -p studia/semestr6/abc && mkdir -p studia/semestr5/xyz && mkdir -p studia/semestr5/prs && touch studia/semestr6/scr/ocena.txt
```
Napisz skrypt zmieniający rekursywnie nazwę każdego katalogu (pomijając pliki!) na wielkie litery.

Wskazówki:
- aby zmienić wielkość znaków w zmiennej możesz wykorzystać znacznik ^^ po nazwie zmiennej, jeżeli jest umieszczona w klamrach, np.:
```
ZMIENNA="hello"
echo $ZMIENNA
echo ${ZMIENNA^^}
```
- skrypt może uruchamiać samego siebie rekursywnie

20. UUID (universally unique identifier) lub GUID jest to identyfikator globalnie unikatowy – identyfikator obiektów między innymi w systemie Windows lub wszędzie, gdzie potrzebny jest unikatowy identyfikator [https://pl.wikipedia.org/wiki/Globally_Unique_Identifier]

Napisz skrypt który z pomocą polecenia uuid wygeneruje 10 unikalnych identyfikatorów, a następnie je posortuje i umieści w pliku id.txt, każdy identyfikator w osobnej linii.

Czy potrzebne jest użycie komendy `uniq`, w celu wykluczenie powtórzeń?

Jeżeli po wpisaniu komendy uuid w konsoli otrzymamy informację: `Command 'uuid' not found`
należy program zainstalować. Instalacja odbywa się przez wywołanie komendy (komenda będzie wymagać hasła): `sudo apt install uuid`


21. Napisz skrypt, który zapyta użytkownika o numer (indeks) wyrazu z ciągu Fibonacciego i zapisze tą wartość do zmiennej. W skrypcie dodaj funkcję, która korzystając z rekurencji obliczy podany wyraz ciągu. Wyświetl obliczoną wartość w terminalu.
