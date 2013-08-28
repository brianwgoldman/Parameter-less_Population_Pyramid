import sys
from os import path
from collections import defaultdict
import numpy

sortable = defaultdict(list)
headers = ['median', 'mad', 'failures', 'hill_climber', 'cluster_ordering', 'no_singles',
           'donate_until_different', 'keep_zeros']

table = defaultdict(dict)
all_problems = set()
for filename in sys.argv[1:]:
    parts = path.basename(filename).strip().split('.')
    problem = '_'.join(parts[0:2])
    version = parts[2:-1]
    with open(filename, 'r') as f:
        # median = map(int, f.read().split('\n')[0].split(' ')[1:])
        lines = f.read().strip().split('\n')
        median = map(int, lines[0].split(' ')[1:])
        values = [int(line.split(' ')[1]) for line in lines[1:]]
        if min(values) < 0 or max(values) > 1000000000:
            print filename, min(values), max(values)
            continue
        median[0] = numpy.mean(values)
    # sortable[problem].append(median + version)
    table[tuple(version)][problem] = median
    all_problems.add(problem)

for version, data in table.items():
    if len(data) == len(all_problems):
        for problem, median in data.items():
            sortable[problem].append(median + list(version))
        for problem in all_problems - set(data.keys()):
            sortable[problem].append([sys.maxint, 0, 0] + list(version))

ranks = [defaultdict(list) for _ in range(len(headers))]
full = defaultdict(list)
problem_order = []
for problem, data in sortable.items():
    print '-----------', problem, '-------------', len(data)
    print '\t'.join(headers)
    problem_order.append(problem)
    for rank, line in enumerate(sorted(data)):
        print '\t'.join(map(str, line))
        full[tuple(line[3:])].append(rank)
        for index, element in enumerate(line):
            ranks[index][element].append(rank)

for index in range(3, len(headers)):
    print '-------', headers[index], '---------'
    orderable = []
    for element, records in ranks[index].items():
        orderable.append((sum(records) / len(records), element, len(records)))
    for line in sorted(orderable):
        print '\t'.join(map(str, line))

print '----- FULL -----'
print problem_order
for version, data in sorted(full.items(), key=lambda X: (-len(X[1]), sum(X[1]))):
    print '\t'.join(version), data, sum(data) / len(data)
