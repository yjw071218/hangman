- [영어 버전](./README_en.md)
- [한국어 버전](./README_kr.md)

# Hangman Game (C++ Console Application)

## Overview
This Hangman game is a console-based word-guessing game where players guess letters to uncover a hidden word. The game offers multiple categories with varying difficulty levels, and players earn points based on their correct guesses. Additional bonus points are awarded for consecutive correct guesses, and there is also a ranking system to track player scores.

## Features
- **Multiple Categories**: Players can choose from different categories like Animals, Fruits, Technology, and more.
- **Difficulty Levels**: Each word has a difficulty level (Easy, Medium, Hard, Extreme), and players earn points based on the difficulty.
- **Bonus Streak**: Players get bonus points for consecutive correct guesses.
- **Ranking System**: After each game, the player's score is saved in a ranking file.
- **Skip Chances**: Players can skip a word up to 3 times during the game.
- **Persistent Score**: The score accumulates as the player continues guessing words across different categories.

## How to Play

1. **Start the Game**: Run the program and choose `Play Game` from the menu.
2. **Enter Player Info**: Enter your student number and name.
3. **Select a Category**: Use the arrow keys to navigate and select a category of words.
4. **Guess the Word**: You will be provided with a hint. Guess letters by typing them one at a time.
5. **Scoring**:
   - You earn points based on the difficulty of the word:
     - Easy: 1000 points
     - Medium: 1500 points
     - Hard: 2000 points
     - Extreme: 4000 points
   - Bonus points: You earn 100 points for each consecutive correct guess.
6. **Game Over**: You have 6 incorrect guesses before the game ends. If you guess the word correctly before using all guesses, you can continue to the next word.
7. **Save Score**: Your score is saved to a ranking file after the game ends.

## Menu Options
- **Play Game**: Start the game by entering player information and selecting a category.
- **View Rules**: Learn how to play the game and understand the scoring system.
- **View Rankings**: See the list of players and their scores.
- **Exit**: Exit the game.

## Input Keys
- **Arrow Keys**: Use to navigate through the menu.
- **Enter**: Select a menu option.
- **Letters**: Guess a letter in the word.
- **Tab**: Skip the current word (limited to 3 skips per game).

## Folder Structure
- **Source Code**: The source code is contained in a single C++ file that can be compiled and run in a Windows environment.
- **Ranking File**: After playing the game, player scores are saved in `ranking.txt`.

## Compilation
To compile the game, use the following command:
```bash
g++ -o hangman hangman.cpp
```

Then, run the game using:
```bash
./hangman
```

## Additional Notes
- Ensure the `ranking.txt` file is in the same directory as the executable for the ranking system to function correctly.
- The game is designed for Windows using the Windows API for console color management and screen handling.

Enjoy playing the game!
