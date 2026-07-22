FROM docker.io/cloudmeshswnetworks/builder:v2.0.0

RUN apt-get update && apt-get install -y \
  default-jre \
  doxygen \
  graphviz \
  plantuml \
  texlive-fonts-recommended  \
  texlive-latex-base  \
  texlive-latex-extra \
  texlive-latex-recommended  \
  && apt-get clean && rm -rf /var/lib/apt/lists/*
