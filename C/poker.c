/* This C program solves Project Euler problem #54.
        
        Program reads in a file with two poker hands per line and determines
        how many are won by hand one. There are no ties.
        
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* Don't really need an enum. Could just use the char (C|D|H|S) that is read in
   from the file. Just an excuse to demonstrate that I know how to use enums. */
enum card_suits { clubs, diamonds, hearts, spades };

typedef struct {
           int  pip;
           enum card_suits suit;
} t_card;

typedef struct {
           int pip;
           int count;
} t_card_count;
        
char* CARD_VALS = "0123456789TJQKA";

/* Define the hand types, from highest to lowest */
#define STRAIGHT_FLUSH  9
#define FOUR_KIND       8
#define FULL_HOUSE      7
#define FLUSH           6
#define STRAIGHT        5
#define THREE_KIND      4
#define TWO_PAIR        3
#define ONE_PAIR        2
#define HIGH_CARD       1

int hand_pip_compare (first_elem, second_elem)
    const void *first_elem, *second_elem;
    {
        /* Do a descending sort */
        int first, second;

        first  = (*(t_card *) first_elem).pip;
        second = (*(t_card *) second_elem).pip;
        if (first < second)
            return 1;
        else if (first > second)
            return -1;
        else
            return 0;
    }

int card_count_pip_compare (first_elem, second_elem)
    const void *first_elem, *second_elem;
    {
        int first, second, pip1, pip2;

        first  = (*(t_card_count *) first_elem).count;
        second = (*(t_card_count *) second_elem).count;
        pip1   = (*(t_card_count *) first_elem).pip;
        pip2   = (*(t_card_count *) second_elem).pip;

        if (first < second)
            return 1;
        else if (first > second)
            return -1;
        /* same occurence count, but higher pip value */
        else if (pip1 < pip2)
            return 1;
        else if (pip1 > pip2)
            return -1;
        else if ((first == 0) && (pip1 == 0))
            /* Same values, but initialized zero values */
            return 0;
        else {
            /* same pip value. Mistake, should have been merged in */
            printf ("Error card_count_pip_compare: same pip value twice: %d\n", pip1);
            return 0;
        }
    }

int count_pips (hand, pip_list)
    t_card *hand;
    t_card_count *pip_list;
    /* Fill in the pip_list array with the count of each pip in the hand.
       Return the number of distinct pip values. */
    {
        int i, j;

        j = 0; /* Count of where we are in pip_list */
        pip_list [0].pip   = hand [0].pip;
        pip_list [0].count = 1;
        /* Clear pip list, start at 1 since already assigned zero */
        for (i = 1; i < 5; i++) {
            pip_list [i].pip   = 0;
            pip_list [i].count = 0;
        }
        for (i = 1; i < 5; i++) {
            /* Could use a nested loop, but with the fixed number, just went for if-else */
            if (hand [i].pip == pip_list [0].pip) {
                pip_list [0].count++;
            } else if (hand [i].pip == pip_list [1].pip) {
                pip_list [1].count++;
            } else if (hand [i].pip == pip_list [2].pip) {
                pip_list [2].count++;
            } else if (hand [i].pip == pip_list [3].pip) {
                pip_list [3].count++;
            } else if (hand [i].pip == pip_list [4].pip) {
                pip_list [4].count++;
            } else { /* New pip value */
                j++;
                pip_list [j].pip   = hand [i].pip;
                pip_list [j].count = 1;
            }
        }
        /* Now sort based upon count */
        qsort (pip_list, 5, sizeof(t_card_count), card_count_pip_compare);
        return j + 1; /* Started at zero. Need to increment by one. */
    }

/* Returns true if hand is a straight. */
_Bool straight (hand)
    t_card *hand;
    {
        /* 
        :param hand: an array with a single hand of five elements
        hand is assumed to already sorted by value from high to low
        Therefore, can determine if it is a straight by making sure
        the next card is one less than previous
        */
        
        int i, last_card;
        /* last_card is set to highest */
        last_card = hand[0].pip;
        /* for next four cards make sure it is one less than previous card */
        for (i = 1; i < 5; i++) {
            if ((last_card-1) == hand[i].pip)
                /* set last card to be the latest card checked */
                last_card = hand[i].pip;
            else
                return false;
        }
        return true;
    }
        
/* Return true if hand is a flush, i.e., suit is same for all five cards */
_Bool flush(hand)
    t_card *hand;
    {
        /* :param hand: an array with a single hand of five elements */
        
        int i;
        enum card_suits suit;
        /* set suit to that of the first card */
        suit = hand[0].suit;
        for (i = 0; i < 5; i++) {
            /* check suit of each remaining card to that of the first card */
            if (suit != hand[i].suit) {
                return false;
            }
        }
        return true;
    }
        
        
_Bool straight_flush(hand)
    t_card *hand;
    {
        /* Call the straight() and flush() functions.
           :param hand: a 2D array, 5x2, with a single hand
           Could save computation by using a combined function to figure out
           all three, but this is clearer
        */
        
        return straight(hand) && flush(hand);
    }

int card_val_convert(pip_value)
    char *pip_value; /* a single string value, from 2 to TJQKA */
        /* Convert the card value into an integer, two to fourteen. */
    {
        char *finder;

        finder = strchr(CARD_VALS, *pip_value);
        return (int) (finder - CARD_VALS);
    }

enum card_suits card_suit_convert(card_suit)
    char card_suit; /* :param card: a single character value, one of C, D, H, or S */
    {
        
        switch (card_suit) {
            case 'C':
                return clubs; /* no break needed */
            case 'D':
                return diamonds;
            case 'H':
                return hearts;
            case 'S':
                return spades;
            default:
                printf ("Error card_suit_convert: Bad input for suit %c", card_suit);
                return (enum card_suits) -1;
        }
    }

int num_vals (card_counts, max_cnt)
    int card_counts [14];
    int *max_cnt;
    {
        /* Count how many unique values are in a hand using the card_counts array
           Also how many times does the most frequent pip value occur */
        int i, count;

        count    = 0;
        *max_cnt = 0;
        for (i = 1; i < 14; i++) {
            if (card_counts [i] != 0) {
                count++;
                if (card_counts [i] > *max_cnt) {
                    *max_cnt = card_counts [i];
                }
            }
        }
        if (count > 5)
            printf ("Error: %d unique values in a hand", count);
        return (count);
    }

int hand_type (hand, unique_vals, how_often)
    t_card hand [5];
    int unique_vals;
    int how_often;
    {
        /* Determines the type of hand.
        :param hand: an array of five elments of type card
        :param unique_vals: the number of unique pip values in the hand
        :param how_often: how often the most frequent pip value occurs
        :return: encodedhand type values (defined using #define)
        Leverages straight() and flush() functions
        */
        /* If there are five unique card values, then it is one of straight, flush,
           straight flush, or just high card */
        if (unique_vals == 5) {
            if (straight_flush (hand))
                return (STRAIGHT_FLUSH);
            else if (flush (hand))
                return (FLUSH);
            else if (straight(hand))
                return (STRAIGHT);
            else
                return (HIGH_CARD);
        }
        /* If there are four unique card values, it can only be one pair */
        else if (unique_vals == 4) {
            return (ONE_PAIR);
        }
        /* If there are three unique card values, then either it's three of a kind or two pair */
        else if (unique_vals == 3) {
            /* pip_list [0].count has the count of the most frequently occuring pip */
            /* If most frequent unique value occurs three times, then it's three of a kind */
            if (how_often == 3)
                return (THREE_KIND);
            else
                return (TWO_PAIR);
        }
        /* If there are two unique card values, it is four of a kind or a full
           house (three and two of a kind) */
        else if (unique_vals == 2) {
            /* If most frequent unique value occurs four times, then it's four of a kind */
            if (how_often == 4)
                return (FOUR_KIND);
            else
                return (FULL_HOUSE);
        }
        /* Without wild cards you cannot have five of a kind */
        else
           printf ("Bad value for number of unique values: %d\n", unique_vals);
    }

int two_pair_tie_breaker(pip_list1, pip_list2)
    t_card_count pip_list1 [5], pip_list2 [5];
    /* Determine the winner when both hands have two pair. It's complicated enough
       to need its own function.
       :param pip_list1: array of struct with pip values and frequency counts for hand1
       :param pip_list2: array of struct with pip values and frequency counts for hand2
       :return: number of winning hand
     */
     {
        /* Check to see who has the highest two pair */
        if (pip_list1 [0].pip > pip_list2 [0].pip) {
            return 1;
        } else if (pip_list2 [0].pip > pip_list1 [0].pip) {
            return 2;
        /* Top pair is equal. Check second pair */
        } else if (pip_list1 [1].pip > pip_list2 [1].pip) {
            return 1;
        } else if (pip_list2 [1].pip > pip_list1 [1].pip) {
            return 2;
        /* Both pair are equal. Check fifth card (third in pip_list array) */
        } else if (pip_list1 [2].pip > pip_list2 [2].pip) {
            return 1;
        } else if (pip_list2 [2].pip > pip_list1 [2].pip) {
            return 2;
        } else {
            printf ("All cards match in two pairs. Illegal input\n");
            return -1;
        }
    }
        
int one_pair_tie_breaker(hand1, hand2, pip_list1, pip_list2)
    t_card hand1 [5], hand2 [5];
    t_card_count pip_list1 [5], pip_list2 [5];
    {
    /* Determine the winner when both hands have one pair. It's complicated enough
       to need its own function.
       :param hand1: an array of five elments of type card
       :param hand2: an array of five elments of type card
       :param pip_list1: array of struct with pip values and frequency counts for hand1
       :param pip_list2: array of struct with pip values and frequency counts for hand2
       :return: number of winning hand
     */
        
        int i;
        /* Check to see who has the higher pair */
        if (pip_list1 [0].pip > pip_list2 [0].pip) {
            return 1;
        } else if (pip_list2 [0].pip > pip_list1 [0].pip) {
            return 2;
        /* Pair has same value, check highest of each hand one by one until mismatch */
        } else {
            for (i = 0; i < 5; i++) {
                if (hand1[i].pip > hand2[i].pip) {
                    return 1;
                } else if (hand2[i].pip > hand1[i].pip) {
                    return 2;
                }
            }
        }
        /* All cards equal */
        printf ("Error one_pair_tie_breaker: All cards equal. Illegal input\n");
        return -1;
    }
        
int high_card_tie_breaker(hand1, hand2)
    t_card hand1 [5], hand2 [5];
    {
    /* Determine the winner when just high card. Has enought steps to need its own funtion.
       :param hand1: an array of five elments of type card
       :param hand2: an array of five elments of type card
       :return: number of winning hand
     */
        int i;
        
        for (i = 0; i < 5; i++) {
            if (hand1[i].pip > hand2[i].pip)
                return 1;
            else if (hand2[i].pip > hand1[i].pip)
                return 2;
        }
        /* All cards equal */
        printf ("Error high_card_tie_breaker: All cards equal. Illegal input\n");
        return -1;
    }
        
int tie_breaker (type_of_hand, hand1, hand2, pip_list1, pip_list2)
    int type_of_hand;
    t_card hand1 [5], hand2 [5];
    t_card_count pip_list1 [5], pip_list2 [5];
    /* Determine the winner when both hands have one pair. It's complicated enough
       to need its own function.
       :param type_of_hand: encoding of hand type
       :param hand1: an array of five elments of type card
       :param hand2: an array of five elments of type card
       :param pip_list1: array of struct with pip values and frequency counts for hand1
       :param pip_list2: array of struct with pip values and frequency counts for hand2
       :return: number of winning hand
     */
    {
        /* If the hand is a flush, straight, or straight flush */
        if ((type_of_hand == STRAIGHT_FLUSH) || (type_of_hand == FLUSH) ||
            (type_of_hand == STRAIGHT)) {
        /* Can determine by comparing the highest card in each hand */
            if (hand1[0].pip > hand2[0].pip) {
                printf ("Hand1 wins\n");
                return 1;
            } else if (hand2[0].pip > hand1[0].pip) {
                return 2;
            } else {
                printf ("Tie in hands - bad data\n");
                return -1;
            }
        }
        /* Hand is four of a kind */
        else if (type_of_hand == FOUR_KIND) {
            /* Card values in the four of a kind are unique. Just compare them */
            if (pip_list1 [0].pip > pip_list2 [0].pip) {
                return 1;
            } else {
                return 2;
            }
        }
        /* Hand is full house or three of kind */
        else if ((type_of_hand == THREE_KIND) || (type_of_hand == FULL_HOUSE)) {
        /* Can determine winner by comparing value of three of a kind. Must be different. */
            if (pip_list1 [0].pip > pip_list2 [0].pip) {
                return 1;
            } else {
                return 2;
            }
        }
        /* Two pair */
        else if (type_of_hand == TWO_PAIR) {
            return two_pair_tie_breaker (pip_list1, pip_list2);
        /* One pair */
        } else if (type_of_hand == ONE_PAIR) {
            return one_pair_tie_breaker (hand1, hand2, pip_list1, pip_list2);
        /* High card */
        } else if (type_of_hand == HIGH_CARD) {
            return high_card_tie_breaker (hand1, hand2);
        } else {
            printf ("Illegal hand type (over nine or less than one\n");
            return -1;
        }
    }
        
int main()
    {
    /* Main loop for Project Euler problem #54
        
       Sets-up main data structures
       Reads each line in the file, and converts into data structure format
       Calculates how often each pip value appears in each hand
       Calculates type and compares to determine winner, or if tie breaker is needed
     */

        int hand1_wins, hand1_type, hand2_type;
        int unique_vals, i;
        t_card hand1 [5], hand2 [5];
        t_card_count pip_list1 [5], pip_list2 [5];
        FILE *in_file;
        char *file_name = "poker.txt";
        char in_buffer [40];
        char *chr_ptr, *token;

        hand1_wins = 0;
        /* Each line is one pair of poker hands */
        in_file = fopen (file_name, "r");
        if (in_file == NULL) {
           printf ("Can't open file %s\n", file_name);
           return;
        }
        while (fgets (in_buffer, sizeof(in_buffer), in_file)) {
            chr_ptr = in_buffer;
            i = 0;
            while (token = strsep (&chr_ptr, " ")) {
                if (i < 5) { /* processing hand 1's cards */
                    hand1 [i].pip  = card_val_convert  (&token [0]);
                    hand1 [i].suit = card_suit_convert  (token [1]);
                } else { /* processing hand 2's cards */
                    hand2 [i-5].pip  = card_val_convert  (&token [0]);
                    hand2 [i-5].suit = card_suit_convert  (token [1]);
                }
                i++;
            }
            qsort (&hand1, 5, sizeof(t_card), hand_pip_compare);
            qsort (&hand2, 5, sizeof(t_card), hand_pip_compare);
            /* Figure out how often each pip value occurs */
            unique_vals = count_pips (hand1, pip_list1);
            hand1_type = hand_type (hand1, unique_vals, pip_list1 [0].count);
            unique_vals = count_pips (hand2, pip_list2);
            hand2_type = hand_type (hand2, unique_vals, pip_list2 [0].count);
            printf ("Hand 1 type: %d, Hand 2 type: %d\n", hand1_type, hand2_type);
            if (hand1_type > hand2_type) {
                printf ("Hand 1 wins\n");
                hand1_wins += 1;
            } else if (hand1_type == hand2_type) {
                if (tie_breaker (hand1_type, hand1, hand2, pip_list1, pip_list2) == 1) {
                    printf ("Hand 1 wins\n");
                    hand1_wins += 1;
                }
            }
            /* Put rest of processing in here.
            Need to do sort. Probably create the pip_list */
        }
        printf ("\nHand 1 wins %d times\n", hand1_wins);
        /*
        
        # Determine the winning hand
        # If different types of hands, the higher encoding wins
        else if hand1_type == hand2_type:
            # Hand types are the same. Call the tie breaking routine
            if tie_breaker(hand1_type, hand1, hand2, counts_card1, counts_card2) == 1:
                print("Hand 1 wins")
                hand1_wins += 1
    */
    }
