# Shelf
Code for Shelf, my submission to the 2023 Guthman musical instrument competition. 

## Organization
`read_distance` contains a sketch that runs on a Bare Conductive Touchboard and writes sensor values to a serial port. 

`patches` contains the Max patch the drive the music engine. `scratch.maxmsp` can be used to read sensor values on a serial port sent by `read_distance.ino`. `shelf.maxpat` is the main entry point for the music engine.  
