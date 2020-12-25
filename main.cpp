#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <stdlib.h> /* 亂數相關函數 */
#include <time.h>	/* 時間相關函數 */
#include <string>
#include "fitness.h"

using namespace std;

const int VALUE_MAX = 100;
const int VALUE_MIN = -100;
const int CHANGE_WEIGHT = 10;
const int REPLACE_WEIGHT = 10;
const double X_CHANCE = 0.5;
const int DNA_COUNT = 20;
const int LEFT_AMOUNT = 10;
const double MUTATION_CHANCE = 0.5;
const int GENERATION_COUNT = 10;
const vector<int> TREE_VARIBALS[3] = {{0, 1, 2, 3, 4, 5, 6}, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, {1, 2, 3, 7, 8, 9, 10, 11, 12, 13}};

double rand_double()
{
	return (double)rand() / RAND_MAX;
}

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
			return '(' + childs[0]->to_string() + operate + childs[1]->to_string() + ')';
		else if (child_length == 1)
			switch (operate)
			{
			case 's':
				return "(" + childs[0]->to_string() + "**2)";
			case 'r':
				return "sqrt(" + childs[0]->to_string() + ")";
			case 'l':
				return "log(" + childs[0]->to_string() + ")";
			case 'e':
				return "exp(" + childs[0]->to_string() + ")";
			}
		return is_variable ? "#" + std::to_string(variable_index) : std::to_string(value);
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
	Node *trees[3];
	double score = -1;

	DNA(DNA *dna = NULL, DNA *cross = NULL)
	{
		if (dna)
		{
			for (int i = 0; i < 3; i++)
			{
				if (cross)
					trees[i] = new Node(i, NULL, dna->trees[i], cross->trees[i]);
				else
					trees[i] = new Node(i, NULL, dna->trees[i]);
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
				trees[i] = new Node(i, NULL);
		}
	}

	DNA *mutation()
	{
		DNA *ret = new DNA(this);
		for (int i = 0; i < 3; i++)
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
		genes.push_back(trees[0]->to_string());
		genes.push_back(trees[1]->to_string());
		genes.push_back(trees[2]->to_string());
		return score = ::fitness(genes);
	}

	void destory()
	{
		for (int i = 0; i < 3; i++)
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

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);

	srand(time(NULL));

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

		cout << "Generation " << i << ": " << DNAs[0]->trees[0]->to_string() << endl;
		cout << "Generation " << i << ": " << DNAs[0]->fitness() << endl;
		cout << "Generation " << i << ": " << DNAs[0]->trees[0]->offspring_count << endl;

		if (DNAs[0]->fitness() > 100000)
			break;
	}
}
