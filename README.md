# STM32L496 Bringup Sample Application

## Setup

- run `git clone --recurse-submodules https://github.com/westernsatelliteteam/STM32L4BaseProject.git`
- install [docker desktop](https://www.docker.com/products/docker-desktop)
- install [st-link](https://github.com/stlink-org/stlink)

## Building Instructions

- run `make nucleo`
- run `make flash_nucleo` to flash nucleo board with st-utils

## Project Structure

The project's structure is fairly standard. Application code goes into applications, and you can check out the Makefile in `development/applications/com/build` to get a sense of what is being included and from where.

