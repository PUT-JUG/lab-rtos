# Konfiguracja sieci, partycje, SSH

## Konfiguracja połączenia sieciowego host-guest

Domyślna konfiguracja sieci pod VirtualBox sprawdza się, kiedy potrzebny jest dostęp z poziomu maszyny wirtualnej do Internetu lub do sieci, do której podłączony jest host.

Do pełnej komunikacji sieciowej pomiędzy systemem goszczonym a hostem, tak jakby były to dwie fizyczne maszyny, konieczna będzie niewielka modyfikacja.


## Połączenia sieciowe


## SSH

SSH (*secure shell*) to popularny standard protokołu komunikacyjnego pozwalający m.in. na zdalne wykonywanie poleceń przez sieć TCP/IP za pośrednictwem terminala. Połączenie jest szyfrowane i uwierzytelnione np. hasłem lub certyfikatem.

Użytkownik, przy pomocy *klienta SSH*, łączy się z akceptującym połączenia zdalne *serwerem SSH*, dzięki czemu może wykonywać z poziomu maszyny *klienta* polecenia i uruchamiać programy konsolowe na *serwerze*.


### screen

Programy uruchomione w sesji terminala są przerywane w momencie zamknięcia sesji (np. zamknięcia terminala lub połączenia SSH). W przypadku połączenia SSH, jeśli połączenie zostanie przerwane np. przez błędy w działaniu sieci, program uruchomiony w takiej sesji również zostanie zakończony.

Czasami następuje konieczność uruchomienia czasochłonnego programu i pozostawienia go uruchomionego na serwerze, w czym pomogą nam wirtualne konsole. Najpopularniejsze programy umożliwiające tworzenie wirtualnych konsol to *screen* oraz *tmux*, podczas zajęć będziemy posługiwali się poleceniem *screen*.
