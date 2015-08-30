Write routines to detect each of these conditions:
 9 - Straight flush      - 5 values
 8 - Four of a kind      - 2 values
 7 - Full house          - 2 values
 6 - Flush               - 5 values
 5 - Straight            - 5 values
 4 - Three of a kind     - 3 values
 3 - Two pair            - 3 values
 2 - One pair            - 4 values
 1 - High card (default) - 5 values

Basic algorithm:
  - Read each pair of hands from a file
  - Convert into a pair of two dimensional arrays for each hand.
    Use numbers for suits and values for easier computation
    Two values per indice; one for suit, one for card value.
    Also have a one dimensional array of card values
    Sort the 2D arrays by card value
  - Using Counters, determine how many of each value in the hand
    After counting, sort to determine what occurs the most
  - Using the sorted counts, determine the card type per the table above
    If more than one option, additional logic to determine which one
  - Compare hand types. If different, determine winner and update counts.
    If hand types the same, use routine detailed below.

Overall routine (hand_type) that calls each of these routines in order,
returning code for type

Winner_routine:
    hand1_type = hand_type(hand1)
    hand2_type = hand_type(hand2)
    if hand1_type > hand2_type:
	Hand 1 wins
    elif hand1_type < hand2_type:
        Hand 2 wins
    else: # same hand type
	if straight flush or flush or straight:
	    Can figure out winner by comparing highest of five cards
	elif four of a kind:
	    Figure out winner by comparing value of card in four of a
	    kind (can't be the same)
	elif full house or three of a kind:
	    Figure out winner by comparing value of the three of a kind
	    (can't be the same)
	elif two pair: # may want to make a separate function
	    calculate value of each pair of pairs
	    if hand1_high_pair > hand2_high_pair:
		Hand 1 wins
	    elif hand2_high_pair > hand1_high_pair:
	    	Hand 2 wins
	    else # high pairs are the same
		if hand1_low_pair > hand2_low_pair:
		    Hand 1 wins
		elif hand2_low_pair > hand1_low_pair:
		    Hand 2 wins
		else # both pairs equal
		    if hand1 5th card > hand2 5th card:
			Hand 1 wins
		    elif hand2 5th card > hand1 5th card:
		        Hand 2 wins
		    else: # Tie should not happen
			TIE!
	elif one pair:
	    calculate value of pairs
	    if hand1_pair > hand2_pair:
		Hand 1 wins
	    elif hand2_pair > hand1_pair:
	    	Hand 2 wins
	    else:
		for i in range(3):
		    compare remaining high cards
	else: # High card situation
	    for i in range(5):
		compare high cards

______________________________________________________
case num of card values:
   1: illegal
   2: four of a kind or full house
	Check if one has four the same
   3: three of a kind or two pair
	Check if one has three the same
   4: one pair
   5: flush, straight, straight flush, or high card
   	Check if flush/straight/straigh flush
	    Else return high card hand
	
