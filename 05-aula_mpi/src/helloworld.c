#include <mpi.h> //biblioteca do OpenMPI
#include <stdio.h>

int main(){
  MPI_Init(NULL, NULL); //Inicializa o ambiente do MPI
  int size, rank; // define variaveis de tamanho e rank do processo

  MPI_Comm_size(MPI_COMM_WORLD, &size); //quantidade de processos disponiveis
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); //numero do worker em exec

  //exibi os dados
  printf("Hello world (rank %d de %d workers)\n", rank, size);

  MPI_Finalize(); // Encerra o MPI
}
