FROM ams21/cmake:release

WORKDIR /home/src

RUN apk update && \
    apk add git \
            ncurses \ 
            ncurses-dev \
            ffmpeg

CMD ["/bin/sh"]