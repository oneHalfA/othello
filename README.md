# Reversi (Othello)

Reversi, also known as Othello, is a strategy board game for two players, played on an 8×8 uncheckered board. The objective is to have the majority of your color discs on the board at the end of the game.

## Game Rules

1. **Starting Position**:
   - The game starts with four discs placed in the center of the board in a square formation. The two black discs are placed on one diagonal, and the two white discs are placed on the other diagonal.

2. **Turns**:
   - Black always goes first.
   - Players take turns placing a disc of their color on an empty square.

3. **Flipping Discs**:
   - When a disc is placed on the board, it must outflank one or more of the opponent's discs.
   - To outflank means to place a disc such that there is a continuous line (horizontal, vertical, or diagonal) of the opponent's discs between the newly placed disc and another disc of the player's color.
   - All of the opponent's discs that are outflanked are flipped to the player's color.

4. **Valid Moves**:
   - If a player cannot make a valid move, they must pass their turn.
   - If both players cannot make a valid move, the game ends.

5. **End of the Game**:
   - The game ends when neither player can make a valid move, usually when the board is full.
   - The player with the majority of their color discs on the board wins.

## Installation

To play the game, clone this repository and build it.

```sh
git clone https://github.com/oneHalfA/othello.git
cd reversi
make   # or compile it on your own.
./exec
