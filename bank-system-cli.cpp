#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>

const unsigned int INITIAL_CAPACITY = 10; 
const unsigned short MIN_USER_AGE = 18;
const unsigned short MAX_USER_AGE = 130;
const unsigned short MAX_NAME_LENGTH = 100;
const char INPUT_DELIMITER = '\n';

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
void get_user_data(std::string &name, 
                   unsigned short &age,
                   unsigned long long &balance_in_cents);
bool is_username_valid(std::string &name); 
bool is_user_age_valid(std::string &temp_input, unsigned short &age);
bool is_monetary_value_valid(std::string &temp_input, unsigned long long &monetary_value);
void register_user(user *&users, 
                 unsigned int &current_capacity, 
                 unsigned int &max_user_index, 
                 const std::string &name, 
                 const unsigned short &age, 
                 const unsigned long long &balance_in_cents);
void print_user_data(const user * const &users, const unsigned int &user_index);
void transfer_between_users(user * const &users, 
                            const unsigned int &sender_index, 
                            const unsigned int &receiver_index, 
                            const unsigned long long &transfer_amount);
void remove_user(user * const &users, 
                 unsigned int &max_user_index,
                 const unsigned int &user_index);
void save_users_to_file(const user * const &users, 
                        const unsigned int &max_user_index, 
                        const unsigned int &current_capacity);
void load_users_from_file(user *&users, 
                     unsigned int &current_capacity,
                     unsigned int &max_user_index);

int main() {
  // Menu

  user *users = new user[INITIAL_CAPACITY];
  if(users == NULL) {
    std::cerr << "Problema na inicialização dos usuários. Encerrando o programa...\n";
    abort();
  }

  for(unsigned int i = 0; i < INITIAL_CAPACITY; i++) {
    users[i] = {
      "",
      0,
      0
    };
  }

  unsigned int current_capacity = INITIAL_CAPACITY;
  unsigned int max_user_index = 0;
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
              << "7. Sair e salvar\n"
              << "------------------------------------\n\n"
              << "Opção desejada: ";

    std::string menu_choice_input;
    while(true) {
      if(!std::getline(std::cin, menu_choice_input, INPUT_DELIMITER)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), INPUT_DELIMITER);
        std::cout << "Erro ao obter um valor do menu. Tente novamente: ";
        continue;
      }
      if(!is_input_valid(menu_choice_input, 1, 0, false)) {
        std::cout << "Opção inválida. Tente novamente: ";
        continue;
      }
      menu_choice = static_cast<unsigned short>(std::stoi(menu_choice_input));
      break;
    }

    switch(menu_choice) {
      case 1: {
        // Inserir novo usuário
        std::string name;
        unsigned short age = 0;
        unsigned long long balance_in_cents = 0;

        std::cout << "\n------ Inserir Novo Usuário ------\n";
        get_user_data(name, age, balance_in_cents);
        std::cout << "----------------------------------\n";
        register_user(users, current_capacity, max_user_index, name, age, balance_in_cents);
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

        std::string insert_amount_input;
        while(true) {
          if(!std::getline(std::cin, insert_amount_input, INPUT_DELIMITER)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), INPUT_DELIMITER);
            std::cout << "Erro ao obter a quantidade. Tente novamente: ";
            continue;
          }
          if(!is_input_valid(insert_amount_input, 4, 0, false)) {
            std::cout << "Insira uma quantidade válida: ";
            continue;
          }
          insert_amount = static_cast<unsigned int>(std::stoi(insert_amount_input));
          if(insert_amount == 0) {
            std::cout << "Insira uma quantidade válida: ";
            continue;
          }
          break;
        }

        for(unsigned int i = 0; i < insert_amount; i++) {
          std::cout << "\n------ Dados para Usuário " << i + 1 << " de " << insert_amount << " ------\n";
          get_user_data(name, age, balance_in_cents);
          std::cout << "----------------------------------------\n";
          register_user(users, current_capacity, max_user_index, name, age, balance_in_cents);
        }
        break;
      }
      case 3: {
        // Bucar usuário por ID
        if(max_user_index == 0) {
          std::cout << "\n--- Buscar Usuário por ID ---\n"
                    << "Não há usuários registrados.\n";
          break;
        }

        unsigned int user_index = 0;
        std::cout << "\n--- Buscar Usuário por ID ---\n"
                  << "ID do usuário: ";

        user_index = get_valid_user_index(users, current_capacity);

        print_user_data(users, user_index);
        break;
      }
      case 4: {
        // Transferência entre usuários
        unsigned int sender_index = 0, receiver_index = 0;
        unsigned long long transfer_amount = 0;

        std::cout << "\n--- Transferência Entre Usuários ---\n"
                  << "Insira o ID do remetente: ";
        sender_index = get_valid_user_index(users, current_capacity);
        std::cout << "Insira o ID do destinatário: ";
        receiver_index = get_valid_user_index(users, current_capacity);

        std::string temp_input;
        do {
          std::cout << "Insira a quantidade a ser transferida: ";
          std::getline(std::cin, temp_input, INPUT_DELIMITER);
        } while(!is_monetary_value_valid(temp_input, transfer_amount));

        transfer_between_users(users, sender_index, receiver_index, transfer_amount);
        break;
      }
      case 5: {
        // Remover usuário por ID
        unsigned int target_user_index;
        std::cout << "\n------ Remoção de Usuário por ID ------\n"
                  << "Insira o ID do usuário a ser removido: ";
        target_user_index = get_valid_user_index(users, current_capacity);
        remove_user(users, max_user_index, target_user_index);
        break;
      }
      case 6: {
        // Carregar arquivo para a memória
        load_users_from_file(users, current_capacity, max_user_index);
        break;
      }
      case 7: {
        // Sair e salvar
        save_users_to_file(users, max_user_index, current_capacity);
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

  if (users == NULL) {
    std::cerr << "Problema na inicialização dos usuários. Encerrando o programa...\n";
    abort();
  }

  if(current_capacity == 0) {
    std::cerr << "Erro: capacidade de usuários é zero. Encerrando o programa...\n";
    abort();
  }

  std::string input;

  while (true) {

    if (!std::getline(std::cin, input, INPUT_DELIMITER)) {
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

  bool has_digit = false; int dot_count = 0;
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
    size_t last_zero = decimal.rfind('0');
    size_t last_non_zero = decimal.find_last_not_of('0');
    if(last_zero != std::string::npos && last_non_zero != std::string::npos) {
      if(last_non_zero > last_zero) {
        decimal = decimal.substr(0, last_non_zero + 1);
      } else {
        decimal = decimal.substr(0, last_zero + 1);
      }
    } else {
      decimal.clear();
    }

    if(decimal.length() > max_decimal_digits) return false;
  }

  return true;
}

void get_user_data(std::string &name,
                   unsigned short &age,
                   unsigned long long &balance_in_cents
) {

  do {
    std::cout << "Insira o nome do usuário: ";
    std::getline(std::cin, name, INPUT_DELIMITER);
  } while(!is_username_valid(name));

  std::string temp_input;
  do {
    std::cout << "Insira a idade do usuário: ";
    std::getline(std::cin, temp_input, INPUT_DELIMITER);
  } while(!is_user_age_valid(temp_input, age));

  do {
    std::cout << "Insira o saldo do usuário: ";
    std::getline(std::cin, temp_input, INPUT_DELIMITER);
  } while(!is_monetary_value_valid(temp_input, balance_in_cents));

}

bool is_username_valid(std::string &name) {
  if(std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), INPUT_DELIMITER);
    std::cout << "Erro na leitura da entrada para o nome. Por favor, tente novamente.\n";
    return false;
  }
  if(name.empty()) {
    std::cout << "Nome inválido. O nome do usuário não pode estar vazio.\n"
              << "Por favor, tente novamente.\n";
    return false;
  }
  if(name.find(",") != std::string::npos) {
    std::cout << "Nome inválido. O nome contém caracteres inválidos.\n"
              << "Por favor, tente novamente.\n";
    return false;
  }
  if(name.length() > MAX_NAME_LENGTH) {
    std::cout << "Nome inválido. O nome excede o limite de " << MAX_NAME_LENGTH << " caracteres.\n"
              << "Por favor, tente novamente.\n";
    return false;
  }
  return true;
}

bool is_user_age_valid(std::string &temp_input, unsigned short &age) {
  if(std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), INPUT_DELIMITER);
    std::cout << "Erro na leitura da entrada para idade. Por favor, tente novamente.\n";
    return false;
  }
  if(!is_input_valid(temp_input, 3, 0, false)) {
    std::cout << "Idade inválida. A idade deve estar entre " << MIN_USER_AGE
              << " e " << MAX_USER_AGE << ".\n";
    return false;
  }
  age = static_cast<unsigned short>(std::stoi(temp_input));
  if(age > MAX_USER_AGE || age < MIN_USER_AGE) {
    std::cout << "Idade inválida. A idade deve estar entre " << MIN_USER_AGE
              << " e " << MAX_USER_AGE << ".\n";
    return false;
  }
  return true;
}

bool is_monetary_value_valid(std::string &temp_input, 
                             unsigned long long &monetary_value
) {
  if(std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), INPUT_DELIMITER);
    std::cout << "Erro ao ler entrada de valor monetário. Por favor, tente novamente: ";
    return false;
  }

  if(!is_input_valid(temp_input, 10, 2, false)) {
    std::cout << "Entrada inválida. O valor deve ser menor que R$10.000.000.000,00 e "
              << "seu formato deve ser XXXXXXXXXX.XX\n"
              << "Por favor, tente novamente: ";
    return false;
  }
    
  size_t dot_position = temp_input.rfind('.');
  if(dot_position != std::string::npos) {
    temp_input.erase(dot_position, 1);
    monetary_value = std::stoull(temp_input);
    return true;
  }

  monetary_value = std::stoull(temp_input) * 100;
  return true;
}

void register_user(user *&users, 
                 unsigned int &current_capacity, 
                 unsigned int &max_user_index, 
                 const std::string &name, 
                 const unsigned short &age, 
                 const unsigned long long &balance_in_cents
) {
  if(users == NULL) {
    std::cerr << "Problema na inicialização dos usuários. Encerrando o programa...\n";
    abort();
  }

  if(current_capacity == 0) {
    std::cerr << "Erro: capacidade de usuários é zero. Encerrando o programa...\n";
    abort();
  }

  if(max_user_index >= current_capacity) { 

    user *new_vector = new user[(current_capacity * 2)];
    if(new_vector == NULL) {
      std::cerr << "\n-------------------------- ERRO --------------------------\n"
                << "Falha ao alocar memória para expandir o vetor de usuários.\n"
                << "O usuário " << name << " com ID " << max_user_index
                << " não pôde ser registrado.\n"
                << "----------------------------------------------------------\n";
      return;
    }

    for(unsigned int i = 0; i < current_capacity; i++) {
      if(!users[i].name.empty()) {
        new_vector[i] = users[i];
      } else {
        new_vector[i] = {
          "",
          0,
          0
        };
      }
    }

    current_capacity *= 2;
    delete[] users;
    users = new_vector;
  }

  users[max_user_index] = {
    name,
    age,
    balance_in_cents
  };

  std::cout << "\n--------- Usuário Registrado com Sucesso ---------\n"
            << "ID do Usuário: " << max_user_index + 1 << "\n"
            << "Nome:  " << users[max_user_index].name << "\n"
            << "Idade: " << users[max_user_index].age << " anos\n"
            << "Saldo: R$ " << (users[max_user_index].balance_in_cents / 100)
            << ",";
  if((users[max_user_index].balance_in_cents % 100) < 10) {
    std::cout << "0";
  }
  std::cout << (users[max_user_index].balance_in_cents % 100) << "\n";
  std::cout << "--------------------------------------------------\n";

  max_user_index += 1;
}

void print_user_data(const user * const &users, const unsigned int &user_index) {
  if(users == NULL) {
    std::cerr << "Problema na inicialização dos usuários. Encerrando o programa...\n";
    abort();
  }

  if(users[user_index].name.empty()) {
    std::cout << "O usuário com ID " << user_index + 1 << " não existe ou foi deletado.\n";
    return;
  }

  std::cout << "\n------ Dados do Usuário (ID: " << user_index + 1 << ") ------\n"
            << "Nome: " << users[user_index].name << "\n"
            << "Idade: " << users[user_index].age << " anos\n"
            << "Saldo: R$ " << (users[user_index].balance_in_cents / 100)
            << ",";
  if((users[user_index].balance_in_cents % 100) < 10) {
    std::cout << "0";
  }
  std::cout << (users[user_index].balance_in_cents % 100) << "\n";
  std::cout << "---------------------------------------\n";
}

void transfer_between_users(user * const &users, 
                            const unsigned int &sender_index, 
                            const unsigned int &receiver_index, 
                            const unsigned long long &transfer_amount
) {
  if(users == NULL) {
    std::cerr << "Problema na inicialização dos usuários. Encerrando o programa...\n";
    abort();
  }
  
  if(sender_index == receiver_index) {
    std::cout << "Você não pode fazer uma transferencia para si mesmo.\n"
              << "Transferencia cancelada.\n";
    return;
  }

  if(users[sender_index].name.empty()) {
    std::cout << "O usuário com ID " << sender_index + 1 << " não existe ou foi deletado.\n"
              << "Não é possível realizar a transferencia.\n";
    return;
  }

  if(users[receiver_index].name.empty()) {
    std::cout << "O usuário com ID " << receiver_index + 1 << " não existe ou foi deletado.\n"
              << "Não é possível realizar a transferencia.\n";
    return;
  }

  if(users[sender_index].balance_in_cents < transfer_amount) {
    std::cout << "O usuário " << users[sender_index].name << " com ID "
              << sender_index + 1 << " não possuí saldo suficiente para realizar a transferencia.\n";
    return;
  }

  users[sender_index].balance_in_cents -= transfer_amount;
  users[receiver_index].balance_in_cents += transfer_amount;

  std::cout << "\n-------------- Transferência Realizada com Sucesso --------------\n"
            << "Valor: R$ " << (transfer_amount / 100)
            << ",";
  if((transfer_amount % 100) < 10) {
    std::cout << "0";
  }
  std::cout << (transfer_amount % 100) << "\n"
            << "De: " << users[sender_index].name << ", ID: " << sender_index + 1 << "\n"
            << "Para: " << users[receiver_index].name << ", ID: " << receiver_index + 1 << "\n"
            << "-----------------------------------------------------------------\n";
}

void remove_user(user * const &users, 
                 unsigned int &max_user_index,
                 const unsigned int &user_index
) {
  if(users == NULL) {
    std::cerr << "Problema na inicialização dos usuários. Encerrando o programa...\n";
    abort();
  }

  if(users[user_index].name.empty()) {
    std::cout << "O usuário com ID " << user_index + 1 << " já está vazio.\n";
    return;
  }

  users[user_index] = {
    "",
    0,
    0
  };

  std::cout << "\n------ Usuário Removido com Sucesso ------\n"
            << "ID do Usuário removido: " << user_index + 1 << "\n"
            << "------------------------------------------\n";
}

void save_users_to_file(const user * const &users, 
                        const unsigned int &max_user_index, 
                        const unsigned int &current_capacity
) {
  if(users == NULL) {
    std::cerr << "Problema na inicialização dos usuários. Encerrando o programa...\n";
    abort();
  }

  std::ofstream file("users.txt", std::ios::out);

  if(file.fail()) {
    std::cout << "Erro ao escrever \"users.txt\".\n";
    file.close();
    return;
  }

  unsigned int user_amount = 0;
  std::string users_data;
  std::ostringstream oss_users_data(users_data);

  for(unsigned int i = 0; i < current_capacity; i++) {
    if(!users[i].name.empty()) {
      user_amount++;
      oss_users_data << users[i].name << ","
           << users[i].age << ","
           << (users[i].balance_in_cents / 100)
           << ".";
      if((users[i].balance_in_cents % 100) < 10) {
        oss_users_data << "0";
      }
      oss_users_data << (users[i].balance_in_cents % 100) << "\n";
    }
  }
  file << user_amount << '\n';
  file << oss_users_data.str();
  file.close();
}

void load_users_from_file(user *&users, 
                     unsigned int &current_capacity,
                     unsigned int &max_user_index
) {
  if(users == NULL) {
    std::cerr << "Problema na inicialização dos usuários. Encerrando o programa...\n";
    abort();
  }
  
  std::ifstream file("users.txt", std::ios::in);

  if(file.fail()) {
    std::cout << "Arquivo users.txt não encontrado.\n";
    file.close();
    return;
  }

  std::string temp;

  // unsigned short total_users;

  if(!std::getline(file, temp)) {
    std::cerr << "Erro: não foi possível extrair a quantidade de usuários.\n";
    file.close();
    return;
  }

  if(!is_input_valid(temp, 6, 0, false)) {
    std::cerr << "Erro: não foi possível extrair a quantidade de usuários.\n";
    file.close();
    return;
  }

  // total_users = static_cast<unsigned short>(std::stoi(temp));

  std::string line;

  std::string name;
  unsigned short age;
  unsigned long long balance_in_cents;

  while(file) {
    std::getline(file, line);

    if(line.empty()) continue;

    std::istringstream ss(line);

    if(!std::getline(ss, name, ',')) {
      std::cout << "Erro: não foi possível extrair o nome." << std::endl;
      return;
    }
    
    if(!is_username_valid(name)) return;
    
    if(!std::getline(ss, temp, ',')) {
      std::cout << "Erro: não foi possível extrair a idade." << std::endl;
      return;
    }

    if(!is_user_age_valid(temp, age)) return;

    if(!std::getline(ss, temp, ',')) {
      std::cout << "Erro: não foi possível extrair o saldo." << std::endl;
      return;
    }

    if(!is_monetary_value_valid(temp, balance_in_cents)) return;

    register_user(users, current_capacity, max_user_index, name, age, balance_in_cents);
  }

  if(!file.eof()) {
    std::cerr << "Erro de leitura no arquivo.\n";
    file.close();
    return;
  }
  file.close();
}
