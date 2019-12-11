/* Same as previous C# version, but uses properties on the card definition. */
using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplication1
{
    public enum Hand_Types
    {
        Straight_Flush = 9,
        Four_Kind      = 8,
        Full_House     = 7,
        Flush          = 6,
        Straight       = 5,
        Three_Kind     = 4,
        Two_Pair       = 3,
        One_Pair       = 2,
        High_Card      = 1
    }

    public enum Card_Suits
    {
        clubs, diamonds, hearts, spades
    }

    public class Global
    {
        public const int hand_size = 5;
    }

    /* Class for a single card. A number of these are needed to create a hand. */
    public class Card : IComparable<Card>
    {
        private int M_pip;
        private Card_Suits M_suit;
        /* A string used in converting the text form of the pip value into an integer.
         * The pip's value is equal to the index in the string of its location. */
        static string card_vals = "0123456789TJQKA";

        /* Properties for accessing pip and suit */
        public int m_pip
        {
            get { return M_pip; }
            set { M_pip = value; }
        }

        public Card_Suits m_suit
        {
            get { return M_suit; }
            set { M_suit = value; }
        }


        /* Need a comparison routine for sorting cards in a hand */
        int IComparable<Card>.CompareTo(Card card2)
        {
            if (card2.m_pip > m_pip)
                return 1;
            else if (card2.m_pip < m_pip)
                return -1;
            else
                return 0;
        }

        public Card (char c, char s)
        {
            m_pip = card_vals.IndexOf(c);
            switch (s)
            {
                case 'c':
                case 'C':
                    m_suit = Card_Suits.clubs;
                    break;
                case 'd':
                case 'D':
                    m_suit = Card_Suits.diamonds;
                    break;
                case 'h':
                case 'H':
                    m_suit = Card_Suits.hearts;
                    break;
                case 's':
                case 'S':
                    m_suit = Card_Suits.spades;
                    break;
                default:
                    Console.WriteLine("Bad suit {0} provided", s);
                    break;
            }
        }
    }


    public class Hand
    {
        public Card[] m_hand;
        public Pip_Count[] m_pip_count;
        int m_unique_vals;
        public Hand_Types m_type_of_hand;

        public class Pip_Count : IComparable<Pip_Count>
        {
            public int m_pip;
            public int m_count;

            int IComparable<Pip_Count>.CompareTo(Pip_Count pip_count2)
            {
                if (pip_count2.m_count > m_count)
                    return 1;
                else if (pip_count2.m_count < m_count)
                    return -1;
                else
                    return 0;
            }

            public Pip_Count()
            {
                m_pip = 0;
                m_count = 0;
            }
        }

        /* This routine calculates how often each pip value occurs in the hand.
         * It also figures out how many different unique pip values there (m_unique_vals.)
         * Finally, it sorts the list for most frequently occuring pip to least occuring.
         * Be aware that if there are any pairs (or above) there will be entries with zero.
         */
        public void Count_Pips()
        {
            int i;
            /* pip_count and m_unique_vals should be zero */
            /* Setting pip of first card. */
            m_pip_count[0].m_pip = m_hand[0].m_pip;
            m_pip_count[0].m_count = 1;
            for (i = 1; i < Global.hand_size; i++)
            {
                /* Could use a nested loop, but with small number, just went for if-else */
                if (m_hand[i].m_pip == m_pip_count[0].m_pip)
                {
                    m_pip_count[0].m_count++;
                }
                else if (m_hand[i].m_pip == m_pip_count[1].m_pip)
                {
                    m_pip_count[1].m_count++;
                }
                else if (m_hand[i].m_pip == m_pip_count[2].m_pip)
                {
                    m_pip_count[2].m_count++;
                }
                else if (m_hand[i].m_pip == m_pip_count[3].m_pip)
                {
                    m_pip_count[3].m_count++;
                }
                else if (m_hand[i].m_pip == m_pip_count[4].m_pip)
                {
                    m_pip_count[4].m_count++;
                }
                else
                {
                    m_unique_vals++;
                    m_pip_count[m_unique_vals].m_pip = m_hand[i].m_pip;
                    m_pip_count[m_unique_vals].m_count = 1;
                }
            }
            m_unique_vals++; /* Need number of unique pips, not array index. Add one */
            Array.Sort(m_pip_count);
        }


        public Hand (string [] card_text)
        {
            int i;
            char c, s;
            m_hand = new Card[Global.hand_size];
            m_pip_count = new Pip_Count[Global.hand_size];

            /* foreach (string [] card_string in card_text) */
            /* Take each pip/suit pair and make a card. */
            for (i = 0; i < Global.hand_size; i++)
            {
                c = card_text[i][0];
                s = card_text[i][1];
                m_pip_count[i] = new Pip_Count();
                m_hand [i] = new Card(c, s);
                
            }
            Array.Sort(m_hand);
            Count_Pips();
            m_type_of_hand = Type_of_hand();
        }

        private bool Straight()
        {
            /* m_hand is assumed to be sorted from highest pip to lowest. Therefore, can
             * determine if it is a straight by making sure the next card is one less than
             * the previous one.
             */
            int i, last_card;

            last_card = m_hand[0].m_pip;
            // For the rest of the card make sure it is one less than the previous card
            for (i = 1; i < Global.hand_size; i++)
            {
                if ((last_card - 1) == m_hand[i].m_pip)
                    // Set last_card to be the latest card checked
                    last_card = m_hand[i].m_pip;
                else
                    return false;
            }
            return true;
        }

        private bool Flush()
        {
            int i;
            Card_Suits suit;

            // Set suit to that of the first card;
            suit = m_hand[0].m_suit;
            // Check suit of each remaining card compared to first card
            for (i = 0; i < Global.hand_size; i++)
            {
                if (suit != m_hand[i].m_suit)
                    return false;
            }
            return true;
        }

        private bool Straight_Flush()
        {
            // Calling the Straight() and Flush() methods. Could save computation
            // by using a combined method, but this is clearer.
            return Straight() && Flush();
        }

        public Hand_Types Type_of_hand()
        {
            // Determine what type of poker hand this is. Leverages the
            // Straight(), Flush(), Straight_Flush() methods, as well as
            // the pip_count structure and m_unique_vals

            // If there are five unique values, then it is one of straight, flush,
            // straight flush, or just high card.
            if (m_unique_vals == Global.hand_size)
            {
                if (Straight_Flush())
                    return Hand_Types.Straight_Flush;
                else if (Flush())
                    return Hand_Types.Flush;
                else if (Straight())
                    return Hand_Types.Straight;
                else
                    return Hand_Types.High_Card;
            }
            else if (m_unique_vals == 4) // Can only be one pair
                return Hand_Types.One_Pair;
            else if (m_unique_vals == 3) // Can be three of a kind or two pair
            {
                if (m_pip_count[0].m_count == 3) // If pip occurs three times, must be three of a kind
                    return Hand_Types.Three_Kind;
                else
                    return Hand_Types.Two_Pair;
            }
            else if (m_unique_vals == 2) // Either four of a kind, or a full house (three and two)
                if (m_pip_count[0].m_count == 4)
                    return Hand_Types.Four_Kind;
                else
                    return Hand_Types.Full_House;
            else // Only one unique value - Can't be five of a kind without wild cards
            {
                Console.WriteLine("Bad value for number of unique values; {0}", m_unique_vals);
                throw new ArgumentOutOfRangeException("Bad value for m_unique_vals");
            }
        }
        
        public void Write_Hand()
        {
            int i;
            for (i = 0; i < Global.hand_size; i++)
            {
                Console.WriteLine("\t{0} {1} - Pip {2}, Count {3}", m_hand[i].m_pip, m_hand[i].m_suit,
                                    m_pip_count[i].m_pip, m_pip_count[i].m_count);
            }
            Console.WriteLine("Hand_type = {0}", Type_of_hand());
        } 
    }

    class Test
    {
        public static int Two_Pair_Tie_Breaker(Hand hand1, Hand hand2)
        {
            // Determine the winner when both hands have two pair. Return 1 or 2.
            // It's complicated enough to need its own method.
            if (hand1.m_pip_count[0].m_pip > hand2.m_pip_count[0].m_pip)
                return 1;
            else if (hand1.m_pip_count[0].m_pip < hand2.m_pip_count[0].m_pip)
                return 2;
            // Top pair is equal. Check second pair.
            else if (hand1.m_pip_count[1].m_pip > hand2.m_pip_count[1].m_pip)
                return 1;
            else if (hand1.m_pip_count[1].m_pip < hand2.m_pip_count[1].m_pip)
                return 2;
            // Both pairs are equal. Check the fifth card (third in pip array)
            else if (hand1.m_pip_count[2].m_pip > hand2.m_pip_count[2].m_pip)
                return 1;
            else if (hand1.m_pip_count[2].m_pip < hand2.m_pip_count[2].m_pip)
                return 2;
            else
            {
                Console.WriteLine("All cards match in two pairs case. Illegal input");
                throw new System.Data.ConstraintException();
            }
        }

        public static int One_Pair_Tie_Breaker(Hand hand1, Hand hand2)
        {
            // Determine the winner when both hands have one pair. Return 1 or 2.
            // It's complicated enough to need its own method.

            int i;

            if (hand1.m_pip_count[0].m_pip > hand2.m_pip_count[0].m_pip)
                return 1;
            else if (hand1.m_pip_count[0].m_pip < hand2.m_pip_count[0].m_pip)
                return 2;
            // Pair has the same value. Check highest of each hand one by one until mismatch
            else
            {
                for (i = 0; i < Global.hand_size; i++)
                    if (hand1.m_hand[i].m_pip > hand2.m_hand[i].m_pip)
                        return 1;
                    else if (hand1.m_hand[i].m_pip < hand2.m_hand[i].m_pip)
                        return 2;
            }
            // All cards equal. Error condition.
            Console.WriteLine("All cards match in one pair case. Illegal input");
            throw new System.Data.ConstraintException();
        }

        public static int High_Card_Tie_Breaker(Hand hand1, Hand hand2)
        {
            // Determine the winner when both hands have just high card.
            // Has enough steps to need its own method.
            int i;

            for (i = 0; i < Global.hand_size; i++)
                if (hand1.m_hand[i].m_pip > hand2.m_hand[i].m_pip)
                    return 1;
                else if (hand1.m_hand[i].m_pip < hand2.m_hand[i].m_pip)
                    return 2;
            // All cards equal. Error condition.
            Console.WriteLine("All cards match in one pair case. Illegal input");
            throw new System.Data.ConstraintException();
        }


        public static int Tie_Breaker(Hand hand1, Hand hand2)
        {
            // Determine the winner when both hands are the same type.
            if ((hand1.m_type_of_hand == Hand_Types.Straight_Flush) ||
                (hand1.m_type_of_hand == Hand_Types.Flush) || (hand1.m_type_of_hand == Hand_Types.Straight))
            {
                // Can determine by comparing the highest card in each hand
                if (hand1.m_hand[0].m_pip > hand2.m_hand[0].m_pip)
                    return 1;
                else if (hand1.m_hand[0].m_pip < hand2.m_hand[0].m_pip)
                    return 2;
                else
                {
                    Console.WriteLine("High cards match in {0} case. Illegal input", hand1.m_type_of_hand);
                    throw new System.Data.ConstraintException();
                }
            }
            else if (hand1.m_type_of_hand == Hand_Types.Four_Kind)
            {
                // Card values in case of four of a kind are unique. Just compare them.
                if (hand1.m_hand[0].m_pip > hand2.m_hand[0].m_pip)
                    return 1;
                else if (hand1.m_hand[0].m_pip < hand2.m_hand[0].m_pip)
                    return 2;
                else
                {
                    Console.WriteLine("Four of kind cards match. Illegal input");
                    throw new System.Data.ConstraintException();
                }
            }
            else if ((hand1.m_type_of_hand == Hand_Types.Three_Kind) ||
                     (hand1.m_type_of_hand == Hand_Types.Full_House))
            {
                // Can determine winner by comparing values of three of a kind, which must be different.
                if (hand1.m_hand[0].m_pip > hand2.m_hand[0].m_pip)
                    return 1;
                else if (hand1.m_hand[0].m_pip < hand2.m_hand[0].m_pip)
                    return 2;
                else
                {
                    Console.WriteLine("Three of kind cards match for {0} hand. Illegal input", hand1.m_type_of_hand);
                    throw new System.Data.ConstraintException();
                }
            }
            else if (hand1.m_type_of_hand == Hand_Types.Two_Pair)
                return Two_Pair_Tie_Breaker(hand1, hand2);
            else if (hand1.m_type_of_hand == Hand_Types.One_Pair)
                return One_Pair_Tie_Breaker(hand1, hand2);
            else if (hand1.m_type_of_hand == Hand_Types.High_Card)
                return High_Card_Tie_Breaker(hand1, hand2);
            else
            {
                // Bad hand type value
                Console.WriteLine("Bad hand type");
                throw new System.Data.ConstraintException();
            }
        }

        public static void Main()
        {
            int hand1_wins;
            string [] hand1_string;
            string [] hand2_string;

            string [] allPokerLines = File.ReadAllLines ("C:/Users/Noah/Documents/GitHub/P54_Poker/poker.txt");
            // Catch a bad file name exception
            hand1_wins = 0;
            foreach (string line_string in allPokerLines)
            {
                hand1_string = line_string.Substring(0, 3 * Global.hand_size).Split(' ');
                hand2_string = line_string.Substring(3 * Global.hand_size).Split(' ');
                Hand hand1 = new Hand(hand1_string);
                Hand hand2 = new Hand(hand2_string);
                // hand2.Write_Hand();
                if (hand1.m_type_of_hand > hand2.m_type_of_hand)
                    hand1_wins++;
                else if (hand1.m_type_of_hand == hand2.m_type_of_hand)
                {
                    if (Tie_Breaker(hand1, hand2) == 1)
                    {
                        Console.WriteLine("Hand 1 wins.");
                        hand1_wins++;
                    }
                }
            }
            Console.WriteLine("Hand 1 wins {0} times", hand1_wins);
            Console.Write("\nPress any key you want to continue...");
            Console.ReadLine();
        }
    }
}


