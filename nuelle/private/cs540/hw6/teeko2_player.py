import random
import copy
import math

class Teeko2Player:
    """ An object representation for an AI game player for the game Teeko2.
    """
    board = [[' ' for j in range(5)] for i in range(5)]
    pieces = ['b', 'r']
    max_depth = 4
    # drop_phase = True

    def __init__(self):
        """ Initializes a Teeko2Player object by randomly selecting red or black as its
        piece color.
        """
        self.my_piece = random.choice(self.pieces)
        self.opp = self.pieces[0] if self.my_piece == self.pieces[1] else self.pieces[1]

    def succ(self, state, drop_phase):
        succ_states = []
        copy_state = copy.deepcopy(state)
        num_pieces = 0

        if not drop_phase: # Can only move adjacent pieces u,d,l,r,diagonal
            for row in range(5):
                for col in range(5):
                     if state[row][col] == self.my_piece:
                         # Generate all the successor boards and add them to a list
                        # placer = copy.copy(copy_state)
                        # print('at state[', row, '][', col, ']')
                        new_board = copy.deepcopy(copy_state)
                        if row + 1 < 5:
                            if col + 1 < 5: # bottom right diag
                                if state[row+1][col+1] == ' ':
                                    new_board[row+1][col+1] = self.my_piece
                                    new_board[row][col] = ' '
                                    succ_states.append(new_board)
                                    # for i in range(5):
                                    #     print(new_board[i]) 
                                    # print('\n')
                                    new_board = copy.deepcopy(copy_state)
                            if col - 1 >= 0: # bottom left diag
                                if state[row+1][col-1] == ' ':
                                    new_board[row+1][col-1] = self.my_piece
                                    new_board[row][col] = ' '
                                    succ_states.append(new_board)
                                    # for i in range(5):
                                    #     print(new_board[i]) 
                                    # print('\n')
                                    new_board = copy.deepcopy(copy_state)
                            if state[row+1][col] == ' ': # down
                                new_board[row+1][col] = self.my_piece
                                new_board[row][col] = ' '
                                succ_states.append(new_board)
                                # for i in range(5):
                                #     print(new_board[i]) 
                                # print('\n')
                                new_board = copy.deepcopy(copy_state)
                        if row - 1 >= 0:
                            if col + 1 < 5: # top right diag
                                if state[row-1][col+1] == ' ':
                                    new_board[row-1][col+1] = self.my_piece
                                    new_board[row][col] = ' '
                                    succ_states.append(new_board)
                                    # for i in range(5):
                                    #     print(new_board[i]) 
                                    # print('\n')
                                    new_board = copy.deepcopy(copy_state)
                            if col - 1 >= 0: # top left diag
                                if state[row-1][col-1] == ' ':
                                    new_board[row-1][col-1] = self.my_piece
                                    new_board[row][col] = ' '
                                    succ_states.append(new_board)
                                    # for i in range(5):
                                    #     print(new_board[i]) 
                                    # print('\n')
                                    new_board = copy.deepcopy(copy_state)
                            if state[row-1][col] == ' ': # up
                                new_board[row-1][col] = self.my_piece
                                new_board[row][col] = ' '
                                succ_states.append(new_board)
                                # for i in range(5):
                                #     print(new_board[i]) 
                                # print('\n')
                                new_board = copy.deepcopy(copy_state)
                        if col + 1 < 5: # right
                            if state[row][col+1] == ' ':
                                new_board[row][col+1] = self.my_piece
                                new_board[row][col] = ' '
                                succ_states.append(new_board)
                                # for i in range(5):
                                #     print(new_board[i]) 
                                # print('\n')
                                new_board = copy.deepcopy(copy_state)
                        if col - 1 >= 0: # left
                            if state[row][col-1] == ' ':
                                new_board[row][col-1] = self.my_piece
                                new_board[row][col] = ' '
                                succ_states.append(new_board)
                                # for i in range(5):
                                #     print(new_board[i])
                                # print('\n') 
                                new_board = copy.deepcopy(copy_state)
                        #  succ_states = self.add_succs(copy_state, row, col, succ_states)

        else: # in drop phase, can add a piece any place where there is not already a piece placed there
            for row in range(5):
                for col in range(5):
                     if state[row][col] == ' ':
                        copy_state[row][col] = self.my_piece
                        succ_states.append(copy_state)
                        copy_state = copy.deepcopy(state)
        
        return succ_states

    def make_move(self, state):
        """ Selects a (row, col) space for the next move. You may assume that whenever
        this function is called, it is this player's turn to move.

        Args:
            state (list of lists): should be the current state of the game as saved in
                this Teeko2Player object. Note that this is NOT assumed to be a copy of
                the game state and should NOT be modified within this method (use
                place_piece() instead). Any modifications (e.g. to generate successors)
                should be done on a deep copy of the state.

                In the "drop phase", the state will contain less than 8 elements which
                are not ' ' (a single space character).

        Return:
            move (list): a list of move tuples such that its format is
                    [(row, col), (source_row, source_col)]
                where the (row, col) tuple is the location to place a piece and the
                optional (source_row, source_col) tuple contains the location of the
                piece the AI plans to relocate (for moves after the drop phase). In
                the drop phase, this list should contain ONLY THE FIRST tuple.

        Note that without drop phase behavior, the AI will just keep placing new markers
            and will eventually take over the board. This is not a valid strategy and
            will earn you no points.
        """
        num_pieces = 0
        drop_phase = True
        succs = []

        for row in range(5):
            for col in range(5):
                if state[row][col] != ' ':
                    num_pieces += 1

        # TODO: detect drop phase
        if num_pieces >= 8:
            drop_phase = False   
        
        # if not drop_phase:
        #     # TODO: choose a piece to move and remove it from the board
        #     # (You may move this condition anywhere, just be sure to handle it)
        #     #
        #     # Until this part is implemented and the move list is updated
        #     # accordingly, the AI will not follow the rules after the drop phase!
        #     succs = self.succ(state, drop_phase)
        # else:
        #     succs = self.succ(state, drop_phase)
        # select an unoccupied space randomly
        # TODO: implement a minimax algorithm to play better
        alpha = float('-inf')
        beta = float('inf')
        (found_move, found_h) = self.max_value(state, 0, drop_phase, alpha, beta)
        # print('found move', found_move, 'found h', found_h)
        move = []
        found_row = None
        found_col = None
        orig_r = None
        orig_c = None
        if drop_phase:
            for row in range(5):
                for col in range(5):
                    # print(found_move[row][col])
                    if found_move[row][col] == self.my_piece and state[row][col] == ' ':
                        # print("here", row, col)
                        found_row = row
                        found_col = col
            # print("move is", found_row, found_col)
            # ensure the destination (row,col) tuple is at the beginning of the move list
            # move.insert(0, (row, col))
            move.insert(0, (found_row, found_col))
        else: 
            for row in range(5):
                for col in range(5):
                    # print(found_move[row][col])
                    if found_move[row][col] == self.my_piece and state[row][col] == ' ':
                        # print("here", row, col)
                        found_row = row
                        found_col = col
                    if state[row][col] == self.my_piece and found_move[row][col] == ' ':
                        # print("here", row, col)
                        orig_r = row
                        orig_c = col
            # print("move is", found_row, found_col)
            # ensure the destination (row,col) tuple is at the beginning of the move list
            # move.insert(0, (row, col))
            move.insert(0, (found_row, found_col))
            move.insert(1, (orig_r, orig_c))

        return move

    def opponent_move(self, move):
        """ Validates the opponent's next move against the internal board representation.
        You don't need to touch this code.

        Args:
            move (list): a list of move tuples such that its format is
                    [(row, col), (source_row, source_col)]
                where the (row, col) tuple is the location to place a piece and the
                optional (source_row, source_col) tuple contains the location of the
                piece the AI plans to relocate (for moves after the drop phase). In
                the drop phase, this list should contain ONLY THE FIRST tuple.
        """
        # validate input
        if len(move) > 1:
            source_row = move[1][0]
            source_col = move[1][1]
            if source_row != None and self.board[source_row][source_col] != self.opp:
                self.print_board()
                print(move)
                raise Exception("You don't have a piece there!")
            if abs(source_row - move[0][0]) > 1 or abs(source_col - move[0][1]) > 1:
                self.print_board()
                print(move)
                raise Exception('Illegal move: Can only move to an adjacent space')
        if self.board[move[0][0]][move[0][1]] != ' ':
            raise Exception("Illegal move detected")
        # make move
        self.place_piece(move, self.opp)

    def place_piece(self, move, piece):
        """ Modifies the board representation using the specified move and piece

        Args:
            move (list): a list of move tuples such that its format is
                    [(row, col), (source_row, source_col)]
                where the (row, col) tuple is the location to place a piece and the
                optional (source_row, source_col) tuple contains the location of the
                piece the AI plans to relocate (for moves after the drop phase). In
                the drop phase, this list should contain ONLY THE FIRST tuple.

                This argument is assumed to have been validated before this method
                is called.
            piece (str): the piece ('b' or 'r') to place on the board
        """
        if len(move) > 1:
            self.board[move[1][0]][move[1][1]] = ' '
        self.board[move[0][0]][move[0][1]] = piece

    def print_board(self):
        """ Formatted printing for the board """
        for row in range(len(self.board)):
            line = str(row)+": "
            for cell in self.board[row]:
                line += cell + " "
            print(line)
        print("   A B C D E")

    def game_value(self, state):
        """ Checks the current board status for a win condition

        Args:
        state (list of lists): either the current state of the game as saved in
            this Teeko2Player object, or a generated successor state.

        Returns:
            int: 1 if this Teeko2Player wins, -1 if the opponent wins, 0 if no winner

        Completed: checks for diagonal and diamond wins
        """
        # check horizontal wins
        for row in state:
            for i in range(2):
                if row[i] != ' ' and row[i] == row[i+1] == row[i+2] == row[i+3]:
                    return 1 if row[i]==self.my_piece else -1

        # check vertical wins
        for col in range(5):
            for i in range(2):
                if state[i][col] != ' ' and state[i][col] == state[i+1][col] == state[i+2][col] == state[i+3][col]:
                    return 1 if state[i][col]==self.my_piece else -1

        # check / diagonal wins
        check_back = 3
        for check_back in range(5):
            for i in range(2):
                if state[i][check_back] != ' ' and state[i][check_back] == state[i+1][check_back-1] == state[i+2][check_back-2] == state[i+3][check_back-3]:
                    return 1 if state[i][check_back]==self.my_piece else -1
        # check \ diagonal wins
        for check_four in range(2):
            for i in range(2):
                if state[check_four][i] != ' ' and state[check_four][i] == state[check_four+1][i+1] == state[check_four+2][i+2] == state[check_four+3][i+3]:
                    return 1 if state[check_four][i]==self.my_piece else -1

        # check diamond wins
        diamond = 1
        i = 1
        for diamond in range(4):
            for i in range(4):
                if state[diamond][i] == ' ' and state[diamond-1][i] != ' ' and state[diamond-1][i] == state[diamond][i+1] == state[diamond+1][i] == state[diamond][i-1]:
                    return 1 if state[diamond-1][i]==self.my_piece else -1


        return 0 # no winner yet

    # A terminal state where your AI player wins should have the maximal positive score (1), 
    # and a terminal state where the opponent wins should have the minimal negative score (-1).
    def heuristic_game_value(self, state):
        # succs = self.succ(state, None)
        # potential_board = state
        # Evaluate non-terminal states, ie states that have returned 0
        # for each_succ in succs:
        found_pieces = 0
        # print(self.game_value(state))
        if self.game_value(state) == 0:
            # calculate heuristic as how many pieces are surrounding a piece. Whichever one is > is better. 
            side_total = 0
            cur_piece = 1
            smt = 0
            sot = 0
            for row in state:
                for i in range(1):
                    if row[i] != ' ':
                        if row[i] == self.my_piece:
                            smt = 0.1
                            if row[i] == row[i+1]:
                                smt = 0.3
                                if row[i] == row[i+2]:
                                    smt = 0.9
                        elif row[i+1] == self.my_piece:
                            smt = 0.1
                            if row[i+1] == row[i+2]:
                                smt = 0.3
                                if row[i+1] == row[i+3]:
                                    smt = 0.9
                        elif row[i+2] == self.my_piece:
                            smt = 0.1
                            if row[i+2] == row[i+3]:
                                smt = 0.3
                                if row[i+2] == row[i+4]:
                                    smt = 0.9
                        if row[i] != self.my_piece:
                            sot = 0.1
                            if row[i] == row[i+1]:
                                sot = 0.3
                                if row[i] == row[i+2]:
                                    sot = 0.9
                        elif row[i+1] != self.my_piece:
                            sot = 0.1
                            if row[i+1] == row[i+2]:
                                sot = 0.3
                                if row[i+1] == row[i+3]:
                                    sot = 0.9
                        elif row[i+2] != self.my_piece:
                            sot = 0.1
                            if row[i+2] == row[i+3]:
                                sot = 0.3
                                if row[i+2] == row[i+4]:
                                    sot = 0.9
            updown_total = 0
            my_total = 0
            opp_total = 0
            for col in range(5):
                for i in range(1):
                    if state[i][col] != ' ':
                        if state[i][col] == self.my_piece:
                            my_total = 0.1
                            if state[i][col] == state[i+1][col]:
                                my_total= 0.3
                                if state[i][col] == state[i+2][col]:
                                    my_total = 0.9
                        elif state[i+1][col] == self.my_piece:
                            my_total = 0.1
                            if state[i+1][col] == state[i+2][col]:
                                my_total = 0.3
                                if state[i+1][col] == state[i+3][col]:
                                    my_total = 0.9
                        elif state[i+2][col] == self.my_piece:
                            my_total = 0.1
                            if state[i+2][col] == state[i+3][col]:
                                my_total = 0.3
                                if state[i+1][col] == state[i+4][col]:
                                    my_total = 0.9
                        if state[i][col] != self.my_piece:
                            opp_total = 0.1
                            if state[i][col] == state[i+1][col]:
                                opp_total= 0.3
                                if state[i][col] == state[i+2][col]:
                                    opp_total = 0.9
                        elif state[i+1][col] != self.my_piece:
                            opp_total = 0.1
                            if state[i+1][col] == state[i+2][col]:
                                opp_total = 0.3
                                if state[i+1][col] == state[i+3][col]:
                                    opp_total = 0.9
                        elif state[i+2][col] != self.my_piece:
                            opp_total = 0.1
                            if state[i+2][col] == state[i+3][col]:
                                opp_total = 0.3
                                if state[i+1][col] == state[i+4][col]:
                                    opp_total = 0.9
            found_pieces = max(my_total, smt) - max(opp_total, sot) 
        return found_pieces

    def max_value(self, state, depth, drop_phase, alpha, beta):
        found_s = None
        # print('depth', depth, 'game value state', self.game_value(state))
        if depth == self.max_depth or self.game_value(state) == 1 or self.game_value(state) == -1:
            return (state, self.heuristic_game_value(state))
        else:
            for s in self.succ(state, drop_phase):
                # for i in range(5):
                #     print(s[i])
                (nextS, nextH) = self.min_value(s, depth+1, drop_phase, alpha, beta)
                # print('max nextH is ', nextH)
                if nextH > alpha:
                    alpha = nextH
                    found_s = s
                if nextH >= beta:
                    return(state, beta)
        return found_s, alpha

    def min_value(self, state, depth, drop_phase, alpha, beta): 
        found_s = None
        # print('depth', depth, 'game value state', self.game_value(state))
        if depth == self.max_depth or self.game_value(state) == 1 or self.game_value(state) == -1:
            # print('REACHED END', state)
            return (state, self.heuristic_game_value(state))
        else:
            for s in self.succ(state, drop_phase):
                # for i in range(5):
                #     print(s[i])
                (nextS, nextH) = self.max_value(s, depth+1, drop_phase, alpha, beta)
                # print('min nextH is ', nextH)
                if nextH < beta:
                    beta = nextH
                    found_s = s
                if alpha >=  nextH:
                    return(state, alpha)
        return found_s, beta 

############################################################################
#
# THE FOLLOWING CODE IS FOR SAMPLE GAMEPLAY ONLY
#
############################################################################
def main():
    print('Hello, this is Samaritan')
    ai = Teeko2Player()
    piece_count = 0
    turn = 0

    # drop phase
    while piece_count < 8 and ai.game_value(ai.board) == 0:

        # get the player or AI's move
        if ai.my_piece == ai.pieces[turn]:
            ai.print_board()
            move = ai.make_move(ai.board)
            ai.place_piece(move, ai.my_piece)
            print(ai.my_piece+" moved at "+chr(move[0][1]+ord("A"))+str(move[0][0]))
        else:
            move_made = False
            ai.print_board()
            print(ai.opp+"'s turn")
            while not move_made:
                player_move = input("Move (e.g. B3): ")
                while player_move[0] not in "ABCDE" or player_move[1] not in "01234":
                    player_move = input("Move (e.g. B3): ")
                try:
                    ai.opponent_move([(int(player_move[1]), ord(player_move[0])-ord("A"))])
                    move_made = True
                except Exception as e:
                    print(e)

        # update the game variables
        piece_count += 1
        turn += 1
        turn %= 2

    # move phase - can't have a winner until all 8 pieces are on the board
    while ai.game_value(ai.board) == 0:

        # get the player or AI's move
        if ai.my_piece == ai.pieces[turn]:
            ai.print_board()
            move = ai.make_move(ai.board)
            ai.place_piece(move, ai.my_piece)
            print(ai.my_piece+" moved from "+chr(move[1][1]+ord("A"))+str(move[1][0]))
            print("  to "+chr(move[0][1]+ord("A"))+str(move[0][0]))
        else:
            move_made = False
            ai.print_board()
            print(ai.opp+"'s turn")
            while not move_made:
                move_from = input("Move from (e.g. B3): ")
                while move_from[0] not in "ABCDE" or move_from[1] not in "01234":
                    move_from = input("Move from (e.g. B3): ")
                move_to = input("Move to (e.g. B3): ")
                while move_to[0] not in "ABCDE" or move_to[1] not in "01234":
                    move_to = input("Move to (e.g. B3): ")
                try:
                    ai.opponent_move([(int(move_to[1]), ord(move_to[0])-ord("A")),
                                    (int(move_from[1]), ord(move_from[0])-ord("A"))])
                    move_made = True
                except Exception as e:
                    print(e)

        # update the game variables
        turn += 1
        turn %= 2

    ai.print_board()
    if ai.game_value(ai.board) == 1:
        print("AI wins! Game over.")
    else:
        print("You win! Game over.")


if __name__ == "__main__":
    main()
