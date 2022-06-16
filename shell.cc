#include "shell.hh"
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <fcntl.h>
#include <string>
#include <sys/syscall.h>
#include <unistd.h>

std::string readfile(const char* file) {
  int fd = syscall(SYS_open, file, O_RDONLY, 0755);
  char byte[1];
  std::string result = "";
  while(syscall(SYS_read, fd, byte, 1))
    result += byte;
  return result;
}

int main()
{ std::string input;

  // ToDo: Vervang onderstaande regel: Laad prompt uit bestand
  std::string prompt = readfile("zz.txt");

  while(true)
  { std::cout << prompt;                   // Print het prompt
    std::getline(std::cin, input);         // Lees een regel
    if (input == "new_file") new_file();   // Kies de functie
    else if (input == "ls") list();        //   op basis van
    else if (input == "src") src();        //   de invoer
    else if (input == "find") find();
    else if (input == "seek") seek();
    else if (input == "exit") return 0;
    else if (input == "quit") return 0;
    else if (input == "error") return 1;

    if (std::cin.eof()) return 0; } }      // EOF is een exit

void new_file() {
  std::string name;
  std::string body;
  std::cout << "Naam van het bestand: ";
  std::cin >> name;
  std::cout << "Inhoud van het bestand:\n";
  while(true) {
    std::string input;
    getline(std::cin, input);
    if (input != "<EOF>")
      body += (input + '\n');
    else break;}
  syscall(SYS_creat, name.c_str(), 0755);
  int fd = syscall(SYS_open, name.c_str(), O_WRONLY, 0755);
  int len = body.length();
  syscall(SYS_write, fd, body.substr(1, len - 2).c_str(), len - 2);
  syscall(SYS_close, fd);
}

void list() {
  int pid = syscall(SYS_fork);
  if (pid == 0) {
    char const *args[] = {"/bin/ls", "-l", "-a", NULL};
    syscall(SYS_execve, args[0], args, NULL);
    syscall(SYS_exit, 0);
  }
  else {
    syscall(SYS_wait4, 0, NULL, NULL, NULL);
  }
}

void find() {
  std::cout << "Enter string: ";
  std::string input;
  getline(std::cin, input);
  int fd[2];
  syscall(SYS_pipe, &fd);
  int findid = syscall(SYS_fork);
  if (findid == 0) {
    syscall(SYS_close, fd[0]);
    syscall(SYS_dup2, fd[1], 1);
    char const *args[] = {"/bin/find", ".", NULL};
    syscall(SYS_execve, args[0], args, NULL);
    syscall(SYS_exit, 0);
  }
  else {
    int grepid = syscall(SYS_fork);
    if (grepid == 0) {
      syscall(SYS_close, fd[1]);
      syscall(SYS_dup2, fd[0], 0);
      char const *args[] = {"/bin/grep", input.c_str()};
      syscall(SYS_execve, args[0], args, NULL);
      syscall(SYS_exit, 0);
    }
    else {
      syscall(SYS_wait4, 0, NULL, NULL, NULL);
    }
  }

}

void seek() {
  int fd;
  syscall(SYS_creat, "seek", 0755);
  fd = syscall(SYS_open, "seek", O_WRONLY, 0755);
  syscall(SYS_write, fd, "x", 1);
  syscall(SYS_lseek, fd, 5000000, SEEK_END);
  syscall(SYS_write, fd, "x", 1);

  syscall(SYS_creat, "loop", 0755);
  fd = syscall(SYS_open, "loop", O_WRONLY, 0755);
  syscall(SYS_write, fd, "x", 1);
  for (int i = 0; i < 5000000; i++){
    syscall(SYS_write, fd, "\0", 1);
  }
  syscall(SYS_write, fd, "x", 1);



}

void src() // Voorbeeld: Gebruikt SYS_open en SYS_read om de source van de shell (shell.cc) te printen.
{ int fd = syscall(SYS_open, "shell.cc", O_RDONLY, 0755); // Gebruik de SYS_open call om een bestand te openen.
  char byte[1];                                           // 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
  while(syscall(SYS_read, fd, byte, 1))                   // Blijf SYS_read herhalen tot het bestand geheel gelezen is,
    std::cout << byte; }                                  //   zet de gelezen byte in "byte" zodat deze geschreven kan worden.
