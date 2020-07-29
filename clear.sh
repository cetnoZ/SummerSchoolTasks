#!/bin/bash
./replication_part1/clear.sh
./replication_part2/clear.sh
./infection/clear.sh
./coevolution/clear.sh
./synthesis_and_struggle/clear.sh

make -C ./cactus_queries clear
make -C ./path_rotation clear
make -C ./stripe_paths clear
make -C ./stripe_paths_2 clear
make -C ./tree_balancing clear

