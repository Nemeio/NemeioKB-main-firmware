#!/bin/bash -e

PUBLIC_KEY_PEM=$1"/public_key/ecdsa_pkg_key_public.pem"
PUBLIC_KEY_C_FILE=$1"/update_package_ecdsa_public_key.c"
PUBLIC_KEY_VAR="update_package_ecdsa_public_key"

module="[generate update package key code]"

command="python generate_key_code.py generate --pem "$PUBLIC_KEY_PEM" --var "$PUBLIC_KEY_VAR" --output "$PUBLIC_KEY_C_FILE
echo $command
$command