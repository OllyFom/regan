# Regan
## Registry monitor for detect and analyse new bootstart executable files im system (Windows)

Poc version of program, has no installer and auto setup for driver

**Components:**

Command line executable file: regan.exe 
Storage interface lib for sqlite: storage.lib
Project logger: svc-logger.lib
Commands parsing: pjct_commands.lib
Service manager: svc-manager.lib

Monitoring service: drv-comm-svc.exe

Driver filesystem mini-filter - separated project
Driver project repository: <will be link yet>


**Realisation plan:**

1. filter comunication port 
    1.1 driver (server)
    1.2 service (client)

2. drv-comm-svc get simple message from driver and put into DB changed data

3. Start/stop monitoring service (into commands)

4. Get process info by pid

5. Enable/disable monitor (bootstart)

6. Testing

## Commands list:

### Monitoring

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

### Analyse

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
.\regan analyse --name="process_name.exe"
```

4. Get report about bootstart execution file by process pid

```powershell
.\regan.exe analyse --pid="1"
```