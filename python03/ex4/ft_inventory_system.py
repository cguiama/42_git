#!/usr/bin/env python3

import sys


def input_parser(args: list[str]) -> dict[str, int]:
    """Build an inventory, keeping the first valid entry for each item."""
    inventory: dict[str, int] = {}
    for arg in args:
        parts = arg.split(":")
        if len(parts) != 2 or not parts[0].strip():
            print(f"Error - invalid parameter '{arg}'")
            continue
        name, quantity = parts
        if name in inventory:
            print(f"Redundant item '{name}' - discarding")
            continue
        try:
            value = int(quantity)
        except ValueError as error:
            print(f"Quantity error for '{name}': {error}")
            continue
        if value < 0:
            print(f"Quantity error for '{name}': must be non-negative")
            continue
        inventory.update({name: value})
    return inventory


def print_results(inventory: dict[str, int]) -> None:
    """Display quantities, percentages and the first extremes in a tie."""
    items = list(inventory.keys())
    total = sum(inventory.values())
    print(f"Got inventory: {inventory}")
    print(f"Item list: {items}")
    print(f"Total quantity of the {len(items)} items: {total}")
    for item in inventory:
        percentage = round(inventory[item] / total * 100, 1) if total else 0.0
        print(f"Item {item} represents {percentage}%")
    if items:
        most = items[0]
        least = items[0]
        for item in inventory:
            if inventory[item] > inventory[most]:
                most = item
            if inventory[item] < inventory[least]:
                least = item
        print(f"Item most abundant: {most} with quantity {inventory[most]}")
        print(f"Item least abundant: {least} with quantity {inventory[least]}")
    else:
        print("Inventory is empty: no most or least abundant item.")


def main(args: list[str]) -> None:
    print("=== Inventory System Analysis ===")
    inventory = input_parser(args)
    print_results(inventory)
    new_item = "magic_item"
    while new_item in inventory:
        new_item += "_new"
    inventory.update({new_item: 1})
    print(f"Updated inventory: {inventory}")


if __name__ == "__main__":
    main(sys.argv[1:])
