#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);

/* Task 1 */
game_state_t* create_default_state() {
  // TODO: Implement this function.
  // return NULL;
  game_state_t *default_state = malloc(sizeof(game_state_t));
  default_state->num_rows = 18;
  default_state->num_snakes = 1;
  default_state->snakes = malloc(default_state->num_snakes * sizeof(snake_t));
  default_state->snakes->tail_row = 2;
  default_state->snakes->tail_col = 2;
  default_state->snakes->head_row = 2;
  default_state->snakes->head_col = 4;
  default_state->snakes->live = true;
  default_state->board = malloc(default_state->num_rows * sizeof(char*));
  for (int i = 0; i < default_state->num_rows; i++) {
    default_state->board[i] = (char*)malloc(21 * sizeof(char)); // C语言char* of char[]均以'\0'结尾，不管怎么操作!
    strcpy(default_state->board[i], "#                  #"); // 第一次尝试是用字符，但是会导致要求' '空格，实质上是真空的————0。
  }
  strcpy(default_state->board[0], "####################");
  strcpy(default_state->board[default_state->num_rows-1], "####################");
  default_state->board[2][9] = '*';
  default_state->board[2][2] = 'd';
  default_state->board[2][4] = 'D';
  default_state->board[2][3] = '>';
  return default_state;
}

/* Task 2 */
void free_state(game_state_t* state) {
  // TODO: Implement this function.
  // return;
  // free(default_state->snakes);
  // for (int i = 0; i < default_state->num_rows; i++) {
  //   free(default_state->board[i]);
  // }
  // free(default_state->board);
  free(state->snakes);
  for (int i = 0; i < state->num_rows; i++) {
    free(state->board[i]);
  }
  free(state->board);
  free(state); // malloc了什么就都一个一个都free掉，state也是malloc来的
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.
  // return;
  for (int i = 0; i < state->num_rows; i++) {
    fprintf(fp, state->board[i]);
    fprintf(fp, "\n");
  }
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) {
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  // TODO: Implement this function.
  // return true;
  return c=='w' || c=='a' || c=='s' || c=='d';
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
  // return true;
  return c=='W' || c=='A' || c=='S' || c=='D' || c=='X';
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  // return true;
  return c=='^' || c=='<' || c=='v' || c=='>' || is_tail(c) || is_snake(c);
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function.
  // return '?';
  if (c=='^') return 'w';
  if (c=='<') return 'a';
  if (c=='v') return 's';
  if (c=='>') return 'd';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
  // return '?';
  if (c=='W') return '^';
  if (c=='A') return '<';
  if (c=='S') return 'v';
  if (c=='D') return '>';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  // TODO: Implement this function.
  // return cur_row;
  if (c=='v' || c=='s' || c=='S') return ++cur_row;
  else if (c=='^' || c=='w' || c=='W') return --cur_row;
  else return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  // TODO: Implement this function.
  // return cur_col;
  if (c=='>' || c=='d' || c=='D') return ++cur_col; // not cur_col++!!
  else if (c=='<' || c=='a' || c=='A') return --cur_col;
  else return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the charactler in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  // return '?';
  // snake_t* snake = &state->snakes[snum];
  snake_t snake = state->snakes[snum];
  char head = get_board_at(state, snake.head_row, snake.head_col);
  return get_board_at(state, get_next_row(snake.head_row, head), get_next_col(snake.head_col, head));
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  // return;
  snake_t* moving_snake = &state->snakes[snum];
  char head = get_board_at(state, moving_snake->head_row, moving_snake->head_col);
  set_board_at(state, moving_snake->head_row, moving_snake->head_col, head_to_body(head));

  moving_snake->head_row = get_next_row(moving_snake->head_row, head);
  moving_snake->head_col = get_next_col(moving_snake->head_col, head);
  set_board_at(state, moving_snake->head_row, moving_snake->head_col, head); //参见尾巴更新，这里用head是否完全合理？
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  // return;
  snake_t* moving_snake = &state->snakes[snum];
  char tail = get_board_at(state, moving_snake->tail_row, moving_snake->tail_col);
  set_board_at(state, moving_snake->tail_row, moving_snake->tail_col, ' ');

  moving_snake->tail_row = get_next_row(moving_snake->tail_row, tail);
  moving_snake->tail_col = get_next_col(moving_snake->tail_col, tail);
  char old_new_tail = get_board_at(state, moving_snake->tail_row, moving_snake->tail_col);
  set_board_at(state, moving_snake->tail_row, moving_snake->tail_col, body_to_tail(old_new_tail));
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
// equal to: typedef int (*add_food)(game_state_t* state); void update_state(game_state_t* state, add_food func) {...func(state);
  // TODO: Implement this function.
  // return;
  for (int i = 0; i < state->num_snakes; i++){
    snake_t* moving_snake = &state->snakes[i];
    char head = get_board_at(state, moving_snake->head_row, moving_snake->head_col);
    char new_head = next_square(state, i);
    if (new_head == ' ') {
      update_head(state, i);
      update_tail(state, i);
    } else if (new_head == '*') {
      update_head(state, i);
      add_food(state);
    } else {
      set_board_at(state, moving_snake->head_row, moving_snake->head_col, 'x');
      moving_snake->live = false;
    }
  }
}

/* Task 5 */
game_state_t* load_board(char* filename) {
  // TODO: Implement this function.
  // return NULL;
  FILE * fp = fopen(filename, "r");
  if (fp == NULL) return NULL;
  game_state_t* state = (game_state_t*)malloc(sizeof(game_state_t));
  state->num_rows = 0;
  char *helper = (char*)malloc(1024 * 1024); // sizeof(char)==1;

  while (1) {
    char *single_helper = (char*)malloc(1024*1023);// 针对“到顶了？“的问题，这里改成了1024*1023，<1024*1024。
    fgets(single_helper, 1024*100, fp);
    strcat(helper, single_helper);
    if (feof(fp)!=0) { //最后一次get到的fp流是EOF，所以进入分支然后break了。一开始将此条作为286行判断，检测的fp还是上一次的流，即棋盘的最后一行，所以还会再多进入一次，导致numrows多一个
      break;
    }
    state->num_rows++;
  }
  fclose(fp);
  // free(fp);

  state->board = malloc(state->num_rows * sizeof(char*));
  int k = 0; // k refers to index of all the chars
  int i = 0; // i refers to index of the rows
  while (helper[k]!='\0') { // traverse all the chars including '\n' until helper[k]=='\0'
    char* single_line = (char*)malloc(102400); // 问题1.没有写casting，malloc返回值是void*类型；2.这里写小了！一行有100000个字符。
    int j = 0;
    for ( ;helper[k]!='\n'; j++, k++) { // j refers to the index of columns; k instead of j because j is may not consistent
      single_line[j] = helper[k];
    }
    state->board[i] = (char*)malloc(j+1); // omit *sizeof(char) since sizeof(char) is equals to 1;
    single_line[j] = '\0';
    // strcat(single_line, '\0');
    strcpy(state->board[i], single_line);
    // state->board[i] = single_line;    SEG FAULT
    i++;
    k++; // 跳过'\n'
  }

  return state;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  // return;
  snake_t* searched_snake = &state->snakes[snum];
  unsigned int next_row = searched_snake->tail_row; // tail's row
  unsigned int next_col = searched_snake->tail_col; // tail's col
  char whereishead = get_board_at(state, searched_snake->tail_row, searched_snake->tail_col); // tail's position
  while (!is_head(whereishead)) {
    next_row = get_next_row(next_row, whereishead);
    next_col = get_next_col(next_col, whereishead);
    whereishead = get_board_at(state, next_row, next_col);
  }
  searched_snake->head_row = next_row;
  searched_snake->head_col = next_col;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  // return NULL;
  if (state == NULL) return NULL;
  state->num_snakes = 0;
  for (int i = 0; i < state->num_rows; i++) {
    for (int j = 0; j < strlen(state->board[i]); j++) {
      if (is_head(get_board_at(state, i, j))) {
        state->num_snakes++;
      }
    }
  }

  state->snakes = malloc(sizeof(snake_t) * state->num_snakes);
  int cnt = 0;
  for (int i = 0; i < state->num_rows; i++) {
    for (int j = 0; j < strlen(state->board[i]); j++) {
      if (is_tail(get_board_at(state, i, j))) {
        state->snakes[cnt].tail_row = i;
        state->snakes[cnt].tail_col = j;
        find_head(state, cnt);
        cnt++;
      }
      if (cnt == state->num_snakes) {
        return state;
      }
    }
  }
}
