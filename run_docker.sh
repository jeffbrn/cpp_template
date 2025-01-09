#!/bin/bash

function set_env() {
    if [ ! -f .env ]; then
        echo "USER=${USER}" > .env
        echo "USER_UID=`id -u`" >> .env
        echo "USER_GID=`id -g`" >> .env
    fi
}

pushd .devcontainer
set_env
docker-compose up -d
docker exec -ti -u ${USER} --workdir /workspace cpp-user-dev /bin/bash
popd