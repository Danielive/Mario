# Mario
### The game developed on the base graphics library SFML

> The developed application is a software implementation of the famous game "Mario".
>
> The goal of the game is to go through the Mushroom Kingdom and get the key, escaping or destroying the soldiers of King Coopa to open the door of the castle.
>
> The main task is to get the key and open the door of the lock. On the way to the goal, Mario collects diamonds, being careful of obstacles. The obstacles are the abyss with lava, as well as opponents who, when confronted with Mario, kill him. Mario destroys opponents by jumping on top of them (this method only destroys Gumba mushrooms, King Coopa crabs must be avoided).
>
> The game consists of 2 levels, which go successively as you progress through the level. At level 1, Mario needs to enter the castle, and further, at level 2, exit.
>
> During the passage of each level, a stopwatch is started, to record the time that the player has passed the level, and the number of diamonds collected by the player is also counted. Such diamonds at each level, the player can collect a maximum of 15. As soon as the Mario takes the key, the door of the castle opens, and, to complete the level, the player needs to get to the door of the castle.

## Description of elements in the game
* Castle <img height="32" weight="32" src="https://camo.githubusercontent.com/5a0abb28dd0c7658e61022f95d54ce50f8f0f563/68747470733a2f2f70702e757365726170692e636f6d2f633835303633322f763835303633323238352f39326339322f6e3353354f56335871774d2e6a7067"/>
     
      To complete the level
    
* Key <img src="https://camo.githubusercontent.com/7285c86a11a493ce65a18dd10c72d5bcb9eb7a69/68747470733a2f2f70702e757365726170692e636f6d2f633835303633322f763835303633323238352f39326339392f646a7841586e6d463472732e6a7067"/>

      To open the castle lock
      
* Mario <img src="https://camo.githubusercontent.com/79b62aed351da2c9b1d305efab3f5043e2480441/68747470733a2f2f70702e757365726170692e636f6d2f633835303633322f763835303633323238352f39326361302f6362612d6536597646304d2e6a7067"/>

      The main character
      
* Lava <img src="https://camo.githubusercontent.com/e98d0e44b94437883fc57b9c47143b4a97326dfc/68747470733a2f2f70702e757365726170692e636f6d2f633835303633322f763835303633323238352f39326361372f6956656e347575306347672e6a7067"/>

      The interaction of the Mario with lava kill him
      
* Gumba mushroom <img src="https://camo.githubusercontent.com/5b14905fee2b92391b64b76b9d300f161e04ca30/68747470733a2f2f70702e757365726170692e636f6d2f633835303633322f763835303633323238352f39326361652f70323134444152452d76732e6a7067"/>

      Moves horizontally in different directions. Tries to destroy the Mario.
      
* King Coopa crab <img src="https://camo.githubusercontent.com/80d1c1abbf285503fce155a28d2ea66afbf8cc32/68747470733a2f2f70702e757365726170692e636f6d2f633835303633322f763835303633323238352f39326362632f314d4c33796a384155414d2e6a7067"/>

      Moves vertically. Attacks a Mario only at a certain distance.

## Gameplay control keys
* `↑` – up
* `↓` – down
* `←` – left
* `→` – right
* `Esc` – enable / disable menu

## Brief demonstration of the game
### Beginning of the Level 1
<img src="https://camo.githubusercontent.com/620b7b76acd1b1a9a4095d7c3abd6985cf6d5f7c/68747470733a2f2f70702e757365726170692e636f6d2f633835303633322f763835303633323238352f39326334392f4c534236524531483051552e6a7067"/>

### Getting the key
<img src="https://camo.githubusercontent.com/b57b7bf76f9ed4911451cb8b88e9c882efa782c3/68747470733a2f2f70702e757365726170692e636f6d2f633835303633322f763835303633323238352f39326335302f735a535972656b663853512e6a7067"/>

### Beginning of the next Level 2
<img src="https://camo.githubusercontent.com/fa67846703a7daf2317b747e1d456184b71fc8b5/68747470733a2f2f70702e757365726170692e636f6d2f633835303633322f763835303633323238352f39326335372f795245455f75537531356b2e6a7067"/>

### End of the game
<img src="https://camo.githubusercontent.com/a0f90ecdc7bd5b910cca6eafb8647dafe14ccd3b/68747470733a2f2f70702e757365726170692e636f6d2f633835303633322f763835303633323238352f39326334322f48315350745468456e58452e6a7067"/>
