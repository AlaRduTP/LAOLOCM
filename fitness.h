#ifndef FITNESS
#define FITNESS

#include <vector>
#include <string>

int fitness_initialize();
float fitness(std::vector<std::string> & genes, int game_count);
void fitness_finalize();

#endif