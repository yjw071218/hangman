- [영어 버전](./README_en.md)
- [한국어 버전](./README_kr.md)



### **[English Version]**

# Hangman Game (C++ Console Application)

## Overview
This Hangman game is a console-based word-guessing game where players guess letters to uncover a hidden word. The game offers multiple categories such as **Animals**, **Fruits**, **Technology**, and more. Each word comes with a difficulty level (Easy, Medium, Hard, Extreme), and players earn points based on the difficulty of the word. The game tracks player progress, offers skip chances, and has a ranking system to compare scores. Players can also earn bonus points for consecutive correct guesses, which rewards skill and strategic guessing.

This game is written in **C++** and uses basic **Windows API functions** for console management such as color changes and screen buffering. The game runs entirely in the terminal window, providing a retro gaming experience with text-based graphics.

## Features
- **Multiple Categories**: 
    - Categories include **Animals**, **Fruits**, **Technology**, and more. Each category contains a list of words with different difficulty levels. Players can freely choose a category to play.
    - Categories contain words with varying difficulty levels (Easy, Medium, Hard, Extreme), but difficulty levels are randomly selected.

- **Difficulty Levels**: 
    - Easy words offer 1000 points, while Extreme words offer 4000 points.
    - Each difficulty level influences how challenging the word is and how many incorrect guesses it takes to lose.

- **Bonus Streak**: 
    - For every consecutive correct letter guess, players earn additional points.
    - This incentivizes careful and thoughtful guesses instead of random attempts.

- **Ranking System**: 
    - Player scores are saved after each game. Scores are ranked based on performance, and players can compare their scores with others.
    - The ranking system records the **student number**, **player name**, and **score**, allowing multiple players to compete on the same machine.

- **Skip Chances**: 
    - Players are provided with 3 skip chances per game to pass difficult words. Using skips wisely can improve the chances of completing the game with a higher score.
  
- **Persistent Score**: 
    - As long as the player continues guessing words correctly, their score persists across categories. This system encourages long-term gameplay and rewards consistency.

- **Timer**: 
    - The game has a **30-second timer** for each guess. If the player doesn't make a guess within the time limit, they lose a turn.

- **Customizable Colors and Animations**: 
    - Each difficulty level is associated with different colors: Green for Easy, Yellow for Medium, Red for Hard, and Purple for Extreme. Additionally, a **progress bar** shows how much time is left for each guess.
    - Hangman drawing changes after each wrong guess, and as time runs out, a shaking timer adds extra pressure.

- **Double Buffering**: 
    - The game uses double buffering to ensure smooth console updates and prevent flickering during gameplay.

- **Loading Animations**: 
    - Several animations enhance user experience, including a loading spinner, score animations, and welcome messages.

## How to Play

1. **Start the Game**: 
    - Run the program by executing the compiled binary or script. You will be presented with a menu offering options to play the game, view rules, or check rankings.

2. **Enter Player Info**: 
    - Players are asked to enter their **student number** (5 digits) and **name**. This information will be used in the ranking system to track player performance.

3. **Select a Category**: 
    - Use the arrow keys to navigate between categories. Each category contains words of various difficulties. Choose the one you prefer and start guessing.

4. **Guess the Word**: 
    - The game will provide a hint for the word, such as “Large animal with a trunk” for the word “Elephant.”
    - Guess letters by typing them on your keyboard. If the letter is correct, it will be revealed in the word. If not, you lose a chance.
    - You can press the **Tab** key to skip the current word (you have 3 skip chances).

5. **Scoring**:
    - You earn points based on the difficulty of the word:
      - Easy: 1000 points
      - Medium: 1500 points
      - Hard: 2000 points
      - Extreme: 4000 points
    - **Bonus points**: You earn bonus points for each consecutive correct guess.

6. **Game Over**: 
    - You have 6 incorrect guesses before the game ends. The game will also end if the timer runs out.
    - If you guess the word correctly before using all guesses, you can continue to the next word, or choose a new category.
    - At the end of the game, your score will be recorded and ranked in the ranking system.

7. **Save Score**: 
    - Your score will automatically be saved in the `ranking.txt` file, along with your name, student number, and timestamp of the game.

## Game Logic

### Word Selection
- Words are chosen randomly from the selected category, and once a word is guessed or skipped, it is removed from the pool to prevent repetition. Players continue guessing words within the chosen category until all words have been guessed or the player runs out of chances.

### Hangman Drawing
- The hangman figure is progressively drawn with each incorrect guess. Players start with 6 lives, and each incorrect guess reduces the number of lives by one. After 6 incorrect guesses, the full hangman figure is drawn, and the game ends with a "GAME OVER" message.

### Scoring System
- Players earn points based on the difficulty of the word:
  - Easy: 1000 points
  - Medium: 1500 points
  - Hard: 2000 points
  - Extreme: 4000 points
- Bonus points are awarded for consecutive correct letter guesses (100 points per streak).
- Players can accumulate scores across multiple categories, with their total score being saved in the ranking system once the game ends.
- If players guess words consecutively without losing a life, they earn a 10% bonus for clearing a category without skipping any words.

### Timer System
- Each guess must be made within a **30-second** time limit, indicated by a progress bar and a countdown timer on the screen. If the time limit expires, the player loses a life.
- When the remaining time falls below **10 seconds**, the timer starts shaking to visually alert the player.

### Skip Chances
- Players can skip difficult words up to 3 times per game. Skipping a word does not affect the player’s lives, but reduces the number of available skips.

### Rankings and Score Saving
- At the end of the game, the player's **student number**, **name**, and **score** are saved in a `ranking.txt` file. Players can view rankings to compare scores with previous players, sorted by score and timestamp.

## Menu Options

1. **Play Game**: 
    - Start the game by entering your student number and name, then choose a category to begin guessing words.
  
2. **View Rules**: 
    - Displays the rules of the game, including scoring details and gameplay instructions.

3. **View Rankings**: 
    - Shows the leaderboard with previous players' scores, sorted by highest score and timestamp.

4. **Exit**: 
    - Exits the game and returns to the terminal.

## Troubleshooting

### Common Issues

1. **Ranking System Not Saving**:
    - Ensure that the game has write permissions to the folder where it is saving `ranking.txt`.
    - Verify that the file is not open in another program while the game is running.

2. **Input Issues (Backspace Not Working)**:
    - On some terminals, the backspace key may not behave as expected. The game handles this by allowing you to press backspace and visually remove characters.

3. **Console Colors Not Displaying Properly**:
    - The game uses Windows API to handle colors in the console. Ensure that your terminal supports colors, and you are running the game in a compatible environment (e.g., Command Prompt on Windows).

4. **Timer Not Working Correctly**:
    - The timer relies on `std::chrono` and accurate system clock functionality. If the timer seems to be inconsistent, check your system clock settings.

5. **Hangman Drawing Not Appearing**:
    - Ensure your console window is large enough to display the hangman drawing. Adjust the window size if needed.

## Folder Structure

The game expects the following folder structure:

```
/hangman
│
├── /x64         # Compiled binary files
│   └── hangman.exe
│
├── /data          # Game data files
│   └── categories.txt    # Words, hints, and difficulty levels
│   └── ranking.txt       # Player rankings and scores
│
├── /src           # Source code
│   └── hangman.cpp
│
└── README.md      # Game documentation
```

- Ensure that `categories.txt` (containing words, hints, and difficulty levels) and `ranking.txt` (for storing player scores) are in the `data` folder for the game to function properly.

## Compilation

To compile the game, use a C++ compiler such as `g++`. The following command will compile the game on any system with GCC installed:

```bash
g++ -o hangman hangman.cpp
```

After compiling, you can run the game using:

```bash
./hangman
```

## Additional Notes

- **Compatibility**: 
    - This game is designed for **Windows** using the Windows API for console color management and screen handling. It may not work properly on non-Windows platforms unless the API calls are adjusted for other operating systems.
  
- **System Requirements**: 
    - The game uses `std::chrono

` for time management, so make sure your system clock is correctly set to ensure proper functionality of the timer.
  
- **File Paths**: 
    - Ensure the `ranking.txt` file is in the same directory as the executable for the ranking system to function correctly.

## Enjoy playing the game!
1. **Ranking System Not Saving**:
    - Ensure that the game has write permissions to the folder where it is saving `ranking.txt`.
    - Verify that the file is not open in another program while the game is running.

2. **Input Issues (Backspace Not Working)**:
    - On some terminals, the backspace key may not behave as expected. The game handles this by allowing you to press backspace and visually remove characters.

3. **Console Colors Not Displaying Properly**:
    - The game uses Windows API to handle colors in the console. Ensure that your terminal supports colors, and you are running the game in a compatible environment (e.g., Command Prompt on Windows).

4. **Timer Not Working Correctly**:
    - The timer relies on `std::chrono` and accurate system clock functionality. If the timer seems to be inconsistent, check your system clock settings.

## Folder Structure

- **Source Code**: 
    - The entire game is contained in a single C++ file (`hangman.cpp`). This file includes all logic, menu, and game functions.
  
- **Ranking File**: 
    - After each game, player scores are saved in a file called `ranking.txt`. This file stores the student number, player name, score, and timestamp.

## Compilation

To compile the game, use a C++ compiler such as `g++`. The following command will compile the game on any system with GCC installed:

```bash
g++ -o hangman hangman.cpp
```

After compiling, you can run the game using:

```bash
./hangman
```

Make sure you have the necessary permissions to run the executable, and that the `ranking.txt` file is located in the same directory as the executable for the ranking system to work correctly.

## Additional Notes

- Ensure the `ranking.txt` file is in the same directory as the executable for the ranking system to function correctly.
- The game is designed for **Windows** using the Windows API for console color management and screen handling. It may not work properly on non-Windows platforms unless the API calls are adjusted for other operating systems.
- The game uses `std::chrono` for time management, so make sure your system clock is correctly set to ensure proper functionality of the timer.


## Enjoy playing the game!
