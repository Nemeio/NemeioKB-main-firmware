import click
from ecdsa import VerifyingKey

@click.group()
def cli():
    # No options
    pass

def wrap_key_code(key_code):
    header = """
#include "stdint.h"

"""
    return header + key_code

def to_c_code(vk, var):
    binary_key = vk.to_string()
    byte_format = '0x{0:02x}'.format

    key_x = ', '.join(map(byte_format,
                                    binary_key[:32]))
    key_y = ', '.join(map(byte_format,
                                    binary_key[32:]))

    key_code = """
/* ECDSA public key for update packages */
const uint8_t {0}[64] =
{{
{1},
{2}
}};
"""
    key_code_formatted = key_code.format(var, key_x, key_y)
    output_code = wrap_key_code(key_code_formatted)
    return output_code

def do_generate(pem, var, output):
    with open(pem, 'r') as f:
        pubkey = f.read()
        vk = VerifyingKey.from_pem(pubkey)
    
    output_code = to_c_code(vk, var)

    with open(output, "w") as f:
        f.write(output_code)

@cli.command(short_help='Generate C file from PEM key')
@click.option('--pem', required=True, type=click.STRING, help='Path to the signing key')
@click.option('--var', required=True, type=click.STRING, help='C array variable name')
@click.option('--output', required=True, type=click.STRING, help='Output C file path')
def generate(pem, var, output):
    do_generate(pem, var, output)

if __name__ == '__main__':
    cli()
