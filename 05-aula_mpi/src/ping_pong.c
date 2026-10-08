#include <mpi.h> //biblioteca do OpenMPI
#include <stdio.h>

int main(){
    int rank, size; // define variaveis de tamanho e rank do processo

    MPI_Init(NULL, NULL); //Inicializa o ambiente do MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //numero do worker em exec
    MPI_Comm_size(MPI_COMM_WORLD, &size); //quantidade de processos disponiveis

    int count=0;
    int sender_rank = 0;
    int recvrs_rank = 1;
    do{
        //Se o rank do processo for igual ao rank do remetente, envia a mensagem
        if(rank == sender_rank){
            count++; //incrementa o contador
            MPI_Send(&count, 1, MPI_INT, recvrs_rank, 0, MPI_COMM_WORLD); //envia o valor de count uma vez para o rank do receptor com a tag 0
            printf("mensagem \"%d\" enviada de %d para %d \n", count, sender_rank, recvrs_rank); //exibe a mensagem enviada e quem mandou para quem
        }

        //Se o rank do processo for igual ao rank do receptor, recebe a mensagem
        if(rank == recvrs_rank){
            MPI_Recv(&count, 1, MPI_INT, sender_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //recebe o valor do count uma vez do rank do remetente com a tag 0
            printf("mensagem \"%d\" de %d recebida por %d\n", count, sender_rank, recvrs_rank); //exibe a mensagem recebida e quem enviou
        }
    }while(count < 10);

    MPI_Finalize(); // Encerra o MPI
    if(rank == 0){
       printf("fim da execução");
    }
    return 0;
}
