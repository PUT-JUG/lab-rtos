# Filters, streams and pipe processing

## Standard input/output
By default, each process uses standard data streams that are an abstraction of the data source or sink. For each process, the system creates a standard input stream (*standard input*), representing input devices such as the keyboard and disk, a standard output stream (*standard output*), which can be a terminal (computer monitor) or file, and a diagnostic stream (*standard error*). Input, output and diagnostic streams are marked accordingly: `stdin`, `stdout`, `stderr`. Each stream has corresponding values: `stdin` - 0, `stdout` - 1 and `stderr` - 2.

![Standard process input-output](../images/lab_04_proc_std.png)

Standard process flows are characterised by the following features:

* The data is read through the standard input.
* The data is written out on the standard output or standard diagnostic input.
* Once read, the data cannot be read again.
  
The operation of the standard streams is illustrated by the `cat` program. Starting this program without arguments rewrites what will be typed from the keyboard to the screen. This command can be terminated with *Ctrl-D* (represented in lists as `^D`).

## Process input/output redirection
It is possible to readdress output and input streams. The standard input, output and diagnostic output can be changed by operators: `>`, `<`, `>>`, `<<`.

The operator `>` readdresses the standard output, i.e., creates a file and saves in it what the process would write on the standard output. If the specified file already exists, it will be deleted and re-created.

```bash
cat > file.txt
This is a file.
Ala has a cat.
^D
```

The operator `<` causes the standard process input to be re-addressed, i.e. the input data is retrieved from the specified file:

```bash
cat < file.txt
This is a file.
Ala has a cat.
```

The `>` and `<` operators can be used simultaneously by re-addressing both the output and input, causing the contents of the `file.txt` file to be copied to the `file_new.txt` file:

```bash
cat < file.txt > file_new.txt
```

The operator `>>` readdresses the standard output, adding the results of the program to the end of the existing file:

```bash
cat >> file.txt
The cat has Ala.
^D
```

The operator `<<<` causes the process to transfer data from the standard input until the indicated text string is encountered:

```bash
cat << break
> Ala has a cat
> The cat has Ala.
> break
Ala has a cat
The cat has Ale.
```

Some commands, in parallel with the information displayed on the standard output, send additional information about processing errors to the standard diagnostic output. The diagnostic stream can be independently redirected through the operator `>` preceded by the number of the diagnostic output, i.e. 2:

```bash
cat file1.txt file2.txt 2> file3.err
```

This command will display the contents of the `file1.txt` and `file2.txt` files and save error information to the file `file3.err`.

To skip error messages, the diagnostic output can be redirected to the `/dev/null` file. Everything that is sent to the `null` file in the `/dev` directory is lost:

```bash
cat file1.txt file2.txt> file3.txt 2> /dev/null
```

This command will save the contents of `file1.txt` file and `file2.txt` file to `file3.txt` and at the same time ignore error messages.

If you want the diagnostic stream to go to the same place as output stream, use `2>&1`:

```bash
cat file1.txt file2.txt> file3.txt file 2>&1
```

Many console programs using streams can also take the file name as an input: the `cat < file.txt` command will generate the same effect as the `cat file.txt`. In addition, many programs accept the `-` argument as a file name, which, depending on the context, means a standard input or output.

## Pipeline processing
The standard output of one process can be connected to the standard input of another process, creating a so-called stream between these processes.

![Standard process input-output](../images/lab_04_stream.png)

Pipeline processing is based on caching the data produced by the first process by the system and then reading the data by the second process. In other words, the process in the pipeline reads data from the input that was redirected from the output of the previous process. Several processes may be involved in a pipeline at the same time. The following are examples of streams:

The `ls` process gives the result to the `more `process, which in effect displays listing page by page:
```bash
ls -al|more
```

The `who` process gives its results to the sorting process, giving a sorted list of employees working in the system:
```bash
who -a | sort
```

The `ps` process gives the result to the `grep` process, searching for lines containing the word `csh` in the list of processes:

```bash
ps -ef | grep csh
```

The `ls` process gives the result to the `sort` process, which then gives the result to the `head` process, displaying the first 10 smallest files:
```bash
ls -l /usr/bin | sort -bnr +4 -5 | head
```
## Filters
There are programs whose task is to read data from the standard input, process these data and write them to the standard output. Such programs are called filters and are widely used in pipeline processing. The most commonly used filters are listed below:

* `cat` - the simplest filter that does not change the data being processed. Useful switches:
  * `-s` makes empty line from a few ones
  * `-n` numbers all lines
  * `-b` numbers non-empty lines
  * `-A` shows special characters
* `head` - displays the initial part of the file with the specified name or input data received from the pipe when the file name is not specified. By default, the first 10 lines of read data are displayed. This number can be changed using the switches:
  * `-c` allows you to specify the number of characters to be displayed
  * `-n` allows you to specify the number of lines displayed
* `tail` - displays last lines of file with the specified name or input received from the pipe when the file name is not specified. The last 10 data lines are displayed by default. You can change this number using the switches:
  * `-c` allows you to specify the number of characters to be displayed
  * `-n` allows you to specify the number of lines displayed
  * `-f` displays data on the standard output even when stored in a file.
* `sort` - is used to sort the input data, which by default are sorted by lexicography. Data sorting is done by lines. The most important switches:
  * `-n` allows you to sort numerically
  * `-b` ignores spaces at the beginning of the line when sorting
  * `-t` allows you to change the default column separator, which is a tab or space character.
  * `-f` allows you to ignore letter size when sorting
  * `-r` reverses the sorting order
  * `+number` allows sorting by any column (not the beginning of the line). The number specifies the number of columns to omitt during sorting
  * `-o file_name` saves the sorting result to a file with the specified filename.
* `uniq` - Deletes repeating adjacent input lines.
  * `-d` displays only repeating lines from the input data
  * `-u` displays only unique lines from the input data
  * `-c` counts the number of repetitions
* `wc` counts the characters, words and lines in the input data provided. By default, all three values are displayed, but this can be changed:
  * `-l` counts lines
  * `-w` counts only words
  * `-c` counts only characters
* `tr` - allows you to replace text strings that are given as input arguments. Characters from the first string are replaced by characters from the second string. Additionally, the following processing is possible thanks to the switches:
  * `-d` removes the characters specified after the switch
  * `-s` removes repetitive adjacent characters


TU KONIEC


* `cut` - displays line segments of the input data. Usually this is cutting out the appropriate columns
  * `c` allows you to specify the character position of the cut-outs of lines, e.g. `c 1-72` displays the first 72 characters of each line.
  * `-f` allows you to specify the numbers of columns to be cut, e.g. `-f1,3-5,10` displays the first column, columns from 3 to 5 and column 10.
  * The `-d` allows you to change the default column separator, which is the tab character
* `grep [options] expression [file_list]` - searches the data from the standard input or files listed in the file list, printing only the lines containing the search term. The search term is written using a regular expression. The most important switches:
  * `-v` searches for lines that do not contain the pattern you are looking for
  * `c` gives the number of words found
  * `-i` ignores letter size when searching for
  * `-n` displays the line numbers of lines containing the pattern
  * `-h`when displaying lines containing the searched pattern ignores file names
  * `-r` allows recursive searches, e.g. grep pattern -r directory
  * `-l` shows the names of files containing a specified pattern
  * `-L` shows the names of files that do not contain a specific pattern

The principles of construction of basic regular expressions describing the pattern to be searched for are as follows:
* `.` - represents any character
* `[abc]` - means one of the characters a, b or c
* `[a-z]` - means one of the characters from the specified set
* `[^0-9]` - means completion of the specified harvest
* `.*` - stands for any character string
* `*` - represents the repetition of any number of times an expression on the left-hand side, e.g. `A[a]*` specifies A, Aa, Aaaaaaa, Aaaaaaaaa, etc.
* ^^` - represents the beginning of the line
* `$` - represents the end of the line
* `Association `Association} - indicates a n-fold occurrence of a character appearing directly on the left side of the parentheses.
* `Association{n, {Name}} - indicates at least one occurrence of a character directly to the left side of the parentheses.
* `Association `, Mixture}` - means that the sign on the left side of the parentheses should not appear more than once.
* `Association{n,mœucces}` - means at least n times and at most m times the character on the left side of the parentheses.

In addition, there is a special group of characters with special meaning. These characters include: `. * {} () ^ [ ] \ < > $`. In order to use these characters as normal characters, they must be preceded by the ``Realignment` character. In addition to the basic regular expressions, there are extended expressions that allow you to describe the phrase you are looking for in a shorter way and offer richer ways to describe expressions. To use extended regular expressions, use the grep command with the `-E` switch, or use the `egrep` command.

Translated with www.DeepL.com/Translator

## Zadania do samodzielnego wykonania
1. Wyświetl plik */etc/passwd* z podziałem na strony przyjmując, że strona ma 5 linii tekstu. W oknie terminala wyświetlaj w danej chwili tylko jedną stronę tekstu.
2. Korzystając z polecenia `cat` utwórz plik *tekst3*, który będzie składał się z zawartości pliku *tekst1*, ciągu znaków podanego ze standardowego wejścia (klawiatury) i pliku *tekst2*.
3. Wyświetl po 5 pierwszych linii wszystkich plików w swoim katalogu domowym w taki sposób, aby nie były wyświetlane ich nazwy.
4. Wyświetl linie o numerach 3, 4 i 5 z pliku */etc/passwd*
5. Wyświetl linie o numerach 5, 6 i 7 od końca pliku */etc/passwd*
6. Wyświetl zawartość */etc/passwd* w jednej linii
7. Za pomocą filtru `tr` wykonaj modyfikację pliku, polegającą na umieszczeniu każdego słowa w osobnej linii.
8. Zlicz wszystkie pliki znajdujące się w katalogu */etc* i jego podkatalogach
9. Napisać polecenie zliczające sumę znaków z pierwszych trzech linii pliku */etc/passwd*
10. Wyświetl listę plików z aktualnego katalogu, zamieniając wszystkie małe litery na duże.
11. Wyświetl listę praw dostępu do plików w aktualnym katalogu, ich rozmiar i nazwę
12. Wyświetl listę plików w aktualnym katalogu, posortowaną według rozmiaru pliku
13. Wyświetl zawartość pliku */etc/passwd* posortowaną wg numerów *UID* w kolejności od największego do najmniejszego
14. Wyświetl zawartość pliku */etc/passwd* posortowaną najpierw wg numerów *GID* w kolejności od największego do najmniejszego, a następnie *UID*
15. Podaj liczbę plików każdego użytkownika
16. Sporządź statystykę praw dostępu (dla każdego z praw dostępu podaj ile razy zostało ono przydzielone)
17. Podaj nazwy trzech najmniejszych plików w katalogu posortowane wg nazwy
18. Podaj pięciu użytkowników o największej liczbie uruchomionych procesów
19. Wyświetl zawartość 3 największych podkatalogów katalogu bieżącego
20. Wyświetl nazwy tych użytkowników, którzy domyślnie używają innego interpretera niż *bash*
21. Wyświetl nazwy wszystkich plików nagłówkowych posortowane wykorzystywanych w plikach bieżącego katalogu
22. Wyświetl statystykę używanych komend (bez argumentów) w postaci posortowanej listy: ilość komenda ( wskazówka: należy użyć polecenia *history*)
