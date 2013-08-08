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

for problem, data in sortable.items():
    print '-----------', problem, '-------------', len(data)
    print '\t'.join(headers)
    for line in sorted(data):
        print '\t'.join(map(str, line))
