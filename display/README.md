# Display

IO will be a big part of V2.

The plan is to use web sockets to create general purpose interaction channels. 

This folder will contain a python display device that will eventually connect to the main vm.

requires pygame

## Details:
Instructions are passed by the VM through the network socket in 32-bit chunks, this display example is a one-way device. No messages will be sent back to the vm.

The vm will have an instruction for writing raw data to the IO buffer. Then a special IO instruction will initiate the transaction. It is up to the programmer to write commands that the display can interpret.

## Commands:
There are 32-pixels the first 16 bits are responsible for specifying the coordinate being accessed, and the method to use.
### Colour pixel:
|1st 8-bits     |2nd 8-bits     |3rd 8-bits and 4th 8-bits      |
|---            |---            |---                            |
|(001)-(x-coord)|(001)-(y-coord)|(0000)-(Red)-(Green)-(Blue)    |

### Write character: 
|1st 8-bits     |2nd 8-bits     |3rd 8-bits     |4th 8-bits     |
|---            |---            |---            |---            |
|(010)-(x-coord)|(010)-(y-coord)|(00)-(Red)-(Green)-(Blue)|(ASCII CHAR)    |

### Write Block: (not implemented)
Initiates block writing loop, all future data will be interpreted as text and written line by line. To quit this mode pass the quit command.
|1st 8-bits     |2nd 8-bits     |3rd 8-bits     |4th 8-bits     |
|---            |---            |---            |---            |
|(11111111)     |(00000000)     |(00000000)     |(10000000)     |

Then pass text, as follows:
|1st 8-bits     |2nd 8-bits     |3rd 8-bits     |4th 8-bits     |
|---            |---            |---            |---            |
|(00)-(Red)-(Green)-(Blue)|(ASCII CHAR)    |(00)-(Red)-(Green)-(Blue)|(ASCII CHAR)    |

Lines and the entire screen will wrap.

### Color Block: (not implemented)
Color rectangle with passed color:
|1st 8-bits     |2nd 8-bits     |3rd 8-bits and 4th 8-bits     |
|---            |---            |---                           |
|(11111111)     |(00000001)     |(0000)-(Red)-(Green)-(Blue)   |

Then:
|1st 8-bits     |2nd 8-bits     |3rd 8-bits     |4th 8-bits     |
|---            |---            |---            |---            |
|(001)-(x-coord-left)|(001)-(y-coord-top)|(001)-(x-right)|(001)-(y-bottom)|

No quit is required.

### Quit:
|1st 8-bits     |2nd 8-bits     |3rd 8-bits     |4th 8-bits     |
|---            |---            |---            |---            |
|(11111111)     |(11111111)     |(111111111)    |(1111111)      |
