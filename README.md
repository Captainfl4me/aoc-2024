# 🎄 Advent of Code 2024 🎄

<img src="./ressources/c-logo.png" width="164">

Link to the website with puzzle: [Advent Of Code 2024](https://adventofcode.com/2024)

## Makefile

To create a new day simply run: ```make new-day-%``` (and replace `%` with the day formatted as two digits integer).

The same logic apply to `build`, `run` and `test` with: `make build-%`, `make run-%` and `make test-%`

## Use Algorithm

Small list of interesting algorithm used:

Algorithm|File|Small description
--|--|--
DFS|[day 12](./src/day_12.c)|DFS graph exploration
Face counter|[day 12](./src/day_12.c)|Count face for a predefine region
Image Entropy|[day 14 part 2](./src/day_14.c)|Measure of entropy of an image to find what images are random noise.
Dijkstra's algorithm|[day 16](./src/day_16.c)|Find min cost path in weighted graph.
