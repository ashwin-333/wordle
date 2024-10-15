#include "wordle_lib.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(void) {
  char **vocabulary;
  size_t num_words;
  int num_guesses = 0;

  srand(time(NULL));

  vocabulary = load_vocabulary("vocabulary.txt", &num_words);

  int word_index = rand() % num_words;
  char *secret = vocabulary[word_index];
  
  char guess[80];

  char result[6] = {0};
  bool success = false;
  
  char **past_guesses = malloc(sizeof(char *) * 50);

  printf("time to guess a 5-letter word! (press ctrl-D to exit)\n");
  while (!success) {
    printf("guess: ");
    if (fgets(guess, 80, stdin) == NULL) {
      break;
    }
    guess[5] = '\0';

    if (!valid_guess(guess, vocabulary, num_words)) {
      printf("not a valid guess\n");
      continue;
    } 
    else if (!hardmode(guess, past_guesses, secret)) {
      printf("hey! you can't make that guess, it's inconsistent with what I already told you!\n");
      continue;
    } 
    else {
      past_guesses[num_guesses] = strdup(guess);
      num_guesses++;
    }
    success = score_guess(secret, guess, result);
    printf("%s\n", guess);
    printf("%s\n", result);
  }

  if (success) {
    printf("you win, in %d guesses!\n", num_guesses);
  }
  for (int i = 0; i < num_guesses; i++) {
        free(past_guesses[i]);
  }
  free(past_guesses);
  
  free_vocabulary(vocabulary, num_words);

  return 0;
}
