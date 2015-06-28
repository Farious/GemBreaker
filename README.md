# GemBreaker
A clone of Miniclip's ORES made purely in SDL2.0 with C++

## Resume
This game was made in the context of an challenge that was presented to me.
The challenge was to make a Miniclip's ORES with SDL2.0 and with C++ or Obj-C.

My choice simple as it, was given my background, was to take on this challenge with C++ (THE TIME FOR C++11+ HAS COME!)

This is still a work in progress as I intend to add features to it.

## Data structures
Yes. YES!

I choose the structure map, initially unordered_map, to hold the table's state.
Maybe now I would have gone with vector, still I would hit some problems (accessing an coordinate that had no values and mapping, back and forward, with the Block structure's position)
With map/unordered_map it was really simple to access the values by coordinate and to add new ones in constant time!
On the other hand I add a new problem: updating the block's position when they need to cascade down (or move sideways).

I'm thinking of, maybe, in the future re-implement the table's logic with vector and see what kind of problems I get myself into.


## TO-DO
In the near future I intened to port the project unto CMAKE to easen up the porting process.
After this step I will try to port this over to .JS with emscripten (and integrate with humblebundle's API)

## Credits
All the assets are from [Kenney NL](http://kenney.nl/)!!

The base code was STRONGLY based on [Lazy Foo's tutorials](http://lazyfoo.net/tutorials/SDL/index.php) as this was, kind of, my first encounter with SDL.
And I should say that it really did a great job of introducing the API.
