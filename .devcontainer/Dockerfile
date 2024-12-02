FROM ubuntu:24.04 as dev_base

# fix for ubuntu stupidly creating a default user
RUN touch /var/mail/ubuntu && chown ubuntu /var/mail/ubuntu && userdel -r ubuntu

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt upgrade -y
RUN apt install -y apt-utils
RUN apt install -y build-essential cmake gdb
RUN apt install -y valgrind
RUN apt install -y git

FROM dev_base

ARG USERNAME
ARG ARG_UID
ARG ARG_GID

# Add host user to the container so that file permissions are correct
RUN groupadd -o --gid $ARG_GID $USERNAME \
    && useradd -l -s /bin/bash --uid $ARG_UID --gid $ARG_GID -m $USERNAME \
    && mkdir -p /etc/sudoers.d \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

USER $USERNAME
