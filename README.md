# ATM
Requirements
At least the following flow should be implemented:

Insert Card => PIN number => Select Account => See Balance/Deposit/Withdraw



For simplification, there are only 1 dollar bills in this world, no cents. Thus account balance can be represented in integer.



Your code doesn't need to integrate with a real bank system, but keep in mind that we may want to integrate it with a real bank system in the future. It doesn't have to integrate with a real cash bin in the ATM, but keep in mind that we'd want to integrate with that in the future. And even if we integrate it with them, we'd like to test our code. Implementing bank integration and ATM hardware like cash bin and card reader is not a scope of this task, but testing the controller part (not including bank system, cash bin etc) is within the scope.



A bank API wouldn't give the ATM the PIN number, but it can tell you if the PIN number is correct or not.



Based on your work, another engineer should be able to implement the user interface. You don't need to implement any REST API, RPC, network communication etc, but just functions/classes/methods, etc.



You can simplify some complex real world problems if you think it's not worth illustrating in the project.

How To Use:
1. In the main function, before go to the ATM Loop, It will create 100 users (id 1->100) with 3 accounts (from 0 - 2)for each user.
2. Go to ATM loop:
  1. Request Insert Card: input userid
  2. Request Input PIN: input PIN default 123456
  3. Request Select Account: input account id 
  4. Select Action Option:
    0: view balance
    1: withdraw
    2: deposit
    3: change PIN
