# FPS_SI
This is a simple prototype in UE 5.3 from mid 2023 which I made in ~20h 

Make a first person shooter prototype which includes: 
1. Player with basic movement and one weapon.
2. Few enemy types with their own weapons and behaviors (see below). Tech stack: Sight perception, Behavior Trees, EQS, Gameplay Ability System for attack abilities and probably for some gameplay effects or attributes. 
3. Simple endless spawn logic. Enemies: 
	1. Ranged. This enemy keeps a distance to the player with a rule: not too close and not too far away. Periodically changes position in a small radius and can take a pre-defined 1m cover spot (with crouching anim state). He has a need to change cover if he was flanked by the Player. 
	2. Melee. This enemies charge the player aggressively, attempting to close the gap and attack with melee weapons 
	3. Ranged medic. 1st priority - instant reviving of dead allies if some of them in a specific radius, 2nd priority - regular ranged behavior. Any enemy might be revived only once. 
	
Important notes: - Only C++ is allowed for core logics - Usage of marketplace plugins with gameplay logics or copy pasted code from tutorials is not allowed. 
Exception: Unreal template projects. - Visual part is not importatnt.
