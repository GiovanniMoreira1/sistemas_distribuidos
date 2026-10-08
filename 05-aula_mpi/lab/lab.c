#include <mpi.h> //biblioteca do OpenMPI
#include <stdio.h>
#include <stdlib.h> // necessário para malloc e free
#include <unistd.h> // necessário para sleep

int main(int argc, char** argv) {
    int rank, size; // define variaveis de tamanho e rank do processo
    int *array_local, *array; // array_local: pedaço de cada processo | array: array completo (só existe no rank 0)
    int soma_local, soma; // soma_local: soma do pedaço de cada processo | soma: soma total (só vale no rank 0)
    int tamanho = 16; // tamanho total do array (alterado de 8 para 16)
    int tamanho_local; // quantidade de elementos que cada processo vai receber

    MPI_Init(&argc, &argv); //Inicializa o ambiente do MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //numero do worker em exec
    MPI_Comm_size(MPI_COMM_WORLD, &size); //quantidade de processos disponiveis

    tamanho_local = tamanho / size; // divide o array igualmente entre os processos
    array_local = (int*)malloc(tamanho_local * sizeof(int)); // todos os processos reservam memória para o seu pedaço

    if (rank == 0) { // somente o processo 0 cria e preenche o array completo
        array = (int*)malloc(tamanho * sizeof(int)); // reserva memória para o array completo
        printf("Array: ");
        for (int i = 0; i < tamanho; i++) {
            array[i] = i + 1; // preenche com 1, 2, 3, ... , tamanho
            printf("%d ", array[i]); // exibe o array completo
        }
        printf("\n");
    }

    // distribui o array: o rank 0 envia um pedaço de tamanho_local elementos para cada processo (inclusive ele mesmo)
    // parâmetros: (origem, qtd enviada a cada um, tipo, destino, qtd recebida, tipo, rank que envia, comunicador)
    MPI_Scatter(array, tamanho_local, MPI_INT, array_local, tamanho_local, MPI_INT, 0, MPI_COMM_WORLD);

    soma_local = 0; // zera a soma antes de acumular
    printf("Array recebido pelo rank %2d: ", rank); //exibe qual processo recebeu o pedaço
    for (int i = 0; i < tamanho_local; i++) {
        printf("%3d ", array_local[i]); // exibe os elementos recebidos
        soma_local += array_local[i]; // soma os elementos do pedaço local
    }
    printf("-- soma local: %4d\n", soma_local); // exibe a soma parcial deste processo

    // junta as somas parciais: soma o soma_local de todos os processos e guarda o resultado em "soma" no rank 0
    MPI_Reduce(&soma_local, &soma, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) { // somente o processo 0 possui o resultado final
        sleep(1); // espera 1 segundo para os prints dos outros processos aparecerem antes
        printf("Soma do array: %d\n", soma); // exibe a soma calculada em paralelo
        printf("Valor esperado: %d\n", tamanho * (tamanho + 1) / 2); // fórmula da soma de 1 até n: n(n+1)/2
        free(array); // libera o array completo
    }

    free(array_local); // todos os processos liberam o seu pedaço

    MPI_Finalize(); // encerra o ambiente MPI
}