#!/bin/bash
cd server
/home/ubuntu/ostrta/server/serverGlue.sh &
disown
nohup http-server /home/ubuntu/ostrta/client &
disown
