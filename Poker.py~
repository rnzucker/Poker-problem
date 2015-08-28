__author__ = 'rzucker'

import collections
import operator

CARD_VALS = "_123456789TJQKA" # Used for converting to 1..13
SUIT_VALS = "CDHS"            # Used for converting suits to 1..4
# Used to be a constant for hand types for comparison
HAND_TYPE_VALS = {'Straight Flush': 9,
                  '4 Kind':         8,
                  'Full House':     7,
                  'Flush':          6,
                  'Straight':       5,
                  '3 Kind':         4,
                  'Two Pair':       3,
                  'One Pair':       2,
                  'High Card':      1}

def straight(hand):
    # Returns true if hand is a straight
    # hand is a two dimension array, 5x2, with a single hand
    # hand is assumed to already sorted by value from high to low
    # Therefore, can determine if it is a straight by making sure next card is one less than previous
    #
    # last_card is set to highest
    last_card = hand[0][0]
    for i in range(1,5):
        # for next four cards make sure it is one less than the previous card
        if (last_card-1) == hand[i][0]:
            # set last card to be the latest card checked
            last_card = hand[i][0]
        else:
            return False
    return True

def flush(hand):
    # Returns true if hand is a flush; i.e., suit is same for all five cards
    # hand is a two dimension array, 5x2, with a single hand
    #
    # set suit to that of the first card
    suit = hand[0][1]
    for i in range(1,5):
        # check suit of each remaining card to that of the first card
        if suit != hand[i][1]:
            return False
    return True

def straight_flush(hand):
    # Call the straight and flush functions. Could save computation by using a combined function
    # to figure out all three, but this is clearer
    if straight(hand) and flush(hand):
        return True
    return False

def card_val_convert(card):
    # Convert the card value into an integer, two to fourteen
    card_val = CARD_VALS.find(card)
    return card_val

def card_suit_convert(card):
    # Convert the suit into an integer, zero to three
    card_suit_num = SUIT_VALS.find(card)
    return card_suit_num

def hand_type(hand, card_counts):
    unique_vals = len(card_counts)
    if unique_vals == 5:
        if straight_flush(hand):
            return HAND_TYPE_VALS['Straight Flush']
        elif flush(hand):
            return HAND_TYPE_VALS['Flush']
        elif straight(hand):
            return HAND_TYPE_VALS['Straight']
        else:
            return HAND_TYPE_VALS['High Card']
    elif unique_vals == 4:
        return HAND_TYPE_VALS['One Pair']
    elif unique_vals == 3:
        if card_counts[0][1] == 3:
            return HAND_TYPE_VALS['3 Kind']
        else:
            return HAND_TYPE_VALS['Two Pair']
    elif unique_vals == 2:
        if card_counts[0][1] == 4:
            return HAND_TYPE_VALS['4 Kind']
        else:
            return HAND_TYPE_VALS['Full House']
    else:
        print( "Bad value for number of unique values", unique_vals)

def main():
    for line in open('test.txt'):
#       Use rstrip() to get rid of \n at the end
        hands = line.rstrip().split(" ")
#       Create a two dimensional array for hand1 and hand2
#       One for suit, one for value for each of five cards in a hand
        hand1 = [[0 for x in range(2)] for x in range(5)]
        hand2 = [[0 for x in range(2)] for x in range(5)]
        hand1_vals  = [0 for x in range(5)]
        hand1_suits = [0 for x in range(5)]
#       Convert each hand into numeric form
        for i in range(5):
            hand1[i][0] = card_val_convert(  hands[i]  [0])
            # Convert values of cards into a one dimensional list
            hand1_vals [i] = hand1[i][0]
            hand1[i][1] = card_suit_convert( hands[i]  [1])
            hand2[i][0] = card_val_convert(  hands[i+5][0])
            hand2[i][1] = card_suit_convert( hands[i+5][1])
            hand1.sort(reverse=True)
            hand2.sort(reverse=True)
        cards = collections.Counter()
        for each_card in hand1_vals:
            # Increment count of hash table value
            cards [each_card] += 1
        print ("Cards dup", cards)
        counts_card = sorted(cards.items(), key=operator.itemgetter(1),reverse=True)
        print ("Hand 1 is", hand1, ", Hand 2 is", hand2)
        print ("Hand 1 type is", hand_type(hand1, counts_card))
        print ("Card counts sorted", counts_card, len(counts_card), "values", "\n" )
        # print ("Hand 1 is a flush =", flush(hand1))
        # print ("Hand 1 is a straight =", straight(hand1))
        # print ("Hand 1 is a straight flush =", straight_flush(hand1))
        # print( "" )



main()