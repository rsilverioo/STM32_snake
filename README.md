# Snake game using an STM32 board and an OLED screen

In this project the classic Snake game has been programmed using the STM32L476-Discovery board with FreeRTOS, the FAT file system to load and save the game files and the SSD1306 OLED display connected via I2C. Two different game modes have been implemented: one using the board's integrated D-pad to move the snake and the other using the integrated gyroscope.

![](https://i.ibb.co/rHQWGTG/snake.png)

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

What things you need to install the software and how to install them:

* The necessary files and libraries are located in the *snake_Game* folder, which is an STM32CubeIDE project. It is recommended to use the same firmware for which the project was created, as porting often causes problems and breaks things.
* The SSD1306 OLED display has been connected via I2C to ports PB6 (I2C1_SDA) and PB7 (I2C1_SCL). It is possible to use the on-board power supply, just check if your display model is running at 5V or 3.3V.
* The STM32CubeMX profile for the configuration of the microcontroller pins is also included in the project as an .ioc file.
* Once the board is programmed you have to add the files from the *board_internal_files* folder using USB OTG. These are the score log and the sound files that the game will use.

### Usage example

The aim of the game is very simple and familiar: eat as much fruit as you can without dying. To lose the game you have to either crash into a part of your body or crash into the walls. There will always be 3 fruits on the screen, and every time the snake eats one, another one is randomly generated.

The menu shows 3 options: the two game modes (to play with the D-pad or with the accelerometer) and the leadaerboard, which shows the highest scores stored in the internal memory of the board. If you beat any of the saved scores, you will be included in the list, if not, you will not. Also, following the aesthetics of retro machines, you can add your initials to your score for them to appear on the leaderboard.

To play with the D-pad, the snake moves in the directions you press (note that you cannot turn 180º).
To play with the accelerometer you will have to tilt the board in the direction you want to go.

And that's it, you can start playing now :)

## Contributing

1. Fork it (<https://github.com/rsilverioo/STM32_snake/fork>)
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request

## Authors

* **Rodrigo Silverio** - Code author - [rsilverioo](https://github.com/rsilverioo)

See also the list of [contributors](https://github.com/rsilverioo/STM32_snake/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Acknowledgments

* STM32 workshops, which are very instructive and easy to follow. ([link](https://www.youtube.com/playlist?list=PLnMKNibPkDnEjzo8KjF1DW2Nfr8n7P4cx))

