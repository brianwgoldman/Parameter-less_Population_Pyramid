import sys
import glob
from os import path, remove
from subprocess import call
import signal
import time

def signal_handler(signal, frame):
        raise Exception('Signal received, exiting')
signal.signal(signal.SIGINT, signal_handler)


executable = path.join("Release", "P3")
folder = "tuning"
jobname = sys.argv[1]
arguments = [path.join("config", "default.txt"),
             path.join("config", "tune.txt"),
             '-runs 1',
             ]

flag_titles = ['-problem', '-length', '-hill_climber', '-cluster_ordering',
               '-no_singles', '-donate_until_different', '-keep_zeros']

def interleave(a, b):
    return [x for pair in zip(a, b) for x in pair]

for (problem, length) in [('DeceptiveTrap', '504'),
                          ('DeceptiveStepTrap', '504'),
                          ('NearestNeighborNK', '500'),
                          ('HIFF', '512'),
                          ('MAXSAT', '100'),
                          ]:
    for hc in ['no_action', 'binary_tournament', 'once_each', 'first_improvement', 'steepest_ascent']:
        for ordering in ['no_action', 'least_linked_first', 'smallest_first', 'random']:
            for no_singles in ['0', '1']:
                for donate_until_different in ['0', '1']:
                    for keep_zeros in ['0', '1']:
                        settings = [problem, length, hc, ordering, no_singles,
                                    donate_until_different, keep_zeros]
                        flags = interleave(flag_titles, settings)
                        filename = path.join(folder, '.'.join(settings))
                        combined = [executable] + arguments + flags + ['-outfile', filename + '.dat']

                        if not path.exists(filename + ".dat") and not path.exists(filename + ".start"):
                            with open(filename + ".start", 'w') as f:
                                f.write(jobname + "\n")
                            print " ".join(combined)
                            try:
                                call(combined)
                            except Exception as e:
                                print e
                            finally:
                                remove(filename + ".start")
                            sys.exit()

open(path.join(folder, "result.txt")).close()
