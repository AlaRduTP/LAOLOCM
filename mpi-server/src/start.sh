ompi-server -r uri
echo sleep 5 secs ...
sleep 5
echo 'To start a Client, run the following command:'
echo 'bash mpi-client/start.sh "'`cat uri`'"'
mpirun --ompi-server file:uri --mca btl_tcp_if_include eth0 -np 1 server $1 $2