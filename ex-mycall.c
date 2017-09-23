/*
MC504 Projeto 2
Implementação de Chamadas de Sistema

Testing Colorful Syscall / Testando Syscall Colorida

Ruy Castilho Barrichelo RA 177012

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  printf("Testing new syscall\n");
  syscall(384); 
  return 0;
}
