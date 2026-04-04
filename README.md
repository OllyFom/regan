# Regan
## Registry monitor for detect and analyse new bootstart executable files in system (Windows)

Poc version of program, has no installer and auto setup for driver

**Components:**

Command line executable file: regan.exe 
Storage interface lib for sqlite: storage.lib
Project logger: svc-logger.lib
Commands parsing: pjct_commands.lib
Service manager: svc-manager.lib

Monitoring service: drv-comm-svc.exe

Driver filesystem mini-filter - separated project

**Building:**

```cmake

cmake -B E:\some-builds-dir\regan -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake

cmake --build E:\some-builds-dir\regan

```

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
.\regan.exe monitor --status
```

2. Start monitoring

```powershell
.\regan.exe monitor --start
```

3. Stop monitoring

```powershell
.\regan.exe monitor --stop
```

### Analyse

1. Get new bootstart execution files list

```powershell
.\regan.exe analyse --get-list
```

2. Get report about all bootstart execution files from list

```powershell
.\regan.exe analyse --all
```

3. Get report about bootstart execution file by process name

```powershell
.\regan.exe analyse --name="process_name.exe"
```

4. Get report about bootstart execution file by process pid

```powershell
.\regan.exe analyse --pid="1"
```