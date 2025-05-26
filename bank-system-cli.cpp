#include <iostream>
#include <string>
#include <limits>

const unsigned int INITIAL_CAPACITY = 10; const unsigned short MAX_USER_AGE = 130;
const unsigned short MAX_NAME_LENGHT = 100;

typedef struct User {
  std::string name;
  unsigned short age;
  unsigned long long balance_in_cents;
} user;

unsigned int get_valid_user_index(const user* const &users, const unsigned int &current_capacity);
bool is_input_valid(const std::string &input, 
                    const unsigned short int &max_integer_digits, 
                    const unsigned short int &max_decimal_digits,
                    bool accept_negative);

int main() {
  // Menu
  user *users = new user[INITIAL_CAPACITY];
  if(users == NULL) {
    std::cout << "Problema na inicialização dos usuários. Abortando o programa...\n";
    abort();
  }

  unsigned int current_capacity = INITIAL_CAPACITY;
  unsigned int user_amount = 4;

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
              << "7. Sair e salvar\n\n"
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
        if(user_amount == 0) {
          std::cout << "\n--- Buscar Usuário por ID ---\n"
                    << "Não há usuários registrados.\n";
          break;
        }

        unsigned int user_id = 0;
        std::cout << "\n--- Buscar Usuário por ID ---\n"
                  << "ID do usuário: ";
        user_id = get_valid_user_index(users, current_capacity);
        // print_user(users, user_id);
        break;
      }
      case 4: {
        // Transferência entre usuários
        unsigned int sender_id = 0, receiver_id = 0;
        unsigned long long transfer_amount = 0;
        std::cout << "\n--- Transferência Entre Usuários ---\n"
                  << "Insira o ID do remetente: ";
        sender_id = get_valid_user_index(users, current_capacity);
        std::cout << "Insira o ID do destinatário: ";
        receiver_id = get_valid_user_index(users, current_capacity);
        std::cout << "Insira a quantidade a ser transferida: ";
        // verificar entrada e fazer a formatação
        std::cin >> transfer_amount;
        // transfer_between_users(users, sender_id, receiver_id, transfer_amount);
        break;
      }
      case 5: {
        // Remover usuário por ID
        unsigned int target_user_id;
        std::cout << "\n------ Remoção de Usuário por ID ------\n"
                  << "Insira o ID do usuário a ser removido: ";
        target_user_id = get_valid_user_index(users, current_capacity);
        // remove_user(users, target_user_id);
        break;
      }
      case 6: {
        // Carregar arquivo para a memória
        break;
      }
      case 7: {
        // Sair e salvar
        running = false;
        break;
      }
      default: {
        std::cout << "Opção inválida. Tente novamente.\n";
        break;
      }
    }
  }
  delete[] users;
  return 0;
}

unsigned int get_valid_user_index(const user* const &users, const unsigned int &current_capacity) {

  if (users == NULL) { // NULL ou nullptr ?
    std::cerr << "Erro: problema na leitura dos usuários. Encerrando o programa...\n";
    abort();
  }

  if(current_capacity == 0) {
    std::cerr << "Erro: capacidade de usuários é zero. Encerrando o programa...\n";
    abort();
  }

  std::string input;

  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  while (true) {

    if (!std::getline(std::cin, input)) {
      std::cerr << "Erro na leitura de entrada.\n";
      continue;
    }

    if (!is_input_valid(input, 4, 0, false)) {
      std::cout << "ID inválido. Por favor, insira um ID válido: ";
      continue;
    }

    unsigned int user_id = std::stoi(input);

    if (user_id == 0 || user_id > current_capacity) {
      std::cout << "ID inválido. Por favor, insira um ID válido: ";
      continue;
    }

    unsigned int user_index = user_id - 1;

    if (users[user_index].name.empty()) {
      std::cout << "O usuário com ID " << user_id << " não existe.\n"
                << "Por favor, insira um ID válido: ";
      continue;
    }

    return user_index;
  }
}

/**
 * @brief Valida se uma string representa um número decimal corretamente formatado.
 * 
 * Verifica se:
 * - Formato numérico válido (dígitos, sinal e ponto decimal)
 * - Quantidade máxima de dígitos nas partes inteira e decimal
 * - Eliminação de zeros insignificantes
 * 
 * @param input String da entrada a ser validada
 * @param max_integer_digits Número máximo permitido de dígitos na parte inteira
 * @param max_decimal_digits Número máximo permitido de dígitos na parte decimal
 * @return bool true se a entrada for válida, false caso contrário
 * 
 * @note Comportamentos especiais:
 * - Sinais (+/-) são opcionais e removidos antes da validação
 * - Zeros insignificantes à esquerda na parte inteira são ignorados
 * - Zeros insignificantes à direita na parte decimal são ignorados
 * - Parte decimal de valor zero (ex: ".000") é considerada válida
 */
bool is_input_valid(const std::string &input, 
  const unsigned short int &max_integer_digits, 
  const unsigned short int &max_decimal_digits,
  bool accept_negative
) {

  if(input.empty()) return false;

  bool has_digit = false;
  int dot_count = 0;
  size_t start_index = 0;
  
  if(input[0] == '+' || (input[0] == '-' && accept_negative)) {
    start_index = 1;
  }

  for(size_t i = start_index; i < input.size(); i++) {
    if(input[i] == '.') {
      if(dot_count++ > 1) return false;
    } else if(std::isdigit(input[i])) {
      has_digit = true;
    } else {
      return false;
    }
  }

  if(!has_digit) return false;

  size_t dot_position = input.find('.');
  std::string integer = input.substr(start_index, dot_position);
  std::string decimal = (dot_position != std::string::npos) ? input.substr(dot_position + 1) : "";

  size_t first_non_zero = integer.find_first_not_of('0');
  if(first_non_zero != std::string::npos) {
    integer = integer.substr(first_non_zero);
  } else {
    integer = "0";
  }

  if(integer.length() > max_integer_digits) return false;

  if(!decimal.empty()) {
    size_t last_non_zero = decimal.find_last_not_of('0');
    if(last_non_zero != std::string::npos) {
      decimal = decimal.substr(0, last_non_zero + 1);
    } else {
      decimal.clear();
    }

    if(decimal.length() > max_decimal_digits) return false;
  }

  return true;
}
