#!/bin/bash
# Author: Jake Mathai
# Purpose: Clean up docker cache
sudo docker-compose down
sudo docker image prune --force
sudo docker container prune --force
sudo docker system prune --force