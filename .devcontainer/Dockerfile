FROM ams21/cmake:release

WORKDIR /home/src

RUN apk update && \
    apk add git \
        add ncurses \ 
        add ncurses-dev \
        add ffmpeg

CMD ["/bin/sh"]