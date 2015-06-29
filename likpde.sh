# /bin/bash!

echo "Rodando likwid com $1 nx=ny=$2"

likwid-perfctr -C 1 -g $1 ./pdeSolver -nx $2 -ny $2 -i 10