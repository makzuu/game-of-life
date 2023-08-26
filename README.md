# Game of Life

![](./2023-sdl/life.gif)

## build

dependencies: [SDL2](https://www.libsdl.org/)

```shell
$ cd 2023-sdl/
$ ./build.sh
```

## usage

```shell
$ ./gol
```

### key bindings

| key                    | mode  | description                             |
| :--------------------- | :---- | :-------------------------------------- |
| `q`                    | both  | quit                                    |
| `space`                | both  | toggle mode                             |
| `r`                    | setup | reset the cells to their original state |
| `left click (on cell)` | setup | toggle the state of the cell            |

## references

[Conway's Game of Life](https://en.wikipedia.org/wiki/Conway's_Game_of_Life)
