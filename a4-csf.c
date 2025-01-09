//-----------------------------------------------------------------------------
// Entertaining Spice Pretending
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
  PLAY,
  CHALLENGE,
  DRAW
} Command;

typedef struct _Card_
{
  int value_;
  char spice_;
  struct _Card_* next_;
} Card;

typedef struct _Player_
{
  int number_;
  int score_;
  Card* hand_;
} Player;


//-----------------------------------------------------------------------------
/// check if the correct number of command line parameters were entered
///
/// @param argc the number of command line parameters entered when calling
///             this program
///
/// @return true if there are not two command line parameters
/// @return flase if there are exactly two command line parameters
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
/// @param file
/// @param filename the second command line paramenter
///
/// @return true
/// @return flase 
/// 
bool configFileError(FILE* file, char *filename)
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
///
/// @param file
/// @param filename the second command line paramenter
///
/// @return true
/// @return flase 
/// 
bool magicNumberError(FILE* file, char *filename)
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
  fseek(file, -1, SEEK_CUR);
  // move one byte back to not skp the first number
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
///
bool addCard (Card** head, int value, char spice)
{
  Card* new_card = malloc(sizeof(Card));
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
  
  Card* current = *head;
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
/// @param player_one.hand_ a linked list made up of cards that this function
///                        fills
/// @param player_two.hand_ a linked list made up of cards that this functions
///                        fills
/// @return false if a memorry error occurs
/// @return true if the function finishes without issue
bool distributeCards(FILE* file, Card** player_one_hand, Card** player_two_hand, Card** draw_pile)
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
void splitList(Card* source, Card** frontRef, Card** backRef) {
    Card* slow = source;
    Card* fast = source->next_;

    while (fast) {
        fast = fast->next_;
        if (fast) {
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
int compareCards(const Card* a, const Card* b)
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
Card* mergeSortedLists(Card* left, Card* right)
{
    if (!left) return right;
    if (!right) return left;

    if (compareCards(left, right) <= 0) {
        left->next_ = mergeSortedLists(left->next_, right);
        return left;
    } else {
        right->next_ = mergeSortedLists(left, right->next_);
        return right;
    }
}

//-----------------------------------------------------------------------------
///
Card* sortCards(Card* head) {
    if (!head || !head->next_) {
        return head;
    }

    Card* left;
    Card* right;

    splitList(head, &left, &right);

    left = sortCards(left);
    right = sortCards(right);

    return mergeSortedLists(left, right);
}

//-----------------------------------------------------------------------------
///
void printCards(Card* head)
{
  Card* current = head;
  while (current != NULL)
  {
    printf(" %d_%c", current->value_, current->spice_);
    current = current->next_;
  }
  printf("\n");
}

//-----------------------------------------------------------------------------
/// distribute six cards to two players, starting with player one and
/// alternating between which player gets the next card
///
/// @param player_one.hand_ a linked list made up of cards that this function
///                        fills
/// @param player_two.hand_ a linked list made up of cards that this functions
///                        fills
/// @return false if a memorry error occurs
/// @return true if the function finishes without issue
void printTurn(Player player, Player opponent, int played_cards_count, Card played_as)
{
  printf("\nPlayer %d:\n", player.number_);
  if (played_as.value_ == 0)
     printf("    latest played card:");
  if (played_as.value_ != 0)
    printf("    latest played card: %d_%c", played_as.value_, played_as.spice_);
  if (opponent.hand_ == NULL)
    printf(" LAST CARD");
  printf("\n    cards played this round: %d\n", played_cards_count);
  printf("    hand cards:");
  printCards(player.hand_);
}

//-----------------------------------------------------------------------------
/// distribute six cards to two players, starting with player one and
/// alternating between which player gets the next card
///
/// @param player_one.hand_ a linked list made up of cards that this function
///                        fills
/// @param player_two.hand_ a linked list made up of cards that this functions
///                        fills
/// 
void freeInput(char** input, int row_count)
{
  for (int i = 0; i < row_count; i++)
    free(input[i]);
  free(input);
}

//-----------------------------------------------------------------------------
/// distribute six cards to two players, starting with player one and
/// alternating between which player gets the next card
///
/// @param player_one.hand_ a linked list made up of cards that this function
///                        fills
/// @param player_two.hand_ a linked list made up of cards that this functions
///                        fills
/// 
bool readUserInput(char ***input, int* row_count)
{
  *row_count = 0;
  int rows = INPUT_LENGTH;
  int col_count = 0;
  int collumns = INPUT_LENGTH;
  char ch;

  *input = malloc(rows * sizeof(char*));
  if (AllocationError(*input))
    return false;

  (*input)[*row_count] = malloc(collumns * sizeof(char));
  if (AllocationError((*input)[*row_count]))
  {
    freeInput(*input, *row_count);
    return false;
  }
    
  while (true)
  {
    ch = getchar();
    if (ch == '\n' || ch == EOF)
    {
      if (col_count > 0)
      {
        (*input)[*row_count][col_count] = '\0';
        (*row_count)++;
      }
      break;
    }
    if (ch == ' ' && col_count > 0)
    {
      (*input)[*row_count][col_count] = '\0';
      (*row_count)++;
      col_count = 0;
      collumns = INPUT_LENGTH;

      if (*row_count == rows)
      {
        rows *= 2;
        char **temp = realloc (*input, rows * sizeof(char *));
        if (AllocationError(temp))
        {
          freeInput(*input, *row_count);
          return false;
        }
          
        *input = temp;
        temp = NULL;
      }

      (*input)[*row_count] = malloc(collumns * sizeof(char));
      if (AllocationError((*input)[*row_count]))
      {
        freeInput(*input, *row_count);
        return false;
      }
    }

    if (ch != ' ')
    {
      (*input)[*row_count][col_count] = tolower(ch);
      col_count++;
    
      if (col_count == collumns)
      {
        collumns *= 2;
        char *temp = realloc ((*input)[*row_count], collumns * sizeof(char));
        if (AllocationError(temp))
        {
          freeInput(*input, *row_count);
          return false;
        }
        
        (*input)[*row_count] = temp;
        temp = NULL;
      }
    }
  }
  
  return true;
}

//-----------------------------------------------------------------------------
///
void removeHandCard(Card** head, Card target)
{
  Card* current = *head;
  Card* previous = NULL;

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
///
void removeDrawPileCard(Card** head)
{
  Card* current = *head;
  *head = current->next_;
  free(current);
}

//-----------------------------------------------------------------------------
/// check if the user entered 'quit'
///
/// @param string a string of characters
///
/// @return true if quit was entered and the program should be terminated
/// @return false if the program should continue
///
bool isQuit(char *string)
{
  return strcmp(string, "quit") == 0;
}

//-----------------------------------------------------------------------------
/// check if the user entered 'quit'
///
/// @param string a string of characters
///
/// @return true if quit was entered and the program should be terminated
/// @return false if the program should continue
///
bool isPlay(char *string)
{
  return strcmp(string, "play") == 0;
}

//-----------------------------------------------------------------------------
/// check if the user entered 'quit'
///
/// @param string a string of characters
///
/// @return true if quit was entered and the program should be terminated
/// @return false if the program should continue
///
bool isChallenge(char *string)
{
  return strcmp(string, "challenge") == 0;
}

//-----------------------------------------------------------------------------
/// check if the user entered 'quit'
///
/// @param string a string of characters
///
/// @return true if quit was entered and the program should be terminated
/// @return false if the program should continue
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
/// @return true if quit was entered and the program should be terminated
/// @return false if the program should continue
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
/// @return true if quit was entered and the program should be terminated
/// @return false if the program should continue
///
bool isValue(char *string)
{
  return strcmp(string, "value") == 0;
}

//-----------------------------------------------------------------------------
///
void drawCard(Player* player, Card** draw_pile)
{
  if (*draw_pile == NULL)
    return;

  addCard(&player->hand_, (*draw_pile)->value_, (*draw_pile)->spice_);
  removeDrawPileCard(draw_pile);
}

//-----------------------------------------------------------------------------
///
Player* challengeCard(Player* player, Player* opponent, Card real_played, Card played_as, Card** draw_pile, char** input, int played_cards_count)
{
  Player* winner;
  Player* loser;
  if (isValue(input[1]))
  {
    if (real_played.value_ == played_as.value_)
    {
      winner = opponent;
      loser = player;
      printf("Challenge failed: %d_%c's value matches the real card %d_%c.\n",
             played_as.value_, played_as.spice_, real_played.value_, real_played.spice_);
    }
    else
    {
      winner = player;
      loser = opponent;
      printf("Challenge successful: %d_%c's value does not match the real card %d_%c.\n",
             played_as.value_, played_as.spice_, real_played.value_, real_played.spice_);
    }
  }

  if (isSpice(input[1]))
  {
    if (real_played.spice_ == played_as.spice_)
    {
      winner = opponent;
      loser = player;
      printf("Challenge failed: %d_%c's spice matches the real card %d_%c.\n",
             played_as.value_, played_as.spice_, real_played.value_, real_played.spice_);
    }
    else
    {
      winner = player;
      loser = opponent;
      printf("Challenge successful: %d_%c's spice does not match the real card %d_%c.\n",
             played_as.value_, played_as.spice_, real_played.value_, real_played.spice_);
    }
  }

  winner->score_ += played_cards_count;
  printf("Player %d gets %d points.\n", winner->number_, played_cards_count);

  drawCard(loser, draw_pile);
  drawCard(loser, draw_pile);

  if (winner->hand_ == NULL)
  {
    printf("Player %d gets 10 bonus points (last card).\n", winner->number_);
    for (int i = 0; i < HAND_SIZE; i++)
      drawCard(winner, draw_pile);
  }

  return winner;
}

//-----------------------------------------------------------------------------
///
int getCardValue(char* parameter)
{
  if (strlen(parameter) == 4)
  {
    char temp[3];
    strncpy(temp, parameter, 2);
    temp[2] = '\0';
    return atoi(temp);
  }
  return atoi(&parameter[0]);
}

//-----------------------------------------------------------------------------
///
char getCardSpice(char* parameter)
{
  if (strlen(parameter) == 4)
  {
    return parameter[3];
  }
  return parameter[2];
}

//-----------------------------------------------------------------------------
///
void playCard(char** input, Player* player, int *played_cards_count, Card* real_played, Card* played_as)
{
  real_played->value_ = getCardValue(input[1]);
  real_played->spice_ = getCardSpice(input[1]);
  
  played_as->value_ = getCardValue(input[2]);
  played_as->spice_ = getCardSpice(input[2]);

  removeHandCard(&player->hand_, *real_played);

  (*played_cards_count)++;
}

//-----------------------------------------------------------------------------
///
bool nonHandCardPlayInput(Card* head, char** input)
{
  Card* current = head;
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
///
bool wrongPlayFormat(char** input)
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
  if (real_played_spice != 'c' && real_played_spice != 'w' && real_played_spice != 'p')
    return true;
  
  if (played_as_value < 1 || played_as_value > 10)
    return true;
  if (played_as_value == 10 && input[2][2] != '_')
    return true;
  if (played_as_value < 10 && input[2][1] != '_')
      return true;
  if (played_as_spice != 'c' && played_as_spice != 'w' && played_as_spice != 'p')
    return true;

  return false;
}

//-----------------------------------------------------------------------------
///
int isCorrectCommand(char** input, int parameter_count, Player player, Player opponent, Card played_as, int played_cards_count, bool is_challengeable)
{
  int real_card_value = 0;
  char real_card_spice = '\0';
  if (parameter_count == 3)
  {
    real_card_value = getCardValue(input[2]);
    real_card_spice = getCardSpice(input[2]);
  }

  
  if (isQuit(input[0]))
  {
    if (parameter_count != 1)
      printf("Please enter the correct number of parameters!\n");
    else
      return QUIT;
  }

  else if (isPlay(input[0]))
  {
    if (parameter_count != 3)
    {
      printf("Please enter the correct number of parameters!\n");
      return 0;
    }
    else if (opponent.hand_ == NULL)
    {
      printf("Please enter a command you can use at the moment!\n");
      return 0;
    }
    else if (wrongPlayFormat(input))
    {
     printf("Please enter the cards in the correct format!\n");
     return 0;
    }
    else if (nonHandCardPlayInput(player.hand_, input))
    {
      printf("Please enter a card in your hand cards!\n");
      return 0;
    }
    else if (played_as.value_ == 0 || played_as.value_ == 10)
    {
      if (real_card_value > 3)
      {
        printf("Please enter a valid VALUE!\n");
        return 0;
      }
    }
    else if (played_as.value_ > 0 && played_as.value_ < 10)
    {
      if (real_card_value < played_as.value_)
      {
        printf("Please enter a valid VALUE!\n");
        return 0;
      }
    }
    else if (played_cards_count > 0)
    {
      if (real_card_spice != played_as.spice_)
      {
        printf("Please enter a valid SPICE!\n");
        return 0;
      }
    }
      return PLAY;
  }

  else if (isChallenge(input[0]))
  {
    if (parameter_count != 2)
      printf("Please enter the correct number of parameters!\n");
    else if (is_challengeable == false)
      printf("Please enter a command you can use at the moment!\n");
    else if (!isValue(input[1]) && !isSpice(input[1]))
      printf("Please choose SPICE or VALUE!\n");
    return CHALLENGE;
  }

  else if (isDraw(input[0]))
  {
  if (parameter_count != 1)
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
///
void switchPlayerTurn(Player** current_player, Player** opposing_player)
{
  Player* temp = *current_player;
  *current_player = *opposing_player;
  *opposing_player = temp;
}

//-----------------------------------------------------------------------------
/// distribute six cards to two players, starting with player one and
/// alternating between which player gets the next card
///
/// @param player_one.hand_ a linked list made up of cards that this function
///                        fills
/// @param player_two.hand_ a linked list made up of cards that this functions
///                        fills
/// 
int gameRounds (Player* player_one, Player* player_two, Card** draw_pile)
{
  Player* current_player = player_one;
  Player* opposing_player = player_two;

  while (*draw_pile != NULL)
  {
    printf("\n-------------------\nROUND START\n-------------------\n");
    int played_cards_count = 0;
    Card played_as = { .value_ = 0, .spice_ = '\0', .next_ = NULL };
    Card real_played = { .value_ = 0, .spice_ = '\0', .next_ = NULL };
    bool is_challengeable = false;
    int command = 0;
    
    while (*draw_pile != NULL)
    {
      current_player->hand_ = sortCards(current_player->hand_);
      printTurn(*current_player, *opposing_player, played_cards_count, played_as);
      
      char** input = NULL;
      int parameter_count = 0;

      while (command == 0)
      {
        printf("P%d > ", current_player->number_);

        if (!readUserInput(&input, &parameter_count))
        {
          freeInput(input, parameter_count);
          return MEMORY_ERROR;
        }
      
        command = isCorrectCommand(input, parameter_count, *current_player, *opposing_player, played_as, played_cards_count, is_challengeable);
        if (command == QUIT)
        {
          freeInput(input, parameter_count);
          return QUIT;
        }
        if (command == PLAY)
        {
          playCard(input, current_player, &played_cards_count, &real_played, &played_as);
          switchPlayerTurn(&current_player, &opposing_player);
          is_challengeable = true;
        }
        if (command == CHALLENGE)
        {
          Player* winner = challengeCard(current_player, opposing_player, real_played, played_as, draw_pile, input, played_cards_count);
          if (winner == opposing_player)
            break;
          if (winner == current_player)
          {
            switchPlayerTurn(&current_player, &opposing_player);
            break;
          }
        }
        if (command == DRAW)
        {
          drawCard(current_player, draw_pile);
          switchPlayerTurn(&current_player, &opposing_player);
          is_challengeable = false;
        }
        freeInput(input, parameter_count);
      }
      if (command == CHALLENGE)
      {
        freeInput(input, parameter_count);
        break;
      }
      command = 0;
    }
  }
  return 0;
}

//-----------------------------------------------------------------------------
///
void gameResults(Player player_one, Player player_two, char *filename)
{
  Player winner;
  Player loser;
  if (player_one.score_ > player_two.score_) 
  {
    winner = player_one;
    loser = player_two;
  }
  if (player_one.score_ < player_two.score_) 
  {
    winner = player_two;
    loser = player_one;
  }
  if (player_one.score_ == player_two.score_)
  {
    printf("\nPlayer %d: %d points\n", player_one.number_, player_one.score_);
    printf("Player %d: %d points\n\n", player_two.number_, player_two.score_);
    printf("Congratulations! Player %d wins the game!\n", player_one.number_);
    printf("Congratulations! Player %d wins the game!\n", player_two.number_);
  }
  else
  {
    printf("\nPlayer %d: %d points\n", winner.number_, winner.score_);
    printf("Player %d: %d points\n\n", loser.number_, loser.score_);
    printf("Congratulations! Player %d wins the game!\n", winner.number_);
  }
    
  FILE *file = fopen(filename, "r+");
  if (file == NULL)
  {
    perror("Warning: Results not written to file!\n");
  }
  else
  {
    fseek(file, 0 , SEEK_END);
    if (player_one.score_ == player_two.score_)
    {
      fprintf(file, "\nPlayer %d: %d points\n", player_one.number_, player_one.score_);
      fprintf(file, "Player %d: %d points\n\n", player_two.number_, player_two.score_);
      fprintf(file, "Congratulations! Player %d wins the game!\n", player_one.number_);
      fprintf(file, "Congratulations! Player %d wins the game!\n", player_two.number_);
    }
    else
    {
      fprintf(file, "\nPlayer %d: %d points\n", winner.number_, winner.score_);
      fprintf(file, "Player %d: %d points\n\n", loser.number_, loser.score_);
      fprintf(file, "Congratulations! Player %d wins the game!\n", winner.number_);
    }
    fclose(file);
  }
  
}


//-----------------------------------------------------------------------------
/// distribute six cards to two players, starting with player one and
/// alternating between which player gets the next card
///
/// @param player_one.hand_ a linked list made up of cards that this function
///                        fills
/// @param player_two.hand_ a linked list made up of cards that this functions
///                        fills
/// 
void freeCards(Card* hand)
{
  Card* current = hand;
  while (current != NULL)
  {
    Card* next = current->next_;
    free(current);
    current = next;
  }
  hand = NULL;
}

//-----------------------------------------------------------------------------
///
void freeAllCards(Player player_one, Player player_two, Card* draw_pile)
{
  freeCards(player_one.hand_);
  freeCards(player_two.hand_);
  freeCards(draw_pile);
}

//-----------------------------------------------------------------------------
/// The actual main function of the program!
/// 
/// @return int
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

  Card* draw_pile = NULL;

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
