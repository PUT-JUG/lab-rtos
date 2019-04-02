# Bash shell scripts

## Command interpreter and environmental variables

A system shell is a computer program that acts as an intermediary between the operating system or applications and the user, accepting their commands and displaying the results of the programs. In Linux systems (including Ubuntu), the most popular shell is `bash` (*Bourne Again Shell*) - further examples will be presented for the `bash` shell.

During the previous classes, each time we opened the terminal emulator, a new instance of our user's shell - the `bash` program - was launched. Some commands are so-called builtin commands (directly into the shell) - e.g. `cd`, `pwd`, others are completely separate programs  called by `bash` shell - e.g. `ls`, `chmod` and so on.

*Environmental variables* are a very convenient and universal way to configure and parameterize system shells and - with their help - other programs as well. The available set ofvariables form the so-called *Process Execution Environment* - this environment is copied to all new processes, so modifications to variables made in the shell are visible in all programs running inside this shell. Each user can define any number of his own variables and assign any values to them. To define an environmental variable, use the assignment operator (the "=" character) as follows:

```bash
VARIABLE1=value
```

In this case, the character string `VARIABLE1` is the name of the variable, and the `value` is its value - note that there must be no space between the variable name, the assigning operator and the value. The value of a variable can be referenced with a special character `$`; for example, it is possible to display the value of a variable on the screen using the `echo` command, which displays the line of text and the value of a variable retrieved with the `$` character:

```bash
SYSTEM=Unix
echo $SYSTEM
```
Result:
```bash
Unix
```

The system command `set` allows you to display the values of all environmental variables, and the command
`unset` deletes an environmental variable, here is an example:

```bash
unset SYSTEM
```

As mentioned above, the process execution environment is passed on to child processes - however, note every variable is passed. The variables that are passed on are called export variables and the variables that are not passed on are called local variables. As a rule, newly created variables are initially local variables and it is necessary to indicate explicitly that they are to be exported. This is called exporting variables and is done with the command:

```bash
export variable_list
```

Here is an example of creating a variable and exporting it:

```bash
SYSTEM=Unix
export SYSTEM
```

The above commands can also be executed with one command:

```bash
export SYSTEM=Unix
```

TO display exported environmental variables use the `env` command.

Each user can easily (e.g. with the `set` command) verify that multiple environmental variables are defined by default in the system. Here are some of the basic ones:

 * `HOME` - the path and name of the user's home directory;
 * `USER` - the name of the logged in user;
 * `PATH` - program search paths;
 * `PS1` - the form of a user prompt;
 * `SHELL` - the full path to the user's default command interpreter.
 
 System variables dont't have a type - all of them are stored as a string, regardless of content.
 
 ## Exercises
 1. define the variable `NAME` and assign a name to it. View the contents of this variable. Export this variable and check if it is available in a new (child) interpreter.
 2. Display a list of exported variables.
 3. Change your own prompt by modifying the variable `PS1`.


## Scripts and arguments
Shells also allow you to create programming structures such as conditional instructions or loops. Combination of environmental variables and commands put into text files allow us to write programs (scripts) in the `bash` language.

Scripts are a very helpful solution when there is a need to execute complex commands or commands that are repeated number of times.

Scripts can be parameterized with invocation arguments - this means that data or parameters can be passed on to the script when it is launched. Call arguments can be referenced in scripts by means of so-called positional parametres, which are marked: *1*, *2*, *3*, ..., *9*. Each positional variable stores the text passed through the corresponding call argument - the first argument is available in the positional variable *1*, and so on. Here is an example of the first script that displays the values of the first three call arguments:

```bash
#!/bin/bash
# first script
echo "argument no. 1: $1".
echo "argument no. 2: $2".
echo "argument no. 3: $3".
```

Such commands should be saved to any file (it is recommended that script files have the `.sh` extension). The first line, beginning with `#!`, allows you to indicate which command interpreter will be used - in this case, it is the `bash` shell. The second line represents the way comments are placed: each line beginning with the `#` character is treated as a comment. The next three lines display the values of the first three arguments to invoke the script using the `echo` command. **In order to run the script, it is necessary to grant the execution rights to the file in which it is saved.** Below is an example of the above script call and its result (it is assumed that the script file is called `script1.sh`):

```bash
./script1.sh abc xyz 12345
```
Output:
```
argument no. 1: abc
argument no. 2: xyz
argument no. 3: 12345
```

## Conditional instructions
Currently, system shells allow scripts to contain structures controlling their execution - the basic instruction in this case is a conditional instruction. The syntax  is as follows:

```bash
if condition
then
    instructions to be followed if the condition is met
else
    instructions to be followed if the condition is not met
fi
```

The condition can be any command - the condition is fulfilled if the program doesn't return an error (it had an output code equal to 0). Example shows using the `ping` program to check communication with the server:

```bash
if ping -c 1 google.com
then
    echo "Gooogle alive"
else
    echo "Google went down"
fi
```

Most conditions are constructed using the built-in `test` program and notation using `[` and `]` characters to perform condition tests, as shown in the following example:

```bash
if [ $1 = xyz ]
then
    echo 'argument 1 = xyz'.
else
    echo 'argument 1 is not xyz'.
fi
```

After the `[` and before the `]` character it is necessary to enter a space character. Verifiable conditions using the `test` program are shown in the table:


| Condition | Description |
|------------------------------------------|--------------|
| `string1` = `string2` | Verification of the equality of the two strings |
| `string1` != `string2` | Verification of the inequality of the two strings |
| `-z string` | Verify that the character string has zero length |
| `-n string` | Verification that the character string has a non-zero length |
| `number1 -eq number2` | Verification if two numbers are equal |
| `number1 -ne number2` | Verification if two numbers are not equal|
| `number1 -gt number2` | Verification whether `number1` is greater than `number2`.|
| `number1 -lt number2` | Verification whether `number1` is smaller than `number2`.|
| `-e name` | Verify if the specified file exists |
| `-f name` | Verify if the file exists and is an ordinary file |
| `-d name` | Verify if the file exists and is the directory |
| `-r name` | Verification whether the user has the right to read the file with the given name |
| `-w name` | Verify that the user has the write right for the file with the given name |
| `-x name` | Verify that the user has the right to execute for the file with the given name |
| `condition1 -a condition2` | Logical product of conditions |
| `condition 1 -o condition 2` | Logical sum of conditions |
| `! condition1` | Negation of condition |



## Pętle
Skrypty powłoki mogą także zawierać pętle - podstawowe dwie z nich to pętla `for` oraz `while`. Pętla `for` wykonywana jest z góry określoną ilość razy, a jej ogólna składnia jest następująca:

```bash
for zmienna in lista
do
    instrukcje do wykonania
done
```

Wykonanie pętli powoduje przypisywanie zmiennej zmienna kolejnych wartości wymienionych na liście lista; ilość iteracji, jest zatem zależna od długości podanej listy. Jako listę można w pętli for można podawać wzorce uogólniające powłoki. Poniższy przykład skryptu prezentuje zastosowanie pętli for do usunięcia wszystkich plików z rozszerzeniem `.tmp` z katalogu bieżącego:

```bash
#!/bin/bash
for FILE in *.tmp
do
    rm -v $FILE
done
```

Ilość iteracji powyższej pętli bezie zatem determinowana ilością plików z rozszerzeniem `*.tmp`, które utworzą listę wartości dla zmiennej `FILE`.

Realizacja pętli numerycznej z zastosowaniem pętli `for` jest możliwa z użyciem programu zakresów definiowanych operatorem nawiasów klamrowych `{start..stop}`, np.:

```bash
for N in {1..10}
do
    echo $N
done
```
spowoduje dziesięciokrotne wykonanie pętli.

Pętla `while` pozawala na realizację pętli, dla których ilość iteracji nie jest znana z góry, jej składnia dla skryptów powłoki jest następująca:

```bash
while warunek
do
   instrukcje do wykonania
done
```

Warunek może być dowolnym poleceniem i najczęściej jest konstruowany - tak jak w przypadku instrukcji warunkowej - z zastosowaniem programu `test`.

Przykładem zastosowania pętli `while` może być skrypt wypisujący na ekranie wartości wszystkich argumentów wywołania skryptu (niezależnie od ich liczby). Pętla taka będzie wykorzystywała polecenie `shift`, które powoduje przesunięcie argumentów - oto skrypt:

```bash
#!/bin/bash
while [ -n "$1" ]
do
   echo $1
   shift
done
```

Warunkiem wykonania pętli jest sprawdzenie, czy pierwszy argument wywołania skryptu ma niezerową długość - jeśli skrypt został uruchomiony bez żadnych argumentów, to pętla nie zostanie wykonana. Jeśli natomiast skrypt został wykonany z argumentami, to w pierwszym wykonaniu pętli zostanie wyświetlona wartość pierwszego argumentu, a następnie nastąpi przesunięcie argumentów w lewo poleceniem `shift` (drugi argument stanie się pierwszym, trzeci drugim itd.). Pętla zakończy się jeśli zostaną wyświetlone i przesunięte wszystkie argumenty (zmienna pozycyjna `$1` będzie miała wówczas zerową długość).

Obie zaprezentowane pętle mogą zostać przerwane poleceniem `break` - oto przykład zastosowania przerywania pętli:

```bash
#!/bin/bash
for FILE in *.tmp
do
   if [ ! -f $FILE ]
   then
      echo "$FILE nie jest plikiem!"
      break
   fi
   rm -v $FILE
done
```

Jak widać pętla zostanie przerwana, jeśli pobrana nazwa z rozszerzeniem `*.tmp` nie będzie wskazywała na plik zwykły.

Wykonywanie całego skryptu można przerwać poleceniem `exit`.

## Pobieranie wartości do skryptów
Jeśli skrypt wymaga iteracji z użytkownikiem, to niezbędne staje się pobieranie wartości przekazywanych przez użytkownika. Służy do tego polecenie:

```bash
read [argumenty]
```

Argumentami są nazwy zmiennych środowiskowych, które przyjmą wartość odczytana ze standardowego wejścia (do napotkania znaku nowej linii). Jeśli jako argumenty podano kilka zmiennych, to są one inicjowane w ten sposób, że pierwsze słowo trafia do pierwszej zmiennej, drugie do drugiej itd. Polecenie to można przetestować wykonując następujące polecenia:

```bash
read X Y
echo $X
echo $Y
```
Wejście:
```
uzytkownik adam
```
Wyjście:
```
uzytkownik
adam
```

Polecenie `read` może również współpracować z pętlą `while` i potokiem, co pozwala na przetwarzanie wejściowego strumienia tekstu linia po linii:

```bash
cat file.txt | while read line
do
    echo Linia tekstu: $line
done
```

Ponieważ znak spacji jest w powłoce separatorem argumentów, problematyczne mogą stać się zawartości zmiennych bądź nazwy plików zawierające spacje. Umieszczenie tekstu w podwójnym cudzysłowie (`"`) spowoduje, że cała jego zawartość, wraz ze spacjami zostanie potraktowana jako jeden argument.

Przykładowo, przypisanie do zmiennej napisu zawierającego spacje:
```bash
LISTA="abc 123 456"
```

Umieszczenie napisu w pojedynczym cudzysłowie (`'`) spowoduje, że jego zawartość nie będzie interpretowana w żaden sposób (np. znaki `$` nie będą oznaczały odwołań do zmiennych).

Poniższe trzy pętle generują zatem różne rezultaty:

```bash
for ZMIENNA in $LISTA ; do
    echo $ZMIENNA
done
```

```bash
for ZMIENNA in "$LISTA" ; do
    echo $ZMIENNA
done
```

```bash
for ZMIENNA in '$LISTA' ; do
    echo $ZMIENNA
done
```

Jeśli chcemy natychmiast po zawartości zmiennej dokleić dodatkowy tekst, można ująć nazwę zmiennej w klamry, np:

```bash
echo ${A}hello
```

Umieszczenie tekstu w *grawisie* (`` ` ``) powoduje **uruchomienie** zawartego w nim tekstu, a następnie podstawienie w dane miejsce wyjścia uruchomionego programu. Przykładowo, zapisanie bieżącego katalogu roboczego do zmiennej:

```bash
CURRENT_DIR=`pwd`
```

Bash oferuje również prostą arytmetykę na liczbach całkowitych. Kontekst arytmetyczny wywołujemy umieszczając formułę pomiędzy `$((` a `))`:

```bash
a=3
b=$((a+5))
```

## Debugowanie skryptów

Skrypty mogą być także wykonywane w trybie debugowania, np. w celu testowania poprawności działania, warunków, pętli itp. Aby zrealizować wykonanie skryptu z wyświetlaniem informacji kontrolnych należy zastosować przełącznik `-x` wywołania interpretatora poleceń. Można to zrealizować na trzy sposoby:

* dopisać przełącznik w pierwszej linii skryptu:  `#!/bin/bash -x`
* uruchomić skrypt wywołując jawnie interpreter z parametrem `-x` oraz argumentem z nazwą skryptu, np .
```bash
bash -x nazwa_skryptu.sh
```
* wywołując wewnątrz skryptu polecenie `set -x`

Często ma miejsce sytuacja, kiedy niepowodzenie jakiegokolwiek z poleceń (np. utworzenie katalogu roboczego) powoduje, że dalsze operacje nie mają sensu. Przydatną opcją powłoki jest wtedy przełącznik `-e`, który powoduje, że powłoka (np. wykonująca skrypt) zostanie przerwana, w momencie kiedy którykolwiek z użytych w niej programów (poza warunkami w instrukcjach warunkowych/pętlach) zwróci błąd. Ustawienie przełącznika `-e` może odbyć się na sposoby analogiczne do opisanych powyżej. 

## Zadania do samodzielnego wykonania
1. Napisz skrypt, który dla każdego elementu (pliku, folderu) w bieżącym katalogu wyświetli jego nazwę wraz z informacją czy jest to plik czy katalog.
2. Napisz skrypt, który dla każdego z plików podanych jako argumenty wywołania wyświetli nazwę pliku, a następnie jego zawartość posortowaną alfabetycznie.
3. Napisz skrypt, który będzie kopiował plik podany jako pierwszy argument do wszystkich katalogów podanych jako kolejne argumenty wywołania.
4. Napisz skrypt, który wykona kopię zapasową plików podanych jako argumenty, do katalogu `backup` i dopisze do ich nazwy bieżącą datę:

Przykładowo:
```bash
./super_backup.sh notatki.txt zdjecia.tar.gz 
```
Skopiuje:

`notatki.txt` do `backup/notatki.txt_2019-02-29`

`zdjecia.tar.gz` do `backup/zdjecia.tar.gz_2019-02-29`

Jeśli folder `backup` nie istnieje, program powinien go utworzyć.
Jeśli plik docelowy już istnieje, program powinien wyświetlić stosowny komunikat i przerwać pracę.

Podpowiedź: bieżącą datę możesz uzyskać poleceniem `date '+%Y-%m-%d'`

8. Zmodyfikuj skrypt z zadania 7 tak, aby argumentem skryptu była nazwa pliku zawierającego (po jednym na linię) nazwy plików do zarchiwizowania.

9. Napisz skrypt, który będzie oczekiwał na pojawienie się pliku o nazwie wskazanej w argumencie. Skrypt powinien cyklicznie (co 5 sekund) sprawdzać istnienie pliku. Jeśli plik istnieje, skrypt powinien wyświetlić jego zawartość i zakończyć się. Uruchom skrypt, a z poziomu drugiego terminala utwórz monitorowany plik.

***
Autor: *Adam Bondyra*, *Jakub Tomczyński*

Data ostatniej modyfikacji:   *24-03-2019*

Opracowano na podstawie materiałów projektu *Otwartych Studiów Informatycznych (http://wazniak.mimuw.edu.pl/*).
