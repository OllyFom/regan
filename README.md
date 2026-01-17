Cli For regedit monitor

Functions: 
- start monitor
- enable monitor to bootstart
- analyze files for processes (dll for this and use it):
By pid from list


Pattern for cli - fabric
architecture:

analyser    monitor
    ^           ^
    |           |
    command_fabric
          ^
          |
         cli
    
analyser or monitor (BaseCommand childs)

struct of repos is ready

Need sqlite