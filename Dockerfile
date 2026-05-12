from ubuntu:latest

USER root

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        libncurses5-dev \
        libncursesw5-dev \
        ffmpeg \
        jp2a \
    && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /home

CMD ["/bin/bash"]
