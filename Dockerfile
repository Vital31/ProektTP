FROM  madmanfred/qt5-builder

EXPOSE 2323

RUN apk add --no-cache bash

EXPOSE 2323

RUN cd /usr/local \
    && mkdir server \ 
    && cd server \
    && git clone https://github.com/Vital31/ProektTP.git \
    && cd ProektTP \
    && cd Server \
    && qmake \
    && make

CMD cd /usr/local/server/ProektTP/Server \ 
    && ./Server