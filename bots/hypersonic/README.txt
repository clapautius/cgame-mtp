

This is a league based challenge.
For this challenge, multiple versions of the same game are available. Once you have proven your skills on this first version, you will access a higher league and extra rules will be unlocked.
  The Goal
Destroy more boxes than your opponent with your bombs in this Bomberman-inspired game.
  Rules
The game is played on a grid 13 units wide and 11 units high. The coordinate X=0, Y=0 is the top left cell.

For now, the game is played with 2 players only. Each player starts out on a corner of the map.
All actions players take are computed simultaneously.

Boxes are scattered across the grid, and can be destroyed by bombs.

The map works as follows:

    Each cell of the grid is either a floor or a box. Floor cells are indicated by a dot (.), and boxes by a zero (0).
    Floor cells may be occupied by any number of players.
    A random amount of boxes between 30 and 65 inclusive will be placed symmetrically across the grid.

The players work as follows:

    Every turn, a player may move horizontally or vertically to an adjacent floor cell. If a bomb is already occupying that cell, the player won't be able to move there.
    Players can occupy the same cell as a bomb only when the bomb appears on the same turn as when the player enters the cell.
    Using the MOVE command followed by grid coordinates will make the player attempt to move one cell closer to those coordinates. The player will automatically compute the shortest path within the grid to get to the target point. If the given coordinates are impossible to get to, the player will instead target the valid cell closest to the given coordinates.
    Using the BOMB command followed by map coordinates will make the player attempt to place a bomb on the currently occupied cell, then move one cell closer to the given coordinates.
    Players may stay on the cell on which they place a bomb.
    Players can only have 1 bomb in the grid at one time.
    In this league, players are not hurt by bombs (they are using practice explosives).

The bombs work as follows:

    Bombs have an 8 round timer. On each subsequent round, the timer will decrease by 1. On the round where the timers hits 0, the bomb explodes.
    In this league, explosions have a range of 3, meaning they span horizontally and vertically up to 3 squares in each direction unless they encounter a box or other bomb.
    Explosions will cause the boxes they hit to be destroyed.
    Explosions will cause the bombs they hit to also explode.

After 200 rounds, the player who hit the most boxes with their bombs wins.

The game state of every round is given to you as a list of entities, each with a entityType, owner, position, param1 and param2.

The entityType will be:

    For players: 0.
    For bombs: 1.

The owner will be:

    For players: id of the player (0 or 1).
    For bombs: id of the bomb's owner.

The param1 will be:

    For players: number of bombs the player can still place.
    For bombs: number of rounds left until the bomb explodes.

The param2 is not useful for the current league, and will always be:

    For players: explosion range of the player's bombs (=3).
    For bombs: explosion range of the bomb (=3).


Victory Conditions
You are the one who blew up the most boxes.

Lose Conditions

    Your program does not respond in time.
    You provide invalid input.

  Expert Rules
Details on how a round is computed:

    At the start of the round, all bombs have their countdown decreased by 1.
    Any bomb countdown that reaches 0 will cause the bomb to explode immediately, before players move.
    Any bomb caught in an explosion is treated as if it had exploded at the very same moment.
    Explosions do not go through obstructions such as boxes or other bombs, but are included on the cells the obstruction occupies.
    A single obstruction may block the explosions of several bombs that explode on the same turn.
    Once the explosions have been computed, any box hit is then removed. This means that the destruction of 1 box can count for 2 different players.
    Players then perform their actions simultaneously.
    Any bombs placed by a player appear at the end of the round.

  Note
The program must first read initialization input and then, within an infinite loop, read the contextual data from the standard input and provide to the standard output the desired instructions.
  Game Input
Initialization input
Line 1: 3 integers, width, height and myId. width and height represent the size of the grid and will always be 13 and 11 respectively, and are given for purely for convenience. myId is your player's id.
Input for one game turn
First height lines: a string row representing each row of the grid. Each character can be:

    . an empty cell.
    0 a box.

Next line: an integer entities for the amount of entities on the grid.
Next entities lines : 6 space separated integers entityType , owner , x , y , param1 & param2 . Represents a player or a bomb.
Output for one game turn
One of the following:

    MOVE followed by two integers x and y.
    BOMB followed by two integers x and y.


You may append text to your instructions, it will be displayed in the viewer. e.g. MOVE 1 1 hypersonic.
Constraints

Response time per turn ≤ 100 ms

What is in store for me in the higher leagues?
The extra rules available in higher leagues are:

    Items will appear when certain boxes are destroyed. You will need to gather items in order to improve your bombs and increase the number you can carry.
    The grid will also contain walls. Bombs will harm the opposing player.
    Your own bombs will harm you. You will face up to 3 opponents.

_____

one league / level missing?

_____


  Summary of new rules

The grid now contains walls. Enemy bombs are now dangerous to you. Blowing up boxes should remain a priority, but you will need to navigate the grid carefully.

Please refer to the updated statement section for details.
  The Goal
Outlive your opponent.
  Rules
The game is played on a grid 13 units wide and 11 units high. The coordinate X=0, Y=0 is the top left cell.

For now, the game is played with 2 players only. Each player starts out on a corner of the map.
All actions players take are computed simultaneously.

Boxes are scattered across the grid, and can be destroyed by bombs.
A box may have an item inside. Destroying a box will make the item inside appear .

The map works as follows:

    Each cell of the grid is either a floor, a box or a wall. Floor cells are indicated by a dot (.), boxes by a positive integer and walls by a cross (X).
    Floor cells may be occupied by any number of players.
    A random amount of boxes between 30 and 65 inclusive will be placed symmetrically across the grid.

The players work as follows:

    Every turn, a player may move horizontally or vertically to an adjacent floor cell. If a bomb is already occupying that cell, the player won't be able to move there.
    Players can occupy the same cell as a bomb only when the bomb appears on the same turn as when the player enters the cell.
    Using the MOVE command followed by grid coordinates will make the player attempt to move one cell closer to those coordinates. The player will automatically compute the shortest path within the grid to get to the target point. If the given coordinates are impossible to get to, the player will instead target the valid cell closest to the given coordinates.
    Using the BOMB command followed by map coordinates will make the player attempt to place a bomb on the currently occupied cell, then move one cell closer to the given coordinates.
    Players may stay on the cell on which they place a bomb.
    At the start of the game, players can only have 1 bomb in the grid at one time.
    At the start of the game, players can only place bombs with a range of 3.
    Players are immune to their own bombs.

Bombs work as follows:

    Bombs have an 8 round timer. On each subsequent round, the timer will decrease by 1. On the round where the timers hits 0, the bomb explodes.
    Explosions have a range, meaning they span horizontally and vertically up to range squares in each direction unless they encounter a wall, box, item or other bomb.
    Explosions will cause the boxes they hit to be destroyed.
    Explosions will cause the items they hit to be destroyed.
    Explosions will cause the players they hit to be eliminated unless the exploding bomb is their own.
    Explosions will cause the bombs they hit to also explode.

Items work as follow:

    Items appear once all the explosions have been applied.
    A player may collect an item by moving onto the same cell as an item. Several players make collect the same item if they arrive simultaneously.
    There are 2 different items, each represented by an integer:
        1 extra range: the player's bombs will explode over an extra cell in every direction. Does not apply to bombs already in play.
        2 extra bomb: the player can have an additional bomb in play.

The game ends when a player is eliminated. The surviving player wins.
After 200 rounds or if both players are eliminated simultaneously, the player who hit the most boxes with their bombs wins.

The game state of every round is given to you as a list of entities, each with a entityType, owner, position, param1 and param2.

The entityType will be:

    For players: 0.
    For bombs: 1.
    For items: 2.

The owner will be:

    For players: id of the player (0 or 1).
    For bombs: id of the bomb's owner.
    For items: ignored number (=0).

The param1 will be:

    For players: number of bombs the player can still place.
    For bombs: number of rounds left until the bomb explodes.
    For items: the integer representing the item.

The param2 will be:

    For players: current explosion range of the player's bombs.
    For bombs: current explosion range of the bomb.
    For items: ignored number (=0).


Victory Conditions

    Your opponent is eliminated before you.
    You and your opponent are eliminated simultaneously and you are the one who blew up the most boxes.


Lose Conditions

    You are hit by the opponent's bomb.
    You provide incorrect instructions.

  Expert Rules
Details on how a round is computed:

    At the start of the round, all bombs have their countdown decreased by 1.
    Any bomb countdown that reaches 0 will cause the bomb to explode immediately, before players move.
    Any bomb caught in an explosion is treated as if it had exploded at the very same moment.
    Explosions do not go through obstructions such as boxes, items or other bombs, but are included on the cells the obstruction occupies.
    A single obstruction may block the explosions of several bombs that explode on the same turn.
    Once the explosions have been computed, any box hit is then removed. This means that the destruction of 1 box can count for 2 different players.
    Players then perform their actions simultaneously.
    Any bombs placed by a player appear at the end of the round.

  Note
The program must first read initialization input and then, within an infinite loop, read the contextual data from the standard input and provide to the standard output the desired instructions.
  Game Input
Initialization input
Line 1: 3 integers, width, height and myId. width and height represent the size of the grid and will always be 13 and 11 respectively, and are given for purely for convenience. myId is your player's id.
Input for one game turn
First height lines: a string row representing each row of the grid. Each character can be:

    . an empty cell.
    0 an empty box.
    A positive integer: a box containing the item represented by the integer.
    X a wall.

Next line: an integer entities for the amount of entities on the grid.
Next entities lines : 6 space separated integers entityType , owner , x , y , param1 & param2 . Represents a player, bomb or item.
Output for one game turn
One of the following:

    MOVE followed by two integers x and y.
    BOMB followed by two integers x and y.


You may append text to your instructions, it will be displayed in the viewer. e.g. MOVE 1 1 hypersonic.
Constraints

Response time per turn ≤ 100 ms

What is in store for me in the higher leagues?
The extra rules available in higher leagues are:

    Your own bombs will harm you. You will face up to 3 opponents.
