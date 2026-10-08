#include <mpi.h> //biblioteca do OpenMPI
#include <stdio.h>

int main(){
    int rank, size; // define variaveis de tamanho e rank do processo
    MPI_Init(NULL, NULL); //Inicializa o ambiente do MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //numero do worker em exec
    MPI_Comm_size(MPI_COMM_WORLD, &size); //quantidade de processos disponiveis

    int msg; // mensagem que será enviada
    if(rank != 0){ // se não for o processo com o menor rank
        int prev_rank = rank - 1; // rank de quem enviou a mensagem
        MPI_Recv(&msg, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //recebe o valor da msg uma vez do rank do remetente com a tag 0
        printf("msg: %d (%d -> %d)\n", msg, prev_rank, rank); //exibe a mensagem recebida e quem enviou
    } else {
        msg = 0;
    }

    // envia a mensagem para o próximo processo
    msg++;
    MPI_Send(&msg, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);

    // se for o processo com o menor rank, envia a mensagem inicial
    if(rank == 0){
        int prev_rank = size - 1;
        MPI_Recv(&msg, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //recebe o valor da msg uma vez do rank do remetente com a tag 0
        printf("msg: %d (%d -> %d)\n", msg, prev_rank, rank); //exibe a mensagem recebida e quem enviou
    }

    MPI_Finalize();
}
