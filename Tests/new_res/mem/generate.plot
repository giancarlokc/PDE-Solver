set title "3D surface from a grid (matrix) of Z values"
set datafile separator "|
set terminal png size 800,600 enhanced font "Helvetica,20"
set output 'output.png'
plot "grafico_gs_o0_mem.dat" with lines title "-O0", "grafico_gs_o3_mem.dat" with lines title "-O3", 
