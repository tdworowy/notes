import subprocess
import re


def _as_rev_range(start: str, end: str) -> str:
    return start + ".." + end


def _run_git_cmd(git_arguments: list) -> str:
    print(git_arguments)
    result = subprocess.Popen(git_arguments, stdout=subprocess.PIPE).communicate()
    print(result)
    return str(result[0])


def _read_revisions_matching(git_arguments: list) -> list:
    git_log = _run_git_cmd(git_arguments)
    revs = []
    # match a line like: d804759 Documented tree map visualizations
    # ignore everything except the commit number:
    rev_expr = re.compile(r"(\S+)")
    for line in git_log.split("\n"):
        m = rev_expr.search(line)
        if m:
            revs.append(m.group(1))
    return revs[::-1]


def _git_cmd_for(rev_start: str, rev_end: str) -> list:
    rev_range = rev_start + ".." + rev_end
    return ["git", "log", rev_range, "--oneline"]


def read_revs(rev_start: str, rev_end: str) -> list:
    """Returns a list of all commits in the given range."""
    return _read_revisions_matching(git_arguments=_git_cmd_for(rev_start, rev_end))


def read_revs_for(file_name: str, rev_start: str, rev_end: str) -> list:
    return _read_revisions_matching(
        git_arguments=_git_cmd_for(rev_start, rev_end) + [f" -- {file_name}"]
    )


def read_diff_for(rev1: str, rev2: str) -> str:
    return _run_git_cmd(["git", "diff", rev1, rev2])


def read_file_diff_for(file_name: str, rev1: str, rev2: str) -> str:
    return _run_git_cmd(["git", "diff", rev1, rev2, file_name])


def read_version_matching(file_name: str, rev: str) -> str:
    return _run_git_cmd(["git", "show", rev + ":" + file_name])
