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



## Loops
Shell scripts can also contain loops - two of them are the `for` and `while` loops. The `for` loop is executed a predetermined number of times, and its syntax is as follows:

```bash
for variable in list
do
    instructions
done
```

Execution of a loop causes the variable **variable** to be assigned with the successive values listed in the list; the number of iterations depends on the length of the given list. Shell generalizing patterns can be given as a list. The following script shows how to use the `for` loop to remove all files with the `.tmp` extension from the current directory:

```bash
#!/bin/bash
for FILE in *.tmp
while
    rm -v $FILE
done
```

The number of iterations of the above loop is therefore determined by the number of files with the extension `*.tmp`, which will create a list of values for the variable `FILE`.

The execution of a numerical loop using the `for` loop is possible using ranges defined by the `{start..stop}` bracket operator, e.g.:

```bash
for N in {1...10}
while
    echo $N
done
```
will cause the loop to be executed ten times.

The `while` loop allows loop execution for which the number of iterations is not known in advance, its syntax for shell scripts is as follows:

```bash
while condition
do
   instructions
done
```

A condition can be any command and is usually constructed (as in the case of a conditional instruction) with the `test` program.

An example of using the `while` loop is a script that prints on the screen the values of all the script's call arguments (regardless of their number). Such a loop will use the `shift` command, which causes the arguments to shift - here's the script:

```bash
#!/bin/bash
while [ -n "$1" ]
do
   echo $1
   shift
done
```

The condition for executing the loop is to check if the first argument of calling the script is non-zero length - if the script has been run without any arguments, the loop will not be executed. If the script is executed with arguments, the first execution of the loop displays the value of the first argument, and then the arguments are shifted left with the `shift` command (the second argument becomes the first, third argument becomes the second etc.). The loop will end if all arguments are displayed and shifted (the position variable `$1` will then be zero length).

Both presented loops can be interrupted by the `break` command - here is an example of loop interruption:

```bash
#!/bin/bash
for FILE in *.tmp
do
   if [ ! -f $FILE ]
   then
      echo "$FILE is not a file!
      break
   fi
   rm -v $FILE
done
```

As you can see, the loop will be aborted if the name with the `*.tmp extension does not point to an ordinary file.

You can interrupt the execution of the entire script with the `exit` command.


## Passing values to the script
If the script requires interaction with the user, it becomes necessary to retrieve the values given by the user. Here's a command to do this:

```bash
read [arguments]
```

The arguments are the names of environmental variables that will take the value read from the standard input (until a new line character is encountered). If several variables are given as arguments, they are initialized in such a way that the first word goes to the first variable, the second to the second, and so on. This command can be tested by executing the following commands:

```bash
read X Y
echo $X
echo $Y
```

Input:
```
user adam
```

Output:
```
user
Adam
```

The `read` command can also work with the `while` loop and stream, allowing the input stream of text to be processed line by line:

```bash
cat file.txt | while read line
do
    echo Text line: $line
done
```

Because the space character is an argument separator in the shell, the content of variables or file names containing spaces may become problematic. Placing text in double quotation marks (`"`) will result in the entire text, including spaces, being treated as a single argument.

For example, assigning a variable with a string containing spaces:
```bash
LIST="abc 123 456".
```

Placing a string in a single quotation mark (`'`) will cause its contents not to be interpreted in any way (e.g. the `$` characters will not refer to variables).

The following three loops generate different results:

```bash
for VARIABLE in $LIST ; do
    echo $VARIABLE
done
```

```bash
for VARIABLE in "$LIST" ; do
    echo $Change
done
```

```bash
for VARIABLE in '$LIST' ; do
    echo $Change
done
```

If you want to add additional text immediately after the variable content, you can include the name of the variable in brackets, e.g.:

```bash
echo ${A}hello
```

Placing text in *backquotes* (`` ` ``) causes the text contained in it to be launched and then substituted to the given exit point of the running program. For example, saving the current working directory to a variable:

```bash
CURRENT_DIR=`pwd`.
```

Bash also offers simple arithmetic on integers. The arithmetic context is invoked by placing the formula between `$((` and `))`:

```bash
a=3
b=$((a+5))
```

## Debugging scripts

Scripts can also be executed in debug mode, e.g. to test the correctness of operation, conditions, loops, etc. To execute a script with control information display, use the `-x` switch. This can be done in three ways:

* Add a switch in the first line of the script:  `#!/bin/bash -x`
* Run the script by explicitly calling the interpreter with the parameter `-x` and the argument with the script name, e.g. .
```bash
bash -x script.sh
```
* Invoke the `set -x` command inside the script

Quite often in case of the failure of a command (e.g. creating a working directory) makes no sense to continue operations. A useful shell option is then a `-e` switch that causes the shell (e.g., an executing script) to be aborted when any of the programs used in it (except for conditions in conditional instructions/loops) returns an error. The `-e` switch can be set in the same way as described above. 

## Exercises

4. Write a script that for each element (file, directory) in the current directory will display its name along with information whether it is a file or a directory.
5. Write a script that for each file given as a call argument will display the file name and then its contents sorted alphabetically.
6. Write a script that will copy the file given as the first argument to all directories given as next call arguments.
7. Write a script that backs up the files given as arguments to the `backup` directory and adds the current date to their names.

For example:
```bash
./super_backup.sh notes.txt photos.tar.gz 
```
Copies:

`notes.txt` to `backup/notes.txt_2019-02-29`

`photos.tar.gz` to  `backup/photos.tar.gz_2019-02-29`.

If the `backup` folder does not exist, the program should create it.
If the target file already exists, the program should display an appropriate message and stop working.

Hint: you can get the current date with the `date '+%Y-%m-%d'` command.

8. Write a script that waits for the file name indicated in the argument to appear. The script should check periodically (every 5 seconds) the existence of the file. If the file exists, the script should display its contents and finish. Run the script and create a monitored file from  the second terminal.


***
Author: *Adam Bondyra*

Last revision:   *02-04-2019*

Based on the Open IT Studies project (*http://wazniak.mimuw.edu.pl/*).