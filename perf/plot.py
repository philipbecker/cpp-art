#!/usr/bin/python3
import matplotlib.pyplot as plt
import numpy as np
from csv import reader
from collections import defaultdict

file_name = '/home/philip/.CLion2016.1/system/cmake/generated/ART-85c3cfdd/85c3cfdd/Release/perf/results.csv'
#file_name = '/media/philip/Data/Dropbox/art/lookup-int64-dense-16M.csv'

with open(file_name, 'r') as f:
    data = list(reader(f))

results = {
    'baseline': {d[0]: {} for d in data[1:]},
    'absolute': {d[0]: {} for d in data[1:]}
};

for d in data[1:]:
    # in Millions
    number_of_elements = int(d[2])/1000000
    operations_per_second = int(float(d[2]) * float(d[7]))/1000000

    if d[1] in results['baseline'][d[0]]:
        results['baseline'][d[0]][d[1]].append((number_of_elements, d[5]))
        results['absolute'][d[0]][d[1]].append((number_of_elements, operations_per_second))
    else:
        results['baseline'][d[0]][d[1]] = [(number_of_elements, d[5])]
        results['absolute'][d[0]][d[1]] = [(number_of_elements, operations_per_second)]

styles = {
    'ArtMap': {'marker': '*', 'linestyle': '-', 'color': 'cornflowerblue'},
    'UnorderedMap': {'marker': 'o', 'linestyle': ':', 'color': 'black'},
    'Map': {'marker': 'v', 'linestyle': '--', 'color': 'black'},
    'BtreeMap': {'marker': 'x', 'linestyle': '-.', 'color': 'green'}
};

ylabel = {
    'baseline' : 'seconds per operation in comparison to std::map',
    'absolute' : '# of Million Operations/Second'
}

number = 1

for type, result in results.items():
    for group, experiment in result.items():
        plt.figure(number)
        number += 1
        lines = []
        names = []
        plt.suptitle(group + " (" + type + ") ")
        for map_name, map_data in experiment.items():
            line, = plt.plot(
                *zip(*map_data),
                marker=styles[map_name]['marker'],
                linestyle=styles[map_name]['linestyle'],
                color=styles[map_name]['color'],
                linewidth=2.0,
                markersize=10,
                label=map_name
            )
            lines.append(line)
            names.append(map_name)

        plt.legend(lines, names)
        plt.ylabel(ylabel[type])
        plt.xlabel('# of elements (in Millions)')
        plt.savefig(type + "_" + group)

