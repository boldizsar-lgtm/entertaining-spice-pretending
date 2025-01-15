//-----------------------------------------------------------------------------
// Entertaining Spice Pretending
// check if the correct config file has been entered as the 2nd parameter
// when calling this program. Then greet the user and distribute 6 cards
// per player, and the rest into a draw pile.
// the two players take turn entering commands to interact with their hand
// and the draw pile, potentially lying about the card they play
// a round lasts until a player challenges another player's last played card
// the game lasts until the draw pile is empty or the user inputs "quit".
// Lastly print the scores and the winners, and if possible copy that output
// into the config file.
// 
// Tutor: Michael Neuhold
//
// Author: 12417350
//-----------------------------------------------------------------------------
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAGIC_NUM_SIZE 4
#define HAND_SIZE 6
#define INPUT_LENGTH 10

typedef enum
{
  COMMAND_LINE_PARAMETERS_ERROR = 1,
  CONFIG_FILE_ERROR = 2,
  MAGIC_NUM_ERROR = 3,
  MEMORY_ERROR = 4
} Error;

typedef enum
{
  QUIT = 5,
  PLAY = 6,
  CHALLENGE = 7,
  DRAW = 8
} Command;

typedef struct _Card_
{
  int value_;
  char spice_;
  struct _Card_ *next_;
} Card;

typedef struct _Player_
{
  int number_;
  int score_;
  Card *hand_;
} Player;

typedef struct _GameStatistics_
{
  Card real_played_;
  Card played_as_;
  int played_cards_count_;
  bool is_challengeable_;
  Player *challenge_winner_;

} GameStatistics;

//-----------------------------------------------------------------------------
/// check if the correct number of command line parameters were entered
///
/// @param argc the number of command line parameters entered when calling
///        his program
///
/// @return true if there are not exactly two command line parameters
/// @return flase if there are two command line parameters
/// 
bool commandLineParametersError(int argc)
{
  if (argc != 2)
  {
    printf("Usage: ./a4-csf <config file>\n");
    return true;
  }
  return false;
}

//-----------------------------------------------------------------------------
/// check if the file that was entered through command line parameters
/// can be opened
///
/// @param file the file which dictates the cards used by the program
/// @param filename the second command line paramenter
///
/// @return true if the file could not be opened
/// @return flase if the file could be opened
/// 
bool configFileError(FILE *file, char *filename)
{
  if (file == NULL)
  {
    printf("Error: Cannot open file: %s\n", filename);
    return true;
  }
  return false;
}

//-----------------------------------------------------------------------------
/// check if the file that was entered through command line parameters
/// has the correct magic number at the beginning of the file
/// which is supposed to be "ESP\n"
///
/// @param file the file which dictates the cards used by the program
/// @param filename the second command line paramenter
///
/// @return true if the magic number is wrong or not present
/// @return flase if the magic number is correct
/// 
bool magicNumberError(FILE *file, char *filename)
{
  char magic_number[MAGIC_NUM_SIZE + 1];
  int i = 0;
  int ch;
  while ((ch = fgetc(file)) != EOF && i < MAGIC_NUM_SIZE)
  {
    magic_number[i] = ch;
    i++;
  }
  magic_number[i] = '\0';
  if (strcmp(magic_number, "ESP\n") != 0)
  {
    printf("Error: Invalid file: %s\n", filename);
    return true;
  }
  // move one byte back to not skp the first number
  fseek(file, -1, SEEK_CUR);
  return false;
}

//-----------------------------------------------------------------------------
/// check if the allocation of memory failed
///
/// @param ptr a pointer to allocated memory
///
/// @return true if memory allocation failed
/// @return flase if no error ocurred
/// 
bool AllocationError(void *ptr)
{
  if (ptr == NULL)
    {
      printf("Error: Out of memory\n");
      return true;
    }
  return false;
}

//-----------------------------------------------------------------------------
/// Add a card to a player's hand or the draw pile (a linked list)
///
/// @param head the head of a linked list to which a card should be added
/// @param value the value of the card to be added ranging from 1-10
/// @param spice the spice of the card to be added "c", "p", or "w"
///
/// @return true if the card was successfully added
/// @return flase if a memory allocation error occured
/// 
bool addCard (Card **head, int value, char spice)
{
  Card *new_card = malloc(sizeof(Card));
  if (AllocationError(new_card))
  {
    return false;
  }
  new_card->value_ = value;
  new_card->spice_ = spice;
  new_card->next_ = NULL;

  if (*head == NULL)
  {
    *head = new_card;
    return true;
  }
  
  Card *current = *head;
  while(current->next_ != NULL)
    current = current->next_;

  current->next_ = new_card;
  
  return true;
}

//-----------------------------------------------------------------------------
/// distribute six cards to two players, starting with player one and
/// alternating between which player gets the next card, then add the
/// remainings cards to a draw pile
///
/// @param file the file which dictates the cards used by the program
/// @param player_one_hand a linked list made up of cards that this function
///        fills
/// @param player_two_hand a linked list made up of cards that this functions
///        fills
/// @param draw_pile a linked list made up of cards that this functions fills
///
/// @return true if the cards were successfully distributed
/// @return false if a memory allocation error occured
///
bool distributeCards(FILE *file, Card **player_one_hand,
                     Card **player_two_hand, Card **draw_pile)
{
  int ch;
  char value[3];
  int value_count = 0;
  char spice;
  int card_count = 0;

  while ((ch = fgetc(file)) != EOF)
  {
    if (ch >= '0' && ch <= '9')
    {
      value[value_count] = ch;
      value_count++;
    }
    if (ch == 'c' || ch == 'w' || ch == 'p')
      spice = ch;
    if (ch == '\n')
    {
      value[value_count] = '\0';

      if (card_count < HAND_SIZE * 2)
      {
        if (card_count % 2 == 0)
        {
          if(!addCard(player_one_hand, atoi(value), spice))
            return false;
        }
        if (card_count % 2 == 1)
        {
          if(!addCard(player_two_hand, atoi(value), spice))
            return false;
        }
        card_count++;
      }
      else
      {
        if(!addCard(draw_pile, atoi(value), spice))
          return false;
      }

      memset(value, 0, sizeof(value));
      value_count = 0;
    }
  } 
  return true;
}

//-----------------------------------------------------------------------------
///
void splitList(Card *source, Card **frontRef, Card **backRef)
{
  Card *slow = source;
  Card *fast = source->next_;

  while (fast)
  {
    fast = fast->next_;
    if (fast)
    {
      slow = slow->next_;
      fast = fast->next_;
    }
  }

  *frontRef = source;
  *backRef = slow->next_;
  slow->next_ = NULL;
}

//-----------------------------------------------------------------------------
///
int compareCards(const Card *a, const Card *b)
{
  if (a->spice_ != b->spice_)
  {
    if (a->spice_ == 'c') return -1;
    if (b->spice_ == 'c') return 1;
    if (a->spice_ == 'p') return -1;
    if (b->spice_ == 'p') return 1;
  }
  return a->value_ - b->value_;
}

//-----------------------------------------------------------------------------
///
Card *mergeSortedLists(Card *left, Card *right)
{
  if (!left) return right;
  if (!right) return left;

  if (compareCards(left, right) <= 0)
  {
    left->next_ = mergeSortedLists(left->next_, right);
    return left;
  }
  else
  {
    right->next_ = mergeSortedLists(left, right->next_);
    return right;
  }
}

//-----------------------------------------------------------------------------
///
Card *sortCards(Card *head)
{
  if (!head || !head->next_)
    return head;

  Card *left;
  Card *right;

  splitList(head, &left, &right);

  left = sortCards(left);
  right = sortCards(right);

  return mergeSortedLists(left, right);
}

//-----------------------------------------------------------------------------
/// set the GameStatistics struct back to its original state
/// the cards are empty, no cards have been played, challenge is not possible
///
/// @param game a struct made up of the last played card (real and bluff),
///        the number of played cards, and more
///
void resetGameStatistics(GameStatistics *game)
{
  game->real_played_.value_ = 0;
  game->real_played_.spice_ = '\0';
  game->played_as_.value_ = 0;
  game->played_as_.spice_ = '\0';
  game->played_cards_count_ = 0;
  game->is_challengeable_ = false;
}

//-----------------------------------------------------------------------------
/// print an entire linked list of cards
///
/// @param head the head of a linked list of cards
///
void printCards(Card *head)
{
  Card *current = head;
  while (current != NULL)
  {
    printf(" %d_%c", current->value_, current->spice_);
    current = current->next_;
  }
  printf("\n");
}

//-----------------------------------------------------------------------------
/// print the current player, the last card played, if any, and if it was that
/// player's last hand card, print the number of cards played up to this point
/// this round, and the current player's hand cards
///
/// @param player the current player
/// @param opponent the opposing player
/// @param game a struct made up of the last played card (real and bluff),
///        the number of played cards, and more
///
void printTurn(Player player, Player opponent, GameStatistics game)
{
  printf("\nPlayer %d:\n", player.number_);
  if (game.played_as_.value_ == 0)
     printf("    latest played card:");
  if (game.played_as_.value_ != 0)
    printf("    latest played card: %d_%c",
           game.played_as_.value_, game.played_as_.spice_);
  if (opponent.hand_ == NULL)
    printf(" LAST CARD");
  printf("\n    cards played this round: %d\n", game.played_cards_count_);
  printf("    hand cards:");
  printCards(player.hand_);
}

//-----------------------------------------------------------------------------
/// free the 2 dimensional array allocated for the user input
///
/// @param input a 2D array filled with the input
/// @param word_count the number of rows of the input, each word is saved in a
///        seperate row
/// 
void freeInput(char **input, int word_count)
{
  for (int i = 0; i < word_count; i++)
    free(input[i]);
  free(input);
}

//-----------------------------------------------------------------------------
/// free the 2 dimensional array allocated for the user input
///
/// @param input a 2D array filled with the input
/// @param word_count the number of rows of the input, each word is saved in a
///        seperate row
/// @param max_words the current maximum number of words the 1st dimension
///        of the array can store
/// @param max_chars the current maximum number of characters the 2nd
///        dimension of the array can store
///
/// @return true if the memory allocation was successful
/// @return false if a memory allocation error occured
/// 
bool allocateWordMemory(char ***input, int word_count, int *max_words,
                         int max_chars)
{
  if (word_count == *max_words)
  {
    *max_words *= 2;
    char **temp = realloc (*input, *max_words * sizeof(char*));
    if (AllocationError(temp))
    {
      freeInput(*input, word_count);
      return false;
    }
    *input = temp;
    temp = NULL;
  }
  
  (*input)[word_count] = malloc(max_chars * sizeof(char));
  if (AllocationError((*input)[word_count]))
  {
    freeInput(*input, word_count);
    return false;
  }
  
  return true;
}

//-----------------------------------------------------------------------------
/// free the 2 dimensional array allocated for the user input
///
/// @param input a 2D array filled with the input
/// @param word_count the number of rows of the input, each word is saved in a
///        seperate row
/// @param char_count the number of collumns of the input, each character is
///        saved in a seperate collumn
/// @param max_chars the current maximum number of characters the 2nd
///        dimension of the array can store
///
/// @return true if the memory allocation was successful
/// @return false if a memory allocation error occured
/// 
bool reallocCharMemory(char ***input, int word_count, int char_count,
                       int *max_chars)
{
  if (char_count == *max_chars)
  {
    *max_chars *= 2;
    char *temp = realloc ((*input)[word_count], *max_chars * sizeof(char));
    if (AllocationError(temp))
    {
      freeInput(*input, word_count);
      return false;
    }
    (*input)[word_count] = temp;
    temp = NULL;
  }

  return true;
}

//-----------------------------------------------------------------------------
/// read and save the user input into a 2 dimensional array, with each word
/// being saved in a seperate row
///
/// @param input a 2D array filled with the input
/// @param word_count the number of rows of the input, each word is saved in a
///        seperate row
/// 
/// @return true if the input was saved correctly
/// @return false if a memory allocation error occured
///
bool readUserInput(char ***input, int *word_count)
{
  *word_count = 0;
  int max_words = INPUT_LENGTH;
  int char_count = 0;
  int max_chars = INPUT_LENGTH;
  char ch;

  *input = malloc(max_words * sizeof(char*));
  if (AllocationError(*input))
    return false;

  (*input)[*word_count] = malloc(max_chars * sizeof(char));
  if (AllocationError((*input)[*word_count]))
  {
    freeInput(*input, *word_count);
    return false;
  }
    
  while (true)
  {
    ch = getchar();
    if ((ch == '\n' || ch == EOF) && char_count > 0)
    {
      (*input)[*word_count][char_count] = '\0';
      (*word_count)++;
      break;
    }
    if ((ch == '\n' || ch == EOF) && char_count == 0)
    {
      if (*word_count > 0)
        free((*input)[*word_count]);
      if (*word_count == 0)
      {
        (*input)[*word_count][char_count] = '\0';
        (*word_count)++;
      }
      break;
    }
    if (ch == ' ' && char_count > 0)
    {
      (*input)[*word_count][char_count] = '\0';
      (*word_count)++;
      char_count = 0;
      max_chars = INPUT_LENGTH;
      
      if(!allocateWordMemory(input, *word_count, &max_words, max_chars))
        return false;
    }

    if (ch != ' ')
    {
      (*input)[*word_count][char_count] = tolower(ch);
      char_count++;
    
      if(!reallocCharMemory(input, *word_count, char_count, &max_chars))
        return false;
    }
  }
  
  return true;
}

//-----------------------------------------------------------------------------
/// remove a card from a player's hand
///
/// @param head the head of a linked list of cards
/// @param target the card that should be removed
///
void removeHandCard(Card **head, Card target)
{
  Card *current = *head;
  Card *previous = NULL;

  while (current != NULL)
  {
    if (current->value_ == target.value_ && current->spice_ == target.spice_)
    {
      if (previous == NULL)
        *head = current->next_;
      else
        previous->next_ = current->next_;

      free(current);
      return;
    }
    previous = current;
    current = current->next_;
  }
}

//-----------------------------------------------------------------------------
/// remove the top card from the draw pile
///
/// @param head the head of a linked list of cards
///
void removeDrawPileCard(Card **head)
{
  Card *current = *head;
  *head = current->next_;
  free(current);
}

//-----------------------------------------------------------------------------
/// check if the user entered 'quit'
///
/// @param string a string of characters
///
/// @return true if quit was entered
/// @return false if quit was not entered
///
bool isQuit(char *string)
{
  return strcmp(string, "quit") == 0;
}

//-----------------------------------------------------------------------------
/// check if the user entered 'play'
///
/// @param string a string of characters
///
/// @return true if play was entered
/// @return false if play was not entered
///
bool isPlay(char *string)
{
  return strcmp(string, "play") == 0;
}

//-----------------------------------------------------------------------------
/// check if the user entered 'challenge'
///
/// @param string a string of characters
///
/// @return true if challenge was entered
/// @return false if challenge was not entered
///
bool isChallenge(char *string)
{
  return strcmp(string, "challenge") == 0;
}

//-----------------------------------------------------------------------------
/// check if the user entered 'draw'
///
/// @param string a string of characters
///
/// @return true if draw was entered
/// @return false if draw was not entered
///
bool isDraw(char *string)
{
  return strcmp(string, "draw") == 0;
}

//-----------------------------------------------------------------------------
/// check if the user entered 'spice'
///
/// @param string a string of characters
///
/// @return true if spice was entered
/// @return false if spice was not entered
///
bool isSpice(char *string)
{
  return strcmp(string, "spice") == 0;
}

//-----------------------------------------------------------------------------
/// check if the user entered 'value'
///
/// @param string a string of characters
///
/// @return true if value was entered
/// @return false if value was not entered
///
bool isValue(char *string)
{
  return strcmp(string, "value") == 0;
}

//-----------------------------------------------------------------------------
/// convert the user's input into the value of a card.
///
/// @param string one of the parameters the user input when entering the
///        play command
///
/// @return int the value of the card that was input
///
int getCardValue(char *string)
{
  if (strlen(string) == 4)
  {
    char temp[3];
    strncpy(temp, string, 2);
    temp[2] = '\0';
    return atoi(temp);
  }
  return atoi(&string[0]);
}

//-----------------------------------------------------------------------------
/// convert the user's input into the spice of a card.
///
/// @param string one of the parameters the user input when entering the
///        play command
///
/// @return char the spice of the card that was input
///
char getCardSpice(char *string)
{
  if (strlen(string) == 4)
  {
    return string[3];
  }
  return string[2];
}

//-----------------------------------------------------------------------------
/// check if the user's second input parameter corresponds to a hand card
/// if they used the play command
///
/// @param head the head of the linked list of cards that makes up the
///        player's hand
/// @param input a 2D array filled with the input
///
/// @return true if the user's input corresponds to a hand card
/// @return false if the user's input does not match a hand card
///
bool nonHandCardPlayInput(Card *head, char **input)
{
  Card *current = head;
  Card target = {getCardValue(input[1]), getCardSpice(input[1]), NULL};

  while (current != NULL)
  {
    if (current->value_ == target.value_ && current->spice_ == target.spice_)
      return false;

    current = current->next_;
  }
  return true;
}

//-----------------------------------------------------------------------------
/// check if the user's card inputs are in the right format if they used the
/// play command
///
/// @param input a 2D array filled with the input
///
/// @return true if the user's input has the right format
/// @return false if the user's input has the wrong format
///
bool wrongPlayFormat(char **input)
{
  int real_played_value = getCardValue(input[1]);
  char real_played_spice = getCardSpice(input[1]);
  int played_as_value = getCardValue(input[2]);
  char played_as_spice = getCardSpice(input[2]);

  if (real_played_value < 1 || real_played_value > 10)
    return true;
  if (real_played_value == 10 && input[1][2] != '_')
      return true;
  if (real_played_value < 10 && input[1][1] != '_')
      return true;
  if (real_played_spice != 'c' && real_played_spice != 'w'
      && real_played_spice != 'p')
    return true;
  
  if (played_as_value < 1 || played_as_value > 10)
    return true;
  if (played_as_value == 10 && input[2][2] != '_')
    return true;
  if (played_as_value < 10 && input[2][1] != '_')
      return true;
  if (played_as_spice != 'c' && played_as_spice != 'w'
      && played_as_spice != 'p')
    return true;

  return false;
}

//-----------------------------------------------------------------------------
/// check if the user's input was a valid command with the right format and
/// parameters
///
/// @param input a 2D array filled with the input
/// @param word_count the number of words the user input
/// @param player the current player
/// @param opponent the current opposing player
/// @param game a struct made up of the last played card (real and bluff),
///        the number of played cards, and more
///
/// @return QUIT if the input was a valid quit command
/// @return PLAY if the input was a valid play command
/// @return CHALLENGE if the input was a valid challenge command
/// @return DRAW if the input was a valid draw command
/// @return 0 if the input didnt correspond to any valid command
///
int isCorrectCommand(char **input, int word_count, Player player,
                     Player opponent, GameStatistics game)
{
  Card current_played_as;
  if (word_count == 3)
  {
    current_played_as.value_ = getCardValue(input[2]);
    current_played_as.spice_ = getCardSpice(input[2]);
  }
  
  if (isQuit(input[0]))
  {
    if (word_count != 1)
      printf("Please enter the correct number of parameters!\n");
    else
      return QUIT;
  }

  else if (isPlay(input[0]))
  {
    if (word_count != 3)
      printf("Please enter the correct number of parameters!\n");
    else if (opponent.hand_ == NULL)
      printf("Please enter a command you can use at the moment!\n");
    else if (wrongPlayFormat(input))
     printf("Please enter the cards in the correct format!\n");
    else if (nonHandCardPlayInput(player.hand_, input))
      printf("Please enter a card in your hand cards!\n");
    else if ((game.played_as_.value_ == 0 || game.played_as_.value_ == 10)
             && current_played_as.value_  > 3)
      printf("Please enter a valid VALUE!\n");
    else if ((game.played_as_.value_ > 0 && game.played_as_.value_ < 10)
             && current_played_as.value_  < game.played_as_.value_)
      printf("Please enter a valid VALUE!\n");
    else if (current_played_as.spice_ != game.played_as_.spice_
             && game.played_cards_count_ > 0)
      printf("Please enter a valid SPICE!\n");
    else
      return PLAY;
  }

  else if (isChallenge(input[0]))
  {
    if (word_count != 2)
      printf("Please enter the correct number of parameters!\n");
    else if (game.is_challengeable_ == false)
      printf("Please enter a command you can use at the moment!\n");
    else if (!isValue(input[1]) && !isSpice(input[1]))
      printf("Please choose SPICE or VALUE!\n");
    else
      return CHALLENGE;
  }

  else if (isDraw(input[0]))
  {
  if (word_count != 1)
    printf("Please enter the correct number of parameters!\n");
  else if (opponent.hand_ == NULL)
    printf("Please enter a command you can use at the moment!\n");
  else
    return DRAW;
  }

  else
    printf("Please enter a valid command!\n");

  return 0;
}

//-----------------------------------------------------------------------------
/// a player plays a card from their hand and tells the truth or lies
/// about the value and spice of their card. the first parameter of the user's
/// input is the card that is removed from the player's hand.
/// the second parameter of the input is what the card is played as.
///
/// @param input a 2D array filled with the input
/// @param player the current player
/// @param game a struct made up of the last played card (real and bluff),
///        the number of played cards, and more
///
void playCard(char **input, Player *player, GameStatistics *game)
{
  game->real_played_.value_ = getCardValue(input[1]);
  game->real_played_.spice_ = getCardSpice(input[1]);
  
  game->played_as_.value_ = getCardValue(input[2]);
  game->played_as_.spice_ = getCardSpice(input[2]);

  removeHandCard(&player->hand_, game->real_played_);

  (game->played_cards_count_)++;
}

//-----------------------------------------------------------------------------
/// a player draws a card from the draw pile, add that card to the player's
/// hand and remove it from the draw pile
///
/// @param player a struct with player's hands, number, and score
/// @param draw_pile a linked list of cards
///
/// @return if the draw pile is empty return before it is accessed
///
void drawCard(Player *player, Card **draw_pile)
{
  if (*draw_pile == NULL)
    return;

  addCard(&player->hand_, (*draw_pile)->value_, (*draw_pile)->spice_);
  removeDrawPileCard(draw_pile);
}

//-----------------------------------------------------------------------------
/// a player challenges the previously played card from the opponent, if the
/// opponent lied and the last played card does not match the one that got
/// removed from their hand the opponent loses. Otherwise the player loses.
///
/// @param input a 2D array filled with the input
/// @param player the current player
/// @param opponent the current opposing player
/// @param draw_pile a linked list of cards
/// @param game a struct made up of the last played card (real and bluff),
///        the number of played cards, and more
///
/// @return Player the player who won the challenge
///
Player* challengeCard(char **input, Player *player, Player *opponent,
                      Card **draw_pile, GameStatistics game)
{
  Player *winner;
  Player *loser;
  if (isValue(input[1]))
  {
    if (game.real_played_.value_ == game.played_as_.value_)
    {
      winner = opponent;
      loser = player;
      printf("Challenge failed: %d_%c's value matches the real card %d_%c.\n",
             game.played_as_.value_, game.played_as_.spice_,
             game.real_played_.value_, game.real_played_.spice_);
    }
    else
    {
      winner = player;
      loser = opponent;
      printf("Challenge successful: %d_%c's value "
             "does not match the real card %d_%c.\n",
             game.played_as_.value_, game.played_as_.spice_,
             game.real_played_.value_, game.real_played_.spice_);
    }
  }

  if (isSpice(input[1]))
  {
    if (game.real_played_.spice_ == game.played_as_.spice_)
    {
      winner = opponent;
      loser = player;
      printf("Challenge failed: %d_%c's spice matches the real card %d_%c.\n",
             game.played_as_.value_, game.played_as_.spice_,
             game.real_played_.value_, game.real_played_.spice_);
    }
    else
    {
      winner = player;
      loser = opponent;
      printf("Challenge successful: %d_%c's spice "
             "does not match the real card %d_%c.\n",
             game.played_as_.value_, game.played_as_.spice_,
             game.real_played_.value_, game.real_played_.spice_);
    }
  }

  winner->score_ += game.played_cards_count_;
  printf("Player %d gets %d points.\n", winner->number_, game.played_cards_count_);

  drawCard(loser, draw_pile);
  drawCard(loser, draw_pile);

  if (winner->hand_ == NULL)
  {
    winner->score_ += 10;
    printf("Player %d gets 10 bonus points (last card).\n", winner->number_);
    for (int i = 0; i < HAND_SIZE; i++)
      drawCard(winner, draw_pile);
  }

  return winner;
}

//-----------------------------------------------------------------------------
/// switch turns between the current player and the opposing Player
///
/// @param player the current player
/// @param opponent the current opposing player
///
void switchPlayerTurn(Player **player, Player **opponent)
{
  Player *temp = *player;
  *player = *opponent;
  *opponent = temp;
}

//-----------------------------------------------------------------------------
/// game rounds and turns are played until the user enters quit or
/// the last draw_pile card is drawn.
///
/// @param player_one a struct with player one's hands, number, and score
/// @param player_two a struct with player two's hands, number, and score
/// @param draw_pile a linked list made up of cards
/// 
/// @return MEMORY_ERROR if a memory allocation error occured and the
///         program needs to terminate
/// @return QUIT if quit was entered and the program should terminate
/// @return 0 if the game has come to an end because the last card was drawn
///
int gameRounds(Player *player_one, Player *player_two, Card **draw_pile)
{
  Player *player = player_one;
  Player *opponent = player_two;
  GameStatistics game;

  // Round Loop
  while (*draw_pile != NULL)
  {
    printf("\n-------------------\nROUND START\n-------------------\n");
    resetGameStatistics(&game);
    int command = 0;
    
    // Turn Loop
    while (*draw_pile != NULL)
    {
      player->hand_ = sortCards(player->hand_);
      printTurn(*player, *opponent, game);
      
      char **input = NULL;
      int word_count = 0;

      // Wrong Input Loop
      while (command == 0)
      {
        printf("P%d > ", player->number_);

        if (!readUserInput(&input, &word_count))
        {
          freeInput(input, word_count);
          return MEMORY_ERROR;
        }
      
        command = isCorrectCommand(input, word_count, *player, *opponent,
                                   game);
        if (command == QUIT)
        {
          freeInput(input, word_count);
          return QUIT;
        }
        if (command == PLAY)
        {
          playCard(input, player, &game);
          switchPlayerTurn(&player, &opponent);
          game.is_challengeable_ = true;
        }
        if (command == CHALLENGE)
        {
          game.challenge_winner_ = challengeCard(input, player, opponent,
                                                 draw_pile, game);
          if (game.challenge_winner_== opponent)
            break;
          if (game.challenge_winner_== player)
          {
            switchPlayerTurn(&player, &opponent);
            break;
          }
        }
        if (command == DRAW)
        {
          drawCard(player, draw_pile);
          switchPlayerTurn(&player, &opponent);
          game.is_challengeable_ = false;
        }
        freeInput(input, word_count);
      }
      if (command == CHALLENGE)
      {
        freeInput(input, word_count);
        break;
      }
      command = 0;
    }
  }
  return 0;
}

//-----------------------------------------------------------------------------
/// print the results of the game, and if possible write it into the config
/// file
///
/// @param player_one a struct with player one's hands, number, and score
/// @param player_two a struct with player two's hands, number, and score
/// @param filename the second command line paramenter
///
void gameResults(Player player_one, Player player_two, char *filename)
{
  Player winner = player_one;
  Player loser = player_two;
  if (player_one.score_ < player_two.score_) 
  {
    winner = player_two;
    loser = player_one;
  }
  printf("\nPlayer %d: %d points\n", winner.number_, winner.score_);
  printf("Player %d: %d points\n\n", loser.number_, loser.score_);
  printf("Congratulations! Player %d wins the game!\n", winner.number_);
  if (player_one.score_ == player_two.score_)
  {
    printf("Congratulations! Player %d wins the game!\n", loser.number_);
  }
    
  FILE *file = fopen(filename, "r+");
  if (file == NULL)
  {
    printf("Warning: Results not written to file!\n");
  }
  else
  {
    fseek(file, 0 , SEEK_END);
    fprintf(file, "\nPlayer %d: %d points\n", winner.number_, winner.score_);
    fprintf(file, "Player %d: %d points\n\n", loser.number_, loser.score_);
    fprintf(file, "Congratulations! Player %d wins the game!\n",
            winner.number_);
    if (player_one.score_ == player_two.score_)
    {
      fprintf(file, "Congratulations! Player %d wins the game!\n",
            loser.number_);
    }
    fclose(file);
  }
}

//-----------------------------------------------------------------------------
/// free the allocated memory for a linked list of cards
///
/// @param head the head of a linked list of cards
/// 
void freeCards(Card *head)
{
  Card *current = head;
  while (current != NULL)
  {
    Card *next = current->next_;
    free(current);
    current = next;
  }
  head = NULL;
}

//-----------------------------------------------------------------------------
/// free the allocated memory for a player one's hand, player two's hand and
/// the draw pile
///
/// @param player_one a struct with player one's hands, number, and score
/// @param player_two a struct with player two's hands, number, and score
/// @param draw_pile a linked list of cards
///
void freeAllCards(Player player_one, Player player_two, Card *draw_pile)
{
  freeCards(player_one.hand_);
  freeCards(player_two.hand_);
  freeCards(draw_pile);
}

//-----------------------------------------------------------------------------
/// The actual main function of the program!
/// 
/// @param argc number of command line parameters
/// @param argv string of command line parameters
///
/// @return COMMAND_LINE_PARAMETERS_ERROR if the wrong number of command
///         line parameters were entered
/// @return CONFIG_FILE_ERROR if the config file can't be opened for
///         reading
/// @return MAGIC_NUM_ERROR if the config file does not have the right
///         magic number at the start
/// @return MEMORY_ERROR if a memory allocation error occurs
/// @return 0 if quit was entered as a player's input or if the program 
///         finishes as intended
/// 
int main(int argc, char *argv[])
{
  if (commandLineParametersError(argc))
    return COMMAND_LINE_PARAMETERS_ERROR;
  
  FILE *file = fopen(argv[1], "r");
  if (configFileError(file, argv[1]))
    return CONFIG_FILE_ERROR;
  
  if (magicNumberError(file, argv[1]))
  {
    fclose(file);
    return MAGIC_NUM_ERROR;
  }

  printf("Welcome to Entertaining Spice Pretending!\n");

  Player player_one = {1, 0, NULL};
  Player player_two = {2, 0, NULL};
  Card *draw_pile = NULL;

  if (!distributeCards(file, &player_one.hand_, &player_two.hand_, &draw_pile))
  {
    freeAllCards(player_one, player_two, draw_pile);
    fclose(file);
    return MEMORY_ERROR;
  }

  int game_rounds = gameRounds(&player_one, &player_two, &draw_pile);
  if(game_rounds != 0)
  {
    freeAllCards(player_one, player_two, draw_pile);
    fclose(file);
    if (game_rounds == MEMORY_ERROR)
      return MEMORY_ERROR;
    if (game_rounds == QUIT)
      return 0;
  }

  fclose(file);
  gameResults(player_one, player_two, argv[1]);
  
  freeAllCards(player_one, player_two, draw_pile);
  return 0;
}
