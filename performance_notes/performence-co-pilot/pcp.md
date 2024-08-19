packages needed
```bash
apt install pcp pcp-gui pcp-webjs pcp-webapi pcp-doc
```

run data-gathering daemon
```bash
sudo systemctl enable pmcd pmlogger
sudo systemctl start pmcd pmlogger
```

basic statistic 
```bash
pmstat
```

display information about performance metrics
```bash
pminfo
pminfo -T <metric name> 
pminfo -d <metric name> (description)
pminfo -f <metric name> (value)
```

display chart
```bash
pmchart
```