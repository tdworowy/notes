import re

leading_tabs_expr = re.compile(r"^(\t+)")
leading_spaces_expr = re.compile(r"^( +)")
empty_line_expr = re.compile(r"^\s*$")


def n_log_tabs(line: str) -> int:
    pattern = re.compile(r" +")
    wo_spaces = re.sub(pattern, "", line)
    m = leading_tabs_expr.search(wo_spaces)
    if m:
        tabs = m.group()
        return len(tabs)
    return 0


def n_log_spaces(line: str) -> int:
    pattern = re.compile(r"\t+")
    wo_tabs = re.sub(pattern, "", line)
    m = leading_spaces_expr.search(wo_tabs)
    if m:
        spaces = m.group()
        return len(spaces)
    return 0


def contains_code(line: str) -> bool:
    return not empty_line_expr.match(line)


def complexity_of(line: str) -> float:
    return n_log_tabs(line) + (n_log_spaces(line) / 4)
