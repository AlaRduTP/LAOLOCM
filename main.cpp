#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <stdlib.h> /* 亂數相關函數 */
#include <time.h>	/* 時間相關函數 */
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <type_traits>
#include "fitness.h"

using namespace std;

double rand_double()
{
	return (double)rand() / RAND_MAX;
}

unordered_map<string, string> config;

template <typename T>
T get_config(string key)
{
	if (config.empty())
	{
		fstream file;
		file.open("config", ios::in);
		string line;
		while (getline(file, line))
		{
			istringstream is_line(line);
			string key;
			if (getline(is_line, key, '='))
			{
				string value;
				if (getline(is_line, value))
					config.insert({key, value});
			}
		}
	}
	if (is_same_v<T, int>)
		return stoi(config[key]);
	if (is_same_v<T, double>)
		return stod(config[key]);
}

const int VALUE_MAX = get_config<int>("VALUE_MAX");
const int VALUE_MIN = get_config<int>("VALUE_MIN");
const int CHANGE_WEIGHT = get_config<int>("CHANGE_WEIGHT");
const int REPLACE_WEIGHT = get_config<int>("REPLACE_WEIGHT");
const double X_CHANCE = get_config<double>("X_CHANCE");
const int DNA_COUNT = get_config<int>("DNA_COUNT");
const int LEFT_AMOUNT = get_config<int>("LEFT_AMOUNT");
const double MUTATION_CHANCE = get_config<double>("MUTATION_CHANCE");
const int GENERATION_COUNT = get_config<int>("GENERATION_COUNT");
const int TREE_COUNT = 1;
const vector<int> TREE_VARIBALS[3] = {
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
	{1, 2, 3, 4, 5, 6, 7, 8, 12, 13, 14, 15, 16, 17, 18},
};

class Node
{
public:
	Node *childs[2];
	Node *parent;
	int value;
	int child_length;
	bool is_variable;
	int variable_index;
	char operate;
	int offspring_count = 0;
	int tree_index;

	Node(int index, Node *p, double child_chance = 0.5) : parent(p), tree_index(index)
	{
		if (rand_double() < child_chance)
		{
			child_length = rand_double() < 0.5 ? 1 : 2;
			change();
			for (int i = 0; i < child_length; i++)
			{
				childs[i] = new Node(tree_index, this, child_chance - 0.2);
				offspring_count += 1 + childs[i]->offspring_count;
			}
		}
		else
		{
			child_length = 0;
			change();
		}
	}

	Node(int index, Node *parent, Node *target, Node *cross = NULL) : parent(parent), value(target->value), child_length(target->child_length),
																	  is_variable(target->is_variable), variable_index(target->variable_index),
																	  operate(target->operate), tree_index(index)
	{
		for (int i = 0; i < child_length; i++)
		{
			childs[i] = new Node(i, this, target->childs[i]);
			offspring_count += 1 + childs[i]->offspring_count;
		}

		if (cross)
		{
			Node *change = random_child();
			Node *cross_target = cross->random_child(true);
			if (!change || !cross_target)
				return;

			change->delete_child();
			Node *change_parent = change->parent;
			for (int i = 0; i < change_parent->child_length; i++)
			{
				if (change == change_parent->childs[i])
				{
					delete change;
					change_parent->childs[i] = new Node(tree_index, change_parent, cross_target);
					change_parent->update_offspring();
					break;
				}
			}
		}
	}

	void update_offspring()
	{
		offspring_count = 0;
		for (int i = 0; i < child_length; i++)
			offspring_count += 1 + childs[i]->offspring_count;

		if (parent)
			parent->update_offspring();
	}

	Node *get_child(int index)
	{
		int self_index = child_length > 0 ? childs[0]->offspring_count + 1 : 0;
		if (index == self_index)
			return this;
		if (index < self_index)
			return childs[0]->get_child(index);
		else
			return childs[1]->get_child(index - self_index - 1);
	}

	Node *random_child(bool has_self = false)
	{
		if (child_length == 0 && !has_self)
			return NULL;
		int self_index = child_length > 0 ? childs[0]->offspring_count + 1 : 0;
		int index;
		do
			index = rand() % (offspring_count + 1);
		while (!has_self && index == self_index);

		return get_child(index);
	}

	void change()
	{
		if (child_length == 2)
		{
			operate = "+-*/"[rand() % 4];
			return;
		}

		if (child_length == 1)
		{
			// square, root, log, exp
			operate = "srle"[rand() % 4];
			return;
		}

		is_variable = rand_double() < X_CHANCE;
		if (!is_variable)
		{
			do
				value = (rand() % (VALUE_MAX - VALUE_MIN)) + VALUE_MIN;
			while (value == 0);
		}
		else
		{
			variable_index = TREE_VARIBALS[tree_index][rand() % TREE_VARIBALS[tree_index].size()];
		}
	}

	string to_string()
	{
		if (child_length == 2)
			return '(' + childs[0]->to_string() + operate + ' ' + childs[1]->to_string() + ')';
		else if (child_length == 1)
			switch (operate)
			{
			case 's':
				return "p2(" + childs[0]->to_string() + ")";
			case 'r':
				return "sqrt(" + childs[0]->to_string() + ")";
			case 'l':
				return "log(" + childs[0]->to_string() + ")";
			case 'e':
				return "exp(" + childs[0]->to_string() + ")";
			}
		return is_variable ? "{" + std::to_string(variable_index) + "}" : std::to_string(value);
	}

	void delete_child()
	{
		for (int i = 0; i < child_length; i++)
		{
			childs[i]->delete_child();
			delete childs[i];
		}
		child_length = 0;
		change();
	}
};

class DNA
{
public:
	Node *trees[TREE_COUNT];
	double score = -1;

	DNA(DNA *dna = NULL, DNA *cross = NULL)
	{
		if (dna)
		{
			for (int i = 0; i < TREE_COUNT; i++)
			{
				if (cross)
					trees[i] = new Node(i, NULL, dna->trees[i], cross->trees[i]);
				else
					trees[i] = new Node(i, NULL, dna->trees[i]);
			}
		}
		else
		{
			for (int i = 0; i < TREE_COUNT; i++)
				trees[i] = new Node(i, NULL);
		}
	}

	DNA *mutation()
	{
		DNA *ret = new DNA(this);
		for (int i = 0; i < TREE_COUNT; i++)
		{
			Node *change = ret->trees[i]->random_child(true);

			int random = rand() % (CHANGE_WEIGHT + REPLACE_WEIGHT);
			if (random < CHANGE_WEIGHT || change == ret->trees[i])
				change->change();
			else
			{
				change->delete_child();
				Node *change_parent = change->parent;
				for (int i = 0; i < change_parent->child_length; i++)
				{
					if (change == change_parent->childs[i])
					{
						delete change;
						change_parent->childs[i] = new Node(i, change_parent);
						change_parent->update_offspring();
						break;
					}
				}
			}
		}

		return ret;
	}

	DNA *cross(DNA *target)
	{
		return new DNA(this, target);
	}

	double fitness()
	{
		if (score != -1)
			return score;
		vector<string> genes;
		for (int i = 0; i < TREE_COUNT; i++)
			genes.push_back(trees[i]->to_string());
		score = ::fitness(genes);
		cout << '.' << flush;
		return score;
	}

	void destory()
	{
		for (int i = 0; i < TREE_COUNT; i++)
		{
			trees[i]->delete_child();
			delete trees[i];
		}
		delete this;
	}
};

bool compare(DNA *a, DNA *b)
{
	double v1 = a->fitness();
	double v2 = b->fitness();

	if (isinf(v1))
		return true;
	else if (isinf(v2))
		return false;
	else
		return v1 > v2;
}

int main(int argc, char *argv[])
{
	fitness_initialize();

	ios::sync_with_stdio(false);
	cin.tie(0);

	srand(time(NULL));
	fstream file;
	if (argc != 2)
	{
		cout << "usage: ./main <filename>|none" << endl;
		return 0;
	}
	if (string(argv[1]) != "none")
		file.open(argv[1], ios::out | ios::app);

	DNA *DNAs[DNA_COUNT];
	for (int i = 0; i < LEFT_AMOUNT; i++)
		DNAs[i] = new DNA();

	for (int i = 0; i < GENERATION_COUNT; i++)
	{
		for (int j = LEFT_AMOUNT; j < DNA_COUNT; j++)
		{
			if ((double)rand() / RAND_MAX < MUTATION_CHANCE)
				DNAs[j] = DNAs[j % LEFT_AMOUNT]->mutation();
			else
				DNAs[j] = DNAs[j % LEFT_AMOUNT]->cross(DNAs[rand() % LEFT_AMOUNT]);
		}
		sort(DNAs, DNAs + DNA_COUNT, compare);
		for (int j = LEFT_AMOUNT; j < DNA_COUNT; j++)
			DNAs[j]->destory();

		cout << "\nGeneration " << i << endl;
		cout << DNAs[0]->trees[0]->to_string() << endl;
		cout << DNAs[0]->fitness() << endl;
		if (file)
		{
			file << "Generation " << i << endl
				 << "Fitness: ";
			for (int j = 0; j < LEFT_AMOUNT; j++)
				file << DNAs[j]->fitness() << " ";
			file << endl;
			for (int j = 0; j < LEFT_AMOUNT; j++)
				file << DNAs[j]->trees[0]->to_string() << endl;
		}

		if (DNAs[0]->fitness() >= 1)
			break;
	}
	if (file)
		file.close();
	fitness_finalize();
}
