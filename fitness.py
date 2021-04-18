from jinja2 import FileSystemLoader, Environment
from pathlib import Path

import subprocess
import os

AGENTS_FOLDER = Path('Agents')
TMP_FOLDER = AGENTS_FOLDER / 'templates'
LOL_FOLDER = AGENTS_FOLDER / 'LOL'

TMPL = Environment(
    loader=FileSystemLoader(str(TMP_FOLDER))).get_template('newTemplate.cpp')

AGENT_SRC = LOL_FOLDER / 'agent.cpp'
AGENT_BIN = LOL_FOLDER / 'agent'

GP_OPERAND = [
    'cost_',
    'attack_',
    'defense_',
    'abilities_[0]',
    'abilities_[1]',
    'abilities_[2]',
    'abilities_[3]',
    'abilities_[4]',
    'abilities_[5]',
    'myHealthChange_',
    'opponentHealthChange_',
    'cardDraw_',
    'enemyTotalHP',
    'ownTotalHP',
    'enemyTotalAttack',
    'ownTotalAttack',
    'attacker.attack_',
    'attacker.defense_',
    'attacker.abilities_[0]',
    'attacker.abilities_[1]',
    'attacker.abilities_[2]',
    'attacker.abilities_[3]',
    'attacker.abilities_[4]',
    'attacker.abilities_[5]',
    'abilities_[6]',
]

REFEREE = './referee/Tester'
# BASELINE = str(LOL_FOLDER / 'baseline')
# BASELINE = './Agents/Chad/agent/target/release/agent'
# BASELINE = 'python3\\ Agents/Baseline1/main.py'
BASELINE = 'python3\\ Agents/ReinforcedGreediness/agent.py'
AGENT = f'./{AGENT_BIN}'
GAMES = 10
SEED = 0

def referee(game_count, game_seed):
    os.system(f'g++ -std=c++17 -O3 {AGENT_SRC} -o {AGENT_BIN}')
    ret = subprocess.check_output([
        f'{REFEREE}',
        f'--baseline="{BASELINE}"',
        f'--agent="{AGENT}"',
        f'--games={game_count}',
        f'--seed={game_seed}',
    ]).decode('utf8').split('\n')
    return ret


def gene2expr(gene):
    for i, r in enumerate(GP_OPERAND):
        gene = gene.replace(f'{{{i}}}', r)
    return gene


def test(expression, game_count=GAMES):
    return fitness([expression.encode()], game_count, True)


def fitness(genes, game_count=GAMES, game_seed=SEED, extra_info=False):
    exprs = [gene2expr(gene.decode()) for gene in genes]
    TMPL.stream(exprs=exprs).dump(str(AGENT_SRC))

    result = referee(game_count, game_seed)
    if extra_info:
        for line in result:
            print(' '.join(f'{n: >3}' for n in line.split()))

    baseline_hps = [*map(int, result[0].split())]
    agent_hps = [*map(int, result[1].split())]

    to_fill = game_count - len(baseline_hps)
    baseline_hps += [30] * to_fill
    agent_hps += [0] * to_fill

    avg_hp_diff = sum(
        a - b for a, b in zip(agent_hps, baseline_hps)) / len(agent_hps)
    return avg_hp_diff
