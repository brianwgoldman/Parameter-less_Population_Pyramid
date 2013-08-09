import sys
from os import path
from collections import defaultdict

sortable = defaultdict(list)
headers = ['median', 'mad', 'failures', 'hill_climber', 'cluster_ordering', 'no_singles',
           'donate_until_different', 'keep_zeros']

for filename in sys.argv[1:]:
    parts = path.basename(filename).strip().split('.')
    problem = '_'.join(parts[0:2])
    version = parts[2:-1]

    with open(filename, 'r') as f:
        median = map(int, f.read().split('\n')[0].split(' ')[1:])
    sortable[problem].append(median + version)

ranks = [defaultdict(list) for _ in range(len(headers))]
full = defaultdict(list)
for problem, data in sortable.items():
    print '-----------', problem, '-------------', len(data)
    print '\t'.join(headers)
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
for version, data in sorted(full.items(), key=lambda X: max(X[1])):
    print '\t'.join(version), data
