set title "Time for solving PDE"
set datafile separator "|
set terminal png size 800,600 enhanced font "Helvetica,20"
set output 'output.png'
plot "out.dat" with lines title "out.dat"
