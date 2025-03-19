from lxml import etree
from dataclasses import dataclass
import math
import click

EXTRA_SIZE = 16

@click.group()
def cli():
    # No options
    pass

@dataclass
class Window:
    x: int
    y: int
    w: int
    h: int

def generate_window(window):
    print(f"ScreenDefs::Window{{{window.x}, {window.y}, {window.w}, {window.h}}}", end = "")

def floor_multiple(x, base=8):
    return base * math.floor(x/base)

def ceil_multiple(x, base=8):
    return base * math.ceil(x/base)

def generate_from_xml(file):
    tree = etree.parse(file)
    buttons = tree.xpath("buttons")[0]
    
    lines = dict()
    nb_buttons = 0
    
    for button in buttons:
        y = button.get('y')
        if not y in lines:
            lines[y] = []
        x0 = max(0, floor_multiple(int(button.get('x'))) - EXTRA_SIZE)
        x1 = min(2400, ceil_multiple(int(button.get('x')) + int(button.get('width'))) + EXTRA_SIZE)
        width = x1 - x0
        x0_converted = 2400 - x1

        y0 = max(0, int(y) - EXTRA_SIZE)
        y1 = min(1034, int(y) + int(button.get('height')) + EXTRA_SIZE)
        height = y1 - y0
        lines[y].append(Window(x0_converted, y0, width, height))
        nb_buttons = nb_buttons + 1

    print(f"const etl::array<Window, {int(nb_buttons/2)}> CheckerBoard::k_rectangles = {{")
    for idx, y in enumerate(lines):
        first_x = idx % 2
        line = lines[y]
        for i in range(first_x, len(line), 2):
            button = line[i]
            generate_window(button)
            print(",")
    print("};")

@cli.command(short_help='Generate the checkerboard data from XML file')
@click.option('--xml', required=True, type=click.STRING, help='XML input file from Nemeio desktop application')
def generate(xml):
    generate_from_xml(xml)

if __name__ == '__main__':
    cli()
