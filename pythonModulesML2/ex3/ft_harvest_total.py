def ft_harvest_total() -> None:
    i: int = 1
    qnt: int = 0
    while i <= 3:
        qnt += int(input(f"Day {i} harvest: "))
        i += 1
    print(f"Total harvest: {qnt}")
