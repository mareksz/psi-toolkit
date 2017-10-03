PSI-toolkit
===========

PSI-toolkit is a natural language processing processing toolkit
developed at Adam Mickiewicz University in Poznań, Department of Natural Language Processing.


How to run Docker container
---------------------------

Pull Docker container:

    docker pull skorzewski/psi-toolkit

Run PSI-Toolkit in console mode:

    echo "HERE PUT SOME TEXT" | docker run -i skorzewski/psi-toolkit HERE_PUT_PSI_PIPE

Run PSI-Toolkit in console mode (example):

    echo "Alice has a cat" | docker run -i skorzewski/psi-toolkit taokenize --lang en

Run PSI-Toolkit in server mode on port 3000:

    docker run -p 3000:3000 skorzewski/psi-toolkit

