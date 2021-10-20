#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>
// trabalho final de FUP, antes de conhecer POO

//Agencia Bancária
/* 
-[1.0 P] Cadastrar um cliente com idCliente.
idClientes devem ser únicos no sistema.
Quando o cliente é cadastrado no sistema, automaticamente é aberta uma conta
-[1.0 P] Abrir novas contas para clientes.
Cada conta deve receber do sistema um número único no sistema.
Quando o cliente é cadastrado no sistema, automaticamente é aberta uma conta para ele. 
-[1.0 E] Cada cliente pode ter até 2 contas ativas.
    [1.0 E] Encerrar contas de clientes
    Contas encerradas não podem mais realizar nenhuma operação. Elas não são apagadas. Apenas desabilitadas.
    Contas só podem ser desabilitadas se tiverem com saldo zerado.
[1.0 P] Mostrar os clientes e suas contas ativas.
[1.0 E] Mostre ordenado por nome do cliente.
[1.0 P] Fazer login e logout no sistema.
Um único cliente pode estar logado de cada vez.
[1.0 P] Mostrar dados do cliente, suas contas e saldos.
Cliente precisa estar logado.
Se não tiver implementado encerrar conta, não mostre o status(ativa/inativa).
[1.0 P] Realizar operações de saldo, saque, depósito e extrato, tal como foi implementado no primeiro trabalho.
Cliente só pode fazer operações em suas próprias contas.
Cliente precisa estar logado.
[1.0 P] Realizar operações de transferência entre contas.
No extrato essa ação deve vir com descrição “transferência para/de $conta”.
Verifique se há saldo para efetuar a transação.
Cliente precisa estar logado e possuir a conta de débito.
[1.0 P] Faça um código de inicialização para que seu sistema já inicie com alguns clientes, contas e operações
realizadas.
 */

int max_clientes = 30; 

typedef struct{
    bool status;
    int saldo;
    int id_conta;
}Conta;

typedef struct{
    int id;
    char nome[20];
    Conta contas[2];
    int qtd_contas_cliente;
}Cliente;

bool id_existente(int id, Cliente *clientes, int *qtd_clientes){
    for (int i = 0; i < *qtd_clientes; i++)
        if (clientes[i].id == id)
            return true;

    return false;
}
bool id_conta_existente(int id,Cliente *clientes, int *qtd_clientes){
    for (int i = 0; i < *qtd_clientes; i++){
        if (clientes[i].contas[0].id_conta == id){
            return true;
        }
        if (clientes[i].contas[1].status == true && clientes[i].contas[1].id_conta == id){
            return true;
        }       
    }
    return false;
}

int addCliente(Cliente *clientes, int *qtd_clientes, int *qtd_contas_total){
    if (*qtd_clientes >= max_clientes){
        puts("invalido - limite de clientes alcancado");
        return 0;
    }
    
    int id = rand() % max_clientes;
    while (id_existente(id, clientes, qtd_clientes))
        id = rand() % max_clientes;
    
    int id_conta = rand() % max_clientes + 1;
    while (id_conta_existente(id_conta, clientes, qtd_clientes))
        id_conta = rand() % 1000;

    char nome[20];    
    puts("digite o nome");
    scanf("%s", nome);

    clientes[*qtd_clientes].id = id;
    strcpy(clientes[*qtd_clientes].nome, nome);
    clientes[*qtd_clientes].contas[0].status = true;
    clientes[*qtd_clientes].qtd_contas_cliente = 1;
    clientes[*qtd_clientes].contas[0].id_conta = id_conta;
    clientes[*qtd_clientes].contas[0].saldo = 0;

    *qtd_contas_total += 1;
    if (*qtd_clientes < max_clientes)
        *qtd_clientes += 1;

    puts("done");
    return 0;
}

bool ja_tem_duas(int pos_id_no_vet, Cliente *clientes, int *qtd_clientes){
    if (clientes[pos_id_no_vet].qtd_contas_cliente >= 2)
        return true; 
    else
        return false;
    
}

int abrir_conta(Cliente *clientes, int *qtd_clientes){

    int id_cliente = 0, pos_id_no_vet = 0, id_sort = 0;
    puts("Digite o id cliente para adicinar a ele uma conta");
    scanf("%d", &id_cliente);

    while (!id_existente(id_cliente, clientes, qtd_clientes)){
        puts("id invalido digite outro");
        scanf("%d", &id_cliente);
    }

    for (int i = 0; i < *qtd_clientes; i++){
        if(clientes[i].id == id_cliente){
            pos_id_no_vet = i;
            break;
        }else if (i == *qtd_clientes - 1){
            puts("Cliente não existe (L-126)");
            return 0;
        }
    }

    if (ja_tem_duas(pos_id_no_vet, clientes, qtd_clientes)){
        puts("operacao invalida - ja tem duas contas (L-111)");
        return 0;
    }

    
    
    clientes[pos_id_no_vet].qtd_contas_cliente += 1; 
    clientes[pos_id_no_vet].contas[1].status = true;
    clientes[pos_id_no_vet].contas[1].saldo = 0;

    id_sort = rand() % 1000;
    while (id_conta_existente(id_sort, clientes, qtd_clientes))
        id_sort = rand() % 1000;
    
    clientes[pos_id_no_vet].contas[1].id_conta = id_sort;
    puts("done");
    return 0;
}

void show_all(Cliente *clientes, int *qtd_clientes){
    for (int i = 0; i < *qtd_clientes; i++){
        printf("--nome = %s\n id = %d\n", clientes[i].nome, clientes[i].id);
        for (int j = 0; j < clientes[i].qtd_contas_cliente; j++)
            printf(" conta%d:\n  saldo = %d\n  status = %d\n  id-conta = %d\n\n", j+1, clientes[i].contas[j].saldo, clientes[i].contas[j].status, clientes[i].contas[j].id_conta);

    }
}
void login(bool *adm,bool *user,int *id_user, Cliente *clientes,int *qtd_clientes, char *comand){
    puts("id_Usuario:");
    scanf("%s",comand);

    if (!strcmp(comand, "adm")){
        *adm = true;
        puts("done");
        return;
    }
    while (!id_existente(atoi(comand), clientes, qtd_clientes)){
        puts("id incorreto digite outro");
        scanf("%s", comand);
        if (!strcmp(comand, "adm")){
            *adm = true;
            puts("done");
            return;
        }
    
    }
        *id_user = atoi(comand);
        *user = true;
    puts("done");
}

int get_pos_vet_by_id(int *pos_id_no_vet, int id_user,Cliente *clientes, int qtd_clientes){
    for (int i = 0; i < qtd_clientes; i++){
        if(clientes[i].id == id_user){
            *pos_id_no_vet = i;
            return i;
        }else if (i == qtd_clientes - 1){
            puts("Cliente não existe (L-189)");
        }
    }
    return 0;
}

void user_depositar(int id_pos, Cliente *clientes){
    int valor = 0, conta = 1;
    char answer = ' ';
    if (clientes[id_pos].contas[1].status == true){
        puts("digite a conta (1 ou 2)");
        scanf("%d", &conta);
    }
    while (clientes[id_pos].contas[1].status == true && (conta > 2 && conta < 1)){
        puts("valor invalido, digite 1 ou 2");
        scanf("%d", &conta);
    }

    if (clientes[id_pos].contas[conta - 1].status == false)
        puts("conta desativada, gostaria de alternar para sua conta existente?(s/n)");
    
    while (clientes[id_pos].contas[conta - 1].status == false){
        scanf(" %c", &answer);
        if (answer == 's'){
            conta = 1;
        }else if (answer == 'n'){
            puts("oborted");
            return;
        }else{
            puts("digite 's' ou 'n'");    
        }   
    }

    puts("digite um valor");
    scanf("%d", &valor);
    while (valor <= 0){
        puts("valor invalido, digite um valor maior que 0");
        scanf("%d", &valor);
    }

    clientes[id_pos].contas[conta - 1].saldo += valor;
    puts("done");
}


void user_tansacao(int id_position, Cliente *clientes, int qtd_clientes){
    
    if (qtd_clientes < 2){
        puts("invalido deve haver mais de um cliente");
        return;
    }
    int conta = 1, pos_conta_destino = 0, id_conta_destino = 0, valor = 0;
    if (clientes[id_position].contas[1].status == true){
        puts("escolha uma de suas contas (1 ou 2)");
        scanf("%d", &conta);
    }
    
    puts("digite o id da conta destino");
    scanf("%d", &id_conta_destino);
    while (!id_conta_existente(id_conta_destino, clientes, &qtd_clientes)){
        puts("id invalido digite outro");
        scanf("%d", &id_conta_destino);
    }
    for (int i = 0; i < qtd_clientes; i++){
        for (int j = 0; j < clientes[i].qtd_contas_cliente; j++){
            if(clientes[i].contas[j].id_conta == id_conta_destino){
                pos_conta_destino = j;
                i = qtd_clientes;
            }
        }
    }
    
    printf("sua conta tem %dR$ de saldo\ndigite o quanto vai transferir\n", clientes[id_position].contas[conta-1].saldo);
    scanf("%d", &valor);
    while (valor > clientes[id_position].contas[conta-1].saldo){
        printf("valor invalido\na conta tem %dR$ de saldo\ndigite outro valor\n", clientes[id_position].contas[conta-1].saldo);        
        scanf("%d", &valor);
    }
    clientes[id_position].contas[conta-1].saldo -= valor;
    clientes[id_conta_destino].contas[pos_conta_destino].saldo += valor;
    puts("done");
    
}

int main(){
    srand(time(NULL));
    Cliente clientes[max_clientes];
    for (int i = 0; i < max_clientes; i++){
        clientes[i].contas[1].status = false;
        clientes[i].qtd_contas_cliente = 0;
    }
    
    int qtd_clientes = 0;
    int qtd_contas_total = 0;
    char comand[15];
    int id_user = 0;
    int id_position = 0;
    //int pos_id_no_vet = 0;
    bool adm = false;
    bool user = false;

    while (1){
        puts("digite o id ou adm");
        login(&adm, &user, &id_user, clientes, &qtd_clientes, comand);
        get_pos_vet_by_id(&id_position ,id_user, clientes, qtd_clientes);
        

        while (user){
            
            scanf("%s", comand);

            if (!strcmp(comand, "logout")){
                user = false;
                puts("done");
                break;
            }
            
            if (!strcmp(comand, "depositar")){
                user_depositar(id_position, clientes);
            }
            if (!strcmp(comand, "sacar")){
                //user_saque(id_position, clientes);
            }
            if (!strcmp(comand, "extrato")){
                
            }
            if (!strcmp(comand, "transferir")){
                user_tansacao(id_position, clientes, qtd_clientes);
            }
            if (!strcmp(comand, "desativar")){
                
            }
            //puts("comando nao existente");
        }


        while (adm){
            scanf("%s", comand);
            
            if (!strcmp(comand, "logout")){
                adm = false;
                puts("done");
                break;
            }

            if (!strcmp(comand, "add")){
                addCliente(clientes, &qtd_clientes, &qtd_contas_total);
            }

            if (!strcmp(comand, "show")){
                show_all(clientes, &qtd_clientes);
            }
            if (!strcmp(comand, "2")){
                abrir_conta(clientes, &qtd_clientes);
            }
            //puts("comando nao existente");

        }

        
    }
}
