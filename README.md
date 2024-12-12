# Assignment 4 CSF - Entertaining Spice Pretending

## Introduction

In Assignment 4 for computer science focused studies (A4-CSF) of the ESP practicals (KU), you will implement a card 
game for two players, which is loosely inspired by the card game [Spicy](https://de.wikipedia.org/wiki/Spicy_(Kartenspiel)).
Please take care to follow the assignment description exactly, even if you know the original game, because this version
uses different rules than the original and even slight differences in your implementation can cause
testcases to fail.

This document describes the structure of the assignment and the expected functionality of the program.

## Learning  Goals

- Heap and Memory Management
- File I/O
- Structs
- Enums
- Linked Lists

## Specifications

Some general specifications must be followed for every assignment in this course. A list of these specifications can be found here:
[Assessment of Assignments - Deductions](https://www.notion.so/coding-tugraz/Assessment-of-Assignments-How-do-you-get-your-points-d7cca6cc89a344e38ad74dd3ccb73cb3?pvs=4#e3203e50b1fc4ba787bdbf5fb46262ae). Please follow these specifications to avoid point deductions. As memory management is a central learning goal of this assignment, pay close attention to this section within the specifications.

Additionally, the following specifications must be followed for this assignment (A4-CSF) as well:
* **Attention:** Each card in the game has to be represented as a **struct**. These structs should hold at least two 
  values, the spice associated with each card and the value of the card. Feel free to extend the structs of the cards 
  with different attributes that you deem necessary for your implementation. If structs are not used for the cards, this will result in a **deduction of up to -25 %**.
* **Attention:** All cards must be stored **on the heap**. If this is not the case, this will result in a **deduction of up to -25 %**.
* **Attention:** All rows of cards in the game, namely hand cards and the draw pile, have to be 
  represented internally as a **linked list** data structure. If this is not the case, this will result in a **deduction of up to -25 %**.
* **Attention:** As the user input can be of arbitrary length, it must be read in dynamically using **realloc**. If this is not the case, this will result in a **deduction of up to -25 %**.

## Grading

Assignment 4 CSF is worth **40 points**.

**Attention:** To receive a positive grade on the ESP KU **at least 50 % of the public test cases have to be 
passed** (among other conditions). This means that at least four of the public testcases have to be passed.

Information on how your assignment submission will be assessed can be found here: [Assessment of Assignments](https://www.notion.so/coding-tugraz/Assessment-of-Assignments-How-do-you-get-your-points-d7cca6cc89a344e38ad74dd3ccb73cb3).
Please read this carefully as it also contains guidelines on what your final code should look like (Coding Standard, Style Guide).

Here you can find an overview of how your total grade for the KU will be calculated: [Procedure and Grading](https://www.notion.so/coding-tugraz/Procedure-and-Grading-59975fee4a9c4047867772cd20caf73d)

## Submission
- Deliverables: `a4-csf.c`
- Push to your repository
- **Deadline: 15.01.2025, 23:59 (Austrian time)**

## Weekly Exercises

There are **Weekly Exercises** associated with this assignment. These exercises are provided on the
[onlineGDB](https://www.onlinegdb.com/) website.
These are simple exercises meant to help you practice before starting the assignment, and **must** be solved at least
partially to avoid deductions. Take a look at our [Guide for onlineGDB](https://www.notion.so/coding-tugraz/Guide-for-onlineGDB-c8a3d7e6b5d24b569a6f85593c0c9d75) for information on how to
register and solve the exercises.

**Please pay close attention to the instructions about choosing a username!** If you
do not use the correct username, we won't be able to identify your solutions.

- **Attention**: It is mandatory to correctly solve at least 50 % of the exercises tagged with [A4] before the A4-CSF
  deadline. Otherwise, you will receive a **point deduction of -25 % on A4-CSF**.

## Assignment Description

In this assignment you will implement a card game for two players which is loosely inspired by 
[Spicy](https://de.wikipedia.org/wiki/Spicy_(Kartenspiel)). All cards have two characteristics: a spice and a value. 

The game is played in multiple rounds. Each round consists of the players taking alternating turns. On their turn, a 
player plays a card from their hand **face down** and tells the other player what the card shows. The other player 
can believe what has been said and continue with their own turn, or challenge the first player on one of the two 
characteristics to see if it was a lie. 

Players score points for catching lies, but if a player challenges a card and the challenged characteristic was not a lie, the 
other player gets points. Challenging a card ends the current round and starts the next one. 

The game ends when the draw pile is empty, and the player with the most points wins.

### General Notes
The notes given in this section are important for the whole assignment description.

- `\n` shouldn't be printed as separate characters, instead it represents the newline character.
- Note the leading and trailing whitespaces in the text fields! You can make them visible by selecting the text field.
- Words in uppercase in sharp brackets (like `<AMOUNT>`) should be replaced by calculated or given data. They are *not* 
  part of the output itself! For a better understanding you can look at the example outputs at the end of this document.
- The user input is **case-insensitive**. This means that `QUIT`, `quit` and `qUiT` would all be valid inputs.
- `P<NR> > ` (command prompt, always printed with a leading player identifier and a trailing space) in the output signifies that the program should wait for a user input at this point. Execution should only continue after the user has ended their input with `ENTER`.
- If the user's input is invalid, the corresponding error message should be printed. After this **the command prompt (including the player identifier)** is printed again and the user can make a new input. This process is repeated as long as the input is invalid. 
- Make sure that your program can handle inputs of the wrong type, for example string input when a number is expected. Also make sure that your program can handle inputs of arbitrary length including empty inputs.
- All additional whitespaces (leading, trailing, or additional whitespaces between command and parameters or between two parameters) in the input should be ignored.
- Take care with your memory management! That means that when allocating memory on the heap, you always need to check that the system was able to provide that memory. If the program runs out of memory you should first free all memory previously allocated on the heap, then print the correct error message and terminate the program with the corresponding return value.

## Program Sequence


<details>
<summary><h4>Starting the Game</h4></summary>

The program is called with one command line parameter. This parameter is the path to a
config file that contains the game cards, and should be loaded at start.

For example:

```
./a4-csf configs/config_01.txt
```

If the program is called
- with the wrong number of parameters
- with a config file that cannot be opened for reading
- with a config file that does not start with the correct [magic number](https://en.wikipedia.org/wiki/Magic_number_(programming))

the corresponding error message should be printed and the program should terminate with the correct return
value (see Return Values and Error Messages).

<details>
<summary><h5>Parsing the Config File</h5></summary>

Every config file contains a [magic number](https://de.wikipedia.org/wiki/Magische_Zahl_(Informatik)) in the first line
(`ESP\n`). This string has to be checked for its correctness. If the magic number is correct you can assume that the file is correctly formatted and no further checks of the file's content are needed.

The beginning of a valid config file could for example look like this:

```txt
ESP\n
3_w\n
10_c\n
8_w\n
9_p\n
5_c\n
7_p\n
3_w\n
1_c\n
<...>
```

- The first line specifies the magic number, which has to be `ESP\n` in ASCII-text.
- After that the cards for the game are listed one card per line in the format `<VALUE>_<SPICE>`.
- Keep in mind the newline character `\n` at the end of each line.

Every card consists of an integer number equal or greater than `1` and equal or less than `10` (the value) and one of
three spices  (wasabi `w`, chili `c`, pepper `p`). You can find an example for a valid config file [here](
configs/config_01.txt).

To represent the attributes of the individual cards in a structured way you have to use a **struct** for each card to 
store the numerical value and the spice. Feel free to extend this struct with any member variables that you 
think you need in your program. 

Cards are not unique, so duplicates of the same value and spice combination may appear in the list.

</details>

<details>
<summary><h5>Welcome Message</h5></summary>

At the beginning of the game the following welcome message is printed:

```
Welcome to Entertaining Spice Pretending!\n
```

</details>

<details>
<summary><h5>Distributing the Cards</h5></summary>

Next the cards are distributed in the order that they appear in the config file to the players until both players have 
six hand cards. A config file with the correct magic number will always contain enough cards for this distribution,
plus at least one additional card for the draw pile. 

This would look as follows: `Player 1` gets the first card from the config file, then `Player 2` gets the second card.
Now every player has one card and the second distribution round starts, that means again `Player 1`
receives the third card, then `Player 2` the fourth. Then the third distribution round starts...

The hand cards of each player have to be stored as a **linked list**. 

The leftover cards after six cards per player have been distributed will be the draw pile for the game. The draw 
pile should be stored as a **linked list** in the same order as in the config file as well. This means that the 13th 
card from the config file would be the top card of the draw pile, the 14th card from the config file would be the 
second card of the draw pile, and so on.

After the cards have been distributed the first round starts.

</details>

</details>

<details>
<summary><h4>Playing the Game</h4></summary>

<details>
<summary><h5>Starting a New Round</h5></summary>

Whenever a new round starts, first print this header: 
```
\n
-------------------\n
ROUND START\n
-------------------\n
```
Then start the turn of the starting player. In the first round, the starting player is `Player 1`. A round lasts 
until a player challenges a card (see Command: challenge). In the following round, the player who **lost** the 
challenge will be the starting player.

The round (and the game) ends immediately when the draw pile is empty (see Finishing the Game).

</details>

<details>
<summary><h5>Player Turn</h5></summary>

On the start of a player's turn, important information for their turn is printed. This consists of:

- the latest card that was played
- if the latest played card was the opponent's last hand card
- how many cards have been played in this round (= since the last challenge)
- the current player's hand cards 

All cards are printed in the format <code>&nbsp;\<VALUE>_\<SPICE></code> (Note the leading space!). If there are no 
cards in a line the `\n` is supposed to be directly after the colon.

The player info should be printed in the following format:
``` 
\n 
Player <NR>:\n
    latest played card:<LATEST_PLAYED_CARD><LAST?>\n
    cards played this round: <AMOUNT>\n
    hand cards:<HAND_CARDS>\n
```
- `<NR>` is the current player's identifier (either `1` or `2`)
- `<LATEST_PLAYED_CARD>` is the latest card to be played (`<PLAYED_AS_CARD>` parameter of the latest `play` command, 
  see Command: play), or nothing if the round has just started. `<LATEST_PLAYED_CARD>` might be a card played by the 
  opponent, or the player's own played card if the opponent did not play a card on their last turn.
- `<LAST?>` should be replaced by ` LAST CARD` (Note the leading space!) if the opponent does not have anymore hand 
  cards. Otherwise, it should be omitted.
- `<AMOUNT>` is the total amount of cards already played this round
- `<HAND_CARDS>` is the current player's hand cards **sorted by spice in the order first `chili`, then `pepper`, 
  then `wasabi`**. Within each spice, the cards should be **sorted by ascending value**.

Next, the command prompt `P<NR> > ` is printed and the program waits for user input. The current player can input 
different commands which must be handled including error handling. If a player inputs an unrecognized command or there are 
too many or not enough parameters for the command, it should not be executed. Instead, the correct error message (see 
Return Values and Error Messages) should be printed, the command prompt including the player identifier should be 
printed again and the player should be able to input another command.

<details>
<summary><h5>Command: quit</h5></summary>

- `quit`

It should always be possible to terminate the program with the command `quit` whenever the
program is waiting for user input.

The `quit` command has no parameters. If `quit` is called with any parameters, the correct error message (see Return Values and Error Messages) should be printed, the command prompt should be printed again and the program should wait for a new user input.

If a valid `quit` command is entered, the program should terminate with the return value `0` (see Return Values and Error Messages). Make sure to free all previously allocated memory before termination.

</details>

<details>
<summary><h5>Command: play</h5></summary>

- `play <HAND_CARD> <PLAYED_AS_CARD>`

Example:
```
play 3_w 10_c
```

The `play` command removes a card from the current player's hand cards and places it on the pile of cards that have
already been played this round. It also lets the player tell the truth or lie about what the card shows. 

> **Note**: For a 'real' game, this would be implemented in a way where the players cannot see
> each other's inputs to make actual bluffing possible.

`play` is always possible unless the opponent has played their last card.

The first card played in a round decides which spice will be played this round. For the rest of the round, players have to at 
least pretend to play cards from the same spice, though the spice can be a lie.

Additionally, the value of the card the player says they put down has to be higher than the latest card that has 
been played previously. If there is no previously played card (i.e. this will be the first card of the round), or the 
latest played card has the value `10` (the highest possible value), the next played card has to have a pretend value of 
less than or equal to `3`, though the value can be a lie.

The `play` command has two parameters: 
- `<HAND_CARD>` is the real card to be chosen and will be removed from the current player's hand cards. This can be any 
  card from the player's hand cards, there are no other restrictions.
- `<PLAYED_AS_CARD>` is the card the player says they have put down. This can match the chosen hand card, or be a lie 
in one or both characteristics. Starting from the next turn, this card will appear in the `<LATEST_PLAYED_CARD>` info 
  until another card is played. 
  The `<PLAYED_AS_CARD>` parameter must follow the following **game rules** to be valid:
  - If the latest played card's value is `10` or this is the first card to be played in this round, the value 
    has to be more than or equal to `1` and less than or equal to `3`. Otherwise, the value has to be higher than the 
    latest played card's value.
  - The spice has to be the same as the first card played this round. If it is the first card to be played in this 
    round, there are no restrictions on the spice.

Both the parameters `<HAND_CARD>` and `<PLAYED_AS_CARD>` must be entered in the following format: an integer (= a 
sequence of **only numerical characters**) between `1` and `10` (inclusive), followed by an underline `_`, followed 
by `c`, `p` or `w`.  Remember that all commands and parameters are case-insensitive!

The following errors must be handled for the `play` command:
- invalid number of parameters
- `play` cannot be used at the moment, because the opponent has played their last hand card
- `<HAND_CARD>` or `<PLAYED_AS_CARD>` was not entered in card format
- `<HAND_CARD>` is not in the current player's hand
- `<PLAYED_AS_CARD>`'s `<VALUE>` does not follow the game rules
- `<PLAYED_AS_CARD>`'s `<SPICE>` does not follow the game rules

If any of these errors occur, the correct error message (see Return Values and Error Messages) should be printed, the 
command prompt should be printed again and the program should wait for a new user input from the same player.

On successful execution of a `play` command, the current player's turn ends, the `<AMOUNT>` of cards already played 
this round increases by one, and the other player's turn starts by printing their information.



</details>

<details>
<summary><h5>Command: challenge</h5></summary>

- `challenge <TYPE>`

Example:
```
challenge spICe
```

The `challenge` command challenges the card the other player said they put down in their latest `play` command 
(`<LATEST_PLAYED_CARD>`). A player can use this if they think that the other player is lying.

`challenge` is only possible if the opponent has played a card on their latest turn. It is not possible if the round 
has just started, because there is no card to challenge, or if the other player has just drawn a card. If the opponent 
has just played their **last** hand card, the current player **must** challenge it.

The `challenge` command has one parameter. The parameter `<TYPE>` has to be either `value` or `spice`. Remember that all 
commands and parameters are case-insensitive!

The following errors must be handled for the `challenge` command:
- invalid number of parameters
- `challenge` cannot be used at the moment, because the opponent did not play a card on their latest turn
- `<TYPE>` is not `value` or `spice`

If any of these errors occur, the correct error message (see Return Values and Error Messages) should be printed, the 
command prompt should be printed again and the program should wait for a new user input from the same player.

When a valid `challenge` command is executed, the program checks if the challenge was successful or not.

If the opponent lied about the characteristic that was challenged (spice or value, depending on the `<TYPE>` parameter)
on their latest played card, the challenging player (= current player) wins the challenge and the opponent loses the 
challenge.

If the opponent told the truth about the characteristic that was challenged, the challenging player (= current 
player) loses the challenge and the opponent wins the challenge.

The result of the challenge is not influenced by the characteristic that was not challenged, no matter if the 
opponent lied about it or not.

If the current player wins the challenge, print the following message:
```
Challenge successful: <LATEST_PLAYED_CARD>'s <TYPE> does not match the real card <REAL_CARD>.\n
```

If the current player loses the challenge, print the following message:
```
Challenge failed: <LATEST_PLAYED_CARD>'s <TYPE> matches the real card <REAL_CARD>.\n
```
- `<LATEST_PLAYED_CARD>` should be replaced by the card that the opponent said they put down (the one that is
  currently displayed in the `<LATEST_PLAYED_CARD>` slot of the player info)
- `<REAL_CARD>` should be replaced by the card that the opponent really put down (the one that was removed from
  their hand cards)
- `<TYPE>` is the type parameter of the `challenge` command in lowercase letters

The player who won the challenge gets points equal to the amount of cards previously played in this 
round added to their score. Print the following message:
```
Player <NR> gets <AMOUNT> points.\n
```
- `<NR>` is the identifier of the player who won the challenge
- `<AMOUNT>` is the amount of cards previously played in this round

If the player who put down their last card wins the challenge, another 10 bonus points are added to their score. 
Print the following message:
```
Player <NR> gets 10 bonus points (last card).\n
```
- `<NR>` is the identifier of the player whose last card was challenged

After resolving the challenge, one or both players draw additional cards. First, the player that lost the challenge 
draws two cards. Then, if the winner of the challenge has no hand cards left, they draw six new cards. Drawn cards are 
removed from the top of the draw pile and added to the player's hand cards. If the last card of the draw pile is drawn during 
this process, the game immediately ends (see Program Sequence > Finishing the Game).

On successful execution of a `challenge` command, a new round starts. The starting player of the new round is the 
player that lost the challenge. The `<LATEST_PLAYED_CARD>` slot should be emptied and the amount of cards played this 
round should be reset to `0`. Players keep their hand cards.

</details>


<details>
<summary><h5>Command: draw</h5></summary>

- `draw`

The `draw` command removes the first card from the draw pile and adds it to the current player's hand cards. If the 
drawn card is the last card of the draw pile, the game immediately ends (see Program Sequence > Finishing the Game).

`draw` is always possible unless the opponent has played their last hand card.

`draw` has no parameters. 

The following errors must be handled for the `draw` command:
- invalid number of parameters
- `draw` cannot be used at the moment, because the opponent has played their last hand card

If any of these errors occur, the correct error message (see Return Values and Error Messages) should be printed, the 
command prompt should be printed again and the program should wait for a new user input from the same player.

On successful execution of a `draw` command, the current player's turn ends and the other player's turn starts by
printing their information. The latest played card does not change, but it can't be challenged anymore.

</details>

</details>
</details>


<details>
<summary><h4>Finishing the Game</h4></summary>

When the last card is drawn from the draw pile, the game immediately ends. Afterward, the winner is calculated and the 
results are documented in a file.

<details>
<summary><h5>Scoring</h5></summary>

During the game, the players have gotten points from winning challenges (either their own or their opponent's), 
including bonus points winning when their last hand cards were challenged. All these points from all rounds are added 
together.

The player with the most points wins the game. The program should print a list of both players sorted by their
points in descending order. Then the program should congratulate the winner.

```
\n
Player <NR>: <POINTS> points\n
Player <NR>: <POINTS> points\n
\n
Congratulations! Player <NR> wins the game!\n
```

For example:
```
\n
Player 1: 12 points\n
Player 2: 6 points\n
\n
Congratulations! Player 1 wins the game!\n
```

If both players have the same amount of points they should additionally be sorted in ascending order by player index.
In this case both players should be congratulated in ascending order by player index.

```
\n
Player 1: 12 points\n
Player 2: 12 points\n
\n
Congratulations! Player 1 wins the game!\n
Congratulations! Player 2 wins the game!\n
```

</details>

<details>
<summary><h5>Documenting the Results</h5></summary>

Before you terminate the program you should also append the final score that the players received to the end of the 
loaded config file without overwriting any of the previous content. The score should be written to the file in the 
same format as you printed it to the command line. There should be one empty line between the last card in the 
config file and the scores. The file should end after the final congratulations message without any additional lines.

For example, the end of a config file after documenting the results could look like this:

```txt
<...>
10_w\n
8_c\n
3_p\n
\n
Player 1: 23 points\n
Player 2: 15 points\n
\n
Congratulations! Player 1 wins the game!\n
```

If the file cannot be opened for writing, print the following warning message to the console:

```
Warning: Results not written to file!\n
```

Afterward, the program terminates with return value `0`.

> **Hint:** Executing `make reset` will reset all config files to their original content. Executing `make run` or `make test` also automatically includes this reset.

</details>

</details>

## Return Values and Error Messages

The program has to react to errors during the game and print the corresponding error messages and/or return the correct
return values. You can find all error messages that should be printed and all return values in the table below.

Only one error should be printed at once. If multiple errors occur at the same time,
only the highest message in the below table should be printed.

`<CONFIG_FILE>` should be replaced with the command line parameter that was entered by the user
calling the program in the error messages.

| Return Value | Error Message                                         | Meaning                                                                                                                    | Program sequence / Command |
|--------------|-------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------|----------------------------|
| 0            | -                                                     | The program terminated successfully                                                                                        | -                          |
| 1            | `Usage: ./a4-csf <config file>\n`                     | The wrong number of command line parameters <br/>was entered                                                               | Starting the Game          |
| 2            | `Error: Cannot open file: <CONFIG_FILE>\n`            | The config file could not be opened for reading                                                                            | Starting the Game          |
| 3            | `Error: Invalid file: <CONFIG_FILE>\n`                | The config file contains the wrong *magic number*                                                                          | Starting the Game          |
| 4            | `Error: Out of memory\n`                              | The program was not able to allocate new memory                                                                            | -                          |
|              | `Warning: Results not written to file!\n`             | The file is not writable                                                                                                   | Finishing the Game         |
|              | `Please enter a valid command!\n`                     | The command does not exist                                                                                                 | All Commands               |
|              | `Please enter the correct number of parameters!\n`    | The command was called with too many or too few parameters                                                                 | All Commands               |
|              | `Please enter a command you can use at the moment!\n` | The command can't be used (play/draw: opponent played last card \| challenge: opponent drew a card or first turn of round) | All Commands               |
|              | `Please enter the cards in the correct format!\n`     | `<HAND_CARD>` or `<PLAYED_AS_CARD>` was not given in the correct format                                                    | play                       |
|              | `Please enter a card in your hand cards!\n`           | The `<HAND_CARD>` parameter doesn't match a card in the player's hand cards                                                | play                       |
|              | `Please enter a valid VALUE!\n`                       | The `<PLAYED_AS_CARD>` value did not follow the game rules                                                                 | play                       |
|              | `Please enter a valid SPICE!\n`                       | The `<PLAYED_AS_CARD>` spice did not follow the game rules                                                                 | play                       |
|              | `Please choose SPICE or VALUE!\n`                     | The `<TYPE>` parameter was not `spice` or `value`                                                                          | challenge                  |


## Examples

The following example output was created using [config_example.txt](configs/config_example.txt).

<details>
<summary><h4>Example Output</h4></summary>

```
Welcome to Entertaining Spice Pretending!

-------------------
ROUND START
-------------------

Player 1:
    latest played card:
    cards played this round: 0
    hand cards: 4_p 10_p 3_w 9_w 9_w 10_w
P1 > play 4_p 4_p
Please enter a valid VALUE!
P1 > play 3_w 3_w

Player 2:
    latest played card: 3_w
    cards played this round: 1
    hand cards: 5_c 5_c 7_c 7_p 6_w 10_w
P2 > plaY 6_w 4_w        

Player 1:
    latest played card: 4_w
    cards played this round: 2
    hand cards: 4_p 10_p 9_w 9_w 10_w
P1 > play 10_p 10_w

Player 2:
    latest played card: 10_w
    cards played this round: 3
    hand cards: 5_c 5_c 7_c 7_p 10_w
P2 > play 10_w 11_w
Please enter the cards in the correct format!
P2 > play 10_w 4_w
Please enter a valid VALUE!
P2 > play 10_w 3_w

Player 1:
    latest played card: 3_w
    cards played this round: 4
    hand cards: 4_p 9_w 9_w 10_w
P1 > challenge value
Challenge successful: 3_w's value does not match the real card 10_w.
Player 1 gets 4 points.

-------------------
ROUND START
-------------------

Player 2:
    latest played card:
    cards played this round: 0
    hand cards: 5_c 5_c 7_c 1_p 7_p 4_w
P2 > play 5_p 1_p
Please enter a card in your hand cards!
P2 > play 1_p 2_p

Player 1:
    latest played card: 2_p
    cards played this round: 1
    hand cards: 4_p 9_w 9_w 10_w
P1 > play 4_p 3_p

Player 2:
    latest played card: 3_p
    cards played this round: 2
    hand cards: 5_c 5_c 7_c 7_p 4_w
P2 > challenge spice
Challenge failed: 3_p's spice matches the real card 4_p.
Player 1 gets 2 points.

-------------------
ROUND START
-------------------

Player 2:
    latest played card:
    cards played this round: 0
    hand cards: 3_c 5_c 5_c 7_c 7_p 1_w 4_w
P2 > draw

Player 1:
    latest played card:
    cards played this round: 0
    hand cards: 9_w 9_w 10_w
P1 > draw

Player 2:
    latest played card:
    cards played this round: 0
    hand cards: 3_c 5_c 5_c 7_c 7_p 1_w 2_w 4_w
P2 > draw

Player 1: 6 points
Player 2: 0 points

Congratulations! Player 1 wins the game!
```

</details>

After executing the program the config file should look like this:

<details>
<summary><h4>Example File after Documenting the Results</h4></summary>

```
ESP
3_w
7_p
9_w
5_c
4_p
7_c
9_w
6_w
10_w
5_c
10_p
10_w
1_p
4_w
3_c
1_w
2_w
8_p
10_c

Player 1: 6 points
Player 2: 0 points

Congratulations! Player 1 wins the game!
```

</details>
