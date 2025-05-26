#!/bin/bash

# Verifica se a variável de ambiente CONTENT_LENGTH está definida
if [ -z "$CONTENT_LENGTH" ]; then
    echo "Erro: CONTENT_LENGTH não está definida" >&2
    exit 1
fi

# Lê exatamente $CONTENT_LENGTH bytes do stdin
read_input=$(dd bs=1 count="$CONTENT_LENGTH" 2>/dev/null)

# Imprime a entrada lida
printf '%s\n' 'Content-Type: text/plain'
printf '%s\n' "$read_input"

