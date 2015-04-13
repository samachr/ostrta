#node shell.js  < fifo | ./searchEngine/a.out > fifo

mkfifo fifo0 fifo1
( /home/ubuntu/ostrta/server/a.out > fifo0 < fifo1 ) &
( node /home/ubuntu/ostrta/server/server.js > fifo1 < fifo0 ) &
( exec 30<fifo0 31<fifo1 )
rm fifo0 fifo1
