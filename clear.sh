#!/bin/bash
rm -f replication_part1/tests/in/*
rm -f replication_part1/tests/out/*

rm -f replication_part2/tests/in/*
rm -f replication_part2/tests/out/*

rm -f infection/tests/in/*
rm -f infection/tests/out/*

rm -f coevolution/tests/in/*
rm -f coevolution/tests/out/*

rm -f synthesis_and_struggle/tests/in/*
rm -f synthesis_and_struggle/tests/out/*



touch -f replication_part1/tests/in/0
touch -f replication_part1/tests/out/0

touch -f replication_part2/tests/in/0
touch -f replication_part2/tests/out/0

touch -f infection/tests/in/0
touch -f infection/tests/out/0

touch -f coevolution/tests/in/0
touch -f coevolution/tests/out/0


touch -f synthesis_and_struggle/tests/in/0
touch -f synthesis_and_struggle/tests/out/0