#!/bin/bash

#this is a test script for managing the update.sh script so that is runs everyday in the background.
while true; 
	do 
	/home/sickles-lab/sPHENIX/LightTransmission/upload_lighttrans.sh
    sleep 24h
done