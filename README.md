
Cli Functions: 
- start monitor
- enable monitor to bootstart
- analyze files for processes (dll for this and use it):
By pid from list

Next:

1) simple driver (no functional, only starting and removing from system)

2) drv-comm-svc get simple message from driver and put into DB

3) commands

Commands list:

## Monitoring

1. Get monitoring status

```powershell
.\regan monitor --status
```

2. Start monitoring

```powershell
.\regan monitor --start
```

3. Stop monitoring

```powershell
.\regan monitor --stop
```

## Analyse

1. Get new bootstart execution files list

```powershell
.\regan analyse --get-list
```

2. Get report about all bootstart execution files from list

```powershell
.\regan analyse --all
```

3. Get report about bootstart execution file by process name

```powershell
.\regan analyse --name "process_name.exe"
```

