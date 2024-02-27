from enum import Enum
from typing import Self
from functools import reduce
from operator import add

TileState = Enum("TileState", ["Empty", "Vert", "Hor", "Cross"])
TileStateChar = {
    TileState.Empty : " ",
    TileState.Vert  : "|",
    TileState.Hor   : "-",
    TileState.Cross : "X"
}

CharTileState = {
    " " : TileState.Empty,
    "|" : TileState.Vert,
    "-" : TileState.Hor,
    "X" : TileState.Cross
}

class GameState():
    PLAYER0_MOV = TileState.Hor
    PLAYER1_MOV = TileState.Vert

    def __init__(self, lastPlayer : int, lastTile : tuple[int,int] | None, state : list[TileState] | None = None) -> None:
        assert((state == None and lastTile == None) or (state != None and lastTile != None))

        self.state : list[list[TileState]] = state if state != None else [[TileState.Empty for _ in range(3)] for _ in range(3)]
        self.lastPlayer : int = lastPlayer
        self.lastTile : tuple[int, int] = lastTile
        
        boardStr = self.__board_to_string()
        playerStr = f"{lastPlayer}," + f"{lastTile[0]},{lastTile[1]}," if lastTile != None else "~,"
        self.id = f"{playerStr}{boardStr}"


    def __board_to_string(self):
        return reduce(add, map(lambda s : TileStateChar[s], reduce(add, self.state, [])), "")


    def state_from_play(self, pos : tuple[int, int]) -> Self:
        tMov : TileState = GameState.PLAYER0_MOV if self.lastPlayer == 1 else GameState.PLAYER1_MOV
        cTile : TileState = self.state[pos[1]][pos[0]]
        nGS = [[self.state[y][x] for x in range(3)] for y in range(3)]

        if cTile == TileState.Empty or ():
            nGS[pos[1]][pos[0]] = tMov
        elif cTile != tMov and cTile != TileState.Cross:
            nGS[pos[1]][pos[0]] = TileState.Cross
        else:
            raise Exception("Jugada invalida.")
        
        return GameState((self.lastPlayer + 1) % 2, pos, nGS)


    def can_play(self, pos : tuple[int, int]) -> bool:
        cTile : TileState = self.state[pos[1]][pos[0]]
        return pos != self.lastTile and (cTile == TileState.Empty or (cTile == (TileState.Vert if self.lastPlayer == 1 else TileState.Hor)))
    

    def get_possible_moves(self) -> list[tuple[int,int]]:
        return list(filter(self.can_play, [(x,y) for x in range(3) for y in range(3)]))
    

    def get_possible_states(self) -> list[Self]:
        if self.is_winning_state():
            return []
        
        return list(map(self.state_from_play, self.get_possible_moves()))
    

    def is_winning_state(self) -> bool:
        for r in range(3):
            if any([TileState.Cross != t for t in self.state[r]]):
                continue
            return True
            
        for c in range(3):
            if any([TileState.Cross != self.state[r][c] for r in range(3)]):
                continue
            return True
        
        return (self.state[0][0].value + self.state[1][1].value + self.state[2][2].value == TileState.Cross.value * 3) or \
               (self.state[0][2].value + self.state[1][1].value + self.state[2][0].value == TileState.Cross.value * 3)
    

    def print_state(self):
        for r in self.state:
            print("|",end="")
            for t in r:
                print(f"{TileStateChar[t]}|", end="")
            print()


    def board_from_str(board : str):
        return list(map(lambda r : map(lambda t : CharTileState[t], r), [board[0:3], board[3:6], board[6:]]))


    def state_from_str(enc : str):
        gs = enc.split(",")
        if len(gs) == 2:
            return GameState(int(gs[0]), None, None)
        
        return GameState(int(gs[0]), (int(gs[1]), int(gs[2])), GameState.board_from_str(enc))


def minmax(state : GameState, maximizing : bool, tTable : dict[str, int] = {}):
    if state.is_winning_state():
            val = 10 if state.lastPlayer == 0 else -10
            tTable[state.id] = val
            return val
    
    if maximizing:
        val = -9999
        for ps in state.get_possible_states():
            val = max(val, minmax(ps, False, tTable) if ps.id not in tTable else tTable[ps.id])
        
        tTable[state.id] = val if state.id not in tTable else max(val, tTable[state.id])
        return val
    
    val = 9999
    for ps in state.get_possible_states():
        val = min(val, minmax(ps, True, tTable) if ps.id not in tTable else tTable[ps.id])

    tTable[state.id] = val if state.id not in tTable else min(val, tTable[state.id])
    return val


iniS = GameState(1, None, None)

tTable = {}
minmax(iniS, True, tTable=tTable)
print("Valor del estado inicial: " + tTable[iniS.id])

