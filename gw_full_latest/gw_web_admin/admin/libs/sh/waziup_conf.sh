#!/bin/bash

###################################
# Configure Waziup Orion
###################################

#For example to update write_key for ThingSpeak cloud: 
# jq '.[] | map((select(.script == "python CloudThingSpeak.py") | .write_key) |= "")' clouds.json

tmp=$(mktemp)
if [ $# == 2 ] 
then
        jq '.clouds=([.clouds[]  | select(.script == "python CloudWAZIUP.py") .'$1' = '$2'])' /home/pi/lora_gateway/clouds.json > "$tmp" && mv "$tmp" /home/pi/lora_gateway/clouds.json
	sudo chown -R pi:pi /home/pi/lora_gateway/
        sudo chmod +r /home/pi/lora_gateway/clouds.json
fi
