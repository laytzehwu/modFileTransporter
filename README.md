modFileCopier is a pauseable file copying program under linux environment. Below is the syntex to run it:

./modFileCopier <source file path> <targe file path>

exp: 
./modFileCopier test.mp3 test2.mp3
./modFileCopier /home/music/test.mp3 /home/music/test2.mp3
./modFileCopier /home/music/test.mp3 /home/music/playlist/

Pause and resume
Press "p" then enter to pause copying.
Press "r" then enter to resume copying.

Note: if you want to keep original file, just specify the path til folder. 

modFileCopier is develop by C++ under Ubuntu environment. Windows version pending at thread issue.
Code::Blocks is IDE. Because of fail to add "-lpthread" as compilation param, I only use it for type code and syntex checking:). 

Files
build.sh - batch file of compilation
main.cpp - The only C++ source file ;)
 
Some other pending item:
* Memory lead might cause by fail of termination of key monitoring thread
* UX is not nice. Run out of time to study screen control.
* Because of the thread problem. It is shortly not support windows.




