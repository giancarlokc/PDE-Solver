# /bin/bash!

echo "likwid-perfctr -m -C 1 -g FLOPS_DP ./pdeSolver -nx $1 -ny $1 -i 10"

likwid-perfctr -m -C 1 -g FLOPS_DP ./pdeSolver -nx $1 -ny $1 -i 10
