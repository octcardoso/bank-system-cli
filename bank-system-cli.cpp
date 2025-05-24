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
              << "Opção desejada: ";
    // preciso verificar essa entrada
    std::cin >> menu_choice;
    switch(menu_choice) {
      case 1: {
        // Inserir novo usuário
        std::string name;
        unsigned short age = 0;
        unsigned long long balance_in_cents = 0;
        std::cout << "\n--- Inserir Novo Usuário ---\n";
        // get_user_data(name, age, balance_in_cents);
        // create_user(users, current_capacity, user_amount, name, age, balance_in_cents);
        break;
      }
      case 2: {
        // Inserir vários usuários
        std::string name;
        unsigned short age = 0;
        unsigned long long balance_in_cents = 0;
        unsigned int insert_amount = 0;
        std::cout << "\n------------- Inserir Vários Usuários -------------\n"
                  << "Quantidade desejada de usuários a serem inseridos: ";
        // verificar entrada
        std::cin >> insert_amount;
        for(unsigned int i = 0; i < insert_amount; i++) {
          std::cout << "\n--- Dados para Usuário " << i + 1 << " de " << insert_amount << " ---\n";
          // get_user_data(name, age, balance_in_cents);
          // create_user(users, current_capacity, user_amount, name, age, balance_in_cents);
        }
        break;
      }
      case 3: {
        // Bucar usuário por ID
        unsigned int user_id = 0;
        std::cout << "\n--- Buscar Usuário por ID ---\n"
                  << "ID do usuário: ";
        // verificar entrada
        std::cin >> user_id;
        // print_user(users, user_id);
        break;
      }
      case 4: {
        // Transferência entre usuários
        unsigned int sender_id = 0, receiver_id = 0;
        unsigned long long transfer_amount = 0;
        std::cout << "\n--- Transferência Entre Usuários ---\n"
                  << "Insira o ID do remetente: ";
        // verificar entrada
        std::cin >> sender_id;
        std::cout << "Insira o ID do destinatário: ";
        // verificar entrada
        std::cin >> receiver_id;
        std::cout << "Insira a quantidade a ser transferida: ";
        // verificar entrada e fazer a formatação
        std::cin >> transfer_amount;
        // transfer_between_users(users, current_capacity, sender_id, receiver_id, transfer_amount);
        break;
      }
      case 5: {
        // Remover usuário por ID
        unsigned int target_user_id;
        std::cout << "\n------ Remoção de Usuário por ID ------\n"
                  << "Insira o ID do usuário a ser removido: ";
        // verificar entrada
        std::cin >> target_user_id;
        // remove_user(users, current_capacity, target_user_id);
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
