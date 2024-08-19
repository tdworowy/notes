```bash
git log --all --numstat --date=short --pretty=format:'--%h--%ad--%aN' --no-renames > git_log.txt
java -jar code-maat-1.0.2-standalone.jar -l git_log.txt -c git2 -a summary
java -jar code-maat-1.0.2-standalone.jar -l git_log.txt -c git2 -a revisions > revisions.csv

cloc ./ --unix --by-file --csv --quiet --report-file=complexity.csv
 ```

```bash
python scripts/git_complexity_trend.py --start deab1263a --end 796d31809 --file D:/Projects/react/packages/react-reconciler/src/ReactFiber.old.js
//TODO it doesn't work  
 ```