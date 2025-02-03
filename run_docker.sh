#!/bin/bash

SVC_NAME=cpp-x86-env
CNT_NAME=cpp-x86-env
DO_CLEAN=false

function show_help() {
    echo "Usage: run_docker.sh [options]"
    echo "  -a Run the ARM container"
    echo "  -x Run the x86 container - DEFAULT"
    echo "  -c Cleanup all docker artifacts"
}

function set_env() {
    if [ ! -f .env ]; then
        echo "USER=${USER}" > .env
        echo "USER_UID=`id -u`" >> .env
        echo "USER_GID=`id -g`" >> .env
    fi
}

while getopts "axch" opt; do
    case ${opt} in
        h)
            show_help
            exit 0
            ;;
        a)
            SVC_NAME=cpp-arm-env
            CNT_NAME=cpp-arm-env
            DO_CLEAN=false
            ;;
        x)
            SVC_NAME=cpp-x86-env
            CNT_NAME=cpp-x86-env
            DO_CLEAN=false
            ;;
        c)
            DO_CLEAN=true
            ;;
        \?)
            echo "Invalid option: ${OPTARG}"
            show_help
            ;;
    esac
done

pushd .devcontainer
if [ ${DO_CLEAN} = true ]; then
    docker-compose down --rmi all --remove-orphans
else
    set_env
    docker-compose up -d ${SVC_NAME}
    docker exec -ti -u ${USER} --workdir /workspace ${CNT_NAME} /bin/bash
fi
popd
