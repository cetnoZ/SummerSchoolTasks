#!/bin/bash
./replication_part1/clear.sh
./replication_part2/clear.sh
./infection/clear.sh
./coevolution/clear.sh
./synthesis_and_struggle/clear.sh

make -C ./cactus_queries clean
make -C ./path_rotation clean
make -C ./stripe_paths clean
make -C ./stripe_paths_2 clean
make -C ./tree_balancing clean

