# STM32L496 Bringup Sample Application

## Requirements
- install [docker desktop](https://www.docker.com/products/docker-desktop)

## Setup

- run `git clone --recurse-submodules https://github.com/westernsatelliteteam/STM32L4BaseProject.git`

## Building Instructions


- From Top-Level Folder:
- run `docker compose up` to compile the project

## Project Structure

The project's structure is fairly standard. Application code goes into applications, and you can check out the Makefile in `development/applications/com/build` to get a sense of what is being included and from where.

