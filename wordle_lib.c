#include "wordle_lib.h"
#include <string.h>
#include <stdlib.h>

bool score_guess(char *secret, char *guess, char *result) {
    if (strncmp(guess, secret, 5) == 0){
        for (int i = 0; i < 5; i++){
            result[i] = 'g';
        }
        result[5] = '\0';
        return true;
    }
    bool thereornot[5] = {false};
    for (int i = 0; i < 5; i++){
        if (secret[i] == guess[i]){
            result[i] = 'g';
                thereornot[i] = true;
        }
        else {
            result[i] = 'x';
        }
    }
    for (int i = 0; i < 5; i++) {
          if (result[i] == 'x') {
                for (int j = 0; j < 5; j++) {
                     if (!thereornot[j] && secret[j] == guess[i]) {
                          result[i] = 'y';
                          thereornot[j] = true;
                          break;
                     }
                }
          }
     }
    result[5] = '\0';
    return false;
}

bool valid_guess(char *guess, char **vocabulary, size_t num_words) {
     for (size_t i = 0; i < num_words; i++){
          if (strncmp(guess, vocabulary[i], 5) == 0){
                     return true;
          }
     }
     return false;
}

bool hardmode(char *guess, char **past_guesses, char *secret) {
     for (int i = 0; i < 5; i++) {
          char current_guess_letter = guess[i];
          bool letter_guessed_correctly = false;
          for (unsigned long j = 0; j < strlen(secret); j++) {
                if (past_guesses[j][i] == 'g' && secret[j] == current_guess_letter) {
                     letter_guessed_correctly = true;
                     break;
                }
          }
          if (!letter_guessed_correctly && strchr(secret, current_guess_letter) != NULL) {
                bool letter_in_secret_not_in_same_position = false;
                for (unsigned long j = 0; j < strlen(secret); j++) {
                     if (past_guesses[j][i] != 'g' && secret[j] == current_guess_letter) {
                          letter_in_secret_not_in_same_position = true;
                          break;
                     }
                }
                if (!letter_in_secret_not_in_same_position) {
                     if (strchr(secret, current_guess_letter) == NULL) {
                          bool letter_not_in_past_guesses = true;
                          for (unsigned long j = 0; j < strlen(secret); j++) {
                                if (strchr(past_guesses[j], current_guess_letter) != NULL) {
                                     letter_not_in_past_guesses = false;
                                     break;
                                }
                          }
                          if (letter_not_in_past_guesses) {
                                return false;
                          }
                     }
                }
          }
     }
     return true;
}

char **load_vocabulary(char *filename, size_t *num_words) {
  *num_words = 0;
  char **out = NULL;
  FILE* file = fopen(filename, "r");
  char buffer[7];
  int count = 0;
  while (fgets(buffer, sizeof(buffer), file) != NULL){
          if (strlen(buffer) == 6){
                     out = realloc(out, (count + 1) * sizeof(char*));
                     out[count] = strdup(buffer);
                     count++;
          }
  }
  *num_words = count;
  fclose(file);
  return out;
}

void free_vocabulary(char **vocabulary, size_t num_words) {
    for (size_t i = 0; i < num_words; i++){
        free(vocabulary[i]);
    }
    free(vocabulary);
}
