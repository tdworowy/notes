from math import sqrt


class DescriptiveStats:
    def __init__(self, name: str, all_values: list):
        self.name = name
        self._all_values = all_values
        self.total = sum(all_values)
        self.n_revs = len(all_values)

    def mean(self) -> float:
        return self.total / float(self._protected_n())

    def max_value(self) -> int:
        return max(self._all_values)

    def min_value(self) -> int:
        return min(self._all_values)

    def sd(self) -> float:
        std = 0
        mean = self.mean()
        for a in self._all_values:
            std = std + (a - mean) ** 2
        std = sqrt(std / float(self._protected_n()))
        return std

    def _protected_n(self) -> int:
        n = self.n_revs
        return max(n, 1)
