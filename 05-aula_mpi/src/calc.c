#include <mpi.h> //biblioteca do OpenMPI
#include <stdio.h>

// processo 0 (gerente): lê a conta, escolhe o worker, envia os dados e exibe o resultado
void dados(){
    float v1, v2, res; // v1 e v2 são os operandos, res é o resultado devolvido pelo worker
    char op; // operador da conta (+, -, *, /)

    printf("Entre com a expressao: ");
    scanf("%f %c %f", &v1, &op, &v2); // lê no formato "numero operador numero", ex: 3 + 4

    // escolhe o rank do worker: + -> 1, - -> 2, * -> 3, qualquer outro (/) -> 4
    int func_rank = (op == '+') ? 1 : (op == '-') ? 2 : (op == '*') ? 3 : 4 ;

    // envia operador e operandos para o worker escolhido com a tag 0
    MPI_Send(&op, 1, MPI_CHAR,  func_rank, 0, MPI_COMM_WORLD);
    MPI_Send(&v1, 1, MPI_FLOAT, func_rank, 0, MPI_COMM_WORLD);
    MPI_Send(&v2, 1, MPI_FLOAT, func_rank, 0, MPI_COMM_WORLD);

    // fica bloqueado até receber o resultado do worker escolhido
    MPI_Recv(&res, 1, MPI_FLOAT, func_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("%f %c %f = %f (rank %d)\n",v1, op, v2, res, func_rank); // exibe a conta, o resultado e quem calculou
}

// processo 1 (worker): realiza a soma
void soma(){
    float v1, v2, res;
    char op;

    // recebe do rank 0 o operador e os operandos, na mesma ordem em que foram enviados
    MPI_Recv(&op, 1, MPI_CHAR,  0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&v1, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&v2, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    res = v1 + v2; // calcula a soma

    MPI_Send(&res, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD); // devolve o resultado para o rank 0
}

// processo 2 (worker): realiza a subtração
void subtracao(){
    float v1, v2, res;
    char op;

    // recebe do rank 0 o operador e os operandos
    MPI_Recv(&op, 1, MPI_CHAR,  0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&v1, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&v2, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    res = v1 - v2; // calcula a subtração

    MPI_Send(&res, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD); // devolve o resultado para o rank 0
}

// processo 4 (worker): realiza a divisão
void divisao(){
    float v1, v2, res;
    char op;

    // recebe do rank 0 o operador e os operandos
    MPI_Recv(&op, 1, MPI_CHAR,  0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&v1, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&v2, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    if(v2 != 0) // só divide se o divisor for diferente de zero
        res = v1 / v2; // calcula a divisão
    else
        res = -0; // divisão por zero: devolve 0 para evitar resultado inválido

    MPI_Send(&res, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD); // devolve o resultado para o rank 0
}

// processo 3 (worker): realiza a multiplicação
void multiplicacao(){
    float v1, v2, res;
    char op;

    // recebe do rank 0 o operador e os operandos
    MPI_Recv(&op, 1, MPI_CHAR,  0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&v1, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&v2, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    res = v1 * v2; // calcula a multiplicação

    MPI_Send(&res, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD); // devolve o resultado para o rank 0
}

typedef void(*func)(); // "func" é um apelido para ponteiro de função sem parâmetros e sem retorno

// vetor de funções: a posição no vetor corresponde ao rank do processo
// 0 -> dados | 1 -> soma | 2 -> subtracao | 3 -> multiplicacao | 4 -> divisao
func funcs[] = {dados, soma, subtracao, multiplicacao, divisao};

int main() {
    int rank, size; // define variaveis de tamanho e rank do processo
    MPI_Init(NULL, NULL); //Inicializa o ambiente do MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //numero do worker em exec
    MPI_Comm_size(MPI_COMM_WORLD, &size); //quantidade de processos disponiveis

    int count = 0; // contador de repetições
    do{
        funcs[rank](); // cada processo executa a função correspondente ao seu rank
    }while(count < 10); // atenção: count nunca é incrementado, então o laço nunca termina

    MPI_Finalize(); // encerra o ambiente MPI
    return 0;
}