def count_days(current: int, limit: int) -> None:
    if current <= limit:
        print(f"Day {current}")
        current += 1
        count_days(current, limit)


def ft_count_harvest_recursive() -> None:
    days = int(input("Days until harvest: "))

    count_days(1, days)
    print("Harvest time!")
