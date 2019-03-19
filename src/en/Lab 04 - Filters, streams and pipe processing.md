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
This is a second line of file.
^D
```

The operator `<` causes the standard process input to be re-addressed, i.e. the input data is retrieved from the specified file:

```bash
cat < file.txt
This is a file.
This is a second line of file.
```

The `>` and `<` operators can be used simultaneously by re-addressing both the output and input, causing the contents of the `file.txt` file to be copied to the `file_new.txt` file:

```bash
cat < file.txt > file_new.txt
```

The operator `>>` readdresses the standard output, adding the results of the program to the end of the existing file:

```bash
cat >> file.txt
This is an appended file line.
^D
```

The operator `<<` causes the process to transfer data from the standard input until the indicated text string is encountered:

```bash
cat << break
> This is first line.
> And this is second.
> break
This is first line.
And this is second.
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
* `cut` - cuts and displays line segments of the input data. Usually cutting is done according to the specifed columns.
  * `c` allows you to specify the character position of the cut-outs of lines, e.g. `c 1-72` cuts and displays the first 72 characters of each line.
  * `-f` allows you to specify the numbers of columns to be cut, e.g. `-f1,3-5,10` displays the first column, columns from 3 to 5 and column 10.
  * The `-d` allows you to change the default column separator, which is the tab character
* `grep [options] expression [file_list]` - searches the data from the standard input or files listed in the file list, printing only the lines containing the search term. The search term is written using a regular expression. The most important switches:
  * `-v` searches for lines that do not contain the pattern you are looking for
  * `-c` gives the number of words found
  * `-i` ignores letter sizes during search
  * `-n` displays the line numbers of lines containing the pattern
  * `-h` when displaying lines containing the searched pattern, ignores file names
  * `-r` allows recursive searches, e.g. `grep pattern -r directory`
  * `-l` shows the names of files containing a specified pattern
  * `-L` shows the names of files that do not contain a specific pattern

The principles of construction of basic regular expressions describing the pattern to be searched for are as follows:
* `.` - represents any character
* `[abc]` - means one of the characters a, b or c
* `[a-z]` - means one of the characters from the specified set
* `[^0-9]` - means completion of the specified set
* `.*` - stands for any character string
* `*` - represents the repetition of any number of times an expression on the left side, e.g. `A[a]*` specifies A, Aa, Aaaaaaa, Aaaaaaaaa, etc.
* `^` - represents the beginning of the line
* `$` - represents the end of the line
* `a\{n\}` - indicates a n-fold occurrence of a character appearing directly on the left side of the parentheses.
* `a\{n, \}` - indicates at least one occurrence of a character directly to the left side of the parentheses.
* `a\{, m\}` - means that the character on the left side of the parentheses should not appear more than m times.
* `a\{n,m\}` - means at least n times and at most m times occurence of the character to the left of the parentheses.

In addition, there is a special group of characters with special meaning. These characters include: `. * {} () ^ [ ] \ < > $`. In order to use these characters as normal characters, they must be preceded by the `\` character. In addition to the basic regular expressions, there are extended expressions that allow you to describe the phrase you are looking for in a shorter way and offer richer ways to describe expressions. To use extended regular expressions, use the grep command with the `-E` switch, or use the `egrep` command.


## Exercises
1. Display the */etc/passwd* file divided into pages, assuming that the page has 5 lines of text. In the terminal window, display only one page of text at a time.
2. Use the `cat` command to create a *text3* file that will consist of the contents of the *text1* file, then the text typed into the standard input (keyboard) and then the *text2* file.
3. Display the first 5 lines of all files in your home directory without displaying their file names.
4. Display lines 3, 4, and 5 from */etc/passwd*.
5. Display lines numbered 5, 6 and 7 from the end of the file */etc/passwd*.
6. Display the content of */etc/passwd* in a single line.
7. Use the `tr` filter to modify examplary file by placing each word on a separate line.
8. Count all files in the */etc* directory and its subdirectories.
9. Write a command that counts the sum of the characters from the first three lines of the file */etc/passwd*.
10. Print a list of files in the current directory, replacing all lowercase letters with uppercase letters.
11. Print a list containing file access rights, sizes and names of all files in the current directory.
12. Print a list of files in the current directory, sorted by file size.
13. Print the contents of the */etc/passwd* file sorted by *UID* numbers in order from the largest to the smallest.
14. Display the contents of the */etc/passwd* file sorted first by *GID* numbers in order from the largest to the smallest, then *UID*.
15. Print the names of the three smallest files in the directory, sorted by name.
16. The `/etc/services` file stores a list of popular network services, including port numbers and protocol. List (only) the names of services that use the `UDP` protocol.
17. Print the count of virtual terminals (`dev/tty`) with numbers in the range *50-69* available in the system.
18. Find the PID of the process **cupsd**.

***
Author: *Adam Bondyra*

Last revision:   *12-03-2019*

Based on the Open IT Studies project (*http://wazniak.mimuw.edu.pl/*).