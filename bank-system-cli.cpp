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

// ======================= Validação de Dados =======================
bool is_input_valid(const std::string &input, 
                   unsigned short max_integer_digits, 
                   unsigned short max_decimal_digits,
                   bool accept_negative);
bool is_username_valid(const std::string &name); 
bool is_user_age_valid(const std::string &temp_input, unsigned short &age);
bool is_monetary_value_valid(std::string &temp_input, unsigned long long &monetary_value);

// ======================= Operações de Usuário =======================
unsigned int get_valid_user_index(const user *users, unsigned int current_capacity);
void get_user_data(std::string &name, 
                   unsigned short &age,
                   unsigned long long &balance_in_cents);
void register_user(user *&users, 
                   unsigned int &current_capacity, 
                   unsigned int &last_user_index, 
                   const std::string &name, 
                   unsigned short age, 
                   unsigned long long balance_in_cents);
void print_user_data(const user *users, unsigned int user_index);
void remove_user(user *users, 
                 unsigned int &last_user_index,
                 unsigned int user_index);

// ======================= Operações Financeiras =======================
void transfer_between_users(user *users, 
                            unsigned int sender_index, 
                            unsigned int receiver_index, 
                            unsigned long long transfer_amount);

// ======================= Persistência de Dados =======================
void save_users_to_file(const user *users, 
                        unsigned int last_user_index, 
                        unsigned int current_capacity);
void load_users_from_file(user *&users, 
                          unsigned int &last_user_index,
                          unsigned int &current_capacity);

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
  unsigned int last_user_index = 0;
  unsigned short menu_choice = 0;
  bool running = true;

  while(running) {
    std::cout << "\n=============== MENU ===============\n"
              << "1. Inserir novo usuário\n"
              << "2. Inserir vários usuários\n"
              << "3. Buscar usuário por ID\n"
              << "4. Transferência entre usuários\n"
              << "5. Remover usuários por ID\n"
              << "6. Carregar arquivo para a memória\n"
              << "7. Sair e salvar\n"
              << "====================================\n\n"
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

        std::cout << "\n====== Inserir Novo Usuário ======\n";
        get_user_data(name, age, balance_in_cents);
        std::cout << "=================================-\n";
        register_user(users, current_capacity, last_user_index, name, age, balance_in_cents);
        break;
      }
      case 2: {
        // Inserir vários usuários
        std::string name;
        unsigned short age = 0;
        unsigned long long balance_in_cents = 0;
        unsigned int insert_amount = 0;

        std::cout << "\n============- Inserir Vários Usuários ============-\n"
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
          std::cout << "\n====== Dados para Usuário " << i + 1 << " de " << insert_amount << " ======\n";
          get_user_data(name, age, balance_in_cents);
          std::cout << "=======================================-\n";
          register_user(users, current_capacity, last_user_index, name, age, balance_in_cents);
        }
        break;
      }
      case 3: {
        // Bucar usuário por ID
        if(last_user_index == 0) {
          std::cout << "\n=== Buscar Usuário por ID ===\n"
                    << "Não há usuários registrados.\n";
          break;
        }

        unsigned int user_index = 0;
        std::cout << "\n=== Buscar Usuário por ID ===\n"
                  << "ID do usuário: ";

        user_index = get_valid_user_index(users, current_capacity);

        print_user_data(users, user_index);
        break;
      }
      case 4: {
        // Transferência entre usuários
        unsigned int sender_index = 0, receiver_index = 0;
        unsigned long long transfer_amount = 0;

        std::cout << "\n=== Transferência Entre Usuários ===\n"
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
        std::cout << "\n====== Remoção de Usuário por ID ======\n"
                  << "Insira o ID do usuário a ser removido: ";
        target_user_index = get_valid_user_index(users, current_capacity);
        remove_user(users, last_user_index, target_user_index);
        break;
      }
      case 6: {
        // Carregar arquivo para a memória
        load_users_from_file(users, last_user_index, current_capacity);
        break;
      }
      case 7: {
        // Sair e salvar
        save_users_to_file(users, last_user_index, current_capacity);
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

/**
 * @brief Valida se uma string representa um número decimal corretamente formatado.
 * 
 * Verifica se:
 * - Formato numérico válido (dígitos, sinal e ponto decimal)
 * - Quantidade máxima de dígitos nas partes inteira e decimal
 * 
 * @param input String da entrada a ser validada
 * @param max_integer_digits Número máximo permitido de dígitos na parte inteira
 * @param max_decimal_digits Número máximo permitido de dígitos na parte decimal
 * @param accept_negative Aceita entrada de números negativos ?
 * @return bool true se a entrada for válida, false caso contrário
 * 
 * @note Comportamentos especiais:
 * - Sinais (+/-) são opcionais e removidos antes da validação
 * - Zeros insignificantes à esquerda na parte inteira são ignorados
 * - Zeros insignificantes à direita na parte decimal não são ignorados
 */
bool is_input_valid(const std::string &input, 
                    unsigned short max_integer_digits, 
                    unsigned short max_decimal_digits,
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

/**
 * @brief Valida o nome do usuário.
 * 
 * @param name Nome a ser validado
 * @return true se válido, false se inválido
 * 
 * @note Critérios:
 * - Sem falhas de leitura
 * - Não vazio
 * - Sem vírgulas
 * - Tamanho <= MAX_NAME_LENGTH
 * @note Exibe mensagens de erro específicas para cada critério
 */
bool is_username_valid(const std::string &name) {
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

/**
 * @brief Valida e converte idade do usuário.
 * 
 * @param temp_input Entrada de texto
 * @param age Idade convertida
 * @return true se válido, false se inválido
 * 
 * @note Critérios:
 * - Sem falhas de leitura
 * - Formato numérico válido
 * - Entre MIN_USER_AGE e MAX_USER_AGE
 * @note Usa is_input_valid() para verificação de inteiros, decimais e sinal
 */
bool is_user_age_valid(const std::string &temp_input, unsigned short &age) {
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

/**
 * @brief Valida e converte valor monetário para centavos.
 * 
 * @param temp_input Entrada de texto
 * @param[out] monetary_value Valor em centavos
 * @return true se válido, false se inválido
 * 
 * @note Critérios:
 * - Sem falhas de leitura
 * - Formato válido (até 10 dígitos + 2 decimais)
 * - Converte removendo ponto decimal ou multiplicando por 100
 * @note Usa is_input_valid() para verificação de inteiros, decimais e sinal
 */
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

/**
 * @brief Obtém um índice do vetor de usuários válido via entrada padrão.
 * 
 * @param users Vetor de usuários
 * @param current_capacity Capacidade atual do vetor
 * 
 * @return Índice válido correspondente ao ID fornecido
 * 
 * @note Valida:
 *   - Formato numérico da entrada
 *   - Intervalo válido (1 até current_capacity)
 *   - Conversão de ID para índice (ID-1)
 */
unsigned int get_valid_user_index(const user *users, unsigned int current_capacity) {
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
 * @brief Solicita e valida dados de um usuário via entrada padrão.
 * 
 * @param name Nome do usuário
 * @param age Idade do usuário
 * @param balance_in_cents Saldo em centavos
 * 
 * @note Repete cada solicitação até obter entrada válida
 * @note Usa funções específicas para validação de cada campo
 */
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

/**
 * @brief Registra um novo usuário no vetor dinâmico.
 * 
 * @param users Vetor de usuários (pode ser realocado)
 * @param current_capacity Capacidade atual (dobrada se necessário)
 * @param last_user_index Último índice livre (incrementado após registro)
 * @param name Nome do novo usuário
 * @param age Idade do novo usuário
 * @param balance_in_cents Saldo inicial em centavos
 * 
 * @note Expande o array (dobrando capacidade) se necessário
 * @note Exibe mensagem de erro em falha de alocação
 * @note Exibe mensagem de confirmação com detalhes do usuário registrado
 */
void register_user(user *&users, 
                   unsigned int &current_capacity, 
                   unsigned int &last_user_index, 
                   const std::string &name, 
                   unsigned short age, 
                   unsigned long long balance_in_cents
) {
  if(last_user_index >= current_capacity) { 

    user *new_vector = new user[(current_capacity * 2)];
    if(new_vector == NULL) {
      std::cerr << "\n========================== ERRO ==========================\n"
                << "Falha ao alocar memória para expandir o vetor de usuários.\n"
                << "O usuário " << name << " com ID " << last_user_index
                << " não pôde ser registrado.\n"
                << "=========================================================-\n";
      return;
    }

    for(unsigned int i = 0; i < current_capacity; i++) {
      if(!users[i].name.empty()) {
        new_vector[i] = users[i];
      } else {
        new_vector[i] = {"",0,0};
      }
    }

    current_capacity *= 2;
    delete[] users;
    users = new_vector;
  }

  users[last_user_index] = {
    name,
    age,
    balance_in_cents
  };

  std::cout << "\n========= Usuário Registrado com Sucesso =========\n"
            << "ID do Usuário: " << last_user_index + 1 << "\n"
            << "Nome:  " << users[last_user_index].name << "\n"
            << "Idade: " << users[last_user_index].age << " anos\n"
            << "Saldo: R$ " << (users[last_user_index].balance_in_cents / 100)
            << ",";
  if((users[last_user_index].balance_in_cents % 100) < 10) {
    std::cout << "0";
  }
  std::cout << (users[last_user_index].balance_in_cents % 100) << "\n";
  std::cout << "==================================================\n";

  last_user_index += 1;
}

/**
 * @brief Exibe os dados de um usuário.
 * 
 * @param users Vetor de usuários
 * @param user_index Índice do usuário
 */
void print_user_data(const user *users, unsigned int user_index) {
  if(users[user_index].name.empty()) {
    std::cout << "O usuário com ID " << user_index + 1 << " não existe ou foi deletado.\n";
    return;
  }

  std::cout << "\n====== Dados do Usuário (ID: " << user_index + 1 << ") ======\n"
            << "Nome: " << users[user_index].name << "\n"
            << "Idade: " << users[user_index].age << " anos\n"
            << "Saldo: R$ " << (users[user_index].balance_in_cents / 100)
            << ",";
  if((users[user_index].balance_in_cents % 100) < 10) {
    std::cout << "0";
  }
  std::cout << (users[user_index].balance_in_cents % 100) << "\n";
  std::cout << "=======================================\n";
}

/**
 * @brief Remove um usuário do vetor.
 * 
 * @param users Vetor de usuários
 * @param last_user_index Último índice válido
 * @param user_index Índice do usuário a remover
 * 
 * @note Atualiza last_user_index se o usuário removido for o último
 */
void remove_user(user *users, 
                 unsigned int &last_user_index,
                 unsigned int user_index
) {
  if(users[user_index].name.empty()) {
    std::cout << "O usuário com ID " << user_index + 1 << " já está vazio.\n";
    return;
  }

  std::string deleted_user_name = users[user_index].name;

  users[user_index] = {"", 0, 0};

  if(user_index == last_user_index - 1) last_user_index--;

  std::cout << "\n====== Usuário Removido com Sucesso ======\n"
            << "Nome: " << deleted_user_name << "\n"
            << "ID:   " << user_index + 1 << "\n"
            << "==========================================\n";
}

/**
 * @brief Realiza transferência entre dois usuários.
 * 
 * @param users Vetor de usuários
 * @param sender_index Índice do usuário remetente
 * @param receiver_index Índice do usuário destinatário
 * @param transfer_amount Valor a transferir (em centavos)
 * 
 * @note Validações realizadas:
 * - Não permite auto-transferência
 * - Verifica existência dos usuários
 * - Confere saldo suficiente do remetente
 * @note Atualiza saldos e exibe confirmação em caso de sucesso
 */
void transfer_between_users(user *users, 
                            unsigned int sender_index, 
                            unsigned int receiver_index, 
                            unsigned long long transfer_amount
) {
  if(sender_index == receiver_index) {
    std::cout << "Não é possível realizar transferências para o próprio usuário.\n"
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

  std::cout << "\n============== Transferência Realizada com Sucesso ==============\n"
            << "Valor: R$ " << (transfer_amount / 100) << ",";
  if((transfer_amount % 100) < 10) {
    std::cout << "0";
  }
  std::cout << (transfer_amount % 100) << "\n"
            << "Remetente: " << users[sender_index].name << "( ID: " << sender_index + 1 << " )\n"
            << "Para: " << users[receiver_index].name << "( ID: " << receiver_index + 1 << " )\n"
            << "=================================================================\n";
}

/**
 * @brief Salva os usuários em um arquivo de texto "users.txt".
 * 
 * @param users Vetor de usuários a ser salvo
 * @param last_user_index Último índice válido do vetor
 * @param current_capacity Capacidade atual do vetor
 * 
 * @note Formato do arquivo:
 *   - 1ª linha: total de usuários
 *   - Demais linhas: "Nome,Idade,Saldo" (saldo em formato real.centavos)
 */
void save_users_to_file(const user *users, 
                        unsigned int last_user_index, 
                        unsigned int current_capacity
) {
  std::ofstream file("users.txt", std::ios::out);

  if(file.fail()) {
    std::cerr << "Erro ao escrever arquivo \"users.txt\".\n";
    file.close();
    return;
  }

  unsigned int user_amount = 0;
  std::ostringstream oss_users_data;

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

  if(file.fail()) {
    std::cerr << "Erro durante escrita do arquivo \"users.txt\".\n";
    file.close();
    return;
  }

  file.close();
  std::cout << "Dados salvos com sucesso em \"users.txt\".\n";
}

/**
 * @brief Carrega usuários do arquivo "users.txt" para o vetor dinâmico.
 * 
 * Lê e valida os dados dos usuários no arquivo, registrando-os no vetor. 
 * O arquivo deve ter: 1a linha com total de usuários e demais linhas 
 * no formato "Nome,Idade,Saldo".
 * 
 * @param users Vetor dinâmico de usuários
 * @param last_user_index Último índice válido do vetor
 * @param current_capacity Capacidade atual do vetor
 * 
 * @note Em caso de erro no arquivo ou dados inválidos, exibe mensagem e aborta o carregamento.
 */
void load_users_from_file(user *&users, 
                          unsigned int &last_user_index,
                          unsigned int &current_capacity
) {
  std::ifstream file("users.txt", std::ios::in);

  if(file.fail()) {
    std::cerr << "Arquivo users.txt não encontrado.\n";
    file.close();
    return;
  }

  std::string temp;

  if(!std::getline(file, temp)) {
    std::cerr << "Erro: não foi possível extrair a quantidade de usuários.\n";
    file.close();
    return;
  }

  if(!is_input_valid(temp, 6, 0, false)) {
    std::cerr << "Erro: formato inválido para total de usuários.\n";
    file.close();
    return;
  }

  std::string line;
  unsigned int line_number = 0;

  std::string name;
  unsigned short age;
  unsigned long long balance_in_cents;

  while(std::getline(file, line)) {
    line_number++;
    if(line.empty()) continue;

    std::istringstream ss(line);

    if(!std::getline(ss, name, ',')) {
      std::cerr << "Erro: não foi possível extrair o nome. Linha: " << line_number << ".\n";
      return;
    }
    if(!is_username_valid(name)) return;
    
    if(!std::getline(ss, temp, ',')) {
      std::cerr << "Erro: não foi possível extrair a idade. Linha: " << line_number << ".\n";
      return;
    }
    if(!is_user_age_valid(temp, age)) return;

    if(!std::getline(ss, temp, ',')) {
      std::cerr << "Erro: não foi possível extrair o saldo. Linha: " << line_number << ".\n";
      return;
    }
    if(!is_monetary_value_valid(temp, balance_in_cents)) return;

    register_user(users, current_capacity, last_user_index, name, age, balance_in_cents);
  }

  if(!file.eof()) {
    std::cerr << "Erro de leitura no arquivo.\n";
    file.close();
    return;
  }
  file.close();
}
