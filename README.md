# UniBoCG23-Elaborato-2-v2
## Obiettivi del progetto
Creazione di un’applicazione grafica in C (C++) che utilizzi le librerie Opengl e pipeline
di rendering programmabile mediante vertex shader e fragment shader .
Nello specifico si tratta di realizzare una scena navigabile in grafica 3D, in cui sono
previste interazioni con l’utente e la gestione delle collisioni con la telecamera.
### Requisiti da soddisfare:
* La modellazione della geometria deve avvenire sia tramite assemblaggio di primitive (cubi, piramidi, sfere, cilindri, coni, tori) sia importando dall’esterno modelli tridimensionali in formato .obj.
* Bisogna poter selezionare ogni oggetto, modellato mediante le primitive, con il click del mouse sulla finestra grafica e poter cambiare i materiali ed il tipo di shader da utilizzare per la colorazione. I modelli obj importanti dall’esterno verranno colorati usando le informazioni contenute nel file .mtl (dei materiali). 
* Implementare tramite shader 
  * il modello di illuminazione di Phong e di Blinn Phong mediante shading di tipo interpolativo
  * il modello di illuminazione di Phong e di Blinn Phong mediante shading di tipo Phong
  * La texturizzazione delle primitive.
* Aggiungere una cubemap alla scena
* Muoversi nella scena mediante il trackball, con l’evento passive del mouse e con i tasti wasd per muoversi in avanti, indietro, a destra e a sinistra nella scena.

## How to build the project
### Windows - using Visual Studio Code
#### If MinGW is not installed with required modules
1. Install from here [Mingw (32/64)](https://sourceforge.net/projects/mingw/)
2. Once installed launch MSYS2 Mingw64
3. Run this command `pacman -S mingw-w64-x86_64-toolchain base-devel gcc make mingw-w64-i686-gcc mingw-w64-x86_64-gcc cmake`
4. Install CMake Extension in Visual Studio Code
#### Once MinGW and Visual Studio Code are ready
1. Open project folder
2. Perform a scan of the available kits in the system
3. Select `GCC ... x86_64-w64-mingw32 (mingw64)` as a compiler
4. CTRL + SHIFT + P, configure project TWO times
5. Finally run the project selecting `UniboCG23-Elaborato-1` as target if Visual Studio Code asks
### Windows - using Visual Studio
Simply import the header and implementation files one by one and compile the program using the builtin compiler.
