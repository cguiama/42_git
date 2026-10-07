#!/usr/bin/env python3

import math


def calculate_center_distance(pos: tuple[float, float, float]) -> float:
    distance = 0.0000
    distance = math.sqrt((0.0 - pos[0])**2 + (0.0 - pos[1])**2 +
                         (0.0 - pos[2])**2)
    return distance


def calculate_distance_between(pos1: tuple[float, float, float],
                               pos2: tuple[float, float, float]) -> float:
    distance = 0.0
    distance = math.sqrt((pos2[0] - pos1[0])**2 + (pos2[1] - pos1[1])**2 +
                         (pos2[2] - pos1[2])**2)
    return distance


def get_coordinates() -> tuple[float, float, float]:
    while True:
        try:
            x, y, z = [float(num) for num in input("Enter new coordinates as "
                                                   "floats in format 'x,y,z': "
                                                   ).split(",")]
            break
        except ValueError:
            print("Invalid syntax")
    return x, y, z


def coordinates_parser(text: str) -> float:
    text = text.strip()
    try:
        return float(text)
    except ValueError as e:
        raise ValueError(f"Error on parameter {text!r}: {e}")


def get_second_coordinates() -> tuple[float, float, float]:
    while True:
        text = input("Enter new coordinates as floats in format 'x,y,z': "
                     ).split(",")
        if len(text) != 3:
            continue
        try:
            x = coordinates_parser(text[0])
            y = coordinates_parser(text[1])
            z = coordinates_parser(text[2])
            break
        except ValueError as e:
            print(e)
            continue
    return x, y, z


def get_player_pos() -> None:
    print("=== Game Coordinate System ===\n")
    print("Get a first set of coordinates")
    x, y, z = get_coordinates()
    print(f"Got a first tuple: ({x}, {y}, {z})")
    print(f"It includes: X={x}, Y={y}, Z={z}")
    center_distance: float = calculate_center_distance((x, y, z))
    print(f"Distance to center: {center_distance:0.4f}")
    print("\nGet a second set of coordinates")
    x2, y2, z2 = get_second_coordinates()
    distance_between = calculate_distance_between((x, y, z), (x2, y2, z2))
    print(f"Distance between the 2 sets of coordinates: "
          f"{distance_between:0.4f}")


if __name__ == "__main__":
    get_player_pos()
