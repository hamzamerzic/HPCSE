#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int rank, size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::cout << "Rank: " << rank << std::endl << "Size: " << size << std::endl;

  if (rank == 0) {
    MPI_Status status;
    int count;
    char tito[100];
    // wait for a message
    MPI_Recv(tito, 100, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    std::cout << "Received: " << tito << std::endl;
  } else {
    char tito[] = "Hello world!";
    MPI_Send(tito, sizeof(tito)/sizeof(char), MPI_CHAR, 0, 42, MPI_COMM_WORLD);
  }
  std::vector<double> loco (10 - rank, rank);
  std::vector<double> buffer_r;
  std::vector<int> recvcnts (size), displs (size);
  if (rank == 0) {
    buffer_r.resize(1000);
    for (int i = 0; i < recvcnts.size(); ++i) {
      recvcnts[i] = 10 - i;
      displs[i] = i != 0 ? recvcnts[i - 1] + displs[i - 1] : 0;
    }
  }
  MPI_Gatherv(loco.data(), loco.size(), MPI_DOUBLE, buffer_r.data(), recvcnts.data(),
    displs.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
  std::cerr << "OK" << rank;
  MPI_Barrier(MPI_COMM_WORLD);
  std::cerr << "OK!" << std::endl;
  if (rank == 0) {
    for (int i = 0; i < displs.size(); ++i) {
      for (int j = 0; j < recvcnts[i]; ++j)
        std::cout << buffer_r[displs[i] + j] << " ";
      std::cout << std::endl;
    }
  }
  MPI_Finalize();
  return 0;
}
