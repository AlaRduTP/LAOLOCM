mpirun --ompi-server file:uri --mca btl_tcp_if_include ens4 -np 1 server $1 $2