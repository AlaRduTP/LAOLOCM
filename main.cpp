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
#include "fitness.h"
#include <stack>

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
		if (!file)
		{
			cout << "config not exist!" << endl;
			exit(0);
		}

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
	if (config.count(key) == 0)
	{
		cout << "key " << key << " not exist!" << endl;
		exit(0);
	}
	if (is_same_v<T, int>)
		return stoi(config[key]);
	if (is_same_v<T, double>)
		return stod(config[key]);
}

const int VALUE_MAX = get_config<int>("VALUE_MAX");
const int VALUE_MIN = get_config<int>("VALUE_MIN");
const int CHANGE_CHANCE = get_config<double>("CHANGE_CHANCE");
const double X_CHANCE = get_config<double>("X_CHANCE");
const int DNA_COUNT = get_config<int>("DNA_COUNT");
const int LEFT_AMOUNT = get_config<int>("LEFT_AMOUNT");
const double MUTATION_CHANCE = get_config<double>("MUTATION_CHANCE");
const int GENERATION_COUNT = get_config<int>("GENERATION_COUNT");
const int GAME_COUNT_CAP = get_config<int>("GAME_COUNT_CAP");
const int GAME_COUNT_C1 = get_config<int>("GAME_COUNT_C1");
const int GAME_COUNT_C2 = get_config<int>("GAME_COUNT_C2");
const int GAME_COUNT_C3 = get_config<int>("GAME_COUNT_C3");
const int REFRESH_INTERVAL = get_config<int>("REFRESH_INTERVAL");
const int GAME_COUNT_REFRESH_INTERVAL = get_config<int>("GAME_COUNT_REFRESH_INTERVAL");
const int TREE_COUNT = 1;
const vector<int> TREE_VARIBALS[3] = {
	// {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
	// {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
	{1, 2, 3, 4, 5, 6, 7, 8, 12, 13, 14, 15, 16, 17, 18, 19},
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
	{1, 2, 3, 4, 5, 6, 7, 8, 12, 13, 14, 15, 16, 17, 18},
};
int generation;

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

	Node(int child_length) : parent(NULL), child_length(child_length)
	{
	}

	Node(int index, Node *p, int level = 0) : parent(p), tree_index(index)
	{
		if (level < 3)
		{
			child_length = rand_double() < 0.5 ? 1 : 2;
			change();
			for (int i = 0; i < child_length; i++)
			{
				childs[i] = new Node(tree_index, this, level + 1);
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
		{
			string child1_string = childs[1]->to_string();
			// avoid (1) - (-2) turns into (1--2)
			return '(' + childs[0]->to_string() + operate + (operate == child1_string[0] ? " " : "") + child1_string + ')';
		}
		else if (child_length == 1)
		{
			string child0_string = childs[0]->to_string();
			if (child0_string[0] != '(')
				child0_string = '(' + child0_string + ')';
			switch (operate)
			{
			case 's':
				return "p2" + child0_string;
			case 'r':
				return "sqrt" + child0_string;
			case 'l':
				return "log" + child0_string;
			case 'e':
				return "exp" + child0_string;
			}
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

	static Node *from_string(string s)
	{
		s = '(' + s + ')';
		stack<char> operators;
		vector<Node *> operates;

		string unary_operator[] = {"p2", "sqrt", "log", "exp"};
		string unary_operator_char = "srle";
		for (int i = 0; s[i]; i++)
		{
			bool match = false;
			for (int j = 0; j < 4; j++)
			{
				if (s.compare(i, unary_operator[j].size(), unary_operator[j]) == 0)
				{
					operators.push('(');
					operators.push(unary_operator_char[j]);
					i += unary_operator[j].size();
					match = true;
					break;
				}
			}

			if (s[i] == ' ' || match)
			{
			}
			else if (isdigit(s[i]) || (s[i] == '-' && i > 0 && string("1234567890)}").find(s[i - 1]) == string::npos))
			{
				int j = i + 1;
				while (isdigit(s[j]))
					j++;
				int value = stoi(s.substr(i, j - i));
				Node *n = new Node(0);
				n->value = value;
				n->is_variable = false;

				operates.push_back(n);
				i = j - 1;
			}
			else if (s[i] == '{')
			{
				int j = i + 2;
				while (isdigit(s[j]))
					j++;
				int index = stoi(s.substr(i + 1, j - i - 1));
				Node *n = new Node(0);
				n->variable_index = index;
				n->is_variable = true;

				operates.push_back(n);
				i = j;
			}
			else if (string("(+-*/").find(s[i]) != string::npos)
			{
				operators.push(s[i]);
			}
			else if (s[i] == ')')
			{
				char c = operators.top();
				operators.pop();
				while (c != '(')
				{
					Node *n, *n0, *n1;

					switch (c)
					{
					case '+':
					case '-':
					case '*':
					case '/':
						n = new Node(2);
						n->operate = c;
						n1 = operates.back();
						operates.pop_back();
						n0 = operates.back();
						operates.pop_back();

						n->childs[0] = n0;
						n0->parent = n;
						n->childs[1] = n1;
						n1->parent = n;

						operates.push_back(n);
						break;

					case 's':
					case 'r':
					case 'l':
					case 'e':
						n = new Node(1);
						n->operate = c;
						n0 = operates.back();
						operates.pop_back();

						n->childs[0] = n0;
						n0->parent = n;

						operates.push_back(n);
						break;
					}

					c = operators.top();
					operators.pop();
				}
			}
		}

		return operates[0];
	}
};

class DNA
{
public:
	Node *trees[TREE_COUNT];
	double score = -1;
	int last_generation = -99999;
	int game_played = 0;

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

			if (rand_double() < CHANGE_CHANCE || change == ret->trees[i])
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
		int need_game_count = min(GAME_COUNT_CAP, GAME_COUNT_C1 * generation * generation + GAME_COUNT_C2 * generation + GAME_COUNT_C3);
		if (generation - last_generation < REFRESH_INTERVAL &&
			((need_game_count - game_played < GAME_COUNT_REFRESH_INTERVAL && need_game_count < GAME_COUNT_CAP) || game_played == GAME_COUNT_CAP))
			return score;

		vector<string> genes;
		for (int i = 0; i < TREE_COUNT; i++)
			genes.push_back(trees[i]->to_string());
		score = ::fitness(genes, need_game_count);
		game_played = need_game_count;
		last_generation = generation;
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
	else if (v1 == v2)
		return a->trees[0]->offspring_count < a->trees[0]->offspring_count;
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

	for (generation = 0; generation < GENERATION_COUNT; generation++)
	{
		file << "\nSpawning" << endl;
		for (int j = LEFT_AMOUNT; j < DNA_COUNT; j++)
		{
			if ((double)rand() / RAND_MAX < MUTATION_CHANCE)
			{
				DNAs[j] = DNAs[j % LEFT_AMOUNT]->mutation();
				file << DNAs[j % LEFT_AMOUNT]->trees[0]->to_string() << " 變異出 " << DNAs[j]->trees[0]->to_string() << endl;
			}
			else
			{
				int cross_index = rand() % LEFT_AMOUNT;
				DNAs[j] = DNAs[j % LEFT_AMOUNT]->cross(DNAs[cross_index]);
				file << DNAs[j % LEFT_AMOUNT]->trees[0]->to_string() << " 和 " << DNAs[cross_index]->trees[0]->to_string() << " 交配出 "
					 << DNAs[j]->trees[0]->to_string() << endl;
			}
		}
		sort(DNAs, DNAs + DNA_COUNT, compare);
		for (int j = LEFT_AMOUNT; j < DNA_COUNT; j++)
			DNAs[j]->destory();

		cout << "\nGeneration " << generation << endl;
		cout << DNAs[0]->trees[0]->to_string() << endl;
		cout << DNAs[0]->fitness() << endl;
		if (file)
		{
			file << "\nGeneration " << generation << endl
				 << "Fitness: ";
			for (int j = 0; j < LEFT_AMOUNT; j++)
				file << DNAs[j]->fitness() << " ";
			file << endl;
			for (int j = 0; j < LEFT_AMOUNT; j++)
				file << DNAs[j]->trees[0]->to_string() << endl;
		}

		if (DNAs[0]->fitness() >= 30)
			break;
	}
	if (file)
		file.close();
	fitness_finalize();
}
