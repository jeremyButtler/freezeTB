{
    if($1 ~ /^[a-zA-Z_]*\.[ch]:[0-9]/)
       pBl = 1; # start of error lines

    if(pBl)
       print $0;
};
