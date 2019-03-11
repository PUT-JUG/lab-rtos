# Processes in Linux

## The concept of process
Every program running on Unix is called a process. The process consists of the following elements:

* The binary code of the process loaded from the file.
* Program data: data structures declared in the program and memory dynamically assigned to the process during its operation.
* System data: information about the process maintained by the system.
  
Additionally, when creating a process, the system initiates system data structures describing the process, which are then updated during the process execution. System data includes:

* Process identifier (PID) - a unique integer unambiguously identifying the process.
* Home process identifier (PPID) - the value of the PID of the process that created the process.
* Process environment - a set of environmental variables. Each process has its own, independent from other processes execution environment, which is initially copied from the parent process and then is modified independently from other processes.
* Standard data streams.
  
In addition to processes, in the Unix system (as in most modern systems) there are threads, which are the smallest active elements of the system. A thread is a kind of process that divides the address space with a different process - each thread is assigned with an independent identifier.

## Processes in the system
The process list for the current shell is obtained by invoking the `ps` (processes) command.

```bash
ps
PID             TTY             TIME		CMD
14285           pts/0           0:00            -csh
14286           pts/0           0:00	        ps
^^^^^^          ^^^^^^          ^^^^^           ^^^^^
process number  terminal        activity time   name
```

For complete information on current shell processes, see the `ps -f` (*full*) command.

```bash
ps -f
UID           PID        PPID     C    STIME        TTY         TIME        CMD
student       1633       1625     0    06:00        pts/0       00:00:00    bash
student       2666       1633     0    06:14        pts/0       00:00:00    nano plik.txt
student       2855       1633     0    11:26        pts/0       00:00:00    ps -f
^^^^^^        ^^^^^      ^^^^^^^^      ^^^^^^^      ^^^^^^      ^^^^^       ^^^^^
owner         process    parent        startup      terminal    time        process name
              ID         process ID    time
```

Full information about all processes can be obtained by combining the -`f` option with `-e` (*every process*). Some of the useful switches for the `ps` command are:
* `-a` (*all*) displays a list of all processes (including those of other users). 
* `-l` (*long*) allows you to display additional information about each process 
* `-x` (*long*) includes information about processes not attached to terminals (daemons processes) 
* `-u` adds a username at the beginning of the list 
* `-in` expands the displayed list
* `-l` allows more process details to be displayed. As a result, additional columns containing the following information are displayed: 
   * `S` - status of the process ( `S` - the process is asleep; `R` - the process is currently being executed, `T` - stopped) 
   * `UID` - Identifier of the process owner 
   * `PPID` - master process identifier 
   * `PRI` and `NI` - describe the priorities of the processes 
   * `WCHAN` - allows you to check which system function of the kernel has been triggered by the process

Many of the processes are started at system startup, the others are activated by the users at the moment of a command call. Any process can start another child process and become the parent (superior) of this child process. When a user signs in to the system, his first process is started, i.e. the shell interpreting the user's commands.

All processes working in the system form a hierarchical structure, at the top of which stands the `init` process, which is the parent of all processes. The `init` process always has a `PID` value of 1. The hierarchy of processes can be observed using the `pstree` program. There is also an interactive version of the `ps` command - the `top` program, where, in addition to the data returned by the `ps` command, other information is displayed: the current number of users in the system and the average load (line one), the number of processes and their states (line two), the CPU load (line three) and information about the available memory in the system (lines four and five).

## Deleting processes
Any process can be removed from the system by its owner. It can be done with a `kill` command, which sends an interrupt signal to a process with a given identifier:

```bash
kill [ -name_or_signal_number ] process_identifier
```

Signals have numeric identifiers assigned to them. The identifier of the `TERM` signal is 2 and the `KILL` signal is 9. Numeric values of the signals can also be used in the `kill` command:

```bash
kill -9 process_identifier
```

Stopping all processes with a given name can be done with the `killall` command. For example:

```bash
killall find
```
stops all instances of `find` programs.

Refer to the *signal(7)* system help page (command `man 7 signal`) for a detailed list of signals with their numerical values. An abbreviated list of available signals can be obtained by calling `kill -l`. 

## Process priorities
Each process executed in the system has a priority assigned to it, which can be read by calling the `ps` command with the `-l` switch.

The `PRI` column displayed as a result of this command contains information about the priority value of a specific process assigned by the operating system. This value cannot be changed directly by the user. However, the user can influence the `PRI` value by decreasing the so-called *nice* value. The current *nice* value is displayed in the `NI` column and belongs to the range from -20 to 19. Initially, it is 0. The smaller the *nice* value, the higher the priority of the process. For a running process, the *nice* value can be changed with a command:

```bash
renice change_in_priority [ -p ] pid [ -u user ]
```
For example:
```bash
renice +10 3442
```
increases the *nice* value by 10, which decreases the priority of this task. Normal users can only increase the number of *nice*, i.e. decrease the priority of their tasks, while the *root* user is entitled to make any changes to the *nice* values.

It is possible to run new processes with a new priority already set:

```bash
nice -n changed_priority command
```

# Process management

Processes run from the terminal keyboard are called foreground processes. The shell waits for the completion of the process and only then it is ready to accept further commands from the user.

However, the process can be started in the background. The shell will create a new child process, which is a shell that executes the specified command. Then, the parent shell will be ready for further commands. As a result, the process that has been started in the background starts to work in parallel with the parent terminal.

It is worth noting that working in the background makes sense only in the case of non-interactive programs, i.e. those that do not need user interaction for their work. If the program is started in the background, the command interpreter immediately switches to waiting for the next command (it starts reading the data from the keyboard). Similarly, programs running in the background should not write out information on the screen, because it will be printed asynchronously to the currently performed operations. In this case, the solution to this problem may be to redirect the output of such a program to a file for the further analysis.

The command is run in the background if the last parameter is followed by &:

```bash
command &
```

The currently running process can also be stopped by pressing the Ctrl - Z combination. This will stop the process. The stopped process exists in the system, but no processor is assigned to it. A stopped process can be resumed (continued) in the background with the `bg` command (*background*), or even restored back to the foreground with the `fg` command (*foreground*) at any time, but only if we did not run another process in the background between these commands. You can display a list of currently controlled tasks with the `jobs` command.

If more than one task has been suspended, you will need to identify them. The command interpreter assigns its identifiers internally and the `jobs` command can display their values. A specific process can be referenced using an identifier preceded by a `` character.

```bash
jobs
[1]- Stopped vim file.html
[2]+ Stopped find /usr -name signal.h
fg 1
```

## Process completion status
Each process in the Unix system, after completing its work, provides the system with information on how the processing was completed, determined by the status of the process completion. The termination status is a one-byte number, but it is assumed that the value of 0 means correct completion of the processing. Values other than 0 indicate an error. The exit status of the most recently executed program can be obtained in the following way:

```bash
echo $?
```
The exit status of the process can be used to run conditional commands. In order to execute `command_2` only if `command_1` is successful is written as follows:
```bash
command_1 && command_2 
```
On the other hand, in this case `command_2` can only be executed if the `command_1` fails:
```bash
command_1 || command_2
```
In addition, in the UNIX system we can run several processes with one command, separating each of them with a semicolon:
```bash
command_1; command_2; command_3
```
This sequence can also be pushed to the background:
```bash
(command_1; command_2; command_3) &
```

## Exercises
1. Display a list of your own processes with the `ps` command. Compare the results with the results of commands `ps x` and `ps ax`. Examine the operation of the `l` and `u` switches. 
2. Log in to the system several times through virtual consoles or by opening a new terminal in the graphical environment. Each time you use the `tty` command to check the name of the terminal you are working on.
3. Find the home process for the `ps` process. Find the ancestor of all processes (`PID=1`). Display the process hierarchy with the `pstree` command. 
4. View the list of processes with the `top` command, sorting them by CPU usage and memory usage.
5. Check the identifier of the `init` process. 
6. Read the list of signals on the system manual page `man 7 signal`. 
7. Start a session of the `vi` editor and then, from the second terminal, send a series of signals to the process with the `kill` command. Use signals: `HUP`, `INT`, `TERM`, `QUIT`, `KILL`, `STOP`, `CONT`.
8. Run several `sleep` processes in the background. Switch one of them to foreground mode. Start and pause a session of the `vi` editor with Ctrl-Z, start a new session and pause it too. Display active sessions with the `jobs` command. Resume one of the `sleep` processes to the foreground mode with the `fg` command, then switch it back to the background with the `bg` command.

