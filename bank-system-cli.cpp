#include <iostream>
#include <string>

typedef struct User {
  std::string name;
  unsigned short age;
  unsigned long long balance_in_cents;
} user;

int main() {
  // Menu
  unsigned short menu_choice = 0;
  bool running = true;
  while(running) {
    std::cout << "\n--------------- MENU ---------------\n"
              << "1. Inserir novo usuário\n"
              << "2. Inserir vários usuários\n"
              << "3. Buscar usuário por ID\n"
              << "4. Transferência entre usuários\n"
              << "5. Remover usuários por ID\n"
              << "6. Carregar arquivo para a memória\n"
              << "7. Sair\n\n"
              << "Insira a opção desejada: ";
    std::cin >> menu_choice;
    switch(menu_choice) {
      case 1: {
        // Inserir novo usuário
        break;
      }
      case 2: {
        // Inserir vários usuários
        break;
      }
      case 3: {
        // Bucar usuário por ID
        break;
      }
      case 4: {
        // Transferência entre usuários
        break;
      }
      case 5: {
        // Remover usuários por ID
        break;
      }
      case 6: {
        // Carregar arquivo para a memória
        break;
      }
      case 7: {
        // Sair
        running = false;
        break;
      }
      default: {
        std::cout << "Opção inválida. Tente novamente.\n";
        break;
      }
    }
  }
  return 0;
}
