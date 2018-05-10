#echo "dla psa, dla kota i dla wnuczki" | psi-pipe read-text ! tokenize --lang pl ! lemmatize ! puddle
echo "nie ma tu kosza na śmieci" | psi-pipe read-text ! tokenize --lang pl ! lemmatize ! puddle
