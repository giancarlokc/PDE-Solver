set title "Running time"
set datafile separator "|
set terminal png size 800,600 enhanced font "Helvetica,20"
set output 'output.png'
plot "outO0.dat" with lines title "-O0", "outO3.dat" with lines title "-O3", 
