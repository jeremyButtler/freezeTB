#!/bin/bash
printf "Content-type: text/html\n\n"

printf \
   "<htlm> <body> <p> works %s </p> </body> </html>\n" \
   "$QEURY_STRING";
