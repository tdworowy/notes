import argparse

import git_interactions
from complexity_calculations import complexity_of, contains_code
from desc_stats import DescriptiveStats


def as_csv(result: list[DescriptiveStats]):
    print("rev,n,total,mean,sd")
    for stats in result:
        fields_of_interest = [
            stats.name,
            stats.n_revs,
            stats.total,
            round(stats.mean(), 2),
            round(stats.sd(), 2),
        ]
        printable = [str(field) for field in fields_of_interest]
        print(",".join(printable))


def calculate_complexity_over_range(file_name: str, revision_range: tuple) -> list:
    start_rev, end_rev = revision_range
    revs = git_interactions.read_revs_for(file_name, start_rev, end_rev)
    complexity_by_rev = []
    for rev in revs:
        historic_version = git_interactions.read_version_matching(file_name, rev)
        complexity_by_line = [
            complexity_of(line)
            for line in historic_version.split("\n")
            if contains_code(line)
        ]
        complexity_by_rev.append(DescriptiveStats(rev, complexity_by_line))
    return complexity_by_rev


def run(args):
    revision_range = args.start, args.end
    complexity_trend = calculate_complexity_over_range(args.file, revision_range)
    as_csv(complexity_trend)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Calculates whitespace complexity trends over a range of revisions."
    )
    parser.add_argument(
        "--start", required=True, help="The first commit hash to include"
    )
    parser.add_argument("--end", required=True, help="The last commit hash to include")
    parser.add_argument(
        "--file", required=True, type=str, help="The file to calculate complexity on"
    )

    args = parser.parse_args()
    run(args)
