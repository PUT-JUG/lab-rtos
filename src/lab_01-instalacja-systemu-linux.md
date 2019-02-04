# Instalacja i konfiguracja systemu Xubuntu w środowisku VirtualBox

## Systemy GNU/Linux

Linux to tak naprawdę nazwa darmowego, otwartego jądra systemu uniksopodobnego. Potocznie nazywa się tak jednak całą rodzinę systemów opartych na tym jądrze. Samo jądro nie stanowi kompletnego, nadającego się do użytku systemu operacyjnego, stąd dołączono do niego narzędzia i biblioteki z projektu GNU, stąd właściwa nazwa rodziny systemów: GNU/Linux.

Systemy linuksowe dostarczane są w formie dystrybucji takich jak Ubuntu, Debian czy Fedora, z których każda składa się z jądra oraz zestawu pakietów oprogramowania dobranego przez twórców danej dystrybucji.

Podczas zajęć posługiwać będziemy się dystrybucją **Xubuntu** w wersji **18.04 LTS**, która jest odmianą bardzo popularnej dystrybucji Ubuntu, jednak z lekkim interfejsem graficznym XFCE doskonale nadającym się do pracy w środowisku wirtualizowanym lub na nieco starszych komputerach.

Większość operacji oraz poleceń prezentowanych podczas zajęć jest uniwersalna i powinna działać bez zmian lub z drobnymi modyfikacjami pod większością dystrybucji (wyjątki zostaną wyróżnione).

## Maszyny wirtualne

Aby ułatwić poznanie nowego systemu, zainstalujemy go na maszynie wirtualnej.

Tworząc maszynę wirtualną, nasz komputer będzie "symulował" istnienie zasobów, które tworzą kompletny komputer - chipsetu płyty głównej, dysków, karty graficznej, interfejsów sieciowych. Wirtualizacja nie jest emulacją - w przeciwieństwie do emulacji, goszczony system operacyjny będzie miał niemal bezpośredni dostęp do fizycznego procesora w komputerze. 

Dzięki temu unikniemy ryzyka uszkodzenia głównego systemu operacyjnego, na którym pracujemy na co dzień, bądź utraty plików. Będziemy również w stanie uruchomić - poza systemem głównym (*host*) - jeden lub więcej system "gościa" (*guest*) jednocześnie.

Do zarządzania i uruchamiania maszyn wirtualnych potrzebujemy *hypervisora*. Wykorzystamy projekt **VirtualBox**, który wspiera wiele goszczonych systemów operacyjnych i działa pod popularnymi systemami hosta (Windows, macOS i wiele dystrybucji Linuksa).

## Instalacja programu VirtualBox

* Plik instalacyjny pobierz ze strony https://www.virtualbox.org/wiki/Downloads dla danego systemu operacyjnego **hosta** (w przypadku komputerów w Laboratorium - Windows).

* Zainstaluj program zostawiając domyślne ustawienia.
    * System zapyta o zezwolenie na instalację sterowników interfejsów sieciowych, potwierdź zgodę.

* Pobierz z sekcji **VirtualBox Oracle VM VirtualBox Extension Pack** pakiet dodatków.

* Zainstaluj dodatki otwierając pobrany plik za pomocą aplikacji **VirtualBox Manager** i postępując zgodnie z poleceniami na ekranie.

## Tworzenie maszyny wirtualnej

* Uruchom program VirtualBox.

* Kliknij przycisk *New* z paska narzędziowego. W kolejnych etapach okna dialogowego:

* Wprowadź nazwę maszyny wirtualnej (nazwa może być dowolna, warto aby reprezentowała typ bądź przeznaczenie danej maszyny). Podczas zajęć nazwę ustaw na *Xubuntu 18.04 - Test*.
* Wybierz typ instalowanego systemu operacyjnego oraz architekturę - Linux / Ubuntu 64-bit - VirtualBox stara się rozpoznać typ systemu po nazwie maszyny.

![Tworzenie maszyny wirtualnej](images/lab_01_create_vm.png)

* Ustaw ilość pamięci RAM, jaką otrzyma maszyna wirtualna. Pamiętaj, że system hosta i maszyny wirtualne współdzielą pamięć jednej maszyny. Podczas zajęć ustaw *1024 MB*.

* Stwórz nowy obraz dysku dla maszyny wirtualnej, pozostaw domyśne opcje:
    * Format VDI
    * Dynamicznie przydzielany rozmiar
    * Rozmiar maksymalny 10 GB
    * Lokalizacja obrazu w wewnątrz folderu *\~/Virtualbox VMs/\<nazwa maszyny wirtualnej\>*

* Utworzona zostanie konfiguracja z domyślnymi dla danego sysytemu ustawieniami, które są odpowiednie dla większości zastosowań:

![Utworzona maszyna wirtualna](images/lab_01_fresh_vm.png)

## Instalacja systemu

* Pobierz obraz *ISO* płyty instalacyjnej ze strony https://xubuntu.org/download/ - wersja 18.04.\*, *Desktop* dla architektury 64-bit - wybierz serwer (Mirror) z pobliskiego kraju, a następnie odpowiedni plik ISO (aktualnie *xubuntu-18.04.1-desktop-amd64.iso*).

* Uruchom maszynę kilkając *Start*. Maszyna domyślnie ustawiona jest na boot z wirtualnego napędu CD. Przy pierwszym uruchomieniu maszyny VirtualBox poprosi o wskazanie obrazu ISO płyty:

![Wybór obrazu ISO](images/lab_01_choose_iso.png)

| Obsługa maszyny wirtualnej |
| ------------- |
| Kiedy aktywne jest okno maszyny wirtualnej, wszystkie naciśnięcia klawiszy przekazywane są do systemu gościa. Kliknięcie w obszarze obrazu maszyny spowoduje także przechwycenie kursora. Uwolnienie kursora i wyłączenie przechwytywania klawiszy może wymagać naciśnięcia tzw. *host key*. Aktualnie ustawiony host key wyświetlany jest w prawym dolnym rogu okna maszyny, pod systemem Windows domyślnie jest to *prawy control*. |

* Pobrany obraz to tzw. *Live-CD*, co oznacza, że system można uruchomić bezpośrednio z płyty, bez konieczności instalacji, w celu np. przetestowania lub awaryjnego naprawienia istniejącego systemu. Podczas zajęć będziemy się posługiwać trwałą instalacją, dlatego wybierz opcję *Install Xubuntu*, pozostawiając domyślny, angielski język systemu.

* Wybierz układ klawiatury *Polish*/*Polish*, przejdź dalej przyciskiem *Continue*.

![Wybór obrazu ISO](images/lab_01_xubuntu_keyboard.png)

* Wszystkie pozostałe opcje mogą pozostać domyślne, przejdź przez kolejne etapy instalacji przyciskiem *Continue*.

    * Typ instalacji (Installation type)
    
        Podczas instalacji na maszynie wirtualnej możesz bez obaw wybrać usunięcie danych i użycie całego dysku - opcja *Erase disk and install Xubuntu*. Jedyne do czego ma dostęp maszyna wirtualna, to uprzednio stworzony obraz wirtualnego dysku. Podczas ewentualnej instalacji na fizycznym komputerze masz możliwość instalacji obok istniejącego systemu operacyjnego (np. Windows) wraz z bootloaderem pozwalającym na wybór systemu podczas uruchamiania. Wymaga to jednak podziału dysku na partycje oraz pewnej wiedzy i doświadczenia.
        
        **WAŻNE: Przed instalacją jakiegokolwiek systemu na fizycznym komputerze, na którym znajdują się dane WYKONAJ KOPIĘ ZAPASOWĄ**

* Podaj dane użytkownika i nazwę komputera.

    Podczas zajęć posługujemy się loginem *student*, nazwą komputera *vbox-xubuntu-labrtos* i hasłem *Over9000*

![Dane użytkownika](images/lab_01_xubuntu_user.png)

* Po zakończeniu zatwierdź prośbę o restart maszyny. Instalator poprosi o wyjęcie płyty z napędu (VirtualBox zrobi to automatycznie), co potwierdź naciskając *Enter*. Po ponownym uruchomieniu zostaniesz powitany graficznym interfejsem użytkownika. Zaloguj się podając hasło użyte podczas instalacji.

## Instalacja kompilatora i dodatków systemu gościa

Świeżo po instalacji systemu gościa VirtualBox musi w pełni symulować niektóre urządzenia sprzętowe. Wydajność i wygodę obsługi maszyny wirtualnej można poprawić instalując tzw. dodatki gościa (*Guest Additions*).

* Dla systemów goszczonych Windows od wersji NT dodatki są dostarczone w formie graficznego instalatora.
    
* Dla systemów goszczonych Linux, aby osiągnąć pełną funkcjonalność, instalator musi skompilować tzw. *moduły jądra* (sterowniki) dedykowane dla wirtualizowanego sprzętu i systemu, co będzie wymagało zainstalowania kompilatora.
    
### Instalacja kompilatora i pierwsze kroki w terminalu
    
* Programy i biblioteki dla wielu dystrubucji dostarczane są w formie *pakietów* (paczek), którymi zarządza menadżer paczek używany w danej dystrybucji. Menadżer ten automatycznie rozwiązuje *zależności* pomiędzy paczkami (dany program może wymagać do działania innego programu lub specyficznych bibliotek), pobiera je z *repozytoriów* i instaluje w systemie.

    * Współczesne dystrybucje dostarczają też często graficzny \"sklep\" z popularnymi aplikacjami użytkowymi.
    
* W systemach z rodziny Debian (w tym Ubuntu i pochodne) wykorzystywany jest menadżer paczek *APT* (*Advanced Packaging Tool*). Paczki przechowywane są w archiwach *.deb*. Funkcje *APT* są dostępne z poziomu linii poleceń pod komentą `apt`.

Uruchom aplikację *Terminal Emulator* korzystając z ikony menu Xubuntu w lewym górnym rogu. Jeśli nie wyświetla się na liście, możesz wpisać w wyszukiwarce \"terminal\". Powinno pojawić się następujące okno:

![Terminal Emulator](images/lab_01_xubuntu_terminal.png)

Linia poleceń zawiera m.in. Twoją nazwę użytkownika i nazwę komputera, a na końcu *znak zachęty* ($). Po znaku zachęty możesz wpisywać polecenia do wykonania, zatwierdzane enterem.

Sprawdź czy Twoja maszyna ma połączenie z Internetem, odpytując znany serwer. Domyślna konfiguracja VirtualBoxa powinna zapewniać udostępnienie połączenia z hosta do gościa. Wykonaj polecenie `ping` wraz z adresem, który ma odpytać.

```bash
ping www.google.com
```

W terminalu co sekundę powinna pokazywać się odpowiedź od serwera.

![Terminal Emulator](images/lab_01_xubuntu_ping.png)

Działanie programów konsolowych możesz skrótem klawiszowym *control-c*. Pamiętaj, że prawy control jest skonfigurowany jako specjalny *klawisz hosta*, zatem do skrótów używaj lewego klawisza control. Przerwij działanie polecenia `ping`.

Spróbuj zainstalować pakiet `build-essential`, który spowoduje zainstalowanie kompilatora i wszystkich niezbędnych narzędzi. Wykonaj polecenie:

```bash
apt install build-essential
```

Polecenie odpowie komunikatem o braku wymaganych uprawnień. Dodaj na początku polecenia komendę `sudo`, co oznacza w uproszczeniu *uruchom jako administrator*:

```bash
sudo apt install build-essential
```

Polecenie `sudo` zapyta o hasło - wpisz swoje hasło użytkownika podane podczas instalacji. Menadżer wyświetli listę paczek, które zostaną zainstalowane jako zależności i zapyta o potwierdzenie. Domyślna odpowiedź wyróżniona jest wielką literą (*Y*), zatwierdź instalację klawiszem *Enter*.

### Instalacja dodatków

Z menu w oknie z maszyną wirtualną wybierz *Devices* &rarr; *Insert Guest Additions* CD image. Xubuntu powinno automatycznie otworzyć okno przeglądarki plików. Zwróć uwagę na ścieżkę, pod jaką znajduje się zawartość płyty.

Wróć do okna terminala i uruchom polecenie `autorun.sh` z płyty. Ponieważ program znajduje się na płycie, podaj pełną ścieżkę do polecenia, podmieniając ewentualnie numer wersji dodatków na aktualny:

```bash
/media/student/VBox_GAs_6.0.4/autorun.sh
```
Instalator zapyta ponownie o hasło użytkownika, a następnie wyświetli dodatkowe okno terminala z komunikatami. Po zakończeniu instalacji powinno wyglądać następująco:

![Additions installed](images/lab_01_xubuntu_additions.png)

Możesz zamknąć teminale, a następnie usunąć płytę z dodatkami z wirtualnego napędu korzystając z ikony w prawym dolnym rogu. Konieczne może być wymuszenie wyjęcia płyty (*Force Unmount*):

![Remove CD](images/lab_01_vbox_remove_cd.png)

Zrestartuj maszynę wirtualną korzystając z menu Xubuntu:

![Additions installed](images/lab_01_xubuntu_shutdown.png)

Po ponownym uruchomieniu włącz współdzielony schowek w menu VirtualBoxa: *Devices* &rarr; *Shared Clipboard* &rarr; *Bidirectional*, co znacznie ułatwi Ci przenoszenie informacji pomiędzy gościem a hostem. Od tej pory rozdzielczość systemu gościa będzie się także dopasowywała do rozmiaru okna VirtualBoxa. Możesz przełączyć się na pełny ekran naciskając *przycisk hosta*+*F*. 

## Poznaj nowy system!

Do poleceń terminala wrócimy na kolejnych laboratoriach, a na razie - poznaj nowy system od bardziej przyjaznej, graficznej strony!

Uruchom przeglądarkę plików, sprawdź jakie aplikacje i opcje są dostępne w menu. Zajrzyj do ustawień, eksperymentuj. To tylko maszyna wirtualna, którą zawsze możesz w łatwy sposób skonfigurować od zera bez obaw o system, na którym pracujesz na co dzień.