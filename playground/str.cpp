
#include <string.h>
#include <stdio.h>

const int buf_size = 256;
char buf[buf_size];

int main() {
  char* ptr = fgets(buf, buf_size, stdin);
  if (ptr == nullptr) {
    return 0;
  }
  if (!strcmp(buf, "john\n")) {
    __builtin_trap();
  }
  return 0;
}
