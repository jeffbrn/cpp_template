#!/bin/bash

echo "USER=${USER}" > .env
echo "USER_UID=`id -u`" >> .env
echo "USER_GID=`id -g`" >> .env
