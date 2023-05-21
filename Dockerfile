# our local base image
FROM ubuntu:latest

LABEL description="Container for use with Visual Studio" 

# install build dependencies 
RUN apt-get update && apt-get install -y g++ make 

COPY . .

RUN ls
RUN g++ -o upload scripts/upload.cpp
RUN echo "teste"
RUN ls
RUN ./upload scripts/artigo.csv
RUN g++ -o findrec scripts/findrec.cpp 
RUN ./findrec arquivo_dados.bin
