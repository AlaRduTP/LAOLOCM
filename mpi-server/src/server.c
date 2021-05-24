#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#define MPI_SIZE_T MPI_UNSIGNED_LONG_LONG

#define TAG_MIGRATION	0
#define TAG_BUFF_SIZE	1
#define TAG_BUFF	2

#define for_each_client(clt, clts) for(struct client * clt = clts; clt != clts + NUM_CLIENTS; ++clt)

struct client {
	MPI_Comm comm;
	unsigned char flag;
	size_t size;
	char * buffer;
};

char port_name[MPI_MAX_PORT_NAME];

void finalize_handler() {
 	MPI_Unpublish_name("server", MPI_INFO_NULL, port_name);
	MPI_Close_port(port_name);
	MPI_Finalize();
}

/*
 *	./server <number of clients> <number of migrations>
 */
int main(int argc, char * argv[]) {
	if (argc < 3)
	{
		puts("./server <number of clients> <number of migrations>");
		exit(0);
	}

	const int NUM_CLIENTS = atoi(argv[1]);
	const int MAX_MIGRATIONS = atoi(argv[2]);

	MPI_Init(&argc, &argv);
	atexit(finalize_handler);

	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("size: %d\n", size);
	printf("rank: %d\n", rank);

	MPI_Open_port(MPI_INFO_NULL, port_name);
	MPI_Publish_name("server", MPI_INFO_NULL, port_name);

	printf("port name: %s\n", port_name);

	struct client clients[NUM_CLIENTS];
	for_each_client(client, clients)
	{
		client->buffer = NULL;

		const size_t i = client - clients;

		MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &client->comm);
		MPI_Send(&MAX_MIGRATIONS, 1, MPI_INT, 0, TAG_MIGRATION, client->comm);

		printf("Client [%zu] connected.\n", i);
	}

	for (size_t migration = 0; migration < MAX_MIGRATIONS; ++migration)
	{
		printf("* Migration [%zu]\n", migration);
		MPI_Request requests[NUM_CLIENTS];
		MPI_Status tmp_status[NUM_CLIENTS];

		for_each_client(client, clients)
		{
			MPI_Request * request = requests + (client - clients);
			MPI_Irecv(&client->size, 1, MPI_SIZE_T, MPI_ANY_SOURCE, TAG_BUFF_SIZE, client->comm, request);
		}
		MPI_Waitall(NUM_CLIENTS, requests, tmp_status);

		for_each_client(client, clients)
		{
			MPI_Request * request = requests + (client - clients);
			client->buffer = (char *)realloc(client->buffer, client->size);
			MPI_Irecv(client->buffer, client->size, MPI_CHAR, MPI_ANY_SOURCE, TAG_BUFF, client->comm, request);
		}
		MPI_Waitall(NUM_CLIENTS, requests, tmp_status);

		for_each_client(client, clients)
		{
			const size_t next = (client - clients + 1) % NUM_CLIENTS;
			MPI_Send(&client->size, 1, MPI_SIZE_T, 0, TAG_BUFF_SIZE, clients[next].comm);
			MPI_Send(client->buffer, client->size, MPI_CHAR, 0, TAG_BUFF, clients[next].comm);
			printf("Send message to Client [%zu].\n", client - clients);
		}
	}

	for_each_client(client, clients) {
		MPI_Comm_free(&client->comm);
		free(client->buffer);
	}
}
