FROM debian:12-slim

WORKDIR /home/src

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
	    git \
            build-essential \
            cmake \
            libncurses5-dev \
            libncursesw5-dev \
            ffmpeg \
            jp2a && \
    rm -rf /var/lib/apt/lists/*

CMD ["/bin/bash"]

