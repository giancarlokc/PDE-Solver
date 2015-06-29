set title "FLOPS"
set datafile separator "|
set terminal png size 800,600 enhanced font "Helvetica,20"
set output 'output.png'
plot "grafico_gs_o0_flops.dat" with lines title "-O0", "grafico_gs_o3_flops.dat" with lines title "-O3", 
