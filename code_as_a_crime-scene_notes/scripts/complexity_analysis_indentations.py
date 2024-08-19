import argparse

from desc_stats import DescriptiveStats
from complexity_calculations import complexity_of, contains_code


def as_csv(stats: DescriptiveStats):
    print("n,total,mean,sd,max")
    fields_of_interest = [
        stats.n_revs,
        stats.total,
        round(stats.mean(), 2),
        round(stats.sd(), 2),
        stats.max_value(),
    ]
    printable = [str(field) for field in fields_of_interest]
    print(",".join(printable))


def count_indentations(_args: argparse.Namespace):
    with open(_args.file, "r") as file_to_calc:
        source = file_to_calc.read()
        complexity_by_line = [
            complexity_of(line) for line in source.split("\n") if contains_code(line)
        ]
        stats = DescriptiveStats(_args.file, complexity_by_line)
        as_csv(stats)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Calculates whitespace complexity of the given file."
    )
    parser.add_argument("file", type=str, help="The file to calculate complexity on")
    args = parser.parse_args()
    count_indentations(args)
