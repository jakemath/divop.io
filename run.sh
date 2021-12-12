#!/bin/bash
# Author: Jake Mathai
# Purpose: Start web server using docker compose
cd src
sudo docker-compose up -d --build
sudo docker-compose logs -f -t