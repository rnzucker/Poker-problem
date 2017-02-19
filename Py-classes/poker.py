# coding=utf-8
"""This Python program solves Project Euler problem #54.

Program reads in a file with two poker hands per line and determines
how many are won by hand one. There are no ties.

This is a rewrite of the original solution, but using classes
"""
from collections import Counter
from operator import itemgetter
import HandCard


__author__ = 'rzucker'

CARD_VALS = "_123456789TJQKA"  # Used for converting the card values to 2..14
SUIT_VALS = "CDHS"             # Used for converting suits to 0..3

# Used to be a constant for hand types for comparison.
# This dictionary isn't truly necessary, but it makes for easier
# to understand code.
HAND_TYPE_VALS = {'Straight Flush': 9,
                  '4 Kind':         8,
                  'Full House':     7,
                  'Flush':          6,
                  'Straight':       5,
                  '3 Kind':         4,
                  'Two Pair':       3,
                  'One Pair':       2,
                  'High Card':      1}

# This dictionary is just used to make it easier to code debug messages when needed
DEBUG_HAND_TYPE_STRING = {9: "Straight Flush",
                          8: "4 of a Kind",
                          7: "Full House",
                          6: "Flush",
                          5: "Straight",
                          4: "3 of a Kind",
                          3: "Two Pair",
                          2: "One Pair",
                          1: "High Card"}


def main():
    """Main loop for Project Euler problem #54

    Sets-up main data structures
    Reads each line in the file, and converts into data structure format
    Calculates how often each pip value appears in each hand
    Calculates type and compares to determine winner, or if tie breaker is needed
    """

    hand1_wins = 0
    # Create a two dimensional array for hand1 and hand2
    # One for suit, one for value for each of five cards in a hand
    # This will get reinitialized each time through the loop
    hand1 = [[0]*2 for _ in range(5)]
    hand2 = [[0]*2 for _ in range(5)]
    # Each line is one pair of poker hands
    for line in open('poker.txt'):
        # Use rstrip() to get rid of \n at the end
        hands = line.rstrip().split(" ")
        hand1 = HandCard.Hand()
        hand2 = HandCard.Hand()
        # Convert each hand into numeric form
        for i in range(5):
            # Assign into hand data structure. Init routine of card will convert values
            hand1[i].m_hand[i] = HandCard.Card(hands[i][0],   hands[i][1])
            hand2[i].m_hand[i] = HandCard.Card(hands[i+5][0], hands[i+5][1])
        # Sort each hand into descending order
        hand1.Card.sort(reverse=True)
        hand2.Card.sort(reverse=True)

        # Figure out how often each pip value occurs in each hand, and order by most frequent occurence
        hand1.count_pips()
        hand2.count_pips()
        hand1_type = hand1.hand_type()
        hand2_type = hand2.hand_type()
        print("Hand 1 type is", DEBUG_HAND_TYPE_STRING[hand1_type], "   "
              "Hand 2 type is", DEBUG_HAND_TYPE_STRING[hand2_type])

        # Determine the winning hand
        # If different types of hands, the higher encoding wins
        if hand1_type > hand2_type:
            print("Hand 1 wins")
            hand1_wins += 1
        elif hand1_type == hand2_type:
            # Hand types are the same. Call the tie breaking routine
            if hand1.tie_breaker(hand1_type, hand1, hand2) == 1:
                print("Hand 1 wins")
                hand1_wins += 1
    print("\nHand 1 wins", hand1_wins, "times.")


# Check for interactive session
if __name__ == '__main__':
    # execute main program
    main()
