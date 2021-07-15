# Copyright 2017 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ------------------------------------------------------------------------------

# Description:
#   Builds an image to be used when developing in C++. The default CMD is to
#   run build_cxx.
#
#   The image is also used by unit and integration tests. Docker is used to
#   interact with and orchestrate the creation of other docker containers.
#
# Build:
#   $ cd sawtooth-sdk-cxx
#   $ docker build . -t sawtooth-sdk-cxx-local
#
# Run:
#   $ cd sawtooth-sdk-cxx
#   $ docker run -v $(pwd):/project/sawtooth-sdk-cxx sawtooth-sdk-cxx-local

FROM ubuntu:xenial-20210114

RUN echo "deb [arch=amd64] http://repo.sawtooth.me/ubuntu/ci xenial universe" >> /etc/apt/sources.list \
 && (apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 8AA7AF1F1091A5FD \
 || apt-key adv --keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys 8AA7AF1F1091A5FD) \
 && apt-get update \
 && apt-get install -y -q \
    apt-transport-https \
    autoconf  \
    automake  \
    build-essential \
    ca-certificates \
    cmake  \
    curl \
    git \
    g++  \
    libcrypto++-dev \
    liblog4cxx-dev \
    libtool \
    libzmqpp-dev \
    libffi-dev \
    libssl-dev \
    make \
    protobuf \
    python3-pip \
    unzip \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/* \
 && pip3 install \
    cpplint

EXPOSE 4004/tcp

RUN mkdir -p /project/sawtooth-core/ \
 && mkdir -p /var/log/sawtooth \
 && mkdir -p /var/lib/sawtooth \
 && mkdir -p /etc/sawtooth \
 && mkdir -p /etc/sawtooth/keys

ENV PATH=$PATH:/project/sawtooth-sdk-cxx/bin

ENV VERSION=AUTO_STRICT
WORKDIR /project/sawtooth-sdk-cxx
CMD build_cxx
