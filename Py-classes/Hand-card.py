# coding=utf-8
"""These are the card and hand classes used in solving the Project
Euler poker problem.

"""
from collections import Counter
from operator import itemgetter

__author__ = 'rzucker'



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


class Card:
    m_pip  = -1
    m_suit = -1

    CARD_VALS = "_123456789TJQKA"  # Used for converting the card values to 2..14
    SUIT_VALS = "CDHS"             # Used for converting suits to 0..3

    @property
    def m_pip (self):
        return self.m_pip

    @m_pip.setter
    def m_pip (self, pip):
        self.m_pip = self.CARD_VALS.find(pip)

    @property
    def m_suit (self):
        return self.m_suit

    @m_suit.setter
    def m_suit (self, suit):
        self.m_suit = self.SUIT_VALS.find(suit)

    # def __init__(self, pip, suit):
    #     """ Initializes a card value, converting from characters to the numeric encoding
    #     :param pip:  A character read from the file that is the card's value
    #     :param suit: A character read from the file that indicates the card's suit
    #     """
    #     self.m_pip  = self.CARD_VALS.find(pip)
    #     self.m_suit = self.SUIT_VALS.find(suit)

    def __lt__(self, other)
        """ Overload of 'less than' for two cards
        """
        return self.m_pip < other.m_pip

    def __gt__(self, other)
        """ Overload of 'greater than' for two cards
        """
        return self.m_pip > other.m_pip

    def __eq__(self, other):
        """ Overload of equal for two cards
        """
        return self.m_pip == other.m_pip


class Hand:

    class PipCount:
        m_pip   = 0
        m_count = 0

    m_pip_count = [PipCount() for _ in range(5)]
    m_hand = [Card() for _ in range(5)]
    m_unique_vals = 0

    def count_pips(self):
        """This routine calculates how often each pip value occurs in the hand.
        It also figures out how many different unique pip values there (m_unique_vals.)
        Finally, it sorts the list for most frequently occuring pip to least occuring.
        Be aware that if there are any pairs (or above) there will be entries with zero.
        """
        self.m_pip_count[0].m_pip   = self.m_hand[0].m_pip
        self.m_pip_count[0].m_count = 1

        for i in range(5):
            # Could use a nested look, but with small number, just went for an if-else
            if self.m_hand[i].m_pip == self.m_pip_count[0].m_pip:
                self.m_pip_count[0].m_count += 1
            elif self.m_hand[i].m_pip == self.m_pip_count[1].m_pip:
                self.m_pip_count[1].m_count += 1
            elif self.m_hand[i].m_pip == self.m_pip_count[2].m_pip:
                self.m_pip_count[2].m_count += 1
            elif self.m_hand[i].m_pip == self.m_pip_count[3].m_pip:
                self.m_pip_count[3].m_count += 1
            elif self.m_hand[i].m_pip == self.m_pip_count[4].m_pip:
                self.m_pip_count[4].m_count += 1
            else:
                self.m_unique_vals += 1
                self.m_pip_count[self.m_unique_vals].m_pip = self.m_hand[i].m_pip
                self.m_pip_count[self.m_unique_vals].m_count = 1

        self.m_unique_vals += 1 # Need number of unique values, not array index. Add one
        # Sort pip count array based upon count, from highest to lowest
        self.m_pip_count.sort(key=itemgetter(0), reverse=True)


    def straight(self):
        """Returns true if hand is a straight.

        """

        # last_card is set to highest
        last_card = self.m_hand [0].m_pip
        for i in range(1, 5):
            # for next four cards make sure it is one less than the previous card
            if (last_card-1) == self.m_hand[i].m_pip:
                # set last card to be the latest card checked
                last_card = self.m_hand[i].m_pip
            else:
                return False
        return True


    def flush(self):
        """ Returns true if hand is a flush; i.e., suit is same for all five cards.
        """

        # set suit to that of the first card
        suit = self.m_hand[0].m_suit
        for i in range(1, 5):
            # check suit of each remaining card to that of the first card
            if suit != self.m_hand[i].m_suit:
                return False
        return True


    def straight_flush(self):
        """Call the straight and flush functions.
        """

        return self.straight() and self.flush()

    # def card_val_convert(card):
    #     """Convert the card value into an integer, two to fourteen.
    #
    #     :param card: a single string value, from 2 to TJQKA
    #     Use string find method on CARD_VALS list
    #     """
    #
    #     card_val = CARD_VALS.find(card)
    #     return card_val
    #
    #
    # def card_suit_convert(card):
    #     """Convert the suit into an integer, zero to three.
    #
    #     :param card: a single string value, one of C, D, H, or S
    #     """
    #
    #     card_suit_num = SUIT_VALS.find(card)
    #     return card_suit_num


def hand_type(self):
    """Determines the type of hand.

    Leverages straight() and flush() functions
    Also leverage count of most frequently occurring pip value in card_counts
    """

    # If there are five unique card values, then it is one of straight, flush, straight flush, or just high card
    if self.unique_vals == 5:
        if self.straight_flush():
            return HAND_TYPE_VALS['Straight Flush']
        elif self.flush():
            return HAND_TYPE_VALS['Flush']
        elif self.straight():
            return HAND_TYPE_VALS['Straight']
        else:
            return HAND_TYPE_VALS['High Card']
    # If there are four unique card values, it can only be one pair
    elif self.unique_vals == 4:
        return HAND_TYPE_VALS['One Pair']
    # If there are three unique card values, then either it's three of a kind or two pair
    elif self.unique_vals == 3:
        # If most frequent unique value occurs three times, then it's three of a kind
        if self.card_counts[0][1] == 3:
            return HAND_TYPE_VALS['3 Kind']
        else:
            return HAND_TYPE_VALS['Two Pair']
    # If there are two unique card values, it is four of a kind or a full house (three and two of a kind)
    elif self.unique_vals == 2:
        # If most frequent unique value occurs four times, then it's four of a kind
        if self.card_counts[0][1] == 4:
            return HAND_TYPE_VALS['4 Kind']
        else:
            return HAND_TYPE_VALS['Full House']
    # Without wild cards you cannot have five of a kind
    else:
        print("Bad value for number of unique values", self.unique_vals)


def two_pair_tie_breaker(hand1, hand2):
    """Determine the winner when both hands have two pair. It's complicated enough to need its own function.

    :param counts_card1: list of tupes with pip values and frequency counts for hand1
    :param counts_card2: list of tupes with pip values and frequency counts for hand2
    :return: number of winning hand
    """
    # Check to see who has the highest two pair
    if hand1.m_pip_count[0].m_pip > hand2.m_pip_count[0].m_pip:
        return 1
    elif hand2.m_pip_count[0].m_pip > hand1.m_pip_count[0].m_pip:
        return 2
    # Top pair is equal. Check second pair
    elif hand1.m_pip_count[1].m_pip > hand2.m_pip_count[1].m_pip:
        return 1
    elif hand2.m_pip_count[1].m_pip > hand1.m_pip_count[1].m_pip:
        return 2
    # Both pair are equal. Check fifth card (third in count array)
    elif hand1.m_pip_count[2].m_pip > hand2.m_pip_count[2].m_pip:
        return 1
    elif hand2.m_pip_count[2].m_pip > hand1.m_pip_count[2].m_pip:
        return 2
    else:
        print("All cards match in two pairs. Illegal input")
        return -1


def one_pair_tie_breaker(hand1, hand2):
    """Determine the winner when both hands have one pair. It's complicated enough to need its own function.

    :param hand1: a 2D array, 5x2, for hand1
    :param hand2: a 2D array, 5x2, for hand2
    :param counts_card1: list of tupes with pip values and frequency counts for hand1
    :param counts_card2: list of tupes with pip values and frequency counts for hand2
    :return: number of winning hand
    """

    # Compare the value of the pair and return identity of higher value, if not the same
    if hand1.m_pip_count[0].m_pip > hand2.m_pip_count[0].m_pip:
        return 1
    elif hand2.m_pip_count[0].m_pip > hand1.m_pip_count[0].m_pip:
        return 2
    # Pair has same value, check highest of each hand one by one until mismatch
    else:
        for i in range(0, 5):
            if hand1.m_hand[i].m_pip > hand2.m_hand[i].m_pip:
                return 1
            elif hand2.m_hand[i].m_pip > hand1.m_hand[i].m_pip:
                return 2
        # All cards equal
        print("All cards equal. Illegal input")
        return -1


# Determining the winner when just high card has enough steps to needs its own function
def high_card_tie_breaker(hand1, hand2):
    """Determine the winner when just high card. Has enought steps to need its own funtion.

    :param hand1: a 2D array, 5x2, for hand1
    :param hand2: a 2D array, 5x2, for hand2
    :return: number of winning hand
    """

    for i in range(0, 5):
        if hand1.m_hand[i].m_pip > hand2.m_hand[i].m_pip:
            return 1
        elif hand2.m_hand[i].m_pip > hand1.m_hand[i].m_pip:
            return 2
    # All cards equal
    print("All cards equal. Illegal input")
    return -1


# Hand type is the same for both hands. Determine who wins via tie breaker routines
# Params: hand type, hand1 and hand2 (2D array), and sorted card count array for hand1 and hand2
def tie_breaker(type_of_hand, hand1, hand2):
    """Hand type is the same for both hands. Determines winner via various tie-breaker mechanisms.

    :param type_of_hand: an integer
    :param hand1: type Hand
    :param hand2: type Hand
    :return: number of winning hand
    Makes use of hand type codes (HAND_TYPE_VALS) to classify solution
    """

    # If the hand is a flush, straight, or straight flush
    if type_of_hand == 9 or type_of_hand == 6 or type_of_hand == 5:
        # Can determine by comparing the highest card in each hand
        if hand1.m_hand[0].m_pip > hand2.m_hand[0].m_pip:
            print("Hand1 wins")
            return 1
        elif hand2.m_hand[0].m_pip > hand1.m_hand[0].m_pip:
            return 2
        else:
            print("Tie in hands - bad data")
            return -1

    # Hand is four of a kind
    elif type_of_hand == 8:
        # Card values in a four of a kind are unique. Just compare them
        if hand1.m_pip_count[0].m_pip  > hand2.m_pip_count[0].m_pip:
            return 1
        else:
            return 2

    # Hand is full house or three of kind
    elif type_of_hand == 4 or type_of_hand == 7:
        # Can determine winner by comparing value of three of a kind. Must be different.
        if hand1.m_pip_count[0].m_pip  > hand2.m_pip_count[0].m_pip:
            return 1
        else:
            return 2
    # Two pair
    elif type_of_hand == 3:
        return two_pair_tie_breaker(hand1, hand2)
    # One pair
    elif type_of_hand == 2:
        return one_pair_tie_breaker(hand1, hand2)
    # High card
    elif type_of_hand == 1:
        return high_card_tie_breaker(hand1, hand2)
    else:
        print("Illegal hand type (over nine or less than one")
        return -1
