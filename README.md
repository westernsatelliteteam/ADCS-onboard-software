# STM32F412 Bringup Sample Application

## Building Instructions

- Navigate to development/applications/com/build/
- run `make`
- run `make flash` to flash discovery board with st-utils
- run `make pyflash` to flash board with JLink over SWD

## Project Structure

The project's structure is fairly standard. Application code goes into applications, and you can check out the Makefile in `development/applications/com/build` to get a sense of what is being included and from where.

