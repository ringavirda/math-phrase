#include "common.hpp"
#include "math-interpreter.hpp"

// 10.2 + 33 * (23-3/22.2)
// 2+4*(2-3/5)+6*2

int main() {

#ifdef WIN32
  SetConsoleCP(CP_UTF8);
  SetConsoleOutputCP(CP_UTF8);
#endif

  echo_emphasis("Welcome to math-phrase interpreter!");

  int mode = 0;
  while (true) {
    echo(R"""(
Available operation regimes are as follows:
    [1] - Per-phrase.
    [2] - Per-operation.

    [9] - Exit interpreter.
)""");
    read_input("Please select mode:", mode);
    std::cin.ignore();
    switch (mode) {
    case 1:
      echo_emphasis("Entering per-phrase mode...");
      per_phrase_mode();
      break;
    case 2:
      echo_emphasis("Entering per-operation mode...");
      per_operation_mode();
      break;

    case 9:
      echo_warn("Exiting program...");
      return 0;

    default:
      echo_warn("Bad input!");
      break;
    }
  }
}
